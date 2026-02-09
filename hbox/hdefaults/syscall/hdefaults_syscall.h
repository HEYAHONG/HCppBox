/***************************************************************
 * Name:      hdefaults_syscall.h
 * Purpose:   声明hdefaults_syscall接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHDEFAULTS_SYSCALL_H__
#define __HHDEFAULTS_SYSCALL_H__
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef  intptr_t (*hdefaults_syscall_function_t)(uintptr_t number,va_list va_list_args);

/** \brief 查找系统调用函数
 *
 * \param number uintptr_t 系统调用号
 * \return hdefaults_syscall_function_t 系统调用函数
 *
 */
hdefaults_syscall_function_t hdefaults_syscall_function_find(uintptr_t number);



/*
 * 系统调用包装
 */
#include "wrapper/hgettimeofday.h"
#include "wrapper/hsettimeofday.h"
#include "wrapper/hgetrandom.h"
#include "wrapper/hmmap.h"
#include "wrapper/hmsync.h"
#include "wrapper/hmunmap.h"
#include "wrapper/hmprotect.h"
#include "wrapper/hclose.h"
#include "wrapper/hread.h"
#include "wrapper/hwrite.h"
#include "wrapper/hlseek.h"
#include "wrapper/hopen.h"
#include "wrapper/hopenat.h"
#include "wrapper/hclock_getres.h"
#include "wrapper/hclock_gettime.h"
#include "wrapper/hclock_settime.h"





/*
 * hsyscall
 */
#include "hsyscall/time/hsyscall_time.h"
#include "hsyscall/random/hsyscall_random.h"
#include "hsyscall/file/hsyscall_file.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHDEFAULTS_SYSCALL_H__
