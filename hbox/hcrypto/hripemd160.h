/***************************************************************
 * Name:      hripemd160.h
 * Purpose:   声明hripemd160接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRIPEMD160_H__
#define __HRIPEMD160_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * RIPEMD160
 */
typedef uint8_t hripemd160_ripemd160_t[20];

/*
 * RIPEMD160 消息块
 */
typedef uint8_t hripemd160_message_block_t[64];

/*
 * RIPEMD160上下文
 */
typedef struct hripemd160_context hripemd160_context_t;
struct hripemd160_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    hripemd160_message_block_t buffer;
};

/* \brief ripemd160 开始
 *
 * \param ctx hripemd160_context_t* ripemd160上下文指针
 * \return int 0=未出错
 *
 */
int hripemd160_starts(hripemd160_context_t *ctx);

/** \brief ripemd160 更新
 *
 * \param ctx hripemd160_context_t* ripemd160上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hripemd160_update(hripemd160_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief ripemd160 完成
 *
 * \param ctx hripemd160_context_t* ripemd160上下文指针
 * \param output hripemd160_ripemd160_t 输出RIPEMD160
 * \return int 0=未出错
 *
 */
int hripemd160_finish(hripemd160_context_t *ctx,hripemd160_ripemd160_t output);

/** \brief ripemd160计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hripemd160_ripemd160_t 输出RIPEMD160
 * \return int 0=未出错
 *
 */
int hripemd160_ripemd160(const uint8_t *input,size_t ilen,hripemd160_ripemd160_t output);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRIPEMD160_H__
