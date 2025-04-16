/***************************************************************
 * Name:      hsha.h
 * Purpose:   声明hsha接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHA_H__
#define __HSHA_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * SHA1
 */
typedef uint8_t hsha1_sha_t[20];

/*
 * SHA1 消息块
 */
typedef uint8_t hsha1_message_block_t[64];

/*
 * SHA1上下文
 */
typedef struct hsha1_context hsha1_context_t;
struct hsha1_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    hsha1_message_block_t buffer;
};

/* \brief sha1 开始
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \return int 0=未出错
 *
 */
int hsha1_starts(hsha1_context_t *ctx);

/** \brief sha1 更新
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha1_update(hsha1_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha1 完成
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \param output hsha1_sha1_t 输出SHA1
 * \return int 0=未出错
 *
 */
int hsha1_finish(hsha1_context_t *ctx,hsha1_sha_t output);

/** \brief sha1计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha1_sha1_t 输出SHA1
 * \return int 0=未出错
 *
 */
int hsha1_sha(const uint8_t *input,size_t ilen,hsha1_sha_t output);


/*
 * SHA2 224
 */
typedef uint8_t hsha2_sha224_t[28];

/*
 * SHA2 消息块
 */
typedef uint8_t hsha2_message_block_t[64];

/*
 * SHA2 SHA224上下文
 */
typedef struct hsha2_sha224_context hsha2_sha224_context_t;
struct hsha2_sha224_context
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
    hsha2_message_block_t buffer;
};

/* \brief sha2_sha224 开始
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \return int 0=未出错
 *
 */
int hsha2_sha224_starts(hsha2_sha224_context_t *ctx);

/** \brief sha2_sha224 更新
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha2_sha224_update(hsha2_sha224_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha2_sha224 完成
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \param output hsha2_sha224_t 输出SHA2 SHA224
 * \return int 0=未出错
 *
 */
int hsha2_sha224_finish(hsha2_sha224_context_t *ctx,hsha2_sha224_t output);

/** \brief sha2_sha224计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha2_sha224_t 输出SHA2 SHA224
 * \return int 0=未出错
 *
 */
int hsha2_sha224(const uint8_t *input,size_t ilen,hsha2_sha224_t output);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSHA_H__
