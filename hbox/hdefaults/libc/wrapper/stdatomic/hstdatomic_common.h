/***************************************************************
 * Name:      hstdatomic_common.h
 * Purpose:   声明hstdatomic_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTDATOMIC_COMMON_H__
#define __HSTDATOMIC_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hdefaults/hdefaults_libc.h"

#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)

enum hmemory_order
{
        hmemory_order_relaxed = 0,
        hmemory_order_consume ,
        hmemory_order_acquire ,
        hmemory_order_release ,
        hmemory_order_acq_rel ,
        hmemory_order_seq_cst
};

#else

enum hmemory_order
{
        hmemory_order_relaxed = memory_order_relaxed,
        hmemory_order_consume = memory_order_consume,
        hmemory_order_acquire = memory_order_acquire,
        hmemory_order_release = memory_order_release,
        hmemory_order_acq_rel = memory_order_acq_rel,
        hmemory_order_seq_cst = memory_order_seq_cst
};

#endif

typedef enum hmemory_order hmemory_order_t;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTDATOMIC_COMMON_H__
