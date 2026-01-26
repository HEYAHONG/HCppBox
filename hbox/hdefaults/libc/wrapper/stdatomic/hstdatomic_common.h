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

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSTDATOMIC_COMMON_H__
