/***************************************************************
 * Name:      hsm3.h
 * Purpose:   声明hsm3接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSM3_H__
#define __HSM3_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * SM3
 */
typedef uint8_t hsm3_sm3_t[32];

/*
 * SM3 消息块
 */
typedef uint8_t hsm3_message_block_t[64];

/*
 * SM3上下文
 */
typedef struct hsm3_context hsm3_context_t;
struct hsm3_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t F;
    uint32_t G;
    uint32_t H;
    hsm3_message_block_t buffer;
};

/* \brief sm3 开始
 *
 * \param ctx hsm3_context_t* sm3上下文指针
 * \return int 0=未出错
 *
 */
int hsm3_starts(hsm3_context_t *ctx);

/** \brief sm3 更新
 *
 * \param ctx hsm3_context_t* sm3上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsm3_update(hsm3_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sm3 完成
 *
 * \param ctx hsm3_context_t* sm3上下文指针
 * \param output hsm3_sm3_t 输出SM3
 * \return int 0=未出错
 *
 */
int hsm3_finish(hsm3_context_t *ctx,hsm3_sm3_t output);

/** \brief sm3计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsm3_sm3_t 输出SM3
 * \return int 0=未出错
 *
 */
int hsm3_sm3(const uint8_t *input,size_t ilen,hsm3_sm3_t output);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSM3_H__
