/***************************************************************
 * Name:      cpu.c
 * Purpose:   实现cpu接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HOPENBLT_CPU_IMPLEMENTATION  1

#include "boot.h"
#include "hbox.h"

#if !defined(HOPENBLT_CPU_FUNCTION_ATTRIBUTE)
#define HOPENBLT_CPU_FUNCTION_ATTRIBUTE __WEAK
#endif

#if !defined(HOPENBLT_CPU_NO_IMPLEMENTATION)

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuInit(void)
{

}

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuStartUserProgram(void)
{
    /*
     * 启动用户程序，注意：若需返回，需要恢复此函数中可能进行的操作（如关闭中断）。
     * 具体可见openblt的模板
     */
}

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuMemCopy(blt_addr dest, blt_addr src, blt_int16u len)
{
    memcpy((void *)dest,(void *)src,len);
}

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuMemSet(blt_addr dest, blt_int8u value, blt_int16u len)
{
    memset((void *)dest,value,len);
}

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuIrqDisable(void)
{
    hdefaults_mutex_lock(NULL);
}

HOPENBLT_CPU_FUNCTION_ATTRIBUTE
void CpuIrqEnable(void)
{
    hdefaults_mutex_unlock(NULL);
}

#endif


