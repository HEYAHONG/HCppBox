/***************************************************************
 * Name:      risc-v_core_rv32.c
 * Purpose:   实现risc-v_core_rv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_core_rv32.h"
#ifndef  RISCV_ENCODING_H
#include "encoding.out.h"
#endif
#include "h3rdparty.h"
#include H3RDPARTY_SOFTFP_HEADER
#include H3RDPARTY_SOFTFLOAT_HEADER

struct hs_risc_v_core_rv32
{
    hs_risc_v_core_rv32_io_t    io;
    void*                       usr;
    uint32_t                    instruction_sets; /**< 支持的扩展指令集 */
    uint32_t                    exception_pending; /**< 等待执行的异常（狭义的异常） */
    uint32_t                    interrupt_pending; /**< 等待执行的中断 */
};

size_t hs_risc_v_core_rv32_size(void)
{
    return sizeof(hs_risc_v_core_rv32_t);
}


hs_risc_v_core_rv32_t *hs_risc_v_core_rv32_init(void *mem,hs_risc_v_core_rv32_io_t io,void *usr)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_risc_v_core_rv32_t *ret=(hs_risc_v_core_rv32_t *)mem;
    ret->io=io;
    ret->usr=usr;
    hs_risc_v_core_rv32_reset(ret);
    return ret;
}

static uint32_t hs_risc_v_core_rv32_pc_read(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,0,pc.bytes,sizeof(pc.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        return pc.value;
    }
    return 0;
}

static void hs_risc_v_core_rv32_pc_write(hs_risc_v_core_rv32_t *core,uint32_t pc_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=pc_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,0,pc.bytes,sizeof(pc.bytes),core->usr);
    }
}


static uint32_t hs_risc_v_core_rv32_x_register_read(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(address==0)
    {
        return 0;
    }
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static void hs_risc_v_core_rv32_x_register_write(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(address==0)
    {
        return;
    }
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

static uint32_t hs_risc_v_core_rv32_f_register_read32(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static void hs_risc_v_core_rv32_f_register_write32(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_F_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

static uint32_t hs_risc_v_core_rv32_csr_read_default_value(hs_risc_v_core_rv32_t *core,size_t address)
{
    uint32_t reg_value=0;
    switch(address)
    {
    case CSR_MISA:
    {
        reg_value |= (1ULL<<(30));         //XLEN=32
        reg_value |= (1ULL<<(8));          //RV32I
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A))
        {
            reg_value |= (1ULL<<(0));          //RV32A
        }
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M))
        {
            reg_value |= (1ULL<<(12));          //RV32M
        }
    }
    break;
    case CSR_MVENDORID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MARCHID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MIMPID:
    {
        reg_value=0;                        //0=未实现
    }
    break;
    case CSR_MHARTID:
    {
        reg_value=0;                        //0=hart0,必须存在hartid为0的内核
    }
    break;
    case CSR_MSTATUS:
    {

    }
    break;
    case CSR_MSTATUSH:
    {

    }
    break;
    case CSR_MTVEC:
    {
        reg_value=0x00000001;           //基地址为0,向量模式
    }
    break;
    case CSR_MEDELEG:
    {

    }
    break;
    case CSR_MIDELEG:
    {

    }
    break;
    case CSR_MIP:
    {
        /*
         * 默认为内核等待执行的中断（不包括正在执行的中断）。如果涉及到比较复杂的中断系统，用户需要自行实现mip。
         */
        reg_value=core->interrupt_pending;
    }
    break;
    case CSR_MIE:
    {

    }
    break;
    default:
    {

    }
    break;
    }
    return 0;
}

static uint32_t hs_risc_v_core_rv32_csr_read(hs_risc_v_core_rv32_t *core,size_t address)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=hs_risc_v_core_rv32_csr_read_default_value(core,address);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CSR_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        return value.value;
    }
    return 0;
}

static void hs_risc_v_core_rv32_csr_write(hs_risc_v_core_rv32_t *core,size_t address,uint32_t reg_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t value;
        value.value=reg_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CSR_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,value.bytes,sizeof(value.bytes),core->usr);
    }
}

static uint32_t  hs_risc_v_core_rv32_instruction_read(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t instruction;
        instruction.value=0;
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,hs_risc_v_core_rv32_pc_read(core),instruction.bytes,sizeof(instruction.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(instruction);
        return instruction.value;
    }
    return 0;
}

#ifndef HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH
#define HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(NAME,CODE)      \
if(!is_instruction_processed)\
{\
    const hs_risc_v_opcodes_insn_t insn=HS_RISC_V_OPCODES_INSN_NAME(NAME);\
    if((instruction&insn.mask)==insn.match)\
    {\
        is_instruction_processed=true;\
        {\
            CODE;\
        }\
    }\
}

#endif // HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH

static void hs_risc_v_core_rv32_exec_exception_raise(hs_risc_v_core_rv32_t *core,int cause,uint32_t instruction_pc,uint32_t instruction)
{
    //设置标志位，下次指令执行时可能进入跳转相应处理函数。
    hs_risc_v_core_rv32_exception_raise(core,cause,false);
}

static bool hs_risc_v_core_rv32_exec_io(hs_risc_v_core_rv32_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        //默认为内核操作
        return core->io(core,opt|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_CORE,address,data,len,core->usr);
    }
    return false;
}

static void hs_risc_v_core_rv32_exec(hs_risc_v_core_rv32_t * core)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }


    uint32_t instruction= hs_risc_v_core_rv32_instruction_read(core);
    uint32_t pc=hs_risc_v_core_rv32_pc_read(core);
    core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ENTER | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
    uint32_t next_pc=pc+hs_risc_v_common_instruction_length(instruction);
    hs_risc_v_core_rv32_pc_write(core,next_pc);

    //指令是否正确处理，当指令被正确处理时，需要将此标志置位true
    bool is_instruction_processed=false;


    if((0x3&instruction)==0x3)
    {
        //RV32I/RV32E基本指令集

        /*
         *  使用低7位opcode初步分类
         */
        switch(0x7F&instruction)
        {
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lw,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lh,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)value.s_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lb,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(int32_t)value.s_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lhu,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lbu,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)value.value);
            });
        }
        break;
        case  HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE :
        {
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            uint32_t imm=(((instruction >> 7)&((1ULL << (5))-1)) << (0))+(((instruction >> 25)&((1ULL << (7))-1)) << (5))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sw,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sh,
            {
                hs_risc_v_common_memory_halfword_t value;
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sb,
            {
                hs_risc_v_common_memory_byte_t value;
                value.value=rs2_value;
                //HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_BRANCH :
        {
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            uint32_t sb_imm=(((instruction >> 8)&((1ULL << (4))-1)) << (1))+(((instruction >> 25)&((1ULL << (6))-1)) << (5))+(((instruction >> 7)&((1ULL << (1))-1)) << (11))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(beq,
            {
                if(rs1_value==rs2_value)
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bne,
            {
                if(rs1_value!=rs2_value)
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(blt,
            {
                if(((int32_t)rs1_value)<((int32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bltu,
            {
                if(((uint32_t)rs1_value)<((uint32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bge,
            {
                if(((int32_t)rs1_value)>=((int32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(bgeu,
            {
                if(((uint32_t)rs1_value)>=((uint32_t)rs2_value))
                {
                    hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)sb_imm);
                }
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JALR :
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(jalr,
            {
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                hs_risc_v_core_rv32_pc_write(core,(rs1_value+(int32_t)i_imm)&(~(1ULL)));
                hs_risc_v_core_rv32_x_register_write(core,rd,next_pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_0:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_1:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_RESERVED :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MISC_MEM:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fence,
            {
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_FENCE_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(pause,
            {
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_PAUSE_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMADD:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JAL:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(jal,
            {
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                uint32_t ju_imm=(((instruction >> 21)&((1ULL << (10))-1)) << (1))+(((instruction >> 20)&((1ULL << (1))-1)) << (11))+(((instruction >> 12)&((1ULL << (8))-1)) << (12))+(((instruction&(1ULL<<(31)))!=0)?(0xFFF00000):0);
                hs_risc_v_core_rv32_pc_write(core,pc+(int32_t)ju_imm);
                hs_risc_v_core_rv32_x_register_write(core,rd,next_pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(addi,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int32_t)rs1_value)+((int32_t)i_imm));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slti,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((int32_t)rs1_value)<((int32_t)i_imm))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sltiu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((uint32_t)rs1_value)<((uint32_t)i_imm))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xori,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^i_imm);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ori,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|i_imm);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(andi,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&i_imm);
            });
            {
                uint32_t shamt=((instruction>>20)&0x1F);
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slli,
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value<<shamt);
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srli,
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value>>shamt);
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srai,
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,((int64_t)((int32_t)rs1_value))>>shamt);
                });
            }
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP  :
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(add,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value+rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sub,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value-rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sll,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value<<(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(slt,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((int32_t)rs1_value)<((int32_t)rs2_value))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sltu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,(((uint32_t)rs1_value)<((uint32_t)rs2_value))?1:0);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(xor,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value^rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(srl,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value>>(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sra,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((int64_t)((int32_t)rs1_value))>>(rs2_value&((sizeof(rs1_value)*8)-1)));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(or,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value|rs2_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(and,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,rs1_value&rs2_value);
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_SYSTEM:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ecall,
            {
                int cause=HS_RISC_V_OPCODES_EXCEPTION_NAME(CAUSE_MACHINE_ECALL);    //默认将执行MACHINE_ECALL
                hs_risc_v_core_rv32_exec_exception_raise(core,cause,pc,instruction);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ECALL_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(ebreak,
            {
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EBREAK_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
            });
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AUIPC:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(auipc,
            {
                uint32_t u_imm=(instruction&INSN_FIELD_IMM20);
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                hs_risc_v_core_rv32_x_register_write(core,rd,u_imm+pc);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LUI:
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lui,
            {
                uint32_t u_imm=(instruction&INSN_FIELD_IMM20);
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                hs_risc_v_core_rv32_x_register_write(core,rd,u_imm);
            })
        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_V:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_VE :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM_32:
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_32 :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_3 :
        {

        }
        break;
        case HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_4:
        {

        }
        break;
        default:
        {

        }
        break;
        }
    }
    else
    {
        //RVC扩展指令集
        if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C))
        {
            /*
             *  使用象限初步分类
             */
            switch(0x3&instruction)
            {
            case 0:                     //象限0
            {

            }
            break;
            case 1:                     //象限1
            {

            }
            break;
            case 2:                     //象限2
            {

            }
            break;
            default:                    //象限3（32位指令与更高位数指令）,理论上不会出现这种情况。
            {

            }
            break;
            }
        }
    }

    //Zifencei扩展指令集
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI)&& !is_instruction_processed)
    {
        HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fence_i,
        {
            hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_FENCE_I_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
        })
    }

    //Zicsr扩展指令集
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR) && !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_SYSTEM)
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t uimm=rs1;
            uint32_t csr=((instruction&INSN_FIELD_CSR)>>20);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrw,
            {
                if(rd!=0)
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
                hs_risc_v_core_rv32_csr_write(core,csr,rs1_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrs,
            {
                uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                if(rs1!=0)
                {
                    hs_risc_v_core_rv32_csr_write(core,csr,csr_value|rs1_value);
                }
                hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrc,
            {
                uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                if(rs1!=0)
                {
                    hs_risc_v_core_rv32_csr_write(core,csr,csr_value&(~rs1_value));
                }
                hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrwi,
            {
                if(rd!=0)
                {
                    uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                    hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
                }
                hs_risc_v_core_rv32_csr_write(core,csr,uimm);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrsi,
            {
                uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                if(uimm!=0)
                {
                    hs_risc_v_core_rv32_csr_write(core,csr,csr_value|uimm);
                }
                hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(csrrci,
            {
                uint32_t csr_value=hs_risc_v_core_rv32_csr_read(core,csr);
                if(uimm!=0)
                {
                    hs_risc_v_core_rv32_csr_write(core,csr,csr_value&(~uimm));
                }
                hs_risc_v_core_rv32_x_register_write(core,rd,csr_value);
            });
        }
    }



    //RV32M
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M)&& !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP)
        {
            //RV32M
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mul,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((int64_t)((int32_t)rs2_value)))));
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulh,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((int64_t)((int32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulhu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((uint64_t)((uint32_t)rs1_value))*((uint64_t)((uint32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(mulhsu,
            {
                hs_risc_v_core_rv32_x_register_write(core,rd,((uint64_t)(((int64_t)((int32_t)rs1_value))*((uint64_t)((uint32_t)rs2_value)))) >> 32);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(div,
            {
                int64_t lhs=(int32_t)rs1_value;
                int64_t rhs=(int32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)UINT64_MAX);
                }
                else if(lhs==INT64_MIN && rhs== -1)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)lhs);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs/rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(divu,
            {
                uint64_t lhs=(uint32_t)rs1_value;
                uint64_t rhs=(uint32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)UINT64_MAX);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs/rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(rem,
            {
                int64_t lhs=(int32_t)rs1_value;
                int64_t rhs=(int32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)lhs);
                }
                else if(lhs==INT64_MIN && rhs== -1)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)0);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs%rhs));
                }
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(remu,
            {
                uint64_t lhs=(uint32_t)rs1_value;
                uint64_t rhs=(uint32_t)rs2_value;
                if(rhs==0)
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(int64_t)(int32_t)rs1_value);
                }
                else
                {
                    hs_risc_v_core_rv32_x_register_write(core,rd,(uint32_t)(uint64_t)(lhs%rhs));
                }
            });
        }
    }

    //RV32A
    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A) && !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO)
        {
            uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
            uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
            uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
            uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(lr_w,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_LR,rs1_value,value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
            });
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(sc_w,
            {
                hs_risc_v_common_memory_word_t value;
                value.value=hs_risc_v_core_rv32_x_register_read(core,rs2);
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                bool have_reservation=hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_SC,rs1_value,value.bytes,sizeof(value.bytes));
                hs_risc_v_core_rv32_x_register_write(core,rd,have_reservation?0:0xFFFFFFFF);
            });
            {
                uint32_t rs2_value=hs_risc_v_core_rv32_x_register_read(core,rs2);
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoswap_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoadd_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.s_value+=(int32_t)rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoxor_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value^=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoand_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value&=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amoor_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    value.value|=rs2_value;
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomin_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.s_value > ((int32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomax_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.s_value < ((int32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amominu_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.value > ((uint32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
                HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(amomaxu_w,
                {
                    hs_risc_v_common_memory_word_t value;
                    value.value=0;
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_READ,rs1_value,value.bytes,sizeof(value.bytes));
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_x_register_write(core,rd,value.value);
                    if(value.value < ((uint32_t)rs2_value))
                    {
                        value.value=rs2_value;
                    }
                    HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                    hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_AMO_WRITE,rs1_value,value.bytes,sizeof(value.bytes));
                });
            }
        }
    }

    if(hs_risc_v_common_instruction_set_sets_has_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F) && !is_instruction_processed)
    {
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP)
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(flw,
            {
                uint32_t rd=((instruction&INSN_FIELD_RD)>>7);
                uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t i_imm=(((instruction >> 20)&((1ULL << (12))-1)) << (0))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                hs_risc_v_common_memory_word_t value;
                value.value=0;
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_READ,rs1_value+((int32_t)i_imm),value.bytes,sizeof(value.bytes));
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_f_register_write32(core,rd,value.value);
            });
        }
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP)
        {
            HS_RISC_V_CORE_RV32_EXEC_INSN_MATCH(fsw,
            {
                uint32_t rs1=((instruction&INSN_FIELD_RS1)>>15);
                uint32_t rs1_value=hs_risc_v_core_rv32_x_register_read(core,rs1);
                uint32_t rs2=((instruction&INSN_FIELD_RS2)>>20);
                uint32_t rs2_value=hs_risc_v_core_rv32_f_register_read32(core,rs2);
                uint32_t imm=(((instruction >> 7)&((1ULL << (5))-1)) << (0))+(((instruction >> 25)&((1ULL << (7))-1)) << (5))+(((instruction&(1ULL<<(31)))!=0)?(0xFFFFF000):0);
                hs_risc_v_common_memory_word_t value;
                value.value=rs2_value;
                HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(value);
                hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,rs1_value+((int32_t)imm),value.bytes,sizeof(value.bytes));
            });
        }
        if((instruction&0x7F)==HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP)
        {
            uint32_t fcsr=hs_risc_v_core_rv32_csr_read(core,CSR_FCSR);

            hs_risc_v_core_rv32_csr_write(core,CSR_FCSR,fcsr);
        }
    }

    if(!is_instruction_processed)
    {
        //指令未处理，可能是custom指令
        hs_risc_v_core_rv32_exec_io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_CUSTOM_INSTRUCTION_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction));
    }

    core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EXIT | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
}


void hs_risc_v_core_rv32_tick(hs_risc_v_core_rv32_t * core,size_t cycles)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }
    while(cycles--)
    {
        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_TICK_ENTER | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);

        hs_risc_v_core_rv32_exec(core);

        core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_TICK_EXIT | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);
    };
}


void hs_risc_v_core_rv32_reset(hs_risc_v_core_rv32_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        {
            hs_risc_v_core_rv32_io_t    io=core->io;
            void* usr=core->usr;
            memset(core,0,sizeof(hs_risc_v_core_rv32_t));
            core->usr=usr;
            core->io=io;
        }
        {
            //初始化指令集支持
            core->instruction_sets=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I;
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M);
            core->instruction_sets=hs_risc_v_common_instruction_set_sets_set_set(core->instruction_sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A);
        }
        {
            uint32_t val=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_RESET | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,(uint8_t *)&val,sizeof(val),core->usr);
        }
        {
            //初始化PC值
            hs_risc_v_common_memory_word_t pc;
            pc.value=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_ENTRY_PC_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,pc.bytes,sizeof(pc.bytes),core->usr);
            /*
             *   由于传入传出皆为字节数据，因此可不进行字节序修复
             */
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,pc.bytes,sizeof(pc.bytes),core->usr);
        }
        {
            //初始化SP值(X2)
            hs_risc_v_common_memory_word_t x;
            x.value=0;
            core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_INITIAL_SP_READ | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,0,x.bytes,sizeof(x.bytes),core->usr);
            if(x.value!=0)
            {
                /*
                 *   由于传入传出皆为字节数据，因此可不进行字节序修复
                 */
                core->io(core,(uint32_t)HS_RISC_V_CORE_RV32_IO_X_REGISTER_WRITE | (uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_HW,2,x.bytes,sizeof(x.bytes),core->usr);
            }
        }
    }
}

bool hs_risc_v_core_rv32_io(hs_risc_v_core_rv32_t *core,hs_risc_v_core_rv32_io_opt_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,opt|(uint32_t)HS_RISC_V_CORE_RV32_IO_OPERATOR_EXTERN,address,data,len,core->usr);
    }
    return false;
}

uint32_t hs_risc_v_core_rv32_sets_get(hs_risc_v_core_rv32_t *core)
{
    if(core==NULL)
    {
        return 0;
    }

    core->instruction_sets=hs_risc_v_common_instruction_set_sets_format(core->instruction_sets);

    return core->instruction_sets;
}

uint32_t hs_risc_v_core_rv32_sets_disable_set(hs_risc_v_core_rv32_t *core,hs_risc_v_common_instruction_set_t ins_set)
{
    if(core==NULL)
    {
        return 0;
    }

    core->instruction_sets=hs_risc_v_common_instruction_set_sets_clear_set(core->instruction_sets,ins_set);

    return core->instruction_sets;
}

bool hs_risc_v_core_rv32_exception_raise(hs_risc_v_core_rv32_t *core,int cause,bool interrupt)
{
    if(core==NULL || core->io == NULL)
    {
        return false;
    }
    if(cause > 31 || cause < 0)
    {
        return false;
    }
    if(interrupt)
    {
        core->interrupt_pending |= (1ULL<<cause);
    }
    else
    {
        core->exception_pending |= (1ULL<<cause);
    }
    return false;
}
