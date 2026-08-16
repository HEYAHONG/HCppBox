/***************************************************************
 * Name:      hmkdir.h
 * Purpose:   声明hmkdir接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMKDIR_H__
#define __HMKDIR_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hopen.h"

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_SYS_STAT_H)
#include "sys/stat.h"
#endif


/** \brief 创建文件夹
 *
 * \param pathname const char* 文件夹名称
 * \param mode unsigend long 模式，仅对某些操作系统有效
 * \return int 错误代码，0=成功，-1表示出错
 *
 */
int hmkdir(const char * pathname,unsigned long mode);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMKDIR_H__
