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
        //扩展指令集依赖添加

    }
    return sets;
}
