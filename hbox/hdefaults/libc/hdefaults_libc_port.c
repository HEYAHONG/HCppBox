/***************************************************************
 * Name:      hdefaults_libc_port.c
 * Purpose:   实现hdefaults_libc_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults_libc_port.h"

/*
 * 默认优化级别为0
 */
#ifndef HDEFAULTS_LIBC_OPTIMIZE_LEVEL
#define HDEFAULTS_LIBC_OPTIMIZE_LEVEL     0
#endif // HDEFAULTS_LIBC_OPTIMIZE_LEVEL

/*
 * hlibc
 */
#if !defined(HLIBC_NO_IMPLEMENTATION)
#if !defined(HLIBC_NO_ENV)
#include "hlibc/env/hlibc_env.c"
#endif
#if !defined(HLIBC_NO_ATOMIC_FLAG)
#include "hlibc/stdatomic/hlibc_atomic_flag.c"
#endif
#if !defined(HLIBC_NO_TIME)
#include "hlibc/time/hlibc_time.c"
#endif
#else
#ifdef HDEFAULTS_LIBC_TINY
#undef HDEFAULTS_LIBC_TINY
#endif // HDEFAULTS_LIBC_TINY
#define HDEFAULTS_LIBC_TINY 1
#endif

/*
 * libc包装
 */

/*
 * stdio
 */
#include "wrapper/stdio/hputchar.c"
#include "wrapper/stdio/hgetchar.c"
#include "wrapper/stdio/hfputs.c"
#include "wrapper/stdio/hvfprintf.c"
#include "wrapper/stdio/hfprintf.c"
#include "wrapper/stdio/hputs.c"
#include "wrapper/stdio/hferror.c"
#include "wrapper/stdio/hfread.c"
#include "wrapper/stdio/hfwrite.c"

/*
 * stdlib
 */
#include "wrapper/stdlib/hgetenv.c"
#include "wrapper/stdlib/hmalloc.c"
#include "wrapper/stdlib/hfree.c"
#include "wrapper/stdlib/hcalloc.c"
#include "wrapper/stdlib/hrealloc.c"
#include "wrapper/stdlib/habort.c"
#include "wrapper/stdlib/hexit.c"

/*
 * time
 */
#include "wrapper/time/htime.c"
#include "wrapper/time/hclock.c"

/*
 * string
 */
#include "wrapper/string/hstrcmp.c"
#include "wrapper/string/hstrncmp.c"
#include "wrapper/string/hstrlen.c"
#include "wrapper/string/hmemset.c"

/*
 * stdatomic
 */
#include "wrapper/stdatomic/hstdatomic_common.c"
#include "wrapper/stdatomic/hatomic_flag.c"

/*
 * threads
 */
#include "wrapper/threads/hthreads_common.c"

/*
 * posix标准中的函数
 */
#include "wrapper/posix/stdlib/hsetenv.c"
#include "wrapper/posix/stdlib/hunsetenv.c"

/*
 * 非标扩展
 */
#include "wrapper/nonstandard/stdatomic/hatomic_int.c"

void hdefaults_libc_port_init(void)
{
#ifndef HDEFAULTS_LIBC_TINY
#if HDEFAULTS_LIBC_OPTIMIZE_LEVEL > 0
    hlibc_env_init();
#endif // HDEFAULTS_LIBC_OPTIMIZE_LEVEL
#endif // HDEFAULTS_LIBC_TINY
}

void hdefaults_libc_port_loop(void)
{
#ifndef HDEFAULTS_LIBC_TINY

#endif // HDEFAULTS_LIBC_TINY
}

