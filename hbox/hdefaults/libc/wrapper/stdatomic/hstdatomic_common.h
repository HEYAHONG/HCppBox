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

enum hmemory_order
{
        hmemory_order_relaxed = 0,
        hmemory_order_consume ,
        hmemory_order_acquire ,
        hmemory_order_release ,
        hmemory_order_acq_rel ,
        hmemory_order_seq_cst
};


typedef enum hmemory_order hmemory_order_t;


/** \brief 获取C标准对应的memory_order值
 *
 * \param __order hmemory_order_t
 * \return int C标准的枚举值
 *
 */
int hmemory_order_cstd(hmemory_order_t __order);


/**
 * \brief 定义hatomic
 */
#if defined(__cplusplus)
#if defined(HAVE_CXX_ATOMIC) && !defined(hatomic)
#if !defined(HAVE_ATOMIC)
#define HAVE_ATOMIC 1
#endif
#define hatomic(T) std::atomic<T>
#endif
#else
#if defined(HAVE_C_ATOMIC) && !defined(hatomic)
#if !defined(HAVE_ATOMIC)
#define HAVE_ATOMIC 1
#endif
#define hatomic(T) _Atomic T
#endif
#endif
#if !defined(hatomic)
#define hatomic(T) T
#endif

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTDATOMIC_COMMON_H__
