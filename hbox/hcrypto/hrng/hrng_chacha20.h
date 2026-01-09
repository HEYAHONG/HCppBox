/***************************************************************
 * Name:      hrng_chacha20.h
 * Purpose:   声明hrng_chacha20接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRNG_CHACHA20_H__
#define __HRNG_CHACHA20_H__
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#include "../hchacha20.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hrng_chacha20_context;
typedef struct hrng_chacha20_context hrng_chacha20_context_t;
struct hrng_chacha20_context
{
    bool                ctx_init;
    hchacha20_context_t ctx;
};

extern hrng_chacha20_context_t hrng_chacha20_context_default;

/** \brief 使用chacha20算法产生随机数
 *
 * \param ctx hrng_chacha20_context_t * 上下文，当此参数为NULL时此函数不是线程安全的。
 * \param buffer void* 缓冲区
 * \param buffer_length size_t 缓冲区长度
 *
 */
void   hrng_chacha20_rand(hrng_chacha20_context_t *ctx,void *buffer,size_t buffer_length);

/** \brief chacha20算法随机数种子
 *
 * \param ctx hrng_chacha20_context_t* 上下文，当此参数为NULL时此函数不是线程安全的。
 * \param key[32] const uint8_t key,必须32字节
 * \param nonce[12] const uint8_t nonce,必须12字节
 * \param conuter uint32_t conuter
 * \return bool 是否成功
 *
 */
bool   hrng_chacha20_srand(hrng_chacha20_context_t *ctx,const uint8_t key[32],const uint8_t nonce[12],uint32_t conuter);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRNG_CHACHA20_H__
