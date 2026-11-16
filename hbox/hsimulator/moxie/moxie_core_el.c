/***************************************************************
 * Name:      moxie_core_el.c
 * Purpose:   实现moxie小端内核接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "moxie_core_el.h"

/*
 * moxie条件码，与参考实现保持一致。
 */
#define HS_MOXIE_CORE_EL_CC_GT        (1UL << 0)
#define HS_MOXIE_CORE_EL_CC_LT        (1UL << 1)
#define HS_MOXIE_CORE_EL_CC_EQ        (1UL << 2)
#define HS_MOXIE_CORE_EL_CC_GTU       (1UL << 3)
#define HS_MOXIE_CORE_EL_CC_LTU       (1UL << 4)

struct hs_moxie_core_el
{
    hs_moxie_core_el_io_t  io;                 /**< IO回调 */
    void*               usr;                /**< 用户指针 */
    uint32_t            cc;                 /**< 条件码 */
    uint32_t            exception_cause;    /**< 等待处理的异常原因 */
    uint32_t            exception_value;    /**< 等待处理的异常附带值 */
    uint32_t            interrupt_value;    /**< 等待处理的中断附带值 */
    uint32_t            fault_cause;        /**< 当前指令产生的异常原因 */
    uint32_t            fault_value;        /**< 当前指令产生的异常附带值 */
    struct
    {
        uint32_t        exception_pending:1;    /**< 存在等待处理的异常 */
        uint32_t        interrupt_pending:1;    /**< 存在等待处理的中断 */
        uint32_t        halted:1;               /**< 因无法进入异常处理程序而暂停 */
        uint32_t        halted_by_interrupt:1;  /**< 暂停由中断引起 */
    }                   flags;
};

/*
 * 小端序读写辅助函数。
 * 内存及IO中的多字节数据均按小端序存放。
 */
static inline uint16_t hs_moxie_core_el_read_u16(hs_moxie_core_el_t *core,const uint8_t *data)
{
    (void)core;
    return (uint16_t)(((uint16_t)data[1] << 8) | (uint16_t)data[0]);
}

static inline uint32_t hs_moxie_core_el_read_u32(hs_moxie_core_el_t *core,const uint8_t *data)
{
    (void)core;
    return ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];
}

static inline void hs_moxie_core_el_write_u16(hs_moxie_core_el_t *core,uint8_t *data,uint16_t value)
{
    (void)core;
    data[0]=(uint8_t)(value >> 0);
    data[1]=(uint8_t)(value >> 8);
}

static inline void hs_moxie_core_el_write_u32(hs_moxie_core_el_t *core,uint8_t *data,uint32_t value)
{
    (void)core;
    data[0]=(uint8_t)(value >> 0);
    data[1]=(uint8_t)(value >> 8);
    data[2]=(uint8_t)(value >> 16);
    data[3]=(uint8_t)(value >> 24);
}

static inline bool hs_moxie_core_el_io_call(hs_moxie_core_el_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    return core->io(core,opt|(uint32_t)HS_MOXIE_CORE_EL_IO_OPERATOR_CORE,address,data,len,core->usr);
}

static inline uint32_t hs_moxie_core_el_register_read(hs_moxie_core_el_t *core,uint32_t index)
{
    uint8_t data[4];
    uint32_t value=0;
    if(index<16 && hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_REGISTER_READ,index,data,sizeof(data)))
    {
        value=hs_moxie_core_el_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_el_register_write(hs_moxie_core_el_t *core,uint32_t index,uint32_t value)
{
    uint8_t data[4];
    if(index<16)
    {
        hs_moxie_core_el_write_u32(core,data,value);
        hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_REGISTER_WRITE,index,data,sizeof(data));
    }
}

static inline uint32_t hs_moxie_core_el_sreg_read(hs_moxie_core_el_t *core,uint32_t index)
{
    uint8_t data[4];
    uint32_t value=0;
    if(index<256 && hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_SREG_READ,index,data,sizeof(data)))
    {
        value=hs_moxie_core_el_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_el_sreg_write(hs_moxie_core_el_t *core,uint32_t index,uint32_t value)
{
    uint8_t data[4];
    if(index<256)
    {
        hs_moxie_core_el_write_u32(core,data,value);
        hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_SREG_WRITE,index,data,sizeof(data));
    }
}

static inline uint32_t hs_moxie_core_el_pc_read(hs_moxie_core_el_t *core)
{
    uint8_t data[4];
    uint32_t value=0;
    if(hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_PC_REGISTER_READ,0,data,sizeof(data)))
    {
        value=hs_moxie_core_el_read_u32(core,data);
    }
    return value;
}

static inline void hs_moxie_core_el_pc_write(hs_moxie_core_el_t *core,uint32_t value)
{
    uint8_t data[4];
    hs_moxie_core_el_write_u32(core,data,value);
    hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_PC_REGISTER_WRITE,0,data,sizeof(data));
}

static inline void hs_moxie_core_el_fault_set(hs_moxie_core_el_t *core,uint32_t cause,uint32_t value)
{
    if(core!=NULL)
    {
        core->fault_cause=cause;
        core->fault_value=value;
    }
}

static bool hs_moxie_core_el_memory_read(hs_moxie_core_el_t *core,uint32_t address,uint8_t *data,size_t len)
{
    if(hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_MEMORY_READ,address,data,len))
    {
        return true;
    }
    hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_MMU_MISS,address);
    return false;
}

static bool hs_moxie_core_el_memory_write(hs_moxie_core_el_t *core,uint32_t address,const uint8_t *data,size_t len)
{
    if(hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_MEMORY_WRITE,address,(uint8_t *)data,len))
    {
        return true;
    }
    hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_MMU_MISS,address);
    return false;
}

static inline bool hs_moxie_core_el_load_u8(hs_moxie_core_el_t *core,uint32_t address,uint8_t *value)
{
    return hs_moxie_core_el_memory_read(core,address,value,1);
}

static inline bool hs_moxie_core_el_load_u16(hs_moxie_core_el_t *core,uint32_t address,uint16_t *value)
{
    uint8_t data[2];
    if(hs_moxie_core_el_memory_read(core,address,data,sizeof(data)))
    {
        *value=hs_moxie_core_el_read_u16(core,data);
        return true;
    }
    return false;
}

static inline bool hs_moxie_core_el_load_u32(hs_moxie_core_el_t *core,uint32_t address,uint32_t *value)
{
    uint8_t data[4];
    if(hs_moxie_core_el_memory_read(core,address,data,sizeof(data)))
    {
        *value=hs_moxie_core_el_read_u32(core,data);
        return true;
    }
    return false;
}

static inline bool hs_moxie_core_el_store_u8(hs_moxie_core_el_t *core,uint32_t address,uint8_t value)
{
    return hs_moxie_core_el_memory_write(core,address,&value,1);
}

static inline bool hs_moxie_core_el_store_u16(hs_moxie_core_el_t *core,uint32_t address,uint16_t value)
{
    uint8_t data[2];
    hs_moxie_core_el_write_u16(core,data,value);
    return hs_moxie_core_el_memory_write(core,address,data,sizeof(data));
}

static inline bool hs_moxie_core_el_store_u32(hs_moxie_core_el_t *core,uint32_t address,uint32_t value)
{
    uint8_t data[4];
    hs_moxie_core_el_write_u32(core,data,value);
    return hs_moxie_core_el_memory_write(core,address,data,sizeof(data));
}

/*
 * 进入异常处理。
 * 若异常处理程序地址有效（swi总是尝试进入，其它异常还需状态寄存器bit0置位），
 * 则保存现场并跳转到异常处理程序；否则进入暂停状态，等待用户清除异常。
 */
static void hs_moxie_core_el_exception_enter(hs_moxie_core_el_t *core,uint32_t cause,uint32_t value,uint32_t pc)
{
    if(core==NULL)
    {
        return;
    }

    hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_CAUSE,cause);
    if(cause==HS_MOXIE_CORE_EL_EXCEPTION_SWI)
    {
        hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_SWI_NUMBER,value);
    }
    hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_PC,pc);

    uint32_t handler=hs_moxie_core_el_sreg_read(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_HANDLER);
    uint32_t status=hs_moxie_core_el_sreg_read(core,HS_MOXIE_CORE_EL_SREG_STATUS);
    bool vector=(handler!=0);
    if(cause!=HS_MOXIE_CORE_EL_EXCEPTION_SWI)
    {
        /*
         * 进入异常处理时关闭中断，异常处理程序可在适当时机重新开启。
         * swi与参考实现一致，不在此处关闭中断。
         */
        vector=vector && ((status&0x01)!=0);
    }

    if(vector)
    {
        if(cause!=HS_MOXIE_CORE_EL_EXCEPTION_SWI)
        {
            hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_STATUS,status&(~0x01UL));
        }
        hs_moxie_core_el_pc_write(core,handler);
        core->flags.halted=0;
        core->flags.halted_by_interrupt=0;
    }
    else
    {
        core->flags.halted=1;
        core->flags.halted_by_interrupt=(cause==HS_MOXIE_CORE_EL_EXCEPTION_IRQ);
    }
}

/*
 * 在指令边界处理等待中的中断。
 * 中断使能(状态寄存器bit0)且异常处理程序地址有效时进入异常处理程序，
 * 否则保留中断请求并继续执行。
 */
static bool hs_moxie_core_el_interrupt_process(hs_moxie_core_el_t *core)
{
    if(core==NULL || core->flags.interrupt_pending==0)
    {
        return false;
    }

    uint32_t status=hs_moxie_core_el_sreg_read(core,HS_MOXIE_CORE_EL_SREG_STATUS);
    if((status&0x01)==0)
    {
        return false;
    }

    uint32_t handler=hs_moxie_core_el_sreg_read(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_HANDLER);
    if(handler==0)
    {
        core->flags.halted=1;
        core->flags.halted_by_interrupt=1;
        return true;
    }

    core->flags.interrupt_pending=0;
    hs_moxie_core_el_exception_enter(core,HS_MOXIE_CORE_EL_EXCEPTION_IRQ,core->interrupt_value,hs_moxie_core_el_pc_read(core));
    return true;
}

/*
 * 获取指令长度。
 * Form 1部分指令带16位或32位立即数，注意moxie的32位立即数紧跟16位指令字，总长为6字节。
 */
static size_t hs_moxie_core_el_instruction_length(uint16_t instruction)
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

static inline int32_t hs_moxie_core_el_sign_extend_8(uint8_t value)
{
    return (value&0x80)?((int32_t)value-256):(int32_t)value;
}

static inline int32_t hs_moxie_core_el_sign_extend_10(uint16_t value)
{
    value&=0x03FF;
    return (value&0x0200)?((int32_t)value-1024):(int32_t)value;
}

static inline int32_t hs_moxie_core_el_sign_extend_16(uint16_t value)
{
    return (value&0x8000)?((int32_t)value-65536):(int32_t)value;
}

/*
 * 执行一条指令。
 */
static void hs_moxie_core_el_exec(hs_moxie_core_el_t *core)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    uint32_t pc=hs_moxie_core_el_pc_read(core);
    uint8_t opcode_data[2];
    if(!hs_moxie_core_el_memory_read(core,pc,opcode_data,sizeof(opcode_data)))
    {
        hs_moxie_core_el_exception_enter(core,core->fault_cause,core->fault_value,pc);
        return;
    }

    uint16_t instruction=hs_moxie_core_el_read_u16(core,opcode_data);
    size_t instruction_length=hs_moxie_core_el_instruction_length(instruction);
    uint16_t imm16=0;
    uint32_t imm32=0;
    uint8_t imm_data[4];

    if(instruction_length==4)
    {
        if(!hs_moxie_core_el_memory_read(core,pc+2,imm_data,2))
        {
            hs_moxie_core_el_exception_enter(core,core->fault_cause,core->fault_value,pc);
            return;
        }
        imm16=hs_moxie_core_el_read_u16(core,imm_data);
    }
    else if(instruction_length==6)
    {
        if(!hs_moxie_core_el_memory_read(core,pc+2,imm_data,4))
        {
            hs_moxie_core_el_exception_enter(core,core->fault_cause,core->fault_value,pc);
            return;
        }
        imm32=hs_moxie_core_el_read_u32(core,imm_data);
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

    hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_INSTRUCTION_ENTER,pc,(uint8_t *)&trace_instruction,sizeof(trace_instruction));

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
            hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        case 0x01:  //ldi.l
        {
            hs_moxie_core_el_register_write(core,reg_a,imm32);
        }
        break;
        case 0x02:  //mov
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            (void)value_a;
            hs_moxie_core_el_register_write(core,reg_a,value_b);
        }
        break;
        case 0x03:  //jsra
        {
            uint32_t sp=hs_moxie_core_el_register_read(core,1);
            uint32_t new_sp=sp-12;
            if(!hs_moxie_core_el_store_u32(core,new_sp+4,pc+(uint32_t)instruction_length))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_el_store_u32(core,new_sp,hs_moxie_core_el_register_read(core,0)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,1,new_sp);
            hs_moxie_core_el_register_write(core,0,new_sp);
            next_pc=imm32;
        }
        break;
        case 0x04:  //ret
        {
            uint32_t frame_pointer=hs_moxie_core_el_register_read(core,0);
            uint32_t new_frame_pointer=0;
            uint32_t return_address=0;
            if(!hs_moxie_core_el_load_u32(core,frame_pointer,&new_frame_pointer))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_el_load_u32(core,frame_pointer+4,&return_address))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,0,new_frame_pointer);
            hs_moxie_core_el_register_write(core,1,frame_pointer+12);
            next_pc=return_address;
        }
        break;
        case 0x05:  //add
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a+value_b);
        }
        break;
        case 0x06:  //push
        {
            uint32_t sp=hs_moxie_core_el_register_read(core,reg_a);
            uint32_t new_sp=sp-4;
            if(!hs_moxie_core_el_store_u32(core,new_sp,hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,new_sp);
        }
        break;
        case 0x07:  //pop
        {
            uint32_t sp=hs_moxie_core_el_register_read(core,reg_a);
            uint32_t value=0;
            if(!hs_moxie_core_el_load_u32(core,sp,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_b,value);
            hs_moxie_core_el_register_write(core,reg_a,sp+4);
        }
        break;
        case 0x08:  //lda.l
        {
            uint32_t value=0;
            if(!hs_moxie_core_el_load_u32(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x09:  //sta.l
        {
            if(!hs_moxie_core_el_store_u32(core,imm32,hs_moxie_core_el_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0a:  //ld.l
        {
            uint32_t value=0;
            if(!hs_moxie_core_el_load_u32(core,hs_moxie_core_el_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x0b:  //st.l
        {
            if(!hs_moxie_core_el_store_u32(core,hs_moxie_core_el_register_read(core,reg_a),hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0c:  //ldo.l
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_b)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            uint32_t value=0;
            if(!hs_moxie_core_el_load_u32(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x0d:  //sto.l
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_a)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            if(!hs_moxie_core_el_store_u32(core,address,hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x0e:  //cmp
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            uint32_t cc=0;
            if(value_a==value_b)
            {
                cc=HS_MOXIE_CORE_EL_CC_EQ;
            }
            else
            {
                int32_t signed_a=(int32_t)value_a;
                int32_t signed_b=(int32_t)value_b;
                if(signed_a<signed_b)
                {
                    cc|=HS_MOXIE_CORE_EL_CC_LT;
                }
                if(signed_a>signed_b)
                {
                    cc|=HS_MOXIE_CORE_EL_CC_GT;
                }
                if(value_a<value_b)
                {
                    cc|=HS_MOXIE_CORE_EL_CC_LTU;
                }
                if(value_a>value_b)
                {
                    cc|=HS_MOXIE_CORE_EL_CC_GTU;
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
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)hs_moxie_core_el_sign_extend_8((uint8_t)(value_b&0xFF)));
        }
        break;
        case 0x11:  //sex.s
        {
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)hs_moxie_core_el_sign_extend_16((uint16_t)(value_b&0xFFFF)));
        }
        break;
        case 0x12:  //zex.b
        {
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_b&0xFF);
        }
        break;
        case 0x13:  //zex.s
        {
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_b&0xFFFF);
        }
        break;
        case 0x14:  //umul.x
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            uint64_t product=(uint64_t)value_a*(uint64_t)value_b;
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)(product>>32));
        }
        break;
        case 0x15:  //mul.x
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            int64_t product=(int64_t)(int32_t)value_a*(int64_t)(int32_t)value_b;
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)((uint64_t)product>>32));
        }
        break;
        case 0x19:  //jsr
        {
            uint32_t target=hs_moxie_core_el_register_read(core,reg_a);
            uint32_t sp=hs_moxie_core_el_register_read(core,1);
            uint32_t new_sp=sp-12;
            if(!hs_moxie_core_el_store_u32(core,new_sp+4,pc+(uint32_t)instruction_length))
            {
                faulted=true;
                break;
            }
            if(!hs_moxie_core_el_store_u32(core,new_sp,hs_moxie_core_el_register_read(core,0)))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,1,new_sp);
            hs_moxie_core_el_register_write(core,0,new_sp);
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
            hs_moxie_core_el_register_write(core,reg_a,imm32);
        }
        break;
        case 0x1c:  //ld.b
        {
            uint8_t value=0;
            if(!hs_moxie_core_el_load_u8(core,hs_moxie_core_el_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x1d:  //lda.b
        {
            uint8_t value=0;
            if(!hs_moxie_core_el_load_u8(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x1e:  //st.b
        {
            if(!hs_moxie_core_el_store_u8(core,hs_moxie_core_el_register_read(core,reg_a),(uint8_t)hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x1f:  //sta.b
        {
            if(!hs_moxie_core_el_store_u8(core,imm32,(uint8_t)hs_moxie_core_el_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x20:  //ldi.s
        {
            hs_moxie_core_el_register_write(core,reg_a,imm32);
        }
        break;
        case 0x21:  //ld.s
        {
            uint16_t value=0;
            if(!hs_moxie_core_el_load_u16(core,hs_moxie_core_el_register_read(core,reg_b),&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x22:  //lda.s
        {
            uint16_t value=0;
            if(!hs_moxie_core_el_load_u16(core,imm32,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x23:  //st.s
        {
            if(!hs_moxie_core_el_store_u16(core,hs_moxie_core_el_register_read(core,reg_a),(uint16_t)hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x24:  //sta.s
        {
            if(!hs_moxie_core_el_store_u16(core,imm32,(uint16_t)hs_moxie_core_el_register_read(core,reg_a)))
            {
                faulted=true;
            }
        }
        break;
        case 0x25:  //jmp
        {
            next_pc=hs_moxie_core_el_register_read(core,reg_a);
        }
        break;
        case 0x26:  //and
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a&value_b);
        }
        break;
        case 0x27:  //lshr
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a>>(value_b&0x1F));
        }
        break;
        case 0x28:  //ashl
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a<<(value_b&0x1F));
        }
        break;
        case 0x29:  //sub
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a-value_b);
        }
        break;
        case 0x2a:  //neg
        {
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)(0-value_b));
        }
        break;
        case 0x2b:  //or
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a|value_b);
        }
        break;
        case 0x2c:  //not
        {
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,~value_b);
        }
        break;
        case 0x2d:  //ashr
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)((int32_t)value_a>>(value_b&0x1F)));
        }
        break;
        case 0x2e:  //xor
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,value_a^value_b);
        }
        break;
        case 0x2f:  //mul
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            hs_moxie_core_el_register_write(core,reg_a,(uint32_t)((uint64_t)value_a*(uint64_t)value_b));
        }
        break;
        case 0x30:  //swi
        {
            hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_SWI_NUMBER,imm32);
            hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_CAUSE,HS_MOXIE_CORE_EL_EXCEPTION_SWI);
            bool handled=false;
            if(core->io!=NULL)
            {
                handled=core->io(core,(uint32_t)HS_MOXIE_CORE_EL_IO_SWI|(uint32_t)HS_MOXIE_CORE_EL_IO_OPERATOR_CORE,imm32,NULL,0,core->usr);
            }
            if(!handled)
            {
                hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_SWI,imm32);
                faulted=true;
            }
            else
            {
                /*
                 * swi已由外部服务处理，清理swi上下文并继续正常运行。
                 */
                hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_EXCEPTION_CAUSE,0);
                hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_SWI_NUMBER,0);
            }
        }
        break;
        case 0x31:  //div
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            if((int32_t)value_a==INT32_MIN && (int32_t)value_b==-1)
            {
                hs_moxie_core_el_register_write(core,reg_a,(uint32_t)INT32_MIN);
            }
            else
            {
                hs_moxie_core_el_register_write(core,reg_a,(uint32_t)((int32_t)value_a/(int32_t)value_b));
            }
        }
        break;
        case 0x32:  //udiv
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value_a/value_b);
        }
        break;
        case 0x33:  //mod
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            if((int32_t)value_a==INT32_MIN && (int32_t)value_b==-1)
            {
                hs_moxie_core_el_register_write(core,reg_a,0);
            }
            else
            {
                hs_moxie_core_el_register_write(core,reg_a,(uint32_t)((int32_t)value_a%(int32_t)value_b));
            }
        }
        break;
        case 0x34:  //umod
        {
            value_a=hs_moxie_core_el_register_read(core,reg_a);
            value_b=hs_moxie_core_el_register_read(core,reg_b);
            if(value_b==0)
            {
                hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_DIV0,instruction);
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value_a%value_b);
        }
        break;
        case 0x35:  //brk
        {
            hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_BREAK,instruction);
            faulted=true;
        }
        break;
        case 0x36:  //ldo.b
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_b)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            uint8_t value=0;
            if(!hs_moxie_core_el_load_u8(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x37:  //sto.b
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_a)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            if(!hs_moxie_core_el_store_u8(core,address,(uint8_t)hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        case 0x38:  //ldo.s
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_b)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            uint16_t value=0;
            if(!hs_moxie_core_el_load_u16(core,address,&value))
            {
                faulted=true;
                break;
            }
            hs_moxie_core_el_register_write(core,reg_a,value);
        }
        break;
        case 0x39:  //sto.s
        {
            uint32_t address=hs_moxie_core_el_register_read(core,reg_a)+(uint32_t)hs_moxie_core_el_sign_extend_16(imm16);
            if(!hs_moxie_core_el_store_u16(core,address,(uint16_t)hs_moxie_core_el_register_read(core,reg_b)))
            {
                faulted=true;
            }
        }
        break;
        default:
        {
            hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_BAD,instruction);
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
            hs_moxie_core_el_register_write(core,reg_a,hs_moxie_core_el_register_read(core,reg_a)+immediate);
        }
        break;
        case 0x01:  //dec
        {
            hs_moxie_core_el_register_write(core,reg_a,hs_moxie_core_el_register_read(core,reg_a)-immediate);
        }
        break;
        case 0x02:  //gsr
        {
            hs_moxie_core_el_register_write(core,reg_a,hs_moxie_core_el_sreg_read(core,immediate));
        }
        break;
        case 0x03:  //ssr
        {
            hs_moxie_core_el_sreg_write(core,immediate,hs_moxie_core_el_register_read(core,reg_a));
        }
        break;
        default:
        {
            hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_BAD,instruction);
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
        int32_t offset=hs_moxie_core_el_sign_extend_10(instruction&0x03FF)*2;
        bool take_branch=false;
        switch(opcode)
        {
        case 0x00:  //beq
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_EQ)!=0);
        }
        break;
        case 0x01:  //bne
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_EQ)==0);
        }
        break;
        case 0x02:  //blt
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_LT)!=0);
        }
        break;
        case 0x03:  //bgt
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_GT)!=0);
        }
        break;
        case 0x04:  //bltu
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_LTU)!=0);
        }
        break;
        case 0x05:  //bgtu
        {
            take_branch=((core->cc&HS_MOXIE_CORE_EL_CC_GTU)!=0);
        }
        break;
        case 0x06:  //bge
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_EL_CC_GT|HS_MOXIE_CORE_EL_CC_EQ))!=0);
        }
        break;
        case 0x07:  //ble
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_EL_CC_LT|HS_MOXIE_CORE_EL_CC_EQ))!=0);
        }
        break;
        case 0x08:  //bgeu
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_EL_CC_GTU|HS_MOXIE_CORE_EL_CC_EQ))!=0);
        }
        break;
        case 0x09:  //bleu
        {
            take_branch=((core->cc&(HS_MOXIE_CORE_EL_CC_LTU|HS_MOXIE_CORE_EL_CC_EQ))!=0);
        }
        break;
        default:
        {
            hs_moxie_core_el_fault_set(core,HS_MOXIE_CORE_EL_EXCEPTION_BAD,instruction);
            faulted=true;
        }
        break;
        }

        if(!faulted && take_branch)
        {
            next_pc=pc+2+(uint32_t)offset;
        }
    }

    hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_INSTRUCTION_EXIT,pc,(uint8_t *)&trace_instruction,sizeof(trace_instruction));

    if(faulted)
    {
        hs_moxie_core_el_exception_enter(core,core->fault_cause,core->fault_value,pc);
    }
    else if(core->flags.halted==0)
    {
        hs_moxie_core_el_pc_write(core,next_pc);
    }
}

size_t hs_moxie_core_el_size(void)
{
    return sizeof(hs_moxie_core_el_t);
}

hs_moxie_core_el_t *hs_moxie_core_el_init(void *mem,hs_moxie_core_el_io_t io,void *usr)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_moxie_core_el_t *ret=(hs_moxie_core_el_t *)mem;
    memset(ret,0,sizeof(hs_moxie_core_el_t));
    ret->io=io;
    ret->usr=usr;
    hs_moxie_core_el_reset(ret);
    return ret;
}

void hs_moxie_core_el_tick(hs_moxie_core_el_t * core,size_t cycles)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }

    while(cycles--)
    {
        hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_TICK_ENTER,hs_moxie_core_el_pc_read(core),(uint8_t *)&cycles,sizeof(cycles));

        bool exception_taken=false;
        if(core->flags.exception_pending!=0)
        {
            uint32_t cause=core->exception_cause;
            uint32_t value=core->exception_value;
            core->flags.exception_pending=0;
            hs_moxie_core_el_exception_enter(core,cause,value,hs_moxie_core_el_pc_read(core));
            exception_taken=true;
        }

        if(core->flags.halted==0 && !exception_taken)
        {
            exception_taken=hs_moxie_core_el_interrupt_process(core);
        }

        if(core->flags.halted==0 && !exception_taken)
        {
            hs_moxie_core_el_exec(core);
        }

        hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_TICK_EXIT,hs_moxie_core_el_pc_read(core),(uint8_t *)&cycles,sizeof(cycles));

        if(core->flags.halted!=0)
        {
            break;
        }
    }
}

void hs_moxie_core_el_reset(hs_moxie_core_el_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_moxie_core_el_io_t io=core->io;
        void *usr=core->usr;
        memset(core,0,sizeof(hs_moxie_core_el_t));
        core->io=io;
        core->usr=usr;

        {
            uint32_t value=0;
            hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_RESET,0,(uint8_t *)&value,sizeof(value));
        }

        {
            uint8_t data[4];
            uint32_t entry=0;
            if(hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_ENTRY_PC_READ,0,data,sizeof(data)))
            {
                entry=hs_moxie_core_el_read_u32(core,data);
            }
            hs_moxie_core_el_pc_write(core,entry);
        }

        {
            uint8_t data[4];
            uint32_t sp=0;
            if(hs_moxie_core_el_io_call(core,(uint32_t)HS_MOXIE_CORE_EL_IO_INITIAL_SP_READ,0,data,sizeof(data)))
            {
                sp=hs_moxie_core_el_read_u32(core,data);
            }
            if(sp!=0)
            {
                hs_moxie_core_el_register_write(core,1,sp);
                hs_moxie_core_el_sreg_write(core,HS_MOXIE_CORE_EL_SREG_SIM_RETURN_LEN,sp);
            }
        }
    }
}

bool hs_moxie_core_el_io(hs_moxie_core_el_t *core,hs_moxie_core_el_io_opt_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,(uint32_t)opt|(uint32_t)HS_MOXIE_CORE_EL_IO_OPERATOR_EXTERN,address,data,len,core->usr);
    }
    return false;
}

bool hs_moxie_core_el_exception_raise(hs_moxie_core_el_t *core,uint32_t cause,uint32_t value)
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

bool hs_moxie_core_el_exception_clear(hs_moxie_core_el_t *core)
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

bool hs_moxie_core_el_interrupt_raise(hs_moxie_core_el_t *core,uint32_t value)
{
    if(core==NULL || core->flags.interrupt_pending!=0 || core->flags.halted!=0)
    {
        return false;
    }
    core->interrupt_value=value;
    core->flags.interrupt_pending=1;
    return true;
}

bool hs_moxie_core_el_interrupt_clear(hs_moxie_core_el_t *core)
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

bool hs_moxie_core_el_halted(hs_moxie_core_el_t *core)
{
    if(core==NULL)
    {
        return false;
    }
    return core->flags.halted!=0;
}

uint32_t hs_moxie_core_el_condition_code(hs_moxie_core_el_t *core)
{
    if(core==NULL)
    {
        return 0;
    }
    return core->cc;
}
