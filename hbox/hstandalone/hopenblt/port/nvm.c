/***************************************************************
 * Name:      nvm.c
 * Purpose:   实现nvm接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HOPENBLT_NVM_IMPLEMENTATION  1

#include "boot.h"
#include "hbox.h"

#if !defined(HOPENBLT_NVM_FUNCTION_ATTRIBUTE)
#define HOPENBLT_NVM_FUNCTION_ATTRIBUTE __WEAK
#endif

#if !defined(HOPENBLT_NVM_NO_IMPLEMENTATION)

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
void     NvmInit(void)
{

}

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
blt_bool NvmWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
    (void)addr;
    (void)len;
    (void)data;
    return BLT_FALSE;
}

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
blt_bool NvmErase(blt_addr addr, blt_int32u len)
{
    (void)addr;
    (void)len;
    return BLT_FALSE;
}

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
blt_bool NvmVerifyChecksum(void)
{
    /*
     * 此函数返回FALSE时bootloader不会启动用户程序
     */
    return BLT_FALSE;
}

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
blt_addr NvmGetUserProgBaseAddress(void)
{
    return 0;
}

HOPENBLT_NVM_FUNCTION_ATTRIBUTE
blt_bool NvmDone(void)
{
    return BLT_FALSE;
}


#endif


