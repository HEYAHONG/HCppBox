/***************************************************************
 * Name:      hrng_mt.h
 * Purpose:   声明hrng_mt接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRNG_MT_H__
#define __HRNG_MT_H__
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HRNG_MT_N
#define HRNG_MT_N           (624)
#endif // HRNG_MT_N

#ifndef HRNG_MT_M
#define HRNG_MT_M           (397)
#endif // HRNG_MT_M

#define HRNG_MT_MATRIX_A            0x9908b0dfUL
#define HRNG_MT_UPPER_MASK          0x80000000UL
#define HRNG_MT_LOWER_MASK          0x7fffffffUL

struct hrng_mt_context;
typedef struct hrng_mt_context hrng_mt_context_t;
struct hrng_mt_context
{
    uint32_t mt[HRNG_MT_N];
    size_t   mti;
    uint32_t mag01[2];
};

/*
 * 上下文初始化参数
 */
#define HRNG_MT_CONTEXT_INIT    {\
                                    {0},\
                                    HRNG_MT_N+1,\
                                    {0*HRNG_MT_MATRIX_A,1*HRNG_MT_MATRIX_A}\
                                };

/** \brief 梅森旋转法设置随机数种子
 * 注意:使用时至少需要设置一次种子
 *
 * \param ctx hrng_mt_context_t* 上下文，当此参数为NULL时此函数不是线程安全的。
 * \param seed uint32_t 随机数种子
 *
 */
void hrng_mt_srand(hrng_mt_context_t *ctx,uint32_t seed);


/** \brief 梅森旋转法生成随机数
 *
 * \param ctx hrng_mt_context_t* 当此参数为NULL时此函数不是线程安全的。
 * \return uint32_t 随机数
 *
 */
uint32_t hrng_mt_rand(hrng_mt_context_t *ctx);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRNG_MT_H__
