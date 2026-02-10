/***************************************************************
 * Name:      hioctl.h
 * Purpose:   声明hioctl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HIOCTL_H__
#define __HIOCTL_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_SYS_IOCTL_H)
#include "sys/ioctl.h"
#endif



/** \brief IO控制.
 * 注意：在有操作系统的情况下，仍然推荐使用原操作系统的ioctl
 * \param fd int 文件描述符
 * \param op unsigned long 操作
 * \param ... 其它参数.一般有三种情况，无参数，无符号整型，指针
 * \return int 成功返回0
 *
 */
int hioctl(int fd, unsigned long op, ...);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HIOCTL_H__
