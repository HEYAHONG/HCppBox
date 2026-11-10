/*
pci-vfio.c - VFIO PCI Passthrough
Copyright (C) 2022  LekKit <github.com/LekKit>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/*
 * TODO: Detect custom VFIO drivers like mlx5-vfio-pci
 * TODO: Improve MSI-X BAR detection
 * TODO: IO port BARs via region callbacks
 */

// Expose pread()/pwrite()/readlink(), O_CLOEXEC
#include <util/feature_test.h>

#include <rvvm/rvvm_board.h>
#include <rvvm/rvvm_pci.h>
#include <rvvm/rvvm_region.h>

#include <util/blk_io.h>
#include <util/mem_ops.h>
#include <util/rvtimer.h>
#include <util/threading.h>
#include <util/utils.h>
#include <util/vector.h>

PUSH_OPTIMIZATION_SIZE

// Check that <linux/vfio.h> include is available
#if defined(__linux__) && defined(USE_VFIO) && !CHECK_INCLUDE(linux/vfio.h, 0)
#warning Disabling USE_VFIO as <linux/vfio.h> is unavailable
#undef USE_VFIO
#endif

#if defined(__linux__) && defined(USE_VFIO)

#include <errno.h>       // For errno
#include <fcntl.h>       // For open(), O_RDONLY, O_RDWR
#include <string.h>      // For strerror()
#include <sys/eventfd.h> // For eventfd()
#include <sys/ioctl.h>   // For ioctl()
#include <sys/mman.h>    // For mmap(), munmap(), MAP_*, PROT_*
#include <unistd.h>      // For close(), read(), write(), lseek(), pread(), pwrite(), readlink()

#include <linux/vfio.h>

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif

/*
 * VFIO structures
 */

typedef struct {
    rvvm_pci_func_t* func;
    rvvm_thread_t*   thread;

    uint32_t running;
    int      eventfd;
    int      vector;
} vfio_irq_t;

typedef struct {
    rvvm_pci_func_t*      func;
    vector_t(vfio_irq_t*) irqs;

    // VFIO descriptors
    int container;
    int group;
    int device;

    // Host PCI ID
    char pci_id[16];

    // Driver that originally owned the device
    char orig_driver[32];
} vfio_func_t;

/*
 * VFIO helpers
 */

static size_t vfio_rw_file(const char* path, void* rd, const void* wr, size_t size)
{
    rvfile_t* file = rvopen(path, wr ? RVFILE_WRITE : RVFILE_READ);
    size_t    ret  = rvfilesize(file);
    if (size && wr) {
        ret = rvwrite(file, wr, size, 0);
    } else if (size) {
        ret = rvread(file, rd, size, 0);
    }
    rvclose(file);
    return ret;
}

static size_t vfio_get_driver(const char* pci_id, char* buf, size_t size)
{
    char dev_path[64] = ZERO_INIT;
    char drv_path[64] = ZERO_INIT;
    rvvm_snprintf(dev_path, sizeof(dev_path), "/sys/bus/pci/devices/%s/driver", pci_id);
    if (readlink(dev_path, drv_path, sizeof(drv_path) - 1) > 0) {
        for (size_t i = sizeof(drv_path); --i;) {
            if (drv_path[i] == '/') {
                return rvvm_strlcpy(buf, drv_path + i + 1, size);
            }
        }
    }
    return 0;
}

static bool vfio_unbind_driver(const char* pci_id)
{
    char path[64] = ZERO_INIT;
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/driver/unbind", pci_id);
    return !!vfio_rw_file(path, NULL, pci_id, rvvm_strlen(pci_id));
}

static bool vfio_bind_driver(const char* pci_id, const char* driver)
{
    char path[64]    = ZERO_INIT;
    char ven_dev[16] = ZERO_INIT;
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/vendor", pci_id);
    vfio_rw_file(path, ven_dev, NULL, 6);
    ven_dev[6] = ' ';
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/device", pci_id);
    vfio_rw_file(path, ven_dev + 7, NULL, 6);
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/drivers/%s/new_id", driver);
    vfio_rw_file(path, NULL, ven_dev, rvvm_strlen(ven_dev));
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/drivers/%s/bind", driver);
    return !!vfio_rw_file(path, NULL, pci_id, rvvm_strlen(pci_id));
}

static uint32_t vfio_get_iommu_group(const char* pci_id)
{
    char path[64]       = ZERO_INIT;
    char group_path[64] = ZERO_INIT;
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/iommu_group", pci_id);
    if (readlink(path, group_path, sizeof(group_path) - 1) > 0) {
        for (size_t i = sizeof(group_path); --i;) {
            if (group_path[i] == '/') {
                return str_to_int_dec(group_path + i + 1);
            }
        }
    }
    return -1;
}

static int vfio_open_group(const char* pci_id)
{
    char path[64] = ZERO_INIT;
    rvvm_snprintf(path, sizeof(path), "/dev/vfio/%u", vfio_get_iommu_group(pci_id));
    return open(path, O_RDWR | O_CLOEXEC);
}

static size_t vfio_read_rom(const char* pci_id, void* buffer, size_t size)
{
    char   path[64] = ZERO_INIT;
    size_t ret      = 0;
    rvvm_snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/rom", pci_id);
    vfio_rw_file(path, NULL, "1", 1);
    ret = vfio_rw_file(path, buffer, NULL, size);
    vfio_rw_file(path, NULL, "0", 1);
    return ret;
}

/*
 * VFIO handling
 */

static void vfio_free(vfio_func_t* vfio)
{
    // Shutdown VFIO IRQ eventfd threads
    uint64_t val = 1;
    vector_foreach_back (vfio->irqs, i) {
        vfio_irq_t* irq = vector_at(vfio->irqs, i);
        atomic_store_uint32(&irq->running, false);
        UNUSED(!write(irq->eventfd, &val, sizeof(val)));
        rvvm_thread_join(irq->thread);
        free(irq);
    }
    vector_free(vfio->irqs);

    // Close VFIO descriptors
    if (vfio->device > 0) {
        close(vfio->device);
    }
    if (vfio->group > 0) {
        close(vfio->group);
    }
    if (vfio->container > 0) {
        close(vfio->container);
    }

    // Restore original device driver
    if (!rvvm_strcmp(vfio->orig_driver, "vfio-pci")) {
        vfio_unbind_driver(vfio->pci_id);
        vfio_bind_driver(vfio->pci_id, vfio->orig_driver);
    }

    free(vfio);
}

static void vfio_bar_cleanup(rvvm_reg_dev_t* dev)
{
    vfio_func_t*    vfio = rvvm_region_data(dev);
    rvvm_reg_desc_t desc;
    if (rvvm_region_get_desc(dev, &desc) && desc.mmap && desc.size) {
        // This BAR holds a host PCI MMIO mapping
        munmap(desc.mmap, desc.size);
    }
    if (vfio) {
        // This BAR is responsible for VFIO cleanup
        vfio_free(vfio);
    }
}

static const rvvm_reg_type_t vfio_bar_type = {
    .name    = "pci-vfio-bar",
    .cleanup = vfio_bar_cleanup,
};

static void* vfio_irq_thread(void* data)
{
    vfio_irq_t* irq = data;
    uint64_t    val = 0;
    while (true) {
        UNUSED(!read(irq->eventfd, &val, sizeof(val)));
        if (atomic_load_uint32_relax(&irq->running)) {
            rvvm_pci_send_irq(irq->func, irq->vector);
        } else {
            break;
        }
    }
    return NULL;
}

static void vfio_enable_irqs(vfio_func_t* vfio)
{
    vector_foreach_back (vfio->irqs, i) {
        vfio_irq_t* irq = vector_at(vfio->irqs, i);

        irq->func    = vfio->func;
        irq->running = true;
        irq->thread  = rvvm_thread_create(vfio_irq_thread, irq);
    }
}

static bool vfio_bind(vfio_func_t* vfio)
{
    char driver[32] = ZERO_INIT;
    vfio_get_driver(vfio->pci_id, vfio->orig_driver, sizeof(vfio->orig_driver));
    if (!rvvm_strcmp(vfio->orig_driver, "vfio-pci")) {
        vfio_unbind_driver(vfio->pci_id);
        vfio_bind_driver(vfio->pci_id, "vfio-pci");
    }
    vfio_get_driver(vfio->pci_id, driver, sizeof(driver));
    return rvvm_strcmp(driver, "vfio-pci");
}

static bool vfio_prepare(vfio_func_t* vfio)
{
    struct vfio_group_status group_status = {
        .argsz = sizeof(struct vfio_group_status),
    };

    // Prepare VFIO container
    vfio->container = open("/dev/vfio/vfio", O_RDWR | O_CLOEXEC);
    if (vfio->container < 0) {
        rvvm_debug("Failed to open /dev/vfio/vfio: %s", strerror(errno));
        return false;
    }

    // Prepare VFIO group
    vfio->group = vfio_open_group(vfio->pci_id);
    if (vfio->group < 0) {
        rvvm_debug("Failed to open VFIO group: %s", strerror(errno));
        return false;
    }
    if (ioctl(vfio->group, VFIO_GROUP_GET_STATUS, &group_status) || //
        !(group_status.flags & VFIO_GROUP_FLAGS_VIABLE)) {
        rvvm_debug("VFIO group not viable, are all group devices attached to vfio_pci module?");
        return false;
    }
    if (ioctl(vfio->group, VFIO_GROUP_SET_CONTAINER, &vfio->container)) {
        rvvm_debug("Failed to set VFIO container group: %s", strerror(errno));
        return false;
    }
    if (ioctl(vfio->container, VFIO_SET_IOMMU, VFIO_TYPE1_IOMMU)) {
        rvvm_debug("Failed to set up VFIO IOMMU: %s", strerror(errno));
        return false;
    }

    // Prepare VFIO device
    vfio->device = ioctl(vfio->group, VFIO_GROUP_GET_DEVICE_FD, vfio->pci_id);
    if (vfio->device < 0) {
        rvvm_debug("Failed to get VFIO device fd: %s", strerror(errno));
        return false;
    }
    return true;
}

// Setup MSI-X / MSI interrupts
static bool vfio_setup_irqs(vfio_func_t* vfio, int irq_type)
{
    struct vfio_irq_info irq_info = {
        .argsz = sizeof(struct vfio_irq_info),
        .index = irq_type,
    };
    if (ioctl(vfio->device, VFIO_DEVICE_GET_IRQ_INFO, &irq_info)) {
        return false;
    }
    if (!irq_info.count && irq_type == VFIO_PCI_MSI_IRQ_INDEX) {
        rvvm_debug("VFIO device doesn't have any IRQ vectors");
    }

    size_t irq_size = sizeof(struct vfio_irq_set) + (irq_info.count * sizeof(int));

    struct vfio_irq_set* irq_set = safe_calloc(1, irq_size);

    irq_set->argsz = irq_size;
    irq_set->flags = VFIO_IRQ_SET_DATA_EVENTFD | VFIO_IRQ_SET_ACTION_TRIGGER;
    irq_set->index = irq_type;
    irq_set->start = 0;
    irq_set->count = irq_info.count;

    for (size_t i = 0; i < irq_info.count; ++i) {
        vfio_irq_t* irq = safe_new_obj(vfio_irq_t);
        irq->eventfd    = eventfd(0, 0);
        irq->vector     = i;

        // NOTE: vfio_irq_set data buffer is treated as an int array
        memcpy(((uint8_t*)irq_set->data) + (i * sizeof(int)), &irq->eventfd, sizeof(int));
        vector_push_back(vfio->irqs, irq);

        if (irq->eventfd < 0) {
            rvvm_debug("Failed to create VFIO IRQ eventfd: %s", strerror(errno));
        }
    }

    bool ret = !ioctl(vfio->device, VFIO_DEVICE_SET_IRQS, irq_set);
    free(irq_set);
    return ret;
}

static bool vfio_map_dma(vfio_func_t* vfio, rvvm_machine_t* machine, rvvm_addr_t addr, size_t size)
{
    struct vfio_iommu_type1_dma_map dma_map = {
        .argsz = sizeof(struct vfio_iommu_type1_dma_map),
        .flags = VFIO_DMA_MAP_FLAG_READ | VFIO_DMA_MAP_FLAG_WRITE,
        .iova  = addr,
        .size  = size,
        .vaddr = (size_t)rvvm_get_dma_ptr(machine, addr, size),
    };
    return !ioctl(vfio->container, VFIO_IOMMU_MAP_DMA, &dma_map);
}

// Setup DMA to main memory
static bool vfio_setup_dma(vfio_func_t* vfio, rvvm_machine_t* machine)
{
    rvvm_addr_t mem_base = rvvm_get_opt(machine, RVVM_OPT_MEM_BASE);
    rvvm_addr_t mem_size = rvvm_get_opt(machine, RVVM_OPT_MEM_SIZE);

    // Set up DMA to guest RAM
    if (!vfio_map_dma(vfio, machine, mem_base, mem_size)) {
        // This *kinda* works around a DMA conflict with x86 MSI IRQ vector reserved region
        // More info: https://lore.kernel.org/linux-iommu/20191211082304.2d4fab45@x1.home/
        // cat /sys/kernel/iommu_groups/[iommu group]/reserved_regions

        // LAPIC MSI registers are usually placed on address 0xFEE00000, I/O APIC on address 0xFEС00000
        const rvvm_addr_t msi_x86_low = 0xFEC00000;
        const rvvm_addr_t msi_x86_end = 0xFEF00000;
        rvvm_debug("Workaround reserved x86 MSI IRQ vector by splitting DMA region");
        if (mem_base < msi_x86_low) {
            size_t low_size = EVAL_MIN(mem_size, msi_x86_low - mem_base);
            if (!vfio_map_dma(vfio, machine, mem_base, low_size)) {
                return false;
            }
        }
        if (mem_base + mem_size > msi_x86_end) {
            size_t high_size = (mem_base + mem_size) - msi_x86_end;
            if (!vfio_map_dma(vfio, machine, msi_x86_end, high_size)) {
                return false;
            }
        }
    }
    return true;
}

// Setup configuration space
static bool vfio_setup_cfg(vfio_func_t* vfio, rvvm_pci_func_desc_t* desc)
{
    struct vfio_region_info pci_cfg_info = {
        .argsz = sizeof(struct vfio_region_info),
        .index = VFIO_PCI_CONFIG_REGION_INDEX,
    };
    uint8_t pci_config[64] = ZERO_INIT;

    // Read device PCI config space
    if (ioctl(vfio->device, VFIO_DEVICE_GET_REGION_INFO, &pci_cfg_info)) {
        rvvm_debug("Failed to get VFIO PCI config space info: %s", strerror(errno));
        return false;
    }
    if (pread(vfio->device, pci_config, 64, pci_cfg_info.offset) != 64) {
        rvvm_debug("Failed to read PCI config space: %s", strerror(errno));
        return false;
    }

    // Disable INTx interrupts, Enable Bus Mastering & MMIO BAR access
    write_uint32_le(pci_config + 0x04, 0x0406);
    if (pwrite(vfio->device, pci_config + 4, 4, pci_cfg_info.offset + 4) != 4) {
        rvvm_debug("Failed to write PCI config space: %s", strerror(errno));
        return false;
    }

    desc->vendor_id  = read_uint16_le(pci_config);
    desc->device_id  = read_uint16_le(pci_config + 0x02);
    desc->subsys_ven = read_uint16_le(pci_config + 0x2C);
    desc->subsys_dev = read_uint16_le(pci_config + 0x2E);
    desc->class_code = read_uint16_le(pci_config + 0x0A);
    desc->prog_iface = pci_config[0x09];
    desc->revision   = pci_config[0x08];
    desc->irq_pin    = pci_config[0x3D];
    desc->irq_vecs   = vector_size(vfio->irqs);
    return true;
}

// Setup BAR regions
static bool vfio_setup_bars(vfio_func_t* vfio, rvvm_pci_func_desc_t* desc, rvvm_reg_desc_t* bar)
{
    struct vfio_device_info device_info = {
        .argsz = sizeof(struct vfio_device_info),
    };
    void* data = vfio;
    if (ioctl(vfio->device, VFIO_DEVICE_GET_INFO, &device_info)) {
        rvvm_debug("Failed to get VFIO device info: %s", strerror(errno));
        return false;
    }
    for (uint32_t i = 0; i < device_info.num_regions && i < 6; ++i) {
        struct vfio_region_info region_info = {
            .argsz = sizeof(struct vfio_region_info),
            .index = i,
        };
        if (ioctl(vfio->device, VFIO_DEVICE_GET_REGION_INFO, &region_info)) {
            rvvm_debug("Failed to get VFIO BAR info: %s", strerror(errno));
            return false;
        }
        bool valid = region_info.size && (region_info.flags & VFIO_REGION_INFO_FLAG_MMAP);
        if ((region_info.flags & VFIO_REGION_INFO_FLAG_CAPS) && i >= 4) {
            rvvm_debug("Skipping MSI-X BAR %u", i);
            valid = false;
        }
        if (valid) {
            void* map = mmap(NULL, region_info.size, PROT_READ | PROT_WRITE, MAP_SHARED, //
                             vfio->device, region_info.offset);
            if (map != MAP_FAILED) {
                bar[i].size = region_info.size;
                bar[i].data = data;
                bar[i].mmap = map;
                bar[i].type = &vfio_bar_type;
                bar[i].attr = RVVM_REG_ATTR_BAR64;

                desc->bar[i] = &bar[i];

                // Only one BAR is responsible for VFIO cleanup
                data = NULL;
            } else {
                rvvm_debug("VFIO BAR mmap() failed: %s", strerror(errno));
                return false;
            }
        }
    }
    return true;
}

// Setup expansion ROM region
static void vfio_setup_rom(vfio_func_t* vfio, rvvm_pci_func_desc_t* desc, rvvm_reg_desc_t* rom)
{
    size_t rom_size = vfio_read_rom(vfio->pci_id, NULL, 0);
    if (rom_size) {
        void* map = mmap(NULL, rom_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (map != MAP_FAILED) {
            size_t tmp = 0;
            for (size_t i = 0; i < 10; ++i) {
                tmp = vfio_read_rom(vfio->pci_id, map, rom_size);
                if (tmp == rom_size) {
                    break;
                }
                sleep_ms(10);
            }
            if (tmp == rom_size) {
                rom->mmap = map;
                rom->size = rom_size;
                rom->type = &vfio_bar_type;

                desc->rom = rom;
            } else {
                rvvm_debug("Failed to read PCI ROM! Check whether CSM is disabled in host BIOS");
                munmap(map, rom_size);
            }
        } else {
            rvvm_debug("VFIO ROM BAR mmap() failed: %s", strerror(errno));
        }
    }
}

RVVM_PUBLIC rvvm_pci_func_t* rvvm_pci_vfio_init(rvvm_machine_t* machine, /**/
                                                const char*     pci_id,  /**/
                                                rvvm_pci_addr_t addr)
{
    vfio_func_t* vfio = safe_new_obj(vfio_func_t);

    rvvm_pci_func_desc_t desc   = {0};
    rvvm_reg_desc_t      bar[6] = {0};
    rvvm_reg_desc_t      rom    = {0};

    size_t len = 0;

    // Prepare PCI ID
    if (rvvm_strlen(pci_id) < 12) {
        len = rvvm_strlcpy(vfio->pci_id, "0000:", sizeof(vfio->pci_id));
    }
    rvvm_strlcpy(vfio->pci_id + len, pci_id, sizeof(vfio->pci_id) - len);

    // Probe vfio driver
    UNUSED(!system("modprobe vfio_pci"));

    // Attempt to bind & configure device
    if (!vfio_bind(vfio)) {
        rvvm_error("Failed to bind device to vfio-pci");
    } else if (!vfio_prepare(vfio)) {
        rvvm_error("Failed to attach device to VFIO group");
    } else if (!vfio_setup_dma(vfio, machine)) {
        rvvm_error("Failed to set up VFIO DMA: %s", strerror(errno));
    } else if (!vfio_setup_irqs(vfio, VFIO_PCI_MSIX_IRQ_INDEX) && //
               !vfio_setup_irqs(vfio, VFIO_PCI_MSI_IRQ_INDEX)) {
        rvvm_error("Failed to set up VFIO IRQs");
    } else if (!vfio_setup_cfg(vfio, &desc)) {
        rvvm_error("Failed to set up VFIO configuration space");
    } else if (!vfio_setup_bars(vfio, &desc, bar)) {
        rvvm_error("Failed to set up VFIO BARs");
    } else {
        vfio_setup_rom(vfio, &desc, &rom);
        rvvm_pci_func_t* func = rvvm_pci_func_init(machine, &desc, addr);
        if (func) {
            // Success
            vfio->func = func;
            vfio_enable_irqs(vfio);
            ioctl(vfio->device, VFIO_DEVICE_RESET);
        }
        return func;
    }

    // Failure
    for (size_t i = 0; i < 6; ++i) {
        if (desc.bar[i]) {
            // Cleanup via PCI function to release BARs
            rvvm_pci_func_init(NULL, &desc, 0);
            return NULL;
        }
    }

    // Generic cleanup
    vfio_free(vfio);
    return NULL;
}

#else

RVVM_PUBLIC rvvm_pci_func_t* rvvm_pci_vfio_init(rvvm_machine_t* machine, /**/
                                                const char*     pci_id,  /**/
                                                rvvm_pci_addr_t addr)
{
    UNUSED(machine && pci_id && addr);
    return NULL;
}

#endif

POP_OPTIMIZATION_SIZE
