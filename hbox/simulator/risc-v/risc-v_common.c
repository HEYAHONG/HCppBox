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
