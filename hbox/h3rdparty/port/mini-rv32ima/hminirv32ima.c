/***************************************************************
 * Name:      hminirv32ima.c
 * Purpose:   实现hminirv32ima接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmemory.h"
#include "hminirv32ima.h"

static void hminirv32ima_state_csr_write(struct hminirv32ima_state *state,uint32_t csrno,uint32_t csrvalue)
{
    (void)state;
    (void)csrno;
    (void)csrvalue;
}

static void hminirv32ima_state_csr_read(struct hminirv32ima_state *state,uint32_t csrno,uint32_t* csrvalue)
{
    (void)state;
    (void)csrno;
    (void)csrvalue;
}

 void hminirv32ima_state_post_exec(struct hminirv32ima_state *state,uint32_t *pc,uint32_t *ir,uint32_t *trap)
 {
     (void)state;
     (void)pc;
     (void)ir;
     (void)trap;
 }


static size_t hminirv32ima_memory_load(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,void *ptr,size_t length)
{
    (void)mem;
    (void)ram_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_memory_store(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,const void *ptr,size_t length)
{
    (void)mem;
    (void)ram_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_mmio_load(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,void *ptr,size_t length)
{
    (void)mmio;
    (void)mmio_addr;
    (void)ptr;
    (void)length;
    return 0;
}

static size_t hminirv32ima_mmio_store(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,const void *ptr,size_t length)
{
    (void)mmio;
    (void)mmio_addr;
    (void)ptr;
    (void)length;
    return 0;
}


void hminirv32ima_core_init(hminirv32ima_core_t *core,size_t ram_size)
{
    if(core!=NULL)
    {
        memset(core,0,sizeof(*core));

        /*
         * 初始化状态
         */
        core->state.extraflags |= 3;                       /**< 默认特权为机器级 */
        core->state.csr_read=hminirv32ima_state_csr_read;
        core->state.csr_write=hminirv32ima_state_csr_write;
        core->state.post_exec=hminirv32ima_state_post_exec;

        /*
         * 初始化内存
         */
        core->memory.ram_base=0x80000000;
        core->memory.ram_size=ram_size;
        core->memory.mem_load=hminirv32ima_memory_load;
        core->memory.mem_store=hminirv32ima_memory_store;
        core->state.pc = core->memory.ram_base;                                 /**< 设置PC值 */
        core->state.regs[2]=core->memory.ram_base+core->memory.ram_size;        /**< 初始化SP指针 */

        /*
         * 初始化MMIO
         */
        core->mmio.mmio_base=0x10000000;
        core->mmio.mmio_size= 0x2000000;
        core->mmio.mmio_load=hminirv32ima_mmio_load;
        core->mmio.mmio_store=hminirv32ima_mmio_store;
    }
}

#include H3RDPARTY_MINI_RV32IMA_64MB_DTB_HEADER

static size_t hminirv32ima_machine_default64mb_memory_load(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,void *ptr,size_t length)
{
    const hminirv32ima_core_t                   *   core=GET_STRUCT_PTR_BY_MEMBER_PTR(mem,hminirv32ima_core_t,memory);
    const hminirv32ima_machine_default64mb_t    *   machine=GET_STRUCT_PTR_BY_MEMBER_PTR(core,hminirv32ima_machine_default64mb_t,core);
    if(ram_addr >= mem->ram_base && ram_addr < (mem->ram_base+mem->ram_size) && (ram_addr+length) <= (mem->ram_base+mem->ram_size))
    {
        size_t ofs=ram_addr-mem->ram_base;
        switch(length)
        {
        case 1:
        {
            uint8_t * data=(uint8_t *)ptr;
            data[0]=machine->ram[ofs];
        }
        break;
        case 2:
        {
            uint16_t * data=(uint16_t *)ptr;
            (*data)=machine->ram[ofs+1];
            (*data) <<= 8;
            (*data)+=machine->ram[ofs];
        }
        break;
        case 4:
        {
            uint32_t * data=(uint32_t *)ptr;
            (*data)=machine->ram[ofs+3];
            (*data) <<= 8;
            (*data)+=machine->ram[ofs+2];
            (*data) <<= 8;
            (*data)+=machine->ram[ofs+1];
            (*data) <<= 8;
            (*data)+=machine->ram[ofs];
        }
        break;
        }
        return length;
    }
    return 0;
}

static size_t hminirv32ima_machine_default64mb_memory_store(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,const void *ptr,size_t length)
{
    const hminirv32ima_core_t                   *   core=GET_STRUCT_PTR_BY_MEMBER_PTR(mem,hminirv32ima_core_t,memory);
    hminirv32ima_machine_default64mb_t          *   machine=(hminirv32ima_machine_default64mb_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(core,hminirv32ima_machine_default64mb_t,core);
    if(ram_addr >= mem->ram_base && ram_addr < (mem->ram_base+mem->ram_size) && (ram_addr+length) <= (mem->ram_base+mem->ram_size))
    {
        size_t ofs=ram_addr-mem->ram_base;
        switch(length)
        {
        case 1:
        {
            const uint8_t * data=(const uint8_t *)ptr;
            machine->ram[ofs]=data[0];
        }
        break;
        case 2:
        {
            const uint16_t * data=(const uint16_t *)ptr;
            machine->ram[ofs]=((*data)&0xFF);
            machine->ram[ofs+1]=(((*data)>>8)&0xFF);
        }
        break;
        case 4:
        {
            const uint32_t *data=(const uint32_t *)ptr;
            machine->ram[ofs]=((*data)&0xFF);
            machine->ram[ofs+1]=(((*data)>>8)&0xFF);
            machine->ram[ofs+2]=(((*data)>>16)&0xFF);
            machine->ram[ofs+3]=(((*data)>>24)&0xFF);
        }
        break;
        }
        return length;
    }
    return 0;
}

void hminirv32ima_machine_default64mb_init(hminirv32ima_machine_default64mb_t *machine)
{
    if(machine!=NULL)
    {
        /*
         * 复位机器
         */
        hminirv32ima_machine_default64mb_reset(machine);

        /*
         * 清空内存
         */
        memset(machine->ram,0,sizeof(machine->ram));

        /*
         * 设置回调
         */
        machine->console_has_data=NULL;
        machine->console_put_data=NULL;
        machine->console_get_data=NULL;

    }
}

static size_t hminirv32ima_machine_default64mb_mmio_load(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,void *ptr,size_t length)
{
    const hminirv32ima_core_t                   *   core=GET_STRUCT_PTR_BY_MEMBER_PTR(mmio,hminirv32ima_core_t,mmio);
    const hminirv32ima_machine_default64mb_t    *   machine=GET_STRUCT_PTR_BY_MEMBER_PTR(core,hminirv32ima_machine_default64mb_t,core);
    if(length==4)
    {
        uint32_t *data=(uint32_t *)ptr;
        if(mmio_addr>=0x10000000 && mmio_addr < (0x10000000+0x100))
        {
            /*
             * 8250串口
             */
            size_t offset=mmio_addr-0x10000000;
            uint8_t reg_value=0;
            hs_common_serial_8250_bus_read((hs_common_serial_8250_t *)&machine->serial8250,offset,&reg_value);
            (*data)=reg_value;
        }
        else if(mmio_addr>=0x11000000 && mmio_addr < (0x11000000+0x10000))
        {
            /*
             * clint
             */
            size_t offset=mmio_addr-0x11000000;
            switch(offset)
            {
            case 0xBFF8:
            {
                (*data)=machine->core.state.timerl;
            }
            break;
            case 0xBFFC:
            {
                (*data)=machine->core.state.timerh;
            }
            break;
            default:
            {

            }
            break;
            }

        }
        return length;
    }
    return 0;
}

static size_t hminirv32ima_machine_default64mb_mmio_store(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,const void *ptr,size_t length)
{
    const hminirv32ima_core_t                   *   core=GET_STRUCT_PTR_BY_MEMBER_PTR(mmio,hminirv32ima_core_t,mmio);
    hminirv32ima_machine_default64mb_t          *   machine=(hminirv32ima_machine_default64mb_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(core,hminirv32ima_machine_default64mb_t,core);
    if(length==4)
    {
        const uint32_t *data=(const uint32_t *)ptr;
        if(mmio_addr>=0x10000000 && mmio_addr < (0x10000000+0x100))
        {
            /*
             * 8250串口
             */
            size_t offset=mmio_addr-0x10000000;
            hs_common_serial_8250_bus_write(&machine->serial8250,offset,(*data));

        }
        else if(mmio_addr>=0x11000000 && mmio_addr < (0x11000000+0x10000))
        {
            /*
             * clint
             */
            size_t offset=mmio_addr-0x11000000;
            switch(offset)
            {
            case 0x4000:
            {
                machine->core.state.timermatchl=(*data);
            }
            break;
            case 0x4004:
            {
                machine->core.state.timermatchh=(*data);
            }
            break;
            default:
            {

            }
            break;
            }

        }
        else if(mmio_addr>= 0x11100000 && mmio_addr < (0x11100000+0x1000))
        {
            /*
             * syscon
             */
            size_t offset=mmio_addr-0x11100000;
            switch(offset)
            {
            case 0:
            {
                machine->syscon=(*data);
            }
            break;
            default:
            {

            }
            break;
            }

        }
        return length;
    }
    return 0;
}

static bool hminirv32ima_machine_default64mb_serial_8250_io_callback(struct hs_common_serial_8250 *dev,hs_common_serial_8250_io_operate_t io_operate,uint8_t *data)
{
    if(dev==NULL)
    {
        return false;
    }
    bool ret=false;
    const hminirv32ima_machine_default64mb_t    *   machine=GET_STRUCT_PTR_BY_MEMBER_PTR(dev,hminirv32ima_machine_default64mb_t,serial8250);
    switch(io_operate)
    {
    case HS_COMMON_SERIAL_8250_IO_OPERATE_TICK:
    {
        /*
         * 更新LSR寄存器
         */
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]=0x60;

        if(machine->console_has_data!=NULL)
        {
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR] |=  machine->console_has_data(machine)?0x01:0x00;
        }
        ret=true;
    }
    break;
    case HS_COMMON_SERIAL_8250_IO_OPERATE_TRANSMIT_BYTE:
    {
        if(machine->console_put_data!=NULL)
        {
            machine->console_put_data(machine,(*data));
        }
        ret=true;
    }
    break;
    case HS_COMMON_SERIAL_8250_IO_OPERATE_RECEIVE_BYTE:
    {
        if(machine->console_get_data!=NULL)
        {
            (*data)=machine->console_get_data(machine);
        }
        ret=true;
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
}

void hminirv32ima_machine_default64mb_reset(hminirv32ima_machine_default64mb_t *machine)
{
    if(machine!=NULL)
    {
        /*
         * 初始化core
         */
        hminirv32ima_core_init(&machine->core,sizeof(machine->ram)+sizeof(machine->dtb));

        /*
         * 初始化dtb
         */
        memset(machine->dtb,0,sizeof(machine->dtb));
        memcpy(machine->dtb,default64mbdtb,sizeof(machine->dtb)>sizeof(default64mbdtb)?sizeof(default64mbdtb):sizeof(machine->dtb));

        /*
         * 设置内存信息
         */
        machine->core.memory.mem_load=hminirv32ima_machine_default64mb_memory_load;
        machine->core.memory.mem_store=hminirv32ima_machine_default64mb_memory_store;

        /*
         * 设置MMIO
         */
        machine->core.mmio.mmio_load=hminirv32ima_machine_default64mb_mmio_load;
        machine->core.mmio.mmio_store=hminirv32ima_machine_default64mb_mmio_store;

        /*
         * serial 8250
         */
        hs_common_serial_8250_init(&machine->serial8250,hminirv32ima_machine_default64mb_serial_8250_io_callback,NULL,HS_COMMON_SERIAL_8250_CLK_FREQ);

        /*
         * syscon
         */
        machine->syscon=0;


        /*
         * 初始化寄存器
         */
        machine->core.state.pc = machine->core.memory.ram_base;                                 /**< 初始化PC值 */
        machine->core.state.regs[2]=machine->core.memory.ram_base+sizeof(machine->ram);         /**< 初始化SP指针 */
        machine->core.state.regs[10]=0;                                                         /**< 初始化A0（HART地址） */
        machine->core.state.regs[11]=machine->core.memory.ram_base+sizeof(machine->ram);        /**< 初始化A1(DTS地址) */
    }
}

bool hminirv32ima_machine_default64mb_load_image(hminirv32ima_machine_default64mb_t *machine,const uint8_t *image,size_t image_size)
{
    if(machine!=NULL && image != NULL && image_size < sizeof(machine->ram))
    {
        memcpy(machine->ram,image,image_size);
        return true;
    }
    return false;
}

bool hminirv32ima_machine_default64mb_load_dtb(hminirv32ima_machine_default64mb_t *machine,const uint8_t *dtb,size_t dtb_size)
{
    if(machine!=NULL && dtb != NULL && dtb_size < sizeof(machine->dtb))
    {
        memcpy(machine->dtb,dtb,dtb_size);
        return true;
    }
    return false;
}

int hminirv32ima_machine_default64mb_step(hminirv32ima_machine_default64mb_t *machine,uint32_t elapsedUs, int insn_count )
{
    if(machine==NULL)
    {
        return -1;
    }

    if(machine->syscon==0x5555 || machine->syscon==0x7777)
    {
        /*
         * 关机与重启直接返回
         */
        return machine->syscon;
    }

    hs_common_serial_8250_bus_tick(&machine->serial8250);

    return hminirv32ima_core_step(&machine->core,elapsedUs,insn_count);
}

#define MINIRV32_RAM_IMAGE_OFFSET                      (core->memory.ram_base)
#define MINI_RV32_RAM_SIZE                             (core->memory.ram_size)
#define MINIRV32_MMIO_RANGE(n)                         ((core->mmio.mmio_base) <= (n) && (n) < (core->mmio.mmio_base+core->mmio.mmio_size))
#define MINIRV32_HANDLE_MEM_STORE_CONTROL(addr,rs)     do {core->mmio.mmio_store(&core->mmio,addr,&rs,sizeof(rs));} while(0)
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL(addr,rs)      do {core->mmio.mmio_load(&core->mmio,addr,&rs,sizeof(rs));}  while(0)
#define MINIRV32_OTHERCSR_WRITE(csrno,csrval)          do {core->state.csr_write(&core->state,csrno,csrval);}      while(0)
#define MINIRV32_OTHERCSR_READ(csrno,csrval)           do {core->state.csr_read(&core->state,csrno,&csrval);}      while(0)
#define MINIRV32_POSTEXEC( pc, ir, trap )              do {core->state.post_exec(&core->state,&pc,&ir,&trap);}      while(0)

#define MINIRV32_CUSTOM_MEMORY_BUS 1
static inline void hminirv32ima_store4(hminirv32ima_core_t *core,size_t offset,uint32_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE4( ofs, val ) hminirv32ima_store4(core,ofs,val)
static inline void hminirv32ima_store2(hminirv32ima_core_t *core,size_t offset,uint16_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE2( ofs, val ) hminirv32ima_store2(core,ofs,val)
static inline void hminirv32ima_store1(hminirv32ima_core_t *core,size_t offset,uint8_t val)
{
    core->memory.mem_store(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
}
#define MINIRV32_STORE1( ofs, val ) hminirv32ima_store1(core,ofs,val)
static inline uint32_t hminirv32ima_load4(hminirv32ima_core_t *core,size_t offset)
{
    uint32_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD4( ofs ) hminirv32ima_load4(core,ofs)
static inline uint16_t hminirv32ima_load2(hminirv32ima_core_t *core,size_t offset)
{
    uint16_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD2( ofs ) hminirv32ima_load2(core,ofs)
static inline uint8_t hminirv32ima_load1(hminirv32ima_core_t *core,size_t offset)
{
    uint8_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD1( ofs ) hminirv32ima_load1(core,ofs)
static inline int16_t hminirv32ima_load2_signed(hminirv32ima_core_t *core,size_t offset)
{
    int16_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD2_SIGNED( ofs ) hminirv32ima_load2_signed(core,ofs)
static inline int8_t hminirv32ima_load1_signed(hminirv32ima_core_t *core,size_t offset)
{
    int8_t val=0;
    core->memory.mem_load(&core->memory,offset+core->memory.ram_base,&val,sizeof(val));
    return val;
}
#define MINIRV32_LOAD1_SIGNED( ofs ) hminirv32ima_load1_signed(core,ofs)

#define MINIRV32_CUSTOM_INTERNALS 1
#define CSR( x ) core->state.x
#define SETCSR( x, val ) { core->state.x = val; }
#define REG( x ) core->state.regs[x]
#define REGSET( x, val ) { core->state.regs[x] = val; }

#define MINIRV32_STEPPROTO static int32_t hminirv32ima_step_internal(hminirv32ima_core_t *core,uint32_t vProcAddress, uint32_t elapsedUs, int count )

#define MINIRV32_IMPLEMENTATION 1

#include H3RDPARTY_MINI_RV32IMA_HEADER

int hminirv32ima_core_step(hminirv32ima_core_t *core,uint32_t elapsedUs, int insn_count )
{
    if(core==NULL)
    {
        return -1;
    }
    return hminirv32ima_step_internal(core,0,elapsedUs,insn_count);
}


