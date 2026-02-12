/***************************************************************
 * Name:      hlibc_stdio.h
 * Purpose:   声明hlibc_stdio接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHLIBC_STDIO_H__
#define __HHLIBC_STDIO_H__
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


int hlibc_getchar(void);
int hlibc_putchar(int ch);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_STDIO_H__
