/***************************************************************
 * Name:      risc-v_common.h
 * Purpose:   实现risc-v common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_common.h"

size_t hs_risc_v_common_instruction_length(uint32_t instruction)
{
    if((instruction&0x707F)==0x707F)
    {
        //192位及更高位数
        return 192/8;
    }
    else if((instruction&0x7F)==0x7F)
    {
        //80位指令及更高位数
        return (80+16*((instruction>>12)&0x7))/8;
    }
    else if((instruction&0x7F)==0x3F)
    {
        //64位指令
        return 64/8;
    }
    else if((instruction&0x3F)==0x1F)
    {
        //48位指令
        return 48/8;
    }
    else if((instruction&0x3)==0x3)
    {
        //32位指令
        return 32/8;
    }
    else
    {
        //16位指令
        return 16/8;
    }
}


int32_t hs_risc_v_common_sign_extend_uint12(uint16_t data)
{
    union
    {
        struct
        {
            uint16_t val:12;
        } unsigned_data;
        struct
        {
            int16_t val:12;
        } signed_data;
    } cvt;
    cvt.unsigned_data.val=data;
    return cvt.signed_data.val;
}

int64_t hs_risc_v_common_sign_extend_uint32(uint32_t data)
{
    union
    {
        struct
        {
            uint64_t val:32;
        } unsigned_data;
        struct
        {
            int64_t val:32;
        } signed_data;
    } cvt;
    cvt.unsigned_data.val=data;
    return cvt.signed_data.val;
}

bool hs_risc_v_common_instruction_set_sets_has_rv32g(uint32_t sets)
{
    uint32_t rv32g=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32G;
    if((sets&0xF)==(rv32g&0xF))
    {
        //基本指令集相同,比较指令集扩展
        if(((sets&rv32g)&0xFFFFFFF0)==(rv32g&0xFFFFFFF0))
        {
            return true;
        }
    }
    return false;
}

bool hs_risc_v_common_instruction_set_sets_has_rv64g(uint32_t sets)
{
    uint32_t rv64g=HS_RISC_V_COMMON_INSTRUCTION_SET_RV64G;
    if((sets&0xF)==(rv64g&0xF))
    {
        //基本指令集相同,比较指令集扩展
        if(((sets&rv64g)&0xFFFFFFF0)==(rv64g&0xFFFFFFF0))
        {
            return true;
        }
    }
    return false;
}

bool hs_risc_v_common_instruction_set_sets_has_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set)
{
    if(instruction_set >= (1ULL<<(4)))
    {
        //扩展指令集通过位来判断
        return (instruction_set&sets)!=0;
    }
    else
    {
        //基本指令集需要通过等于来判断
        uint32_t instruction_base_set=(sets&0x0000000F);
        return instruction_base_set==(uint32_t)instruction_set;
    }
}

uint32_t hs_risc_v_common_instruction_set_sets_format(uint32_t sets)
{
    {
        //基本指令集
        uint32_t instruction_base_set=(sets&0x0000000F);
        sets&=(~0x0000000F);
        switch(instruction_base_set)
        {
        case HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I:
        case HS_RISC_V_COMMON_INSTRUCTION_SET_RV32E:
        case HS_RISC_V_COMMON_INSTRUCTION_SET_RV64I:
        case HS_RISC_V_COMMON_INSTRUCTION_SET_RV64E:
        case HS_RISC_V_COMMON_INSTRUCTION_SET_RV128I:
        {

        }
        break;
        default:
        {
            //默认基本指令集为RV32I
            instruction_base_set=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I;
        }
        break;
        }
        sets|=instruction_base_set;
    }

    {
        //扩展指令集依赖添加(默认使用32位的枚举值，64位及更高位通常枚举值相同)


        //指令扩展"D"依赖指令扩展"F"
        if(hs_risc_v_common_instruction_set_sets_has_set(sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D))
        {
            sets|=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F;
        }

        //指令扩展"F"依赖指令扩展"Zicsr"
        if(hs_risc_v_common_instruction_set_sets_has_set(sets,HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F))
        {
            sets|=HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR;
        }
    }
    return sets;
}

uint32_t hs_risc_v_common_instruction_set_sets_set_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set)
{
    if(instruction_set >= (1ULL<<(4)))
    {
        sets|=instruction_set;
    }
    else
    {
        sets &=0xFFFFFFF0;
        sets |=instruction_set;
    }

    return hs_risc_v_common_instruction_set_sets_format(sets);
}

uint32_t hs_risc_v_common_instruction_set_sets_clear_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set)
{
    if(instruction_set >= (1ULL<<(4)))
    {
        sets&=((~instruction_set)&0xFFFFFFF0);
    }
    return hs_risc_v_common_instruction_set_sets_format(sets);
}

