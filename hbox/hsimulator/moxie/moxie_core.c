/***************************************************************
 * Name:      moxie_core.c
 * Purpose:   实现moxie内核接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "moxie_core.h"

/*
 * moxie条件码，与参考实现保持一致。
 */
#define HS_MOXIE_CORE_CC_GT        (1UL << 0)
#define HS_MOXIE_CORE_CC_LT        (1UL << 1)
#define HS_MOXIE_CORE_CC_EQ        (1UL << 2)
#define HS_MOXIE_CORE_CC_GTU       (1UL << 3)
#define HS_MOXIE_CORE_CC_LTU       (1UL << 4)

struct hs_moxie_core
{
    hs_moxie_core_io_t  io;                 /**< IO回调 */
    void*               usr;                /**< 用户指针 */
    uint32_t            cc;                 /**< 条件码 */
    uint32_t            exception_cause;    /**< 等待处理的异常原因 */
    uint32_t            exception_value;    /**< 等待处理的异常附带值 */
    uint32_t            interrupt_value;    /**< 等待处理的中断附带值 */
    uint32_t            fault_cause;        /**< 当前指令产生的异常原因 */
    uint32_t            fault_value;        /**< 当前指令产生的异常附带值 */
    uint32_t            mem_access;         /**< 当前内存访问类型 */
    uint32_t            satp;               /**< Sv32页表基址缓存 */
    uint32_t            mstatus;            /**< 全局中断状态缓存 */
    uint64_t            timer_time;         /**< 定时器计数(time) */
    uint64_t            timer_timecmp;      /**< 定时器比较值(timecmp) */
    struct
    {
        uint32_t        exception_pending:1;    /**< 存在等待处理的异常 */
        uint32_t        interrupt_pending:1;    /**< 存在等待处理的中断 */
        uint32_t        halted:1;               /**< 因无法进入异常处理程序而暂停 */
        uint32_t        halted_by_interrupt:1;  /**< 暂停由中断引起 */
        uint32_t        timer_pending:1;        /**< 定时器中断条件成立 */
        uint32_t        software_interrupt_pending:1; /**< 软件中断等待处理 */
        uint32_t        mpie_pending:1;         /**< 中断返回待恢复MIE(进入中断后有效) */
    }                   flags;
};

/*
 * 内存访问类型。
 */
#define HS_MOXIE_CORE_MEM_ACCESS_FETCH  0
#define HS_MOXIE_CORE_MEM_ACCESS_LOAD   1
#define HS_MOXIE_CORE_MEM_ACCESS_STORE  2

/*
 * Sv32 PTE位定义。
 */
#define HS_MOXIE_CORE_PTE_V             (1UL << 0)
#define HS_MOXIE_CORE_PTE_R             (1UL << 1)
#define HS_MOXIE_CORE_PTE_W             (1UL << 2)
#define HS_MOXIE_CORE_PTE_X             (1UL << 3)
#define HS_MOXIE_CORE_PTE_U             (1UL << 4)
#define HS_MOXIE_CORE_PTE_G             (1UL << 5)
#define HS_MOXIE_CORE_PTE_A             (1UL << 6)
#define HS_MOXIE_CORE_PTE_D             (1UL << 7)

/*
 * 大端序读写辅助函数。
 * 内存及IO中的多字节数据均按大端序存放。
 */
static inline uint16_t hs_moxie_core_read_u16(hs_moxie_core_t *core,const uint8_t *data)
{
    (void)core;
    return (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
}

static inline uint32_t hs_moxie_core_read_u32(hs_moxie_core_t *core,const uint8_t *data)
{
    (void)core;
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | (uint32_t)data[3];
}

static inline void hs_moxie_core_write_u16(hs_moxie_core_t *core,uint8_t *data,uint16_t value)
{
    (void)core;
    data[0]=(uint8_t)(value >> 8);
    data[1]=(uint8_t)(value >> 0);
}

static inline void hs_moxie_core_write_u32(hs_moxie_core_t *core,uint8_t *data,uint32_t value)
{
    (void)core;
    data[0]=(uint8_t)(value >> 24);
    data[1]=(uint8_t)(value >> 16);
    data[2]=(uint8_t)(value >> 8);
    data[3]=(uint8_t)(value >> 0);
}

static inline bool hs_moxie_core_io_call(hs_moxie_core_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    return core->io(core,opt|(uint32_t)HS_MOXIE_CORE_IO_OPERATOR_CORE,address,data,len,core->usr);
}

static inline uint32_t hs_moxie_core_register_read(hs_moxie_core_t *core,uint32_t index)
{
    uint8_t data[4];
    uint32_t value=0;
    if(index<16 && hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_REGISTER_READ,index,data,sizeof(data)))
    {
        value=hs_moxie_core_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_register_write(hs_moxie_core_t *core,uint32_t index,uint32_t value)
{
    uint8_t data[4];
    if(index<16)
    {
        hs_moxie_core_write_u32(core,data,value);
        hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_REGISTER_WRITE,index,data,sizeof(data));
    }
}

static inline uint32_t hs_moxie_core_sreg_read(hs_moxie_core_t *core,uint32_t index)
{
    uint8_t data[4];
    uint32_t value=0;
    if(core!=NULL && index==HS_MOXIE_CORE_SREG_SATP)
    {
        return core->satp;
    }
    if(core!=NULL && index==HS_MOXIE_CORE_SREG_MSTATUS)
    {
        return core->mstatus;
    }
    if(core!=NULL && index==HS_MOXIE_CORE_SREG_MIP)
    {
        return hs_moxie_core_mip_get(core);
    }
    if(index<256 && hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_SREG_READ,index,data,sizeof(data)))
    {
        value=hs_moxie_core_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_sreg_write(hs_moxie_core_t *core,uint32_t index,uint32_t value)
{
    uint8_t data[4];
    if(index<256)
    {
        if(core!=NULL && index==HS_MOXIE_CORE_SREG_SATP)
        {
            core->satp=value;
        }
        if(core!=NULL && index==HS_MOXIE_CORE_SREG_MSTATUS)
        {
            core->mstatus=value;
        }
        if(core!=NULL && index==HS_MOXIE_CORE_SREG_MIP)
        {
            hs_moxie_core_mip_set(core,value);
        }
        hs_moxie_core_write_u32(core,data,value);
        hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_SREG_WRITE,index,data,sizeof(data));
    }
}

static inline uint32_t hs_moxie_core_pc_read(hs_moxie_core_t *core)
{
    uint8_t data[4];
    uint32_t value=0;
    if(hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_PC_REGISTER_READ,0,data,sizeof(data)))
    {
        value=hs_moxie_core_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_pc_write(hs_moxie_core_t *core,uint32_t value)
{
    uint8_t data[4];
    hs_moxie_core_write_u32(core,data,value);
    hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_PC_REGISTER_WRITE,0,data,sizeof(data));
}

static inline void hs_moxie_core_fault_set(hs_moxie_core_t *core,uint32_t cause,uint32_t value)
{
    if(core!=NULL)
    {
        core->fault_cause=cause;
        core->fault_value=value;
    }
}

/*
 * 物理地址内存访问(不经过Sv32翻译)，用于页表遍历。
 */
static bool hs_moxie_core_physical_read(hs_moxie_core_t *core,uint32_t address,uint8_t *data,size_t len)
{
    return hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_MEMORY_READ,address,data,len);
}

static bool hs_moxie_core_physical_write(hs_moxie_core_t *core,uint32_t address,const uint8_t *data,size_t len)
{
    return hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_MEMORY_WRITE,address,(uint8_t *)data,len);
}

static bool hs_moxie_core_mmu_fault(hs_moxie_core_t *core,uint32_t va)
{
    hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_MMU_MISS,va);
    return false;
}

/*
 * Sv32地址翻译。
 * satp.MODE=0时按无MMU处理，保证未启用时行为与修改前一致。
 */
static bool hs_moxie_core_mmu_translate(hs_moxie_core_t *core,uint32_t va,uint32_t access,uint32_t *pa)
{
    if(pa==NULL)
    {
        return false;
    }
    *pa=va;
    if(core==NULL)
    {
        return true;
    }

    uint32_t satp=core->satp;
    if((satp&HS_MOXIE_CORE_SATP_MODE)==0)
    {
        return true;
    }

    uint64_t table=((uint64_t)(satp&HS_MOXIE_CORE_SATP_PPN))<<12;
    uint32_t vpn1=(va>>22)&0x3FF;
    uint32_t vpn0=(va>>12)&0x3FF;
    uint32_t offset=va&0xFFF;

    for(int level=1; level>=0; level--)
    {
        uint32_t vpn=(level==1)?vpn1:vpn0;
        uint64_t pte_address=table+((uint64_t)vpn*4);
        if(pte_address>0xFFFFFFFFULL)
        {
            return hs_moxie_core_mmu_fault(core,va);
        }

        uint8_t pte_data[4];
        if(!hs_moxie_core_physical_read(core,(uint32_t)pte_address,pte_data,sizeof(pte_data)))
        {
            return hs_moxie_core_mmu_fault(core,va);
        }
        uint32_t pte=hs_moxie_core_read_u32(core,pte_data);
        if((pte&HS_MOXIE_CORE_PTE_V)==0 ||
           (((pte&HS_MOXIE_CORE_PTE_R)==0)&&((pte&HS_MOXIE_CORE_PTE_W)!=0)))
        {
            return hs_moxie_core_mmu_fault(core,va);
        }

        if((pte&(HS_MOXIE_CORE_PTE_R|HS_MOXIE_CORE_PTE_X))!=0)
        {
            /*
             * 叶子PTE，检查访问权限。
             */
            if(access==HS_MOXIE_CORE_MEM_ACCESS_FETCH)
            {
                if((pte&HS_MOXIE_CORE_PTE_X)==0)
                {
                    return hs_moxie_core_mmu_fault(core,va);
                }
            }
            else if(access==HS_MOXIE_CORE_MEM_ACCESS_LOAD)
            {
                if((pte&HS_MOXIE_CORE_PTE_R)==0)
                {
                    return hs_moxie_core_mmu_fault(core,va);
                }
            }
            else
            {
                if((pte&HS_MOXIE_CORE_PTE_W)==0)
                {
                    return hs_moxie_core_mmu_fault(core,va);
                }
            }

            uint64_t ppn=(pte>>10)&0x3FFFFFUL;
            if(level==1)
            {
                /*
                 * 4MB超级页，PPN[0]必须为0。
                 */
                if((ppn&0x3FF)!=0)
                {
                    return hs_moxie_core_mmu_fault(core,va);
                }
                ppn=(ppn&0xFFFFFC00UL)|vpn0;
            }

            uint64_t pa64=(ppn<<12)|offset;
            if(pa64>0xFFFFFFFFULL)
            {
                return hs_moxie_core_mmu_fault(core,va);
            }

            /*
             * 硬件更新A/D位。
             */
            uint32_t new_pte=pte;
            if((new_pte&HS_MOXIE_CORE_PTE_A)==0)
            {
                new_pte|=HS_MOXIE_CORE_PTE_A;
            }
            if(access==HS_MOXIE_CORE_MEM_ACCESS_STORE && (new_pte&HS_MOXIE_CORE_PTE_D)==0)
            {
                new_pte|=HS_MOXIE_CORE_PTE_D;
            }
            if(new_pte!=pte)
            {
                uint8_t new_pte_data[4];
                hs_moxie_core_write_u32(core,new_pte_data,new_pte);
                hs_moxie_core_physical_write(core,(uint32_t)pte_address,new_pte_data,sizeof(new_pte_data));
            }

            *pa=(uint32_t)pa64;
            return true;
        }
        else
        {
            if(level==0)
            {
                return hs_moxie_core_mmu_fault(core,va);
            }
            table=((uint64_t)((pte>>10)&0x3FFFFFUL))<<12;
        }
    }

    return hs_moxie_core_mmu_fault(core,va);
}

static bool hs_moxie_core_memory_read(hs_moxie_core_t *core,uint32_t address,uint8_t *data,size_t len)
{
    uint32_t physical=address;
    uint32_t access=(core!=NULL)?core->mem_access:HS_MOXIE_CORE_MEM_ACCESS_FETCH;
    if(!hs_moxie_core_mmu_translate(core,address,access,&physical))
    {
        return false;
    }
    if(hs_moxie_core_physical_read(core,physical,data,len))
    {
        return true;
    }
    hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_MMU_MISS,address);
    return false;
}

static bool hs_moxie_core_memory_write(hs_moxie_core_t *core,uint32_t address,const uint8_t *data,size_t len)
{
    uint32_t physical=address;
    uint32_t access=(core!=NULL)?core->mem_access:HS_MOXIE_CORE_MEM_ACCESS_STORE;
    if(!hs_moxie_core_mmu_translate(core,address,access,&physical))
    {
        return false;
    }
    if(hs_moxie_core_physical_write(core,physical,data,len))
    {
        return true;
    }
    hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_MMU_MISS,address);
    return false;
}

static inline bool hs_moxie_core_load_u8(hs_moxie_core_t *core,uint32_t address,uint8_t *value)
{
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_LOAD;
    }
    return hs_moxie_core_memory_read(core,address,value,1);
}

static inline bool hs_moxie_core_load_u16(hs_moxie_core_t *core,uint32_t address,uint16_t *value)
{
    uint8_t data[2];
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_LOAD;
    }
    if(hs_moxie_core_memory_read(core,address,data,sizeof(data)))
    {
        *value=hs_moxie_core_read_u16(core,data);
        return true;
    }
    return false;
}

static inline bool hs_moxie_core_load_u32(hs_moxie_core_t *core,uint32_t address,uint32_t *value)
{
    uint8_t data[4];
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_LOAD;
    }
    if(hs_moxie_core_memory_read(core,address,data,sizeof(data)))
    {
        *value=hs_moxie_core_read_u32(core,data);
        return true;
    }
    return false;
}

static inline bool hs_moxie_core_store_u8(hs_moxie_core_t *core,uint32_t address,uint8_t value)
{
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_STORE;
    }
    return hs_moxie_core_memory_write(core,address,&value,1);
}

static inline bool hs_moxie_core_store_u16(hs_moxie_core_t *core,uint32_t address,uint16_t value)
{
    uint8_t data[2];
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_STORE;
    }
    hs_moxie_core_write_u16(core,data,value);
    return hs_moxie_core_memory_write(core,address,data,sizeof(data));
}

static inline bool hs_moxie_core_store_u32(hs_moxie_core_t *core,uint32_t address,uint32_t value)
{
    uint8_t data[4];
    if(core!=NULL)
    {
        core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_STORE;
    }
    hs_moxie_core_write_u32(core,data,value);
    return hs_moxie_core_memory_write(core,address,data,sizeof(data));
}

/*
 * 进入异常处理。
 * 若异常处理程序地址有效（swi总是尝试进入，其它异常还需状态寄存器bit0置位），
 * 则保存现场并跳转到异常处理程序；否则进入暂停状态，等待用户清除异常。
 */
static void hs_moxie_core_exception_enter(hs_moxie_core_t *core,uint32_t cause,uint32_t value,uint32_t pc)
{
    if(core==NULL)
    {
        return;
    }

    hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_EXCEPTION_CAUSE,cause);
    if(cause==HS_MOXIE_CORE_EXCEPTION_SWI || cause==HS_MOXIE_CORE_EXCEPTION_IRQ)
    {
        /*
         * 特殊寄存器3在swi时传递swi号，在中断时传递中断号。
         */
        hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_SWI_NUMBER,value);
    }
    hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_EXCEPTION_PC,pc);

    uint32_t handler=hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_EXCEPTION_HANDLER);
    uint32_t status=hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_STATUS);
    bool vector=(handler!=0);
    if(cause!=HS_MOXIE_CORE_EXCEPTION_SWI)
    {
        /*
         * 进入异常处理时关闭中断，异常处理程序可在适当时机重新开启。
         * swi与参考实现一致，不在此处关闭中断。
         */
        vector=vector && ((status&0x01)!=0);
    }

    if(vector)
    {
        if(cause!=HS_MOXIE_CORE_EXCEPTION_SWI)
        {
            /*
             * 进入中断时保存MIE到MPIE并关闭MIE，
             * 由ret返回中断PC时自动恢复。
             */
            if(cause==HS_MOXIE_CORE_EXCEPTION_IRQ && core->flags.mpie_pending==0)
            {
                if((core->mstatus&HS_MOXIE_CORE_MSTATUS_MIE)!=0)
                {
                    core->mstatus|=HS_MOXIE_CORE_MSTATUS_MPIE;
                }
                else
                {
                    core->mstatus&=(~HS_MOXIE_CORE_MSTATUS_MPIE);
                }
                core->mstatus&=(~HS_MOXIE_CORE_MSTATUS_MIE);
                core->flags.mpie_pending=1;
            }
            hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_STATUS,status&(~0x01UL));
        }
        hs_moxie_core_pc_write(core,handler);
        core->flags.halted=0;
        core->flags.halted_by_interrupt=0;
    }
    else
    {
        core->flags.halted=1;
        core->flags.halted_by_interrupt=(cause==HS_MOXIE_CORE_EXCEPTION_IRQ);
    }
}

/*
 * 在指令边界处理等待中的中断。
 * 中断使能(状态寄存器bit0)且异常处理程序地址有效时进入异常处理程序，
 * 否则保留中断请求并继续执行。
 */
static bool hs_moxie_core_interrupt_process(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }

    bool generic_pending=(core->flags.interrupt_pending!=0);
    bool software_pending=(core->flags.software_interrupt_pending!=0);
    bool timer_pending=(core->flags.timer_pending!=0);
    if(generic_pending || software_pending || timer_pending)
    {
        uint32_t mie=hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_MIE);
        generic_pending=generic_pending && ((mie&HS_MOXIE_CORE_MIE_MEIE)!=0);
        software_pending=software_pending && ((mie&HS_MOXIE_CORE_MIE_MSIE)!=0);
        timer_pending=timer_pending && ((mie&HS_MOXIE_CORE_MIE_MTIE)!=0);
    }
    if(!generic_pending && !software_pending && !timer_pending)
    {
        return false;
    }

    if((core->mstatus&HS_MOXIE_CORE_MSTATUS_MIE)==0)
    {
        return false;
    }

    uint32_t status=hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_STATUS);
    if((status&0x01)==0)
    {
        return false;
    }

    uint32_t handler=hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_EXCEPTION_HANDLER);
    if(handler==0)
    {
        core->flags.halted=1;
        core->flags.halted_by_interrupt=1;
        return true;
    }

    uint32_t value=HS_MOXIE_CORE_INTERRUPT_TIMER;
    if(generic_pending)
    {
        value=HS_MOXIE_CORE_INTERRUPT_EXTERNAL;
        core->flags.interrupt_pending=0;
    }
    else if(software_pending)
    {
        value=HS_MOXIE_CORE_INTERRUPT_SOFTWARE;
        core->flags.software_interrupt_pending=0;
    }
    hs_moxie_core_exception_enter(core,HS_MOXIE_CORE_EXCEPTION_IRQ,value,hs_moxie_core_pc_read(core));
    return true;
}

/*
 * 获取指令长度。
 * Form 1部分指令带16位或32位立即数，注意moxie的32位立即数紧跟16位指令字，总长为6字节。
 */
static size_t hs_moxie_core_instruction_length(uint16_t instruction)
{
    if((instruction&0x8000)!=0)
    {
        //Form 2/Form 3
        return 2;
    }

    switch((instruction>>8)&0x7F)
    {
    case 0x01:  //ldi.l
    case 0x03:  //jsra
    case 0x08:  //lda.l
    case 0x09:  //sta.l
    case 0x1a:  //jmpa
    case 0x1b:  //ldi.b
    case 0x1d:  //lda.b
    case 0x1f:  //sta.b
    case 0x20:  //ldi.s
    case 0x22:  //lda.s
    case 0x24:  //sta.s
    case 0x30:  //swi
    {
        return 6;
    }
    case 0x0c:  //ldo.l
    case 0x0d:  //sto.l
    case 0x36:  //ldo.b
    case 0x37:  //sto.b
    case 0x38:  //ldo.s
    case 0x39:  //sto.s
    {
        return 4;
    }
    default:
    {
        return 2;
    }
    }
}

static inline int32_t hs_moxie_core_sign_extend_8(uint8_t value)
{
    return (value&0x80)?((int32_t)value-256):(int32_t)value;
}

static inline int32_t hs_moxie_core_sign_extend_10(uint16_t value)
{
    value&=0x03FF;
    return (value&0x0200)?((int32_t)value-1024):(int32_t)value;
}

static inline int32_t hs_moxie_core_sign_extend_16(uint16_t value)
{
    return (value&0x8000)?((int32_t)value-65536):(int32_t)value;
}

/*
 * 执行一条指令。
 */
static void hs_moxie_core_exec(hs_moxie_core_t *core)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    uint32_t pc=hs_moxie_core_pc_read(core);
    core->mem_access=HS_MOXIE_CORE_MEM_ACCESS_FETCH;
    uint8_t opcode_data[2];
    if(!hs_moxie_core_memory_read(core,pc,opcode_data,sizeof(opcode_data)))
    {
        hs_moxie_core_exception_enter(core,core->fault_cause,core->fault_value,pc);
        return;
    }

    uint16_t instruction=hs_moxie_core_read_u16(core,opcode_data);
    size_t instruction_length=hs_moxie_core_instruction_length(instruction);
    uint16_t imm16=0;
    uint32_t imm32=0;
    uint8_t imm_data[4];

    if(instruction_length==4)
    {
        if(!hs_moxie_core_memory_read(core,pc+2,imm_data,2))
        {
            hs_moxie_core_exception_enter(core,core->fault_cause,core->fault_value,pc);
            return;
        }
        imm16=hs_moxie_core_read_u16(core,imm_data);
    }
    else if(instruction_length==6)
    {
        if(!hs_moxie_core_memory_read(core,pc+2,imm_data,4))
        {
            hs_moxie_core_exception_enter(core,core->fault_cause,core->fault_value,pc);
            return;
        }
        imm32=hs_moxie_core_read_u32(core,imm_data);
    }

    uint32_t trace_instruction=(uint32_t)instruction;
    if(instruction_length==4)
    {
        trace_instruction|=(uint32_t)imm16<<16;
    }
    else if(instruction_length==6)
    {
        trace_instruction|=(imm32&0xFFFF0000UL);
    }

    hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_INSTRUCTION_ENTER,pc,(uint8_t *)&trace_instruction,sizeof(trace_instruction));

    uint32_t next_pc=pc+(uint32_t)instruction_length;
    bool faulted=false;

    if((instruction&0x8000)==0)
    {
        /*
         * Form 1
         */
        uint32_t opcode=(instruction>>8)&0x7F;
        uint32_t reg_a=(instruction>>4)&0x0F;
        uint32_t reg_b=instruction&0x0F;
        uint32_t value_a=0;
        uint32_t value_b=0;

        switch(opcode)
        {
        case 0x00:  //bad
        {
            hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        case 0x01:  //ldi.l
        {
            hs_moxie_core_register_write(core,reg_a,imm32);
        }
        break;
        case 0x02:  //mov
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            (void)value_a;
            hs_moxie_core_register_write(core,reg_a,value_b);
        }
        break;
        case 0x03:  //jsra
        {
            uint32_t sp=hs_moxie_core_register_read(core,1);
            uint32_t new_sp=sp-12;
            if(!hs_moxie_core_store_u32(core,new_sp+4,pc+(uint32_t)instruction_length))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_store_u32(core,new_sp,hs_moxie_core_register_read(core,0)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,1,new_sp);
            hs_moxie_core_register_write(core,0,new_sp);
            next_pc=imm32;
        }
        break;
        case 0x04:  //ret
        {
            uint32_t frame_pointer=hs_moxie_core_register_read(core,0);
            uint32_t new_frame_pointer=0;
            uint32_t return_address=0;
            if(!hs_moxie_core_load_u32(core,frame_pointer,&new_frame_pointer))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_load_u32(core,frame_pointer+4,&return_address))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,0,new_frame_pointer);
            hs_moxie_core_register_write(core,1,frame_pointer+12);
            next_pc=return_address;
            if(core->flags.mpie_pending!=0 &&
               return_address==hs_moxie_core_sreg_read(core,HS_MOXIE_CORE_SREG_EXCEPTION_PC))
            {
                /*
                 * ret恢复到中断前执行地址时，从MPIE恢复MIE。
                 * 仅进入中断后的第一次匹配ret有效。
                 */
                if((core->mstatus&HS_MOXIE_CORE_MSTATUS_MPIE)!=0)
                {
                    core->mstatus|=HS_MOXIE_CORE_MSTATUS_MIE;
                }
                else
                {
                    core->mstatus&=(~HS_MOXIE_CORE_MSTATUS_MIE);
                }
                core->flags.mpie_pending=0;
            }
        }
        break;
        case 0x05:  //add
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a+value_b);
        }
        break;
        case 0x06:  //push
        {
            uint32_t sp=hs_moxie_core_register_read(core,reg_a);
            uint32_t new_sp=sp-4;
            if(!hs_moxie_core_store_u32(core,new_sp,hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,new_sp);
        }
        break;
        case 0x07:  //pop
        {
            uint32_t sp=hs_moxie_core_register_read(core,reg_a);
            uint32_t value=0;
            if(!hs_moxie_core_load_u32(core,sp,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_b,value);
            hs_moxie_core_register_write(core,reg_a,sp+4);
        }
        break;
        case 0x08:  //lda.l
        {
            uint32_t value=0;
            if(!hs_moxie_core_load_u32(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x09:  //sta.l
        {
            if(!hs_moxie_core_store_u32(core,imm32,hs_moxie_core_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0a:  //ld.l
        {
            uint32_t value=0;
            if(!hs_moxie_core_load_u32(core,hs_moxie_core_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x0b:  //st.l
        {
            if(!hs_moxie_core_store_u32(core,hs_moxie_core_register_read(core,reg_a),hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0c:  //ldo.l
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_b)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            uint32_t value=0;
            if(!hs_moxie_core_load_u32(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x0d:  //sto.l
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_a)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            if(!hs_moxie_core_store_u32(core,address,hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0e:  //cmp
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            uint32_t cc=0;
            if(value_a==value_b)
            {
                cc=HS_MOXIE_CORE_CC_EQ;
            }
            else
            {
                int32_t signed_a=(int32_t)value_a;
                int32_t signed_b=(int32_t)value_b;
                if(signed_a<signed_b)
                {
                    cc|=HS_MOXIE_CORE_CC_LT;
                }
                if(signed_a>signed_b)
                {
                    cc|=HS_MOXIE_CORE_CC_GT;
                }
                if(value_a<value_b)
                {
                    cc|=HS_MOXIE_CORE_CC_LTU;
                }
                if(value_a>value_b)
                {
                    cc|=HS_MOXIE_CORE_CC_GTU;
                }
            }
            core->cc=cc;
        }
        break;
        case 0x0f:  //nop
        {
        }
        break;
        case 0x10:  //sex.b
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,(uint32_t)hs_moxie_core_sign_extend_8((uint8_t)(value_b&0xFF)));
        }
        break;
        case 0x11:  //sex.s
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,(uint32_t)hs_moxie_core_sign_extend_16((uint16_t)(value_b&0xFFFF)));
        }
        break;
        case 0x12:  //zex.b
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_b&0xFF);
        }
        break;
        case 0x13:  //zex.s
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_b&0xFFFF);
        }
        break;
        case 0x14:  //umul.x
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            uint64_t product=(uint64_t)value_a*(uint64_t)value_b;
            hs_moxie_core_register_write(core,reg_a,(uint32_t)(product>>32));
        }
        break;
        case 0x15:  //mul.x
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            int64_t product=(int64_t)(int32_t)value_a*(int64_t)(int32_t)value_b;
            hs_moxie_core_register_write(core,reg_a,(uint32_t)((uint64_t)product>>32));
        }
        break;
        case 0x19:  //jsr
        {
            uint32_t target=hs_moxie_core_register_read(core,reg_a);
            uint32_t sp=hs_moxie_core_register_read(core,1);
            uint32_t new_sp=sp-12;
            if(!hs_moxie_core_store_u32(core,new_sp+4,pc+(uint32_t)instruction_length))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_store_u32(core,new_sp,hs_moxie_core_register_read(core,0)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,1,new_sp);
            hs_moxie_core_register_write(core,0,new_sp);
            next_pc=target;
        }
        break;
        case 0x1a:  //jmpa
        {
            next_pc=imm32;
        }
        break;
        case 0x1b:  //ldi.b
        {
            hs_moxie_core_register_write(core,reg_a,imm32);
        }
        break;
        case 0x1c:  //ld.b
        {
            uint8_t value=0;
            if(!hs_moxie_core_load_u8(core,hs_moxie_core_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x1d:  //lda.b
        {
            uint8_t value=0;
            if(!hs_moxie_core_load_u8(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x1e:  //st.b
        {
            if(!hs_moxie_core_store_u8(core,hs_moxie_core_register_read(core,reg_a),(uint8_t)hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x1f:  //sta.b
        {
            if(!hs_moxie_core_store_u8(core,imm32,(uint8_t)hs_moxie_core_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x20:  //ldi.s
        {
            hs_moxie_core_register_write(core,reg_a,imm32);
        }
        break;
        case 0x21:  //ld.s
        {
            uint16_t value=0;
            if(!hs_moxie_core_load_u16(core,hs_moxie_core_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x22:  //lda.s
        {
            uint16_t value=0;
            if(!hs_moxie_core_load_u16(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x23:  //st.s
        {
            if(!hs_moxie_core_store_u16(core,hs_moxie_core_register_read(core,reg_a),(uint16_t)hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x24:  //sta.s
        {
            if(!hs_moxie_core_store_u16(core,imm32,(uint16_t)hs_moxie_core_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x25:  //jmp
        {
            next_pc=hs_moxie_core_register_read(core,reg_a);
        }
        break;
        case 0x26:  //and
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a&value_b);
        }
        break;
        case 0x27:  //lshr
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a>>(value_b&0x1F));
        }
        break;
        case 0x28:  //ashl
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a<<(value_b&0x1F));
        }
        break;
        case 0x29:  //sub
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a-value_b);
        }
        break;
        case 0x2a:  //neg
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,(uint32_t)(0-value_b));
        }
        break;
        case 0x2b:  //or
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a|value_b);
        }
        break;
        case 0x2c:  //not
        {
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,~value_b);
        }
        break;
        case 0x2d:  //ashr
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,(uint32_t)((int32_t)value_a>>(value_b&0x1F)));
        }
        break;
        case 0x2e:  //xor
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,value_a^value_b);
        }
        break;
        case 0x2f:  //mul
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            hs_moxie_core_register_write(core,reg_a,(uint32_t)((uint64_t)value_a*(uint64_t)value_b));
        }
        break;
        case 0x30:  //swi
        {
            hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_SWI_NUMBER,imm32);
            hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_EXCEPTION_CAUSE,HS_MOXIE_CORE_EXCEPTION_SWI);
            bool handled=false;
            if(core->io!=NULL)
            {
                handled=core->io(core,(uint32_t)HS_MOXIE_CORE_IO_SWI|(uint32_t)HS_MOXIE_CORE_IO_OPERATOR_CORE,imm32,NULL,0,core->usr);
            }
            if(!handled)
            {
                hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_SWI,imm32);
                faulted=true;
            }
            else
            {
                /*
                 * swi已由外部服务处理，清理swi上下文并继续正常运行。
                 */
                hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_EXCEPTION_CAUSE,0);
                hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_SWI_NUMBER,0);
            }
        }
        break;
        case 0x31:  //div
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            if((int32_t)value_a==INT32_MIN && (int32_t)value_b==-1)
            {
                hs_moxie_core_register_write(core,reg_a,(uint32_t)INT32_MIN);
            }
            else
            {
                hs_moxie_core_register_write(core,reg_a,(uint32_t)((int32_t)value_a/(int32_t)value_b));
            }
        }
        break;
        case 0x32:  //udiv
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value_a/value_b);
        }
        break;
        case 0x33:  //mod
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            if((int32_t)value_a==INT32_MIN && (int32_t)value_b==-1)
            {
                hs_moxie_core_register_write(core,reg_a,0);
            }
            else
            {
                hs_moxie_core_register_write(core,reg_a,(uint32_t)((int32_t)value_a%(int32_t)value_b));
            }
        }
        break;
        case 0x34:  //umod
        {
            value_a=hs_moxie_core_register_read(core,reg_a);
            value_b=hs_moxie_core_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value_a%value_b);
        }
        break;
        case 0x35:  //brk
        {
            hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_BREAK,instruction);
            faulted=true;
        }
        break;
        case 0x36:  //ldo.b
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_b)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            uint8_t value=0;
            if(!hs_moxie_core_load_u8(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x37:  //sto.b
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_a)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            if(!hs_moxie_core_store_u8(core,address,(uint8_t)hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x38:  //ldo.s
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_b)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            uint16_t value=0;
            if(!hs_moxie_core_load_u16(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_register_write(core,reg_a,value);
        }
        break;
        case 0x39:  //sto.s
        {
            uint32_t address=hs_moxie_core_register_read(core,reg_a)+(uint32_t)hs_moxie_core_sign_extend_16(imm16);
            if(!hs_moxie_core_store_u16(core,address,(uint16_t)hs_moxie_core_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        default:
        {
            hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        }
    }
    else if((instruction&0xC000)==0x8000)
    {
        /*
         * Form 2
         */
        uint32_t reg_a=(instruction>>8)&0x0F;
        uint32_t immediate=instruction&0xFF;
        switch((instruction>>12)&0x03)
        {
        case 0x00:  //inc
        {
            hs_moxie_core_register_write(core,reg_a,hs_moxie_core_register_read(core,reg_a)+immediate);
        }
        break;
        case 0x01:  //dec
        {
            hs_moxie_core_register_write(core,reg_a,hs_moxie_core_register_read(core,reg_a)-immediate);
        }
        break;
        case 0x02:  //gsr
        {
            hs_moxie_core_register_write(core,reg_a,hs_moxie_core_sreg_read(core,immediate));
        }
        break;
        case 0x03:  //ssr
        {
            hs_moxie_core_sreg_write(core,immediate,hs_moxie_core_register_read(core,reg_a));
        }
        break;
        default:
        {
            hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        }
    }
    else
    {
        /*
         * Form 3
         */
        uint32_t opcode=(instruction>>10)&0x0F;
        int32_t offset=hs_moxie_core_sign_extend_10(instruction&0x03FF)*2;
        bool take_branch=false;
        switch(opcode)
        {
        case 0x00:  //beq
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_EQ)!=0);
        }
        break;
        case 0x01:  //bne
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_EQ)==0);
        }
        break;
        case 0x02:  //blt
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_LT)!=0);
        }
        break;
        case 0x03:  //bgt
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_GT)!=0);
        }
        break;
        case 0x04:  //bltu
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_LTU)!=0);
        }
        break;
        case 0x05:  //bgtu
        {
            take_branch=((core->cc&HS_MOXIE_CORE_CC_GTU)!=0);
        }
        break;
        case 0x06:  //bge
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_CC_GT|HS_MOXIE_CORE_CC_EQ))!=0);
        }
        break;
        case 0x07:  //ble
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_CC_LT|HS_MOXIE_CORE_CC_EQ))!=0);
        }
        break;
        case 0x08:  //bgeu
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_CC_GTU|HS_MOXIE_CORE_CC_EQ))!=0);
        }
        break;
        case 0x09:  //bleu
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_CC_LTU|HS_MOXIE_CORE_CC_EQ))!=0);
        }
        break;
        default:
        {
            hs_moxie_core_fault_set(core,HS_MOXIE_CORE_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        }

        if(!faulted && take_branch)
        {
            next_pc=pc+2+(uint32_t)offset;
        }
    }

    hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_INSTRUCTION_EXIT,pc,(uint8_t *)&trace_instruction,sizeof(trace_instruction));

    if(faulted)
    {
        hs_moxie_core_exception_enter(core,core->fault_cause,core->fault_value,pc);
    }
    else if(core->flags.halted==0)
    {
        hs_moxie_core_pc_write(core,next_pc);
    }
}

size_t hs_moxie_core_size(void)
{
    return sizeof(hs_moxie_core_t);
}

hs_moxie_core_t *hs_moxie_core_init(void *mem,hs_moxie_core_io_t io,void *usr)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_moxie_core_t *ret=(hs_moxie_core_t *)mem;
    memset(ret,0,sizeof(hs_moxie_core_t));
    ret->io=io;
    ret->usr=usr;
    hs_moxie_core_reset(ret);
    return ret;
}

void hs_moxie_core_tick(hs_moxie_core_t * core,size_t cycles)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    while(cycles--)
    {
        hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_TICK_ENTER,hs_moxie_core_pc_read(core),(uint8_t *)&cycles,sizeof(cycles));

        /*
         * 定时器比较。
         */
        core->flags.timer_pending=(core->timer_time>=core->timer_timecmp);

        bool exception_taken=false;
        if(core->flags.exception_pending!=0)
        {
            uint32_t cause=core->exception_cause;
            uint32_t value=core->exception_value;
            core->flags.exception_pending=0;
            hs_moxie_core_exception_enter(core,cause,value,hs_moxie_core_pc_read(core));
            exception_taken=true;
        }

        if(core->flags.halted==0 && !exception_taken)
        {
            exception_taken=hs_moxie_core_interrupt_process(core);
        }

        if(core->flags.halted==0 && !exception_taken)
        {
            hs_moxie_core_exec(core);
        }

        hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_TICK_EXIT,hs_moxie_core_pc_read(core),(uint8_t *)&cycles,sizeof(cycles));

        if(core->flags.halted!=0)
        {
            break;
        }
    }
}

void hs_moxie_core_reset(hs_moxie_core_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_moxie_core_io_t io=core->io;
        void *usr=core->usr;
        memset(core,0,sizeof(hs_moxie_core_t));
        core->io=io;
        core->usr=usr;
        core->satp=0;
        core->mstatus=HS_MOXIE_CORE_MSTATUS_DEFAULT;
        core->timer_time=0;
        core->timer_timecmp=UINT64_MAX;

        {
            uint32_t value=0;
            hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_RESET,0,(uint8_t *)&value,sizeof(value));
        }

        {
            uint8_t data[4];
            uint32_t entry=0;
            if(hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_ENTRY_PC_READ,0,data,sizeof(data)))
            {
                entry=hs_moxie_core_read_u32(core,data);
            }
            hs_moxie_core_pc_write(core,entry);
        }

        {
            uint8_t data[4];
            uint32_t sp=0;
            if(hs_moxie_core_io_call(core,(uint32_t)HS_MOXIE_CORE_IO_INITIAL_SP_READ,0,data,sizeof(data)))
            {
                sp=hs_moxie_core_read_u32(core,data);
            }
            if(sp!=0)
            {
                hs_moxie_core_register_write(core,1,sp);
                hs_moxie_core_sreg_write(core,HS_MOXIE_CORE_SREG_SIM_RETURN_LEN,sp);
            }
        }
    }
}

uint32_t hs_moxie_core_satp_get(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->satp;
}

void hs_moxie_core_satp_set(hs_moxie_core_t *core,uint32_t value)
{
    if(core==NULL)
    {
        return;
    }
    core->satp=value;
}

uint32_t hs_moxie_core_mstatus_get(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->mstatus;
}

void hs_moxie_core_mstatus_set(hs_moxie_core_t *core,uint32_t value)
{
    if(core==NULL)
    {
        return;
    }
    core->mstatus=value;
}

uint32_t hs_moxie_core_mip_get(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    uint32_t mip=0;
    if(core->flags.software_interrupt_pending!=0)
    {
        mip|=HS_MOXIE_CORE_MIP_MSIP;
    }
    if(core->flags.timer_pending!=0)
    {
        mip|=HS_MOXIE_CORE_MIP_MTIP;
    }
    if(core->flags.interrupt_pending!=0)
    {
        mip|=HS_MOXIE_CORE_MIP_MEIP;
    }
    return mip;
}

void hs_moxie_core_mip_set(hs_moxie_core_t *core,uint32_t value)
{
    if(core==NULL)
    {
        return;
    }
    if((value&HS_MOXIE_CORE_MIP_MSIP)!=0)
    {
        core->flags.software_interrupt_pending=1;
    }
    else
    {
        core->flags.software_interrupt_pending=0;
    }
}

uint64_t hs_moxie_core_timer_time_get(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->timer_time;
}

void hs_moxie_core_timer_time_set(hs_moxie_core_t *core,uint64_t value)
{
    if(core==NULL)
    {
        return;
    }
    core->timer_time=value;
    core->flags.timer_pending=(core->timer_time>=core->timer_timecmp);
}

void hs_moxie_core_timer_time_advance(hs_moxie_core_t *core,uint64_t delta)
{
    if(core==NULL)
    {
        return;
    }
    core->timer_time+=delta;
    core->flags.timer_pending=(core->timer_time>=core->timer_timecmp);
}

uint64_t hs_moxie_core_timer_timecmp_get(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->timer_timecmp;
}

void hs_moxie_core_timer_timecmp_set(hs_moxie_core_t *core,uint64_t value)
{
    if(core==NULL)
    {
        return;
    }
    core->timer_timecmp=value;
    core->flags.timer_pending=(core->timer_time>=core->timer_timecmp);
}

bool hs_moxie_core_io(hs_moxie_core_t *core,hs_moxie_core_io_opt_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,(uint32_t)opt|(uint32_t)HS_MOXIE_CORE_IO_OPERATOR_EXTERN,address,data,len,core->usr);
    }
    return false;
}

bool hs_moxie_core_exception_raise(hs_moxie_core_t *core,uint32_t cause,uint32_t value)
{
    if(core==NULL || core->flags.exception_pending!=0 || core->flags.halted!=0)
    {
        return false;
    }
    core->exception_cause=cause;
    core->exception_value=value;
    core->flags.exception_pending=1;
    return true;
}

bool hs_moxie_core_exception_clear(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    core->flags.exception_pending=0;
    core->exception_cause=0;
    core->exception_value=0;
    core->flags.halted=0;
    core->flags.halted_by_interrupt=0;
    return true;
}

bool hs_moxie_core_external_interrupt_raise(hs_moxie_core_t *core)
{
    if(core==NULL || core->flags.interrupt_pending!=0 || core->flags.halted!=0)
    {
        return false;
    }
    core->flags.interrupt_pending=1;
    return true;
}

bool hs_moxie_core_external_interrupt_clear(hs_moxie_core_t *core)
{
    return hs_moxie_core_interrupt_clear(core);
}

bool hs_moxie_core_external_interrupt_pending(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    return core->flags.interrupt_pending!=0;
}

bool hs_moxie_core_interrupt_raise(hs_moxie_core_t *core,uint32_t value)
{
    if(!hs_moxie_core_external_interrupt_raise(core))
    {
        return false;
    }
    core->interrupt_value=value;
    return true;
}

bool hs_moxie_core_interrupt_clear(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    core->flags.interrupt_pending=0;
    core->interrupt_value=0;
    if(core->flags.halted_by_interrupt!=0)
    {
        core->flags.halted=0;
        core->flags.halted_by_interrupt=0;
    }
    return true;
}

bool hs_moxie_core_software_interrupt_raise(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    core->flags.software_interrupt_pending=1;
    return true;
}

bool hs_moxie_core_software_interrupt_clear(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    core->flags.software_interrupt_pending=0;
    return true;
}

bool hs_moxie_core_software_interrupt_pending(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    return core->flags.software_interrupt_pending!=0;
}

bool hs_moxie_core_halted(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    return core->flags.halted!=0;
}

uint32_t hs_moxie_core_condition_code(hs_moxie_core_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->cc;
}

void hs_moxie_core_condition_code_set(hs_moxie_core_t *core,uint32_t cc)
{
    if(core==NULL)
    {
        return;
    }
    core->cc=cc;
}
