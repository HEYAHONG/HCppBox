/***************************************************************
 * Name:      moxie_moxiebox.c
 * Purpose:   实现moxie_moxiebox大端虚拟机模型接口(仿moxiebox)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "moxie_moxiebox.h"
#include "hdefaults.h"
#include "stdio.h"
#include "errno.h"

#define HS_MOXIE_MOXIEBOX_DESC_SIZE (8+HS_MOXIE_MOXIEBOX_TAG_SIZE)

/*
 * 大端序读写辅助函数。
 */
static inline uint32_t hs_moxie_moxiebox_read_u32(const uint8_t *data)
{
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | (uint32_t)data[3];
}

static inline void hs_moxie_moxiebox_write_u32(uint8_t *data,uint32_t value)
{
    data[0]=(uint8_t)(value >> 24);
    data[1]=(uint8_t)(value >> 16);
    data[2]=(uint8_t)(value >> 8);
    data[3]=(uint8_t)(value >> 0);
}

static inline uint64_t hs_moxie_moxiebox_max_end(hs_moxie_moxiebox_t *machine)
{
    uint64_t ret=0;
    if(machine!=NULL)
    {
        for(size_t i=0;i<machine->memory_count;i++)
        {
            uint64_t end=(uint64_t)machine->memory[i].start+machine->memory[i].length;
            if(end>ret)
            {
                ret=end;
            }
        }
    }
    return ret;
}

static inline hs_moxie_moxiebox_memory_t *hs_moxie_moxiebox_memory_find(hs_moxie_moxiebox_t *machine,uint32_t address,size_t len,bool want_write,size_t *offset)
{
    if(machine==NULL || len==0)
    {
        return NULL;
    }
    if((uint64_t)address+len > 0x100000000ULL)
    {
        return NULL;
    }
    for(size_t i=0;i<machine->memory_count;i++)
    {
        hs_moxie_moxiebox_memory_t *mem=&machine->memory[i];
        if(address>=mem->start && ((uint64_t)address+len)<=((uint64_t)mem->start+mem->length))
        {
            if(want_write)
            {
                if(mem->flags.readonly)
                {
                    return NULL;
                }
                if(mem->data==NULL && mem->write==NULL)
                {
                    return NULL;
                }
            }
            else
            {
                if(mem->data==NULL && mem->read==NULL)
                {
                    return NULL;
                }
            }
            if(offset!=NULL)
            {
                *offset=(size_t)(address-mem->start);
            }
            return mem;
        }
    }
    return NULL;
}

static void hs_moxie_moxiebox_mmap_cleanup(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    size_t write_index=0;
    for(size_t i=0;i<machine->memory_count;i++)
    {
        if(machine->memory[i].flags.mmap_backed)
        {
            hmunmap(machine->memory[i].data,machine->memory[i].length);
        }
        else
        {
            if(write_index!=i)
            {
                machine->memory[write_index]=machine->memory[i];
            }
            write_index++;
        }
    }
    machine->memory_count=write_index;
    machine->heap_next=machine->heap_base;
    machine->heap_count=0;
}

static void hs_moxie_moxiebox_fd_table_reset(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    for(size_t i=0;i<HS_MOXIE_MOXIEBOX_MAX_FD;i++)
    {
        machine->fd_table[i].host_fd=-1;
        machine->fd_table[i].owned=0;
    }
    if(HS_MOXIE_MOXIEBOX_MAX_FD>0)
    {
        machine->fd_table[0].host_fd=0;
    }
    if(HS_MOXIE_MOXIEBOX_MAX_FD>1)
    {
        machine->fd_table[1].host_fd=1;
    }
    if(HS_MOXIE_MOXIEBOX_MAX_FD>2)
    {
        machine->fd_table[2].host_fd=2;
    }
}

static void hs_moxie_moxiebox_fd_table_close_all(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    for(size_t i=0;i<HS_MOXIE_MOXIEBOX_MAX_FD;i++)
    {
        if(machine->fd_table[i].owned && machine->fd_table[i].host_fd>=0)
        {
            hclose(machine->fd_table[i].host_fd);
        }
    }
}

static int hs_moxie_moxiebox_fd_alloc(hs_moxie_moxiebox_t *machine,int host_fd)
{
    if(machine==NULL || host_fd<0)
    {
        return -1;
    }
    size_t start=(HS_MOXIE_MOXIEBOX_MAX_FD>3)?3:0;
    for(size_t i=start;i<HS_MOXIE_MOXIEBOX_MAX_FD;i++)
    {
        if(machine->fd_table[i].host_fd<0)
        {
            machine->fd_table[i].host_fd=host_fd;
            machine->fd_table[i].owned=1;
            return (int)i;
        }
    }
    return -1;
}

static int hs_moxie_moxiebox_fd_host(hs_moxie_moxiebox_t *machine,uint32_t guest_fd)
{
    if(machine==NULL)
    {
        return -1;
    }
    /*
     * 0/1/2为标准输入/输出/错误，虚拟机无需open即可直接使用。
     */
    if(guest_fd<3)
    {
        return (int)guest_fd;
    }
    if(guest_fd>=HS_MOXIE_MOXIEBOX_MAX_FD)
    {
        return -1;
    }
    return machine->fd_table[guest_fd].host_fd;
}

static int hs_moxie_moxiebox_fd_close(hs_moxie_moxiebox_t *machine,uint32_t guest_fd)
{
    if(machine==NULL)
    {
        return -EBADF;
    }
    /*
     * 标准流不关闭主机文件描述符，保持可直接使用。
     */
    if(guest_fd<3)
    {
        return 0;
    }
    if(guest_fd>=HS_MOXIE_MOXIEBOX_MAX_FD || machine->fd_table[guest_fd].host_fd<0)
    {
        return -EBADF;
    }
    int ret=0;
    if(machine->fd_table[guest_fd].owned)
    {
        if(hclose(machine->fd_table[guest_fd].host_fd)<0)
        {
            ret=-(errno?errno:EIO);
        }
    }
    machine->fd_table[guest_fd].host_fd=-1;
    machine->fd_table[guest_fd].owned=0;
    return ret;
}

static bool hs_moxie_moxiebox_memory_add(hs_moxie_moxiebox_t *machine,uint32_t start,uint32_t length,uint8_t *data,bool readonly,bool mmap_backed,void *usr,hs_moxie_moxiebox_memory_read_t read,hs_moxie_moxiebox_memory_write_t write,const char *name)
{
    if(machine==NULL || length==0)
    {
        return false;
    }
    if(data==NULL && read==NULL && write==NULL)
    {
        return false;
    }
    if(machine->memory_count>=HS_MOXIE_MOXIEBOX_MAX_MEMORY)
    {
        return false;
    }
    if((uint64_t)start+length > 0x100000000ULL)
    {
        return false;
    }
    hs_moxie_moxiebox_memory_t *mem=&machine->memory[machine->memory_count];
    memset(mem,0,sizeof(*mem));
    mem->start=start;
    mem->length=length;
    mem->data=data;
    mem->flags.readonly=readonly;
    mem->flags.mmap_backed=mmap_backed;
    mem->usr=usr;
    mem->read=read;
    mem->write=write;
    snprintf(mem->tags,sizeof(mem->tags),"%s,%s,",readonly?"ro":"rw",name);
    machine->memory_count++;
    return true;
}

bool hs_moxie_moxiebox_memory_read(hs_moxie_moxiebox_t *machine,uint32_t address,void *data,size_t len)
{
    size_t offset=0;
    if(data==NULL)
    {
        return false;
    }
    hs_moxie_moxiebox_memory_t *mem=hs_moxie_moxiebox_memory_find(machine,address,len,false,&offset);
    if(mem==NULL)
    {
        return false;
    }
    if(mem->read!=NULL)
    {
        return mem->read(machine,mem,mem->usr,address,data,len)==len;
    }
    if(mem->data!=NULL)
    {
        memcpy(data,mem->data+offset,len);
        return true;
    }
    return false;
}

bool hs_moxie_moxiebox_memory_write(hs_moxie_moxiebox_t *machine,uint32_t address,const void *data,size_t len)
{
    size_t offset=0;
    if(data==NULL)
    {
        return false;
    }
    hs_moxie_moxiebox_memory_t *mem=hs_moxie_moxiebox_memory_find(machine,address,len,true,&offset);
    if(mem==NULL)
    {
        return false;
    }
    if(mem->write!=NULL)
    {
        return mem->write(machine,mem,mem->usr,address,data,len)==len;
    }
    if(mem->data!=NULL && !mem->flags.readonly)
    {
        memcpy(mem->data+offset,data,len);
        return true;
    }
    return false;
}

/*
 * 查找包含指定地址(不要求整个范围连续)的内存区域。
 */
static hs_moxie_moxiebox_memory_t *hs_moxie_moxiebox_memory_find_address(hs_moxie_moxiebox_t *machine,uint32_t address,bool want_write)
{
    if(machine==NULL)
    {
        return NULL;
    }
    for(size_t i=0;i<machine->memory_count;i++)
    {
        hs_moxie_moxiebox_memory_t *mem=&machine->memory[i];
        if(address>=mem->start && (uint64_t)address<((uint64_t)mem->start+mem->length))
        {
            if(want_write)
            {
                if(mem->flags.readonly)
                {
                    return NULL;
                }
                if(mem->data==NULL && mem->write==NULL)
                {
                    return NULL;
                }
            }
            else
            {
                if(mem->data==NULL && mem->read==NULL)
                {
                    return NULL;
                }
            }
            return mem;
        }
    }
    return NULL;
}

/*
 * 检查一段地址是否被一个或多个连续的内存区域完整覆盖。
 */
static bool hs_moxie_moxiebox_memory_check_range(hs_moxie_moxiebox_t *machine,uint32_t address,size_t len,bool want_write)
{
    while(len>0)
    {
        hs_moxie_moxiebox_memory_t *mem=hs_moxie_moxiebox_memory_find_address(machine,address,want_write);
        if(mem==NULL)
        {
            return false;
        }
        uint64_t end=(uint64_t)mem->start+mem->length;
        size_t step=(size_t)(end-address);
        if(step>len)
        {
            step=len;
        }
        if(step==0)
        {
            return false;
        }
        address+=step;
        len-=step;
    }
    return true;
}

/*
 * 跨内存区域写入数据。
 */
static bool hs_moxie_moxiebox_memory_write_range(hs_moxie_moxiebox_t *machine,uint32_t address,const void *data,size_t len)
{
    const uint8_t *bytes=(const uint8_t *)data;
    while(len>0)
    {
        hs_moxie_moxiebox_memory_t *mem=hs_moxie_moxiebox_memory_find_address(machine,address,true);
        if(mem==NULL)
        {
            return false;
        }
        uint64_t end=(uint64_t)mem->start+mem->length;
        size_t step=(size_t)(end-address);
        if(step>len)
        {
            step=len;
        }
        if(step==0 || !hs_moxie_moxiebox_memory_write(machine,address,bytes,step))
        {
            return false;
        }
        address+=step;
        bytes+=step;
        len-=step;
    }
    return true;
}

/*
 * 跨内存区域清零。
 */
static bool hs_moxie_moxiebox_memory_zero_range(hs_moxie_moxiebox_t *machine,uint32_t address,size_t len)
{
    uint8_t zero[HS_MOXIE_MOXIEBOX_IO_BUFFER_SIZE];
    memset(zero,0,sizeof(zero));
    while(len>0)
    {
        size_t step=len;
        if(step>sizeof(zero))
        {
            step=sizeof(zero);
        }
        if(!hs_moxie_moxiebox_memory_write_range(machine,address,zero,step))
        {
            return false;
        }
        address+=step;
        len-=step;
    }
    return true;
}

void *hs_moxie_moxiebox_memory_to_host(hs_moxie_moxiebox_t *machine,uint32_t address,size_t len,bool want_write)
{
    size_t offset=0;
    hs_moxie_moxiebox_memory_t *mem=hs_moxie_moxiebox_memory_find(machine,address,len,want_write,&offset);
    if(mem==NULL || mem->data==NULL)
    {
        return NULL;
    }
    return mem->data+offset;
}

uint32_t hs_moxie_moxiebox_register_read(hs_moxie_moxiebox_t *machine,uint32_t index)
{
    if(machine==NULL || index>=16)
    {
        return 0;
    }
    return machine->regs[index];
}

void hs_moxie_moxiebox_register_write(hs_moxie_moxiebox_t *machine,uint32_t index,uint32_t value)
{
    if(machine!=NULL && index<16)
    {
        machine->regs[index]=value;
    }
}

/*
 * 将内核定时器值同步到特殊寄存器，保持机器层可见状态一致。
 */
static void hs_moxie_moxiebox_timer_sync_sregs(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    hs_moxie_core_t *core=hs_moxie_moxiebox_core(machine);
    uint64_t time=hs_moxie_core_timer_time_get(core);
    uint64_t timecmp=hs_moxie_core_timer_timecmp_get(core);
    machine->sregs[HS_MOXIE_CORE_SREG_TIME_LO]=(uint32_t)time;
    machine->sregs[HS_MOXIE_CORE_SREG_TIME_HI]=(uint32_t)(time>>32);
    machine->sregs[HS_MOXIE_CORE_SREG_TIMECMP_LO]=(uint32_t)timecmp;
    machine->sregs[HS_MOXIE_CORE_SREG_TIMECMP_HI]=(uint32_t)(timecmp>>32);
}

static void hs_moxie_moxiebox_sreg_write_internal(hs_moxie_moxiebox_t *machine,uint32_t index,uint32_t value)
{
    if(machine==NULL || index>=256)
    {
        return;
    }

    hs_moxie_core_t *core=hs_moxie_moxiebox_core(machine);
    switch(index)
    {
    case HS_MOXIE_CORE_SREG_SATP:
    {
        hs_moxie_core_satp_set(core,value);
        machine->sregs[index]=value;
        return;
    }
    case HS_MOXIE_CORE_SREG_MSTATUS:
    {
        hs_moxie_core_mstatus_set(core,value);
        machine->sregs[index]=value;
        return;
    }
    case HS_MOXIE_CORE_SREG_MIP:
    {
        hs_moxie_core_mip_set(core,value);
        machine->sregs[index]=hs_moxie_core_mip_get(core);
        return;
    }
    case HS_MOXIE_CORE_SREG_CC:
    {
        hs_moxie_core_condition_code_set(core,value);
        machine->sregs[index]=value;
        return;
    }
    case HS_MOXIE_CORE_SREG_TIME_LO:
    {
        uint64_t time=hs_moxie_core_timer_time_get(core);
        hs_moxie_core_timer_time_set(core,(time&0xFFFFFFFF00000000ULL)|value);
        hs_moxie_moxiebox_timer_sync_sregs(machine);
        return;
    }
    case HS_MOXIE_CORE_SREG_TIME_HI:
    {
        uint64_t time=hs_moxie_core_timer_time_get(core);
        hs_moxie_core_timer_time_set(core,(time&0xFFFFFFFFULL)|((uint64_t)value<<32));
        hs_moxie_moxiebox_timer_sync_sregs(machine);
        return;
    }
    case HS_MOXIE_CORE_SREG_TIMECMP_LO:
    {
        uint64_t timecmp=hs_moxie_core_timer_timecmp_get(core);
        hs_moxie_core_timer_timecmp_set(core,(timecmp&0xFFFFFFFF00000000ULL)|value);
        hs_moxie_moxiebox_timer_sync_sregs(machine);
        return;
    }
    case HS_MOXIE_CORE_SREG_TIMECMP_HI:
    {
        uint64_t timecmp=hs_moxie_core_timer_timecmp_get(core);
        hs_moxie_core_timer_timecmp_set(core,(timecmp&0xFFFFFFFFULL)|((uint64_t)value<<32));
        hs_moxie_moxiebox_timer_sync_sregs(machine);
        return;
    }
    default:
    {
    }
    break;
    }

    machine->sregs[index]=value;
    if(index==6)
    {
        machine->flags.return_pending=1;
    }
    else if(index==7 && machine->flags.return_pending)
    {
        machine->flags.return_valid=1;
        machine->flags.return_pending=0;
    }
}

uint32_t hs_moxie_moxiebox_sreg_read(hs_moxie_moxiebox_t *machine,uint32_t index)
{
    if(machine==NULL || index>=256)
    {
        return 0;
    }
    hs_moxie_core_t *core=hs_moxie_moxiebox_core(machine);
    switch(index)
    {
    case HS_MOXIE_CORE_SREG_SATP:
    {
        return hs_moxie_core_satp_get(core);
    }
    case HS_MOXIE_CORE_SREG_MSTATUS:
    {
        return hs_moxie_core_mstatus_get(core);
    }
    case HS_MOXIE_CORE_SREG_MIP:
    {
        return hs_moxie_core_mip_get(core);
    }
    case HS_MOXIE_CORE_SREG_CC:
    {
        return hs_moxie_core_condition_code(core);
    }
    case HS_MOXIE_CORE_SREG_TIME_LO:
    {
        return (uint32_t)hs_moxie_core_timer_time_get(core);
    }
    case HS_MOXIE_CORE_SREG_TIME_HI:
    {
        return (uint32_t)(hs_moxie_core_timer_time_get(core)>>32);
    }
    case HS_MOXIE_CORE_SREG_TIMECMP_LO:
    {
        return (uint32_t)hs_moxie_core_timer_timecmp_get(core);
    }
    case HS_MOXIE_CORE_SREG_TIMECMP_HI:
    {
        return (uint32_t)(hs_moxie_core_timer_timecmp_get(core)>>32);
    }
    default:
    {
    }
    break;
    }
    return machine->sregs[index];
}

void hs_moxie_moxiebox_sreg_write(hs_moxie_moxiebox_t *machine,uint32_t index,uint32_t value)
{
    hs_moxie_moxiebox_sreg_write_internal(machine,index,value);
}

uint64_t hs_moxie_moxiebox_timer_time_get(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return 0;
    }
    return hs_moxie_core_timer_time_get(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_timer_time_set(hs_moxie_moxiebox_t *machine,uint64_t value)
{
    if(machine==NULL)
    {
        return false;
    }
    hs_moxie_core_t *core=hs_moxie_moxiebox_core(machine);
    if(core==NULL)
    {
        return false;
    }
    hs_moxie_core_timer_time_set(core,value);
    hs_moxie_moxiebox_timer_sync_sregs(machine);
    return true;
}

uint64_t hs_moxie_moxiebox_timer_timecmp_get(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return 0;
    }
    return hs_moxie_core_timer_timecmp_get(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_timer_timecmp_set(hs_moxie_moxiebox_t *machine,uint64_t value)
{
    if(machine==NULL)
    {
        return false;
    }
    hs_moxie_core_t *core=hs_moxie_moxiebox_core(machine);
    if(core==NULL)
    {
        return false;
    }
    hs_moxie_core_timer_timecmp_set(core,value);
    hs_moxie_moxiebox_timer_sync_sregs(machine);
    return true;
}

bool hs_moxie_moxiebox_software_interrupt_raise(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_software_interrupt_raise(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_software_interrupt_clear(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_software_interrupt_clear(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_software_interrupt_pending(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_software_interrupt_pending(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_external_interrupt_raise(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_external_interrupt_raise(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_external_interrupt_clear(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_external_interrupt_clear(hs_moxie_moxiebox_core(machine));
}

bool hs_moxie_moxiebox_external_interrupt_pending(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    return hs_moxie_core_external_interrupt_pending(hs_moxie_moxiebox_core(machine));
}

static int hs_moxie_moxiebox_mmap_prot(uint32_t prot)
{
    int ret=HMMAN_PROT_NONE;
    if((prot&HS_MOXIE_MOXIEBOX_PROT_READ)!=0)
    {
        ret|=HMMAN_PROT_READ;
    }
    if((prot&HS_MOXIE_MOXIEBOX_PROT_WRITE)!=0)
    {
        ret|=HMMAN_PROT_WRITE;
    }
    if((prot&HS_MOXIE_MOXIEBOX_PROT_EXEC)!=0)
    {
        ret|=HMMAN_PROT_EXEC;
    }
    return ret;
}

static int hs_moxie_moxiebox_mmap_flags(uint32_t flags)
{
    int ret=0;
    if((flags&HS_MOXIE_MOXIEBOX_MAP_SHARED)!=0)
    {
        ret|=HMMAN_MAP_SHARED;
    }
    if((flags&HS_MOXIE_MOXIEBOX_MAP_PRIVATE)!=0)
    {
        ret|=HMMAN_MAP_PRIVATE;
    }
    if((flags&HS_MOXIE_MOXIEBOX_MAP_ANONYMOUS)!=0)
    {
        ret|=HMMAN_MAP_ANONYMOUS;
    }
    return ret;
}

static bool hs_moxie_moxiebox_sys_mmap(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }

    uint32_t addr=machine->regs[2];
    uint32_t length=machine->regs[3];
    uint32_t prot=machine->regs[4];
    uint32_t flags=machine->regs[5];
    uint64_t offset=(uint64_t)machine->regs[7];
    int fildes=-1;

    if(addr!=0 ||
       length<HS_MOXIE_MOXIEBOX_PAGE_SIZE ||
       (length&(HS_MOXIE_MOXIEBOX_PAGE_SIZE-1))!=0 ||
       prot==0 ||
       (flags&(HS_MOXIE_MOXIEBOX_MAP_SHARED|HS_MOXIE_MOXIEBOX_MAP_PRIVATE))==0)
    {
        machine->regs[2]=(uint32_t)(-EINVAL);
        return true;
    }

    if((flags&HS_MOXIE_MOXIEBOX_MAP_ANONYMOUS)==0)
    {
        /*
         * 文件映射需要将guest fd翻译为host fd。
         */
        fildes=hs_moxie_moxiebox_fd_host(machine,machine->regs[6]);
        if(fildes<0)
        {
            machine->regs[2]=(uint32_t)(-EBADF);
            return true;
        }
        if((offset&(HS_MOXIE_MOXIEBOX_PAGE_SIZE-1))!=0)
        {
            machine->regs[2]=(uint32_t)(-EINVAL);
            return true;
        }
    }

    void *host=hmmap(NULL,length,
                     hs_moxie_moxiebox_mmap_prot(prot),
                     hs_moxie_moxiebox_mmap_flags(flags),
                     fildes,offset);
    if(host==HMMAN_MAP_FAILED)
    {
        machine->regs[2]=(uint32_t)(-ENOMEM);
        return true;
    }

    uint32_t guest=(machine->heap_next+(uint32_t)HS_MOXIE_MOXIEBOX_PAGE_SIZE-1)&(~((uint32_t)HS_MOXIE_MOXIEBOX_PAGE_SIZE-1));
    if((uint64_t)guest+length > 0x100000000ULL)
    {
        machine->regs[2]=(uint32_t)(-ENOMEM);
        return true;
    }

    char name[16];
    snprintf(name,sizeof(name),"heap%u",(unsigned int)machine->heap_count);
    if(!hs_moxie_moxiebox_memory_add(machine,guest,length,(uint8_t *)host,false,true,NULL,NULL,NULL,name))
    {
        machine->regs[2]=(uint32_t)(-ENOMEM);
        return true;
    }
    machine->heap_count++;
    machine->heap_next=guest+length;
    machine->regs[2]=guest;
    return true;
}

static bool hs_moxie_moxiebox_syscall_error(hs_moxie_moxiebox_t *machine,int err)
{
    machine->regs[2]=(uint32_t)(-(int)err);
    return true;
}

static int hs_moxie_moxiebox_copy_guest_string(hs_moxie_moxiebox_t *machine,uint32_t address,char *buffer,size_t buffer_size)
{
    if(buffer==NULL || buffer_size==0)
    {
        return -EINVAL;
    }
    for(size_t i=0;i+1<buffer_size;i++)
    {
        uint8_t ch=0;
        if(!hs_moxie_moxiebox_memory_read(machine,address+(uint32_t)i,&ch,1))
        {
            return -EFAULT;
        }
        buffer[i]=(char)ch;
        if(ch==0)
        {
            return 0;
        }
    }
    buffer[buffer_size-1]=0;
    return -ENAMETOOLONG;
}

static const char *hs_moxie_moxiebox_guest_string(hs_moxie_moxiebox_t *machine,uint32_t address,char *fallback,size_t fallback_size,int *err)
{
    if(err!=NULL)
    {
        *err=0;
    }
    /*
     * 优先直接翻译到主机地址。
     */
    for(size_t i=0;i<machine->memory_count;i++)
    {
        hs_moxie_moxiebox_memory_t *mem=&machine->memory[i];
        if(address>=mem->start && (uint64_t)address<((uint64_t)mem->start+mem->length) && mem->data!=NULL)
        {
            size_t remaining=(size_t)(((uint64_t)mem->start+mem->length)-address);
            char *host=(char *)(mem->data+(address-mem->start));
            if(memchr(host,0,remaining)!=NULL)
            {
                return host;
            }
            break;
        }
    }
    /*
     * 回调方式内存或跨区域字符串使用复制方式。
     */
    int ret=hs_moxie_moxiebox_copy_guest_string(machine,address,fallback,fallback_size);
    if(ret<0)
    {
        if(err!=NULL)
        {
            *err=ret;
        }
        return NULL;
    }
    return fallback;
}

static int hs_moxie_moxiebox_open_flags(uint32_t flags)
{
    int ret=O_RDONLY;
    switch(flags&HS_MOXIE_MOXIEBOX_OPEN_ACCMODE)
    {
    case HS_MOXIE_MOXIEBOX_OPEN_WRONLY:
        ret=O_WRONLY;
        break;
    case HS_MOXIE_MOXIEBOX_OPEN_RDWR:
        ret=O_RDWR;
        break;
    default:
        ret=O_RDONLY;
        break;
    }
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_CREAT)!=0) ret|=O_CREAT;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_EXCL)!=0) ret|=O_EXCL;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_NOCTTY)!=0) ret|=O_NOCTTY;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_TRUNC)!=0) ret|=O_TRUNC;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_APPEND)!=0) ret|=O_APPEND;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_NONBLOCK)!=0) ret|=O_NONBLOCK;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_DSYNC)!=0) ret|=O_DSYNC;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_DIRECT)!=0) ret|=O_DIRECT;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_LARGEFILE)!=0) ret|=O_LARGEFILE;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_DIRECTORY)!=0) ret|=O_DIRECTORY;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_NOFOLLOW)!=0) ret|=O_NOFOLLOW;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_NOATIME)!=0) ret|=O_NOATIME;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_CLOEXEC)!=0) ret|=O_CLOEXEC;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_SYNC)!=0) ret|=O_SYNC;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_PATH)!=0) ret|=O_PATH;
    if((flags&HS_MOXIE_MOXIEBOX_OPEN_TMPFILE)!=0) ret|=O_TMPFILE;
    return ret;
}

static int hs_moxie_moxiebox_lseek_whence(uint32_t whence)
{
    switch(whence)
    {
    case HS_MOXIE_MOXIEBOX_SEEK_CUR:
        return SEEK_CUR;
    case HS_MOXIE_MOXIEBOX_SEEK_END:
        return SEEK_END;
    default:
        return SEEK_SET;
    }
}

static unsigned int hs_moxie_moxiebox_getrandom_flags(uint32_t flags)
{
    unsigned int ret=0;
    if((flags&HS_MOXIE_MOXIEBOX_GETRANDOM_GRND_NONBLOCK)!=0) ret|=HGETRANDOM_GRND_NONBLOCK;
    if((flags&HS_MOXIE_MOXIEBOX_GETRANDOM_GRND_RANDOM)!=0) ret|=HGETRANDOM_GRND_RANDOM;
    if((flags&HS_MOXIE_MOXIEBOX_GETRANDOM_GRND_INSECURE)!=0) ret|=HGETRANDOM_GRND_INSECURE;
    return ret;
}

static bool hs_moxie_moxiebox_sys_read(hs_moxie_moxiebox_t *machine)
{
    uint32_t guest_fd=machine->regs[2];
    uint32_t address=machine->regs[3];
    uint32_t count=machine->regs[4];
    int fd=hs_moxie_moxiebox_fd_host(machine,guest_fd);
    if(fd<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,EBADF);
    }
    void *host_buffer=hs_moxie_moxiebox_memory_to_host(machine,address,count,true);
    if(host_buffer!=NULL)
    {
        hread_ssize_t n=hread(fd,host_buffer,count);
        if(n<0)
        {
            return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
        }
        machine->regs[2]=(uint32_t)n;
        return true;
    }
    uint8_t buffer[HS_MOXIE_MOXIEBOX_IO_BUFFER_SIZE];
    uint32_t done=0;
    while(done<count)
    {
        size_t chunk=count-done;
        if(chunk>sizeof(buffer))
        {
            chunk=sizeof(buffer);
        }
        hread_ssize_t n=hread(fd,buffer,chunk);
        if(n<0)
        {
            if(done==0)
            {
                return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
            }
            break;
        }
        if(n==0)
        {
            break;
        }
        size_t write_len=(size_t)n;
        if(!hs_moxie_moxiebox_memory_write(machine,address+done,buffer,write_len))
        {
            return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
        }
        done+=(uint32_t)write_len;
        if(write_len<chunk)
        {
            break;
        }
    }
    machine->regs[2]=done;
    return true;
}

static bool hs_moxie_moxiebox_sys_write(hs_moxie_moxiebox_t *machine)
{
    uint32_t guest_fd=machine->regs[2];
    uint32_t address=machine->regs[3];
    uint32_t count=machine->regs[4];
    int fd=hs_moxie_moxiebox_fd_host(machine,guest_fd);
    if(fd<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,EBADF);
    }
    void *host_buffer=hs_moxie_moxiebox_memory_to_host(machine,address,count,false);
    if(host_buffer!=NULL)
    {
        hwrite_ssize_t n=hwrite(fd,host_buffer,count);
        if(n<0)
        {
            return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
        }
        machine->regs[2]=(uint32_t)n;
        return true;
    }
    uint8_t buffer[HS_MOXIE_MOXIEBOX_IO_BUFFER_SIZE];
    uint32_t done=0;
    while(done<count)
    {
        size_t chunk=count-done;
        if(chunk>sizeof(buffer))
        {
            chunk=sizeof(buffer);
        }
        if(!hs_moxie_moxiebox_memory_read(machine,address+done,buffer,chunk))
        {
            return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
        }
        hwrite_ssize_t n=hwrite(fd,buffer,chunk);
        if(n<0)
        {
            if(done==0)
            {
                return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
            }
            break;
        }
        if(n==0)
        {
            break;
        }
        done+=(uint32_t)n;
        if((size_t)n<chunk)
        {
            break;
        }
    }
    machine->regs[2]=done;
    return true;
}

static bool hs_moxie_moxiebox_sys_open(hs_moxie_moxiebox_t *machine)
{
    char path_buffer[HS_MOXIE_MOXIEBOX_PATH_MAX];
    int err=0;
    const char *path=hs_moxie_moxiebox_guest_string(machine,machine->regs[2],path_buffer,sizeof(path_buffer),&err);
    if(path==NULL)
    {
        return hs_moxie_moxiebox_syscall_error(machine,err?(-err):EFAULT);
    }
    int host_fd=hopen(path,hs_moxie_moxiebox_open_flags(machine->regs[3]),(unsigned int)machine->regs[4]);
    if(host_fd<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    int guest_fd=hs_moxie_moxiebox_fd_alloc(machine,host_fd);
    if(guest_fd<0)
    {
        hclose(host_fd);
        return hs_moxie_moxiebox_syscall_error(machine,EMFILE);
    }
    machine->regs[2]=(uint32_t)guest_fd;
    return true;
}

static bool hs_moxie_moxiebox_sys_close(hs_moxie_moxiebox_t *machine)
{
    int ret=hs_moxie_moxiebox_fd_close(machine,machine->regs[2]);
    if(ret<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,-ret);
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_lseek(hs_moxie_moxiebox_t *machine)
{
    int fd=hs_moxie_moxiebox_fd_host(machine,machine->regs[2]);
    if(fd<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,EBADF);
    }
    hlseek_off_t ret=hlseek(fd,(int32_t)machine->regs[3],hs_moxie_moxiebox_lseek_whence(machine->regs[4]));
    if(ret<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    machine->regs[2]=(uint32_t)ret;
    return true;
}

static bool hs_moxie_moxiebox_sys_fsync(hs_moxie_moxiebox_t *machine)
{
    int fd=hs_moxie_moxiebox_fd_host(machine,machine->regs[2]);
    if(fd<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,EBADF);
    }
    if(hfsync(fd)<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_mkdir(hs_moxie_moxiebox_t *machine)
{
    char path_buffer[HS_MOXIE_MOXIEBOX_PATH_MAX];
    int err=0;
    const char *path=hs_moxie_moxiebox_guest_string(machine,machine->regs[2],path_buffer,sizeof(path_buffer),&err);
    if(path==NULL)
    {
        return hs_moxie_moxiebox_syscall_error(machine,err?(-err):EFAULT);
    }
    if(hmkdir(path,(unsigned long)machine->regs[3])<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_gettimeofday(hs_moxie_moxiebox_t *machine)
{
    hgettimeofday_timeval_t tv;
    if(hgettimeofday(&tv,NULL)<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    uint32_t address=machine->regs[2];
    void *host_buffer=hs_moxie_moxiebox_memory_to_host(machine,address,8,true);
    if(host_buffer!=NULL)
    {
        hs_moxie_moxiebox_write_u32((uint8_t *)host_buffer+0,(uint32_t)tv.tv_sec);
        hs_moxie_moxiebox_write_u32((uint8_t *)host_buffer+4,(uint32_t)tv.tv_usec);
    }
    else
    {
        uint8_t buffer[8];
        hs_moxie_moxiebox_write_u32(buffer+0,(uint32_t)tv.tv_sec);
        hs_moxie_moxiebox_write_u32(buffer+4,(uint32_t)tv.tv_usec);
        if(!hs_moxie_moxiebox_memory_write(machine,address,buffer,sizeof(buffer)))
        {
            return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
        }
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_settimeofday(hs_moxie_moxiebox_t *machine)
{
    hsettimeofday_timeval_t tv;
    hsettimeofday_timezone_t tz;
    const hsettimeofday_timeval_t *tvp=NULL;
    const hsettimeofday_timezone_t *tzp=NULL;
    uint8_t buffer[8];

    if(machine->regs[2]!=0)
    {
        const uint8_t *src=(const uint8_t *)hs_moxie_moxiebox_memory_to_host(machine,machine->regs[2],8,false);
        if(src==NULL)
        {
            if(!hs_moxie_moxiebox_memory_read(machine,machine->regs[2],buffer,sizeof(buffer)))
            {
                return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
            }
            src=buffer;
        }
        tv.tv_sec=hs_moxie_moxiebox_read_u32(src+0);
        tv.tv_usec=hs_moxie_moxiebox_read_u32(src+4);
        tvp=&tv;
    }

    if(machine->regs[3]!=0)
    {
        const uint8_t *src=(const uint8_t *)hs_moxie_moxiebox_memory_to_host(machine,machine->regs[3],8,false);
        if(src==NULL)
        {
            if(!hs_moxie_moxiebox_memory_read(machine,machine->regs[3],buffer,sizeof(buffer)))
            {
                return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
            }
            src=buffer;
        }
        tz.tz_minuteswest=(int32_t)hs_moxie_moxiebox_read_u32(src+0);
        tz.tz_dsttime=(int32_t)hs_moxie_moxiebox_read_u32(src+4);
        tzp=&tz;
    }

    if(hsettimeofday(tvp,tzp)<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_getrandom(hs_moxie_moxiebox_t *machine)
{
    uint32_t address=machine->regs[2];
    uint32_t length=machine->regs[3];
    unsigned int flags=hs_moxie_moxiebox_getrandom_flags(machine->regs[4]);
    void *host_buffer=hs_moxie_moxiebox_memory_to_host(machine,address,length,true);
    if(host_buffer!=NULL)
    {
        hgetrandom_ssize_t n=hgetrandom(host_buffer,length,flags);
        if(n<0)
        {
            return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
        }
        machine->regs[2]=(uint32_t)n;
        return true;
    }
    uint8_t buffer[HS_MOXIE_MOXIEBOX_IO_BUFFER_SIZE];
    uint32_t done=0;
    while(done<length)
    {
        size_t chunk=length-done;
        if(chunk>sizeof(buffer))
        {
            chunk=sizeof(buffer);
        }
        hgetrandom_ssize_t n=hgetrandom(buffer,chunk,flags);
        if(n<0)
        {
            if(done==0)
            {
                return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
            }
            break;
        }
        if(n==0)
        {
            break;
        }
        size_t write_len=(size_t)n;
        if(!hs_moxie_moxiebox_memory_write(machine,address+done,buffer,write_len))
        {
            return hs_moxie_moxiebox_syscall_error(machine,EFAULT);
        }
        done+=(uint32_t)write_len;
        if(write_len<chunk)
        {
            break;
        }
    }
    machine->regs[2]=done;
    return true;
}

static int hs_moxie_moxiebox_msync_flags(uint32_t flags)
{
    int ret=0;
    if((flags&HS_MOXIE_MOXIEBOX_MS_ASYNC)!=0) ret|=HMMAN_MS_ASYNC;
    if((flags&HS_MOXIE_MOXIEBOX_MS_INVALIDATE)!=0) ret|=HMMAN_MS_INVALIDATE;
    if((flags&HS_MOXIE_MOXIEBOX_MS_SYNC)!=0) ret|=HMMAN_MS_SYNC;
    return ret;
}

static bool hs_moxie_moxiebox_sys_msync(hs_moxie_moxiebox_t *machine)
{
    uint32_t address=machine->regs[2];
    uint32_t length=machine->regs[3];
    void *host=hs_moxie_moxiebox_memory_to_host(machine,address,length,false);
    if(host==NULL)
    {
        return hs_moxie_moxiebox_syscall_error(machine,EINVAL);
    }
    if(hmsync(host,length,hs_moxie_moxiebox_msync_flags(machine->regs[4]))<0)
    {
        return hs_moxie_moxiebox_syscall_error(machine,errno?errno:EIO);
    }
    machine->regs[2]=0;
    return true;
}

static bool hs_moxie_moxiebox_sys_munmap(hs_moxie_moxiebox_t *machine)
{
    uint32_t address=machine->regs[2];
    uint32_t length=machine->regs[3];
    for(size_t i=0;i<machine->memory_count;i++)
    {
        hs_moxie_moxiebox_memory_t *mem=&machine->memory[i];
        if(mem->flags.mmap_backed && mem->start==address && mem->length==length)
        {
            hmunmap(mem->data,mem->length);
            for(size_t j=i;j+1<machine->memory_count;j++)
            {
                machine->memory[j]=machine->memory[j+1];
            }
            machine->memory_count--;
            if(machine->heap_count>0)
            {
                machine->heap_count--;
            }
            machine->regs[2]=0;
            return true;
        }
    }
    return hs_moxie_moxiebox_syscall_error(machine,EINVAL);
}

static bool hs_moxie_moxiebox_swi_handle(hs_moxie_moxiebox_t *machine,uint32_t number)
{
    bool handled=false;

    /*
     * 用户回调优先，可用于接管任意swi（包括mmap/exit）。
     */
    if(machine->swi!=NULL)
    {
        handled=machine->swi(machine,machine->swi_usr,number);
    }

    if(!handled)
    {
        switch(number)
        {
        case HS_MOXIE_MOXIEBOX_SYS_RESET:
        {
            machine->flags.reset_pending=1;
            handled=true;
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_EXIT:
        {
            machine->exit_code=(int)(int32_t)machine->regs[2];
            machine->flags.stopped=1;
            handled=true;
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_READ:
        {
            handled=hs_moxie_moxiebox_sys_read(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_WRITE:
        {
            handled=hs_moxie_moxiebox_sys_write(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_OPEN:
        {
            handled=hs_moxie_moxiebox_sys_open(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_CLOSE:
        {
            handled=hs_moxie_moxiebox_sys_close(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_LSEEK:
        {
            handled=hs_moxie_moxiebox_sys_lseek(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_MKDIR:
        {
            handled=hs_moxie_moxiebox_sys_mkdir(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_GETTIMEOFDAY:
        {
            handled=hs_moxie_moxiebox_sys_gettimeofday(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_SETTIMEOFDAY:
        {
            handled=hs_moxie_moxiebox_sys_settimeofday(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_MMAP:
        {
            handled=hs_moxie_moxiebox_sys_mmap(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_MUNMAP:
        {
            handled=hs_moxie_moxiebox_sys_munmap(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_MSYNC:
        {
            handled=hs_moxie_moxiebox_sys_msync(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_FSYNC:
        case HS_MOXIE_MOXIEBOX_SYS_FDATASYNC:
        {
            handled=hs_moxie_moxiebox_sys_fsync(machine);
        }
        break;
        case HS_MOXIE_MOXIEBOX_SYS_GETRANDOM:
        {
            handled=hs_moxie_moxiebox_sys_getrandom(machine);
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(handled)
    {
        /*
         * 已处理的swi不保留中断上下文。
         */
        machine->sregs[2]=0;
        machine->sregs[3]=0;
    }
    return handled;
}

static bool hs_moxie_moxiebox_io(hs_moxie_core_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len,void *usr)
{
    hs_moxie_moxiebox_t *machine=(hs_moxie_moxiebox_t *)usr;
    if(machine==NULL)
    {
        return false;
    }
    switch(opt&0x00FFFFFFUL)
    {
    case HS_MOXIE_CORE_IO_RESET:
    {
        memset(machine->regs,0,sizeof(machine->regs));
        memset(machine->sregs,0,sizeof(machine->sregs));
        machine->pc=0;
        machine->flags.return_pending=0;
        machine->flags.return_valid=0;
        if(machine->flags.prepared)
        {
            machine->sregs[HS_MOXIE_CORE_SREG_SIM_RETURN_ADDR]=machine->mapdesc_addr;
            machine->sregs[HS_MOXIE_CORE_SREG_SIM_RETURN_LEN]=machine->stack_top;
        }
    }
    break;
    case HS_MOXIE_CORE_IO_ENTRY_PC_READ:
    {
        hs_moxie_moxiebox_write_u32(data,machine->entry);
    }
    break;
    case HS_MOXIE_CORE_IO_INITIAL_SP_READ:
    {
        hs_moxie_moxiebox_write_u32(data,machine->stack_top);
    }
    break;
    case HS_MOXIE_CORE_IO_MEMORY_READ:
    {
        return hs_moxie_moxiebox_memory_read(machine,address,data,len);
    }
    case HS_MOXIE_CORE_IO_MEMORY_WRITE:
    {
        return hs_moxie_moxiebox_memory_write(machine,address,data,len);
    }
    case HS_MOXIE_CORE_IO_SREG_READ:
    {
        hs_moxie_moxiebox_write_u32(data,hs_moxie_moxiebox_sreg_read(machine,address));
    }
    break;
    case HS_MOXIE_CORE_IO_SREG_WRITE:
    {
        hs_moxie_moxiebox_sreg_write_internal(machine,address,hs_moxie_moxiebox_read_u32(data));
    }
    break;
    case HS_MOXIE_CORE_IO_REGISTER_READ:
    {
        hs_moxie_moxiebox_write_u32(data,hs_moxie_moxiebox_register_read(machine,address));
    }
    break;
    case HS_MOXIE_CORE_IO_REGISTER_WRITE:
    {
        hs_moxie_moxiebox_register_write(machine,address,hs_moxie_moxiebox_read_u32(data));
    }
    break;
    case HS_MOXIE_CORE_IO_PC_REGISTER_READ:
    {
        hs_moxie_moxiebox_write_u32(data,machine->pc);
    }
    break;
    case HS_MOXIE_CORE_IO_PC_REGISTER_WRITE:
    {
        machine->pc=hs_moxie_moxiebox_read_u32(data);
    }
    break;
    case HS_MOXIE_CORE_IO_SWI:
    {
        return hs_moxie_moxiebox_swi_handle(machine,address);
    }
    default:
    {
    }
    break;
    }
    (void)core;
    return true;
}

size_t hs_moxie_moxiebox_size(void)
{
    return sizeof(hs_moxie_moxiebox_t);
}

hs_moxie_moxiebox_t *hs_moxie_moxiebox_init(void *mem)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_moxie_moxiebox_t *machine=(hs_moxie_moxiebox_t *)mem;
    memset(machine,0,sizeof(hs_moxie_moxiebox_t));
    hs_moxie_moxiebox_fd_table_reset(machine);
    if(hs_moxie_core_init(machine->core.bytes,hs_moxie_moxiebox_io,machine)==NULL)
    {
        return NULL;
    }
    return machine;
}

void hs_moxie_moxiebox_deinit(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    hs_moxie_moxiebox_fd_table_close_all(machine);
    for(size_t i=0;i<machine->memory_count;i++)
    {
        if(machine->memory[i].flags.mmap_backed)
        {
            hmunmap(machine->memory[i].data,machine->memory[i].length);
        }
    }
    machine->memory_count=0;
    machine->flags.prepared=0;
    machine->flags.started=0;
    machine->flags.stopped=0;
    machine->flags.halted=0;
}

bool hs_moxie_moxiebox_check(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return false;
    }
    if(machine->entry==0 || machine->memory_count==0)
    {
        return false;
    }
    return true;
}

void hs_moxie_moxiebox_set_entry(hs_moxie_moxiebox_t *machine,uint32_t entry)
{
    if(machine!=NULL && !machine->flags.prepared && !machine->flags.started)
    {
        machine->entry=entry;
    }
}

bool hs_moxie_moxiebox_add_memory(hs_moxie_moxiebox_t *machine,uint32_t address,void *data,size_t length,bool readonly,const char *name)
{
    if(machine==NULL || data==NULL || length==0 || length>UINT32_MAX)
    {
        return false;
    }
    if(machine->flags.prepared || machine->flags.started)
    {
        return false;
    }
    return hs_moxie_moxiebox_memory_add(machine,address,(uint32_t)length,(uint8_t *)data,readonly,false,NULL,NULL,NULL,(name!=NULL)?name:"mem");
}

bool hs_moxie_moxiebox_add_memory_callback(hs_moxie_moxiebox_t *machine,uint32_t address,size_t length,
        hs_moxie_moxiebox_memory_read_t read,
        hs_moxie_moxiebox_memory_write_t write,
        void *usr,const char *name)
{
    if(machine==NULL || length==0 || length>UINT32_MAX)
    {
        return false;
    }
    if(machine->flags.prepared || machine->flags.started)
    {
        return false;
    }
    return hs_moxie_moxiebox_memory_add(machine,address,(uint32_t)length,NULL,
                                        write==NULL,false,usr,read,write,
                                        (name!=NULL)?name:"mem");
}

bool hs_moxie_moxiebox_elf_memory_range_get(helf_file_input_t *input_file,uint32_t *base,size_t *length)
{
    if(input_file==NULL || base==NULL || length==NULL)
    {
        return false;
    }
    if(!helf_moxie_is_moxie(input_file) || !helf_moxie_is_executable(input_file))
    {
        return false;
    }
    size_t count=helf_moxie_load_segment_count_get(input_file);
    if(count==0)
    {
        return false;
    }
    uint64_t min_address=UINT64_MAX;
    uint64_t max_end=0;
    bool found=false;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_segment_info_t info;
        if(!helf_moxie_load_segment_info_get(input_file,i,&info))
        {
            return false;
        }
        if(info.filesz>info.memsz)
        {
            return false;
        }
        if(info.memsz==0)
        {
            continue;
        }
        uint64_t start=info.vaddr;
        uint64_t end=start+info.memsz;
        if(end>0x100000000ULL)
        {
            return false;
        }
        if(start<min_address)
        {
            min_address=start;
        }
        if(end>max_end)
        {
            max_end=end;
        }
        found=true;
    }
    if(!found || max_end<=min_address)
    {
        return false;
    }
    if((max_end-min_address)>SIZE_MAX)
    {
        return false;
    }
    *base=(uint32_t)min_address;
    *length=(size_t)(max_end-min_address);
    return true;
}

bool hs_moxie_moxiebox_load_elf(hs_moxie_moxiebox_t *machine,helf_file_input_t *input_file)
{
    if(machine==NULL || input_file==NULL)
    {
        return false;
    }
    if(machine->flags.prepared || machine->flags.started)
    {
        return false;
    }
    if(!helf_moxie_is_moxie(input_file) || !helf_moxie_is_executable(input_file))
    {
        return false;
    }
    uint32_t entry=0;
    if(!helf_moxie_entry_get(input_file,&entry))
    {
        return false;
    }
    size_t count=helf_moxie_load_segment_count_get(input_file);
    if(count==0)
    {
        return false;
    }

    /*
     * 先检查所有段均可写入，避免因内存区域缺失导致部分加载。
     */
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_segment_info_t info;
        if(!helf_moxie_load_segment_info_get(input_file,i,&info))
        {
            return false;
        }
        if(info.filesz>info.memsz)
        {
            return false;
        }
        uint64_t end=(uint64_t)info.vaddr+info.memsz;
        uint64_t file_end=(uint64_t)info.offset+info.filesz;
        if(end>0x100000000ULL || file_end>(uint64_t)UINTPTR_MAX)
        {
            return false;
        }
        if(info.filesz>0 && !hs_moxie_moxiebox_memory_check_range(machine,info.vaddr,info.filesz,true))
        {
            return false;
        }
        if(info.memsz>info.filesz &&
           !hs_moxie_moxiebox_memory_check_range(machine,info.vaddr+info.filesz,info.memsz-info.filesz,true))
        {
            return false;
        }
    }

    uint8_t buffer[HS_MOXIE_MOXIEBOX_IO_BUFFER_SIZE];
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_segment_info_t info;
        if(!helf_moxie_load_segment_info_get(input_file,i,&info))
        {
            return false;
        }
        uint32_t address=info.vaddr;
        uintptr_t offset=(uintptr_t)info.offset;
        size_t remaining=info.filesz;
        while(remaining>0)
        {
            size_t step=remaining;
            if(step>sizeof(buffer))
            {
                step=sizeof(buffer);
            }
            if(helf_file_input_read(input_file,offset,buffer,step)!=step)
            {
                return false;
            }
            if(!hs_moxie_moxiebox_memory_write_range(machine,address,buffer,step))
            {
                return false;
            }
            address+=step;
            offset+=step;
            remaining-=step;
        }
        if(info.memsz>info.filesz)
        {
            if(!hs_moxie_moxiebox_memory_zero_range(machine,info.vaddr+info.filesz,info.memsz-info.filesz))
            {
                return false;
            }
        }
    }
    hs_moxie_moxiebox_set_entry(machine,entry);
    return true;
}

bool hs_moxie_moxiebox_load_elf_with_buffer(hs_moxie_moxiebox_t *machine,helf_file_input_t *input_file,void *buffer,size_t length)
{
    if(machine==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    if(machine->flags.prepared || machine->flags.started)
    {
        return false;
    }
    uint32_t base=0;
    size_t span=0;
    if(!hs_moxie_moxiebox_elf_memory_range_get(input_file,&base,&span))
    {
        return false;
    }
    if(length<span || (uint64_t)base+span>0x100000000ULL)
    {
        return false;
    }
    size_t count=helf_moxie_load_segment_count_get(input_file);
    if(count==0)
    {
        return false;
    }
    if(machine->memory_count+count+2>HS_MOXIE_MOXIEBOX_MAX_MEMORY)
    {
        return false;
    }

    /*
     * 先以可写方式添加内存区域，加载完成后再恢复只读属性。
     */
    size_t old_count=machine->memory_count;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_segment_info_t info;
        if(!helf_moxie_load_segment_info_get(input_file,i,&info))
        {
            machine->memory_count=old_count;
            return false;
        }
        if(info.memsz==0)
        {
            continue;
        }
        uint8_t *segment=(uint8_t *)buffer+(size_t)(info.vaddr-base);
        if(!hs_moxie_moxiebox_add_memory(machine,info.vaddr,segment,info.memsz,false,"elf"))
        {
            machine->memory_count=old_count;
            return false;
        }
    }
    if(!hs_moxie_moxiebox_load_elf(machine,input_file))
    {
        machine->memory_count=old_count;
        return false;
    }

    /*
     * 按PT_LOAD段标志恢复只读属性。
     */
    size_t region=old_count;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_segment_info_t info;
        if(!helf_moxie_load_segment_info_get(input_file,i,&info))
        {
            break;
        }
        if(info.memsz==0)
        {
            continue;
        }
        if(region>=machine->memory_count)
        {
            break;
        }
        bool readonly=((info.flags&HELF_PF_W)==0);
        machine->memory[region].flags.readonly=readonly;
        snprintf(machine->memory[region].tags,sizeof(machine->memory[region].tags),"%s,%s,",readonly?"ro":"rw","elf");
        region++;
    }
    return true;
}

/*
 * 内存中的ELF映像读取上下文。
 */
typedef struct
{
    const uint8_t *data;
    size_t length;
} hs_moxie_moxiebox_elf_memory_input_t;

static size_t hs_moxie_moxiebox_elf_memory_input_read(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)
{
    if(input==NULL || buffer==NULL)
    {
        return 0;
    }
    hs_moxie_moxiebox_elf_memory_input_t *ctx=(hs_moxie_moxiebox_elf_memory_input_t *)input->usr;
    if(ctx==NULL || ctx->data==NULL || address>=ctx->length)
    {
        return 0;
    }
    size_t available=ctx->length-(size_t)address;
    if(buffer_length>available)
    {
        buffer_length=available;
    }
    memcpy(buffer,ctx->data+address,buffer_length);
    return buffer_length;
}

bool hs_moxie_moxiebox_load_elf_memory(hs_moxie_moxiebox_t *machine,const void *data,size_t length)
{
    if(machine==NULL || data==NULL || length==0)
    {
        return false;
    }
    hs_moxie_moxiebox_elf_memory_input_t ctx;
    ctx.data=(const uint8_t *)data;
    ctx.length=length;
    helf_file_input_t input;
    helf_file_input_init(&input,hs_moxie_moxiebox_elf_memory_input_read,&ctx);
    return hs_moxie_moxiebox_load_elf(machine,&input);
}

size_t hs_moxie_moxiebox_prepare_size(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL || machine->flags.prepared || machine->entry==0 || machine->memory_count==0)
    {
        return 0;
    }
    /*
     * 需要新增stack与mapdesc两个区域
     */
    if(machine->memory_count+2>HS_MOXIE_MOXIEBOX_MAX_MEMORY)
    {
        return 0;
    }
    size_t desc_count=machine->memory_count+3;
    size_t desc_size=desc_count*HS_MOXIE_MOXIEBOX_DESC_SIZE;
    if(desc_size>SIZE_MAX-HS_MOXIE_MOXIEBOX_STACK_SIZE)
    {
        return 0;
    }
    return HS_MOXIE_MOXIEBOX_STACK_SIZE+desc_size;
}

bool hs_moxie_moxiebox_prepare(hs_moxie_moxiebox_t *machine,void *mem,size_t len)
{
    if(machine==NULL)
    {
        return false;
    }
    if(machine->flags.prepared)
    {
        return true;
    }
    size_t need=hs_moxie_moxiebox_prepare_size(machine);
    if(mem==NULL || need==0 || len<need)
    {
        return false;
    }

    uint8_t *stack=(uint8_t *)mem;
    uint64_t stack_start64=hs_moxie_moxiebox_max_end(machine)+HS_MOXIE_MOXIEBOX_PAGE_SIZE;
    if(stack_start64>=0x100000000ULL || stack_start64+HS_MOXIE_MOXIEBOX_STACK_SIZE>0x100000000ULL)
    {
        return false;
    }
    memset(stack,0,HS_MOXIE_MOXIEBOX_STACK_SIZE);
    if(!hs_moxie_moxiebox_memory_add(machine,(uint32_t)stack_start64,(uint32_t)HS_MOXIE_MOXIEBOX_STACK_SIZE,stack,false,false,NULL,NULL,NULL,"stack"))
    {
        return false;
    }
    machine->stack_top=(uint32_t)(stack_start64+HS_MOXIE_MOXIEBOX_STACK_SIZE);

    /*
     * 内存描述表:已有区域 + 自身 + 终止项
     */
    size_t old_count=machine->memory_count;
    size_t desc_count=old_count+2;
    uint32_t desc_size=(uint32_t)(desc_count*HS_MOXIE_MOXIEBOX_DESC_SIZE);
    uint64_t mapdesc_start64=hs_moxie_moxiebox_max_end(machine)+HS_MOXIE_MOXIEBOX_PAGE_SIZE;
    if(mapdesc_start64>=0x100000000ULL || mapdesc_start64+desc_size>0x100000000ULL)
    {
        return false;
    }
    uint8_t *desc=stack+HS_MOXIE_MOXIEBOX_STACK_SIZE;
    memset(desc,0,desc_size);

    for(size_t i=0;i<old_count;i++)
    {
        uint8_t *entry=desc+i*HS_MOXIE_MOXIEBOX_DESC_SIZE;
        hs_moxie_moxiebox_write_u32(entry+0,machine->memory[i].start);
        hs_moxie_moxiebox_write_u32(entry+4,machine->memory[i].length);
        strncpy((char *)entry+8,machine->memory[i].tags,HS_MOXIE_MOXIEBOX_TAG_SIZE-1);
    }
    {
        uint8_t *entry=desc+old_count*HS_MOXIE_MOXIEBOX_DESC_SIZE;
        hs_moxie_moxiebox_write_u32(entry+0,(uint32_t)mapdesc_start64);
        hs_moxie_moxiebox_write_u32(entry+4,desc_size);
        strncpy((char *)entry+8,"ro,mapdesc,",HS_MOXIE_MOXIEBOX_TAG_SIZE-1);
    }
    if(!hs_moxie_moxiebox_memory_add(machine,(uint32_t)mapdesc_start64,desc_size,desc,true,false,NULL,NULL,NULL,"mapdesc"))
    {
        return false;
    }
    machine->mapdesc_addr=(uint32_t)mapdesc_start64;
    machine->heap_base=(uint32_t)(((mapdesc_start64+desc_size+HS_MOXIE_MOXIEBOX_PAGE_SIZE)+HS_MOXIE_MOXIEBOX_PAGE_SIZE-1)&(~(uint64_t)(HS_MOXIE_MOXIEBOX_PAGE_SIZE-1)));
    machine->heap_next=machine->heap_base;
    machine->flags.prepared=1;
    return true;
}

void hs_moxie_moxiebox_reset(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    /*
     * 复位时关闭虚拟机打开但尚未关闭的文件。
     */
    hs_moxie_moxiebox_fd_table_close_all(machine);
    hs_moxie_moxiebox_fd_table_reset(machine);
    if(!machine->flags.prepared)
    {
        return;
    }
    hs_moxie_moxiebox_mmap_cleanup(machine);
    machine->exit_code=0;
    machine->flags.stopped=0;
    machine->flags.halted=0;
    machine->flags.return_pending=0;
    machine->flags.return_valid=0;
    machine->flags.reset_pending=0;
    hs_moxie_core_reset(hs_moxie_moxiebox_core(machine));
    machine->sregs[HS_MOXIE_CORE_SREG_MSTATUS]=hs_moxie_core_mstatus_get(hs_moxie_moxiebox_core(machine));
    machine->sregs[HS_MOXIE_CORE_SREG_MIP]=hs_moxie_core_mip_get(hs_moxie_moxiebox_core(machine));
    hs_moxie_moxiebox_timer_sync_sregs(machine);
    machine->flags.started=1;
}

int hs_moxie_moxiebox_step(hs_moxie_moxiebox_t *machine,uint32_t elapsed_us,size_t count)
{
    if(machine==NULL)
    {
        return -1;
    }
    if(!machine->flags.prepared)
    {
        return -1;
    }
    if(!hs_moxie_moxiebox_check(machine))
    {
        return -1;
    }
    if(!machine->flags.started)
    {
        hs_moxie_moxiebox_reset(machine);
    }
    if(machine->flags.stopped)
    {
        return 1;
    }
    hs_moxie_core_timer_time_advance(hs_moxie_moxiebox_core(machine),elapsed_us);
    while(count--)
    {
        hs_moxie_core_tick(hs_moxie_moxiebox_core(machine),1);
        if(machine->flags.reset_pending)
        {
            machine->flags.reset_pending=0;
            hs_moxie_moxiebox_reset(machine);
            return 0;
        }
        if(machine->flags.stopped)
        {
            break;
        }
        if(hs_moxie_core_halted(hs_moxie_moxiebox_core(machine)))
        {
            machine->flags.halted=1;
            machine->flags.stopped=1;
            break;
        }
    }
    return machine->flags.stopped?1:0;
}

hs_moxie_core_t *hs_moxie_moxiebox_core(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return NULL;
    }
    return (hs_moxie_core_t *)machine->core.bytes;
}

bool hs_moxie_moxiebox_stopped(hs_moxie_moxiebox_t *machine)
{
    return (machine!=NULL) && (machine->flags.stopped!=0);
}

bool hs_moxie_moxiebox_halted(hs_moxie_moxiebox_t *machine)
{
    return (machine!=NULL) && (machine->flags.halted!=0);
}

int hs_moxie_moxiebox_exit_code(hs_moxie_moxiebox_t *machine)
{
    if(machine==NULL)
    {
        return -1;
    }
    return machine->exit_code;
}

bool hs_moxie_moxiebox_return_buffer(hs_moxie_moxiebox_t *machine,uint32_t *address,uint32_t *length)
{
    if(machine==NULL || machine->flags.return_valid==0)
    {
        return false;
    }
    if(address!=NULL)
    {
        *address=machine->sregs[HS_MOXIE_CORE_SREG_SIM_RETURN_ADDR];
    }
    if(length!=NULL)
    {
        *length=machine->sregs[HS_MOXIE_CORE_SREG_SIM_RETURN_LEN];
    }
    return true;
}

void hs_moxie_moxiebox_set_swi(hs_moxie_moxiebox_t *machine,hs_moxie_moxiebox_swi_t swi,void *usr)
{
    if(machine!=NULL)
    {
        machine->swi=swi;
        machine->swi_usr=usr;
    }
}
