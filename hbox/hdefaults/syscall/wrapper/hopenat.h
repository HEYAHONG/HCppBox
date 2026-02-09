/***************************************************************
 * Name:      hopenat.h
 * Purpose:   声明hopenat接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HOPENAT_H__
#define __HOPENAT_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hopen.h"


/** \brief 打开文件
 *
 * \param filename const char* 文件名
 * \param dirfd int 指定的文件描述符
 * \param oflag int 打开文件标志
 * \param ... 可选参数，一般情况下只有mode参数(类型为unsigned int)。
 * \return int 文件描述符，-1表示出错
 *
 */
int hopenat(int dirfd,const char * filename,int oflag,...);

/*
 * 64位版本同openat
 */
#define hopenat64 hopenat

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HOPENAT_H__
