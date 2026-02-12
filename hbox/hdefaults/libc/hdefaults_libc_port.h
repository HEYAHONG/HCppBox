/***************************************************************
 * Name:      hdefaults_libc_port.h
 * Purpose:   声明hdefaults_libc_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_PORT_H__
#define __HDEFAULTS_LIBC_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * errno
 */
#include "wrapper/errno/herrno.h"

/*
 * stdio
 */
#include "wrapper/stdio/hputchar.h"
#include "wrapper/stdio/hgetchar.h"
#include "wrapper/stdio/hfputs.h"
#include "wrapper/stdio/hvfprintf.h"
#include "wrapper/stdio/hfprintf.h"
#include "wrapper/stdio/hputs.h"
#include "wrapper/stdio/hferror.h"
#include "wrapper/stdio/hfread.h"
#include "wrapper/stdio/hfwrite.h"

/*
 * stdlib
 */
#include "wrapper/stdlib/hgetenv.h"
#include "wrapper/stdlib/hmalloc.h"
#include "wrapper/stdlib/hfree.h"
#include "wrapper/stdlib/hcalloc.h"
#include "wrapper/stdlib/hrealloc.h"
#include "wrapper/stdlib/habort.h"
#include "wrapper/stdlib/hexit.h"

/*
 * time
 */
#include "wrapper/time/htime.h"
#include "wrapper/time/hclock.h"

/*
 * string
 */
#include "wrapper/string/hstrcmp.h"
#include "wrapper/string/hstrncmp.h"
#include "wrapper/string/hstrlen.h"
#include "wrapper/string/hmemset.h"

/*
 * stdatomic
 */
#include "wrapper/stdatomic/hstdatomic_common.h"
#include "wrapper/stdatomic/hatomic_flag.h"

/*
 * threads
 */
#include "wrapper/threads/hthreads_common.h"
#include "wrapper/threads/hthrd.h"
#include "wrapper/threads/hcall_once.h"
#include "wrapper/threads/hmtx.h"


/*
 * posix标准中的函数
 */

/*
 * stdlib
 */
#include "wrapper/posix/stdlib/hsetenv.h"
#include "wrapper/posix/stdlib/hunsetenv.h"


/*
 * hlibc
 */
#include "hlibc/env/hlibc_env.h"
#include "hlibc/stdatomic/hlibc_atomic_flag.h"
#include "hlibc/time/hlibc_time.h"
#include "hlibc/threads/hlibc_threads.h"
#include "hlibc/stdio/hlibc_stdio.h"

/*
 * 非标扩展
 */
#include "wrapper/nonstandard/stdatomic/hatomic_int.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDEFAULTS_LIBC_PORT_H__
