/***************************************************************
 * Name:      hrng_linearcongruential.h
 * Purpose:   声明hrng_linearcongruential接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRNG_LINEARCONGRUENTIAL_H__
#define __HRNG_LINEARCONGRUENTIAL_H__
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hrng_linearcongruential_context;
typedef struct hrng_linearcongruential_context hrng_linearcongruential_context_t;
struct hrng_linearcongruential_context
{
    uint64_t seed_number;               /**< 种子 */
};

extern hrng_linearcongruential_context_t hrng_linearcongruential_context_rand48;        /**< rand48上下文 */

struct hrng_linearcongruential_config;
typedef struct hrng_linearcongruential_config hrng_linearcongruential_config_t;
struct hrng_linearcongruential_config
{
    uint64_t mult_number;               /**< 乘数 */
    uint64_t add_number;                /**< 加数 */
    uint64_t mod_number;                /**< 模数 */
};


extern const hrng_linearcongruential_config_t hrng_linearcongruential_config_rand48;    /**< rand48配置 */

/** \brief 线性同余随机数更新
 *
 * \param cfg const hrng_linearcongruential_config_t*  配置
 * \param ctx hrng_linearcongruential_context_t*       上下文,当此参数为NULL时此函数不是线程安全的。
 * \return uint64_t 当前随机数
 *
 */
uint64_t hrng_linearcongruential_update(const hrng_linearcongruential_config_t *cfg,hrng_linearcongruential_context_t *ctx);

#ifndef HRNG_LINEARCONGRUENTIAL_RAND48_RANDMAX
#define HRNG_LINEARCONGRUENTIAL_RAND48_RANDMAX   (INT_MAX)
#endif // HRNG_LINEARCONGRUENTIAL_RAND48_RANDMAX


/** \brief RAND48随机数生成
 * 注意：此函数不是线程安全的
 * \return long 0~HRNG_LINEARCONGRUENTIAL_RAND48_RANDMAX的随机数
 *
 */
long hrng_linearcongruential_rand48_rand(void);


/** \brief RAND48随机数种子
 * 注意：此函数不是线程安全的
 * \param seed_number uint64_t种子
 *
 */
void hrng_linearcongruential_rand48_srand(uint64_t seed_number);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRNG_LINEARCONGRUENTIAL_H__
