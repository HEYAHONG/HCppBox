/***************************************************************
 * Name:      hchacha20.h
 * Purpose:   声明hchacha20接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-03
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCHACHA20_H__
#define __HCHACHA20_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
 * 本文件提供简易的CHACHA20加密解密功能。
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hchacha20_context hchacha20_context_t;
struct hchacha20_context
{
    uint32_t state[16];
    uint8_t  keystream8[64];
    size_t   keystream_bytes_used;
};

/** \brief 设置密钥(将初始化上下文,在加解密之前需要再调用hchacha20_starts)
 *
 * \param ctx hchacha20_context_t* 上下文
 * \param key[32] const uint8_t 密钥(必须为32字节)
 * \return int 0=成功
 *
 */
int hchacha20_setkey(hchacha20_context_t *ctx,const uint8_t key[32]);


/** \brief 开始(此函数之前必须调用hchacha20_setkey设置密钥)
 *
 * \param ctx hchacha20_context_t* 上下文
 * \param nonce[12] const uint8_t  临时数字（与密钥、计数器初值共同决定加解密）
 * \param counter uint32_t 计数器初值，一般为0
 * \return int 0=成功
 *
 */
int hchacha20_starts(hchacha20_context_t *ctx,const uint8_t nonce[12],uint32_t counter);

/** \brief 更新(加解密)
 *
 * \param ctx hchacha20_context_t* 上下文
 * \param length size_t 加解密长度
 * \param input const uint8_t * 输入
 * \param output uint8_t * 输出
 * \return int 0=成功
 *
 */
int hchacha20_update(hchacha20_context_t *ctx,size_t length,const uint8_t *input,uint8_t  *output);


/** \brief 加解密
 *
 * \param key[32] const uint8_t 密钥(必须为32字节)
 * \param nonce[12] const uint8_t 临时数字（与密钥、计数器初值共同决定加解密）
 * \param counter uint32_t 计数器初值，一般为0
 * \param length size_t 加解密长度
 * \param input const uint8_t* 输入
 * \param output uint8_t* 输出
 * \return int 0=成功
 *
 */
int hchacha20_crypt(const uint8_t key[32],const uint8_t nonce[12],uint32_t counter,size_t length,const uint8_t *input,uint8_t *output);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCHACHA20_H__
