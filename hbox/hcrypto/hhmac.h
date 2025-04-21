/***************************************************************
 * Name:      hhmac.h
 * Purpose:   声明hhmac接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHMAC_H__
#define __HHMAC_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "hsha.h"
#include "hmd4.h"
#include "hmd5.h"
#include "hsm3.h"
#include "hripemd160.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hhmac_md4_context hhmac_md4_context_t;
struct hhmac_md4_context
{
    hmd4_context_t          ctx;            /**< 上下文 */
    hmd4_md4_t              hash_result;    /**< 结果 */
    hmd4_message_block_t    buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_md4_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_md4_starts(hhmac_md4_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_md4_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_md4_update(hhmac_md4_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_md4_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_md4_finish(hhmac_md4_context_t  *ctx);


typedef struct hhmac_md5_context hhmac_md5_context_t;
struct hhmac_md5_context
{
    hmd5_context_t          ctx;            /**< 上下文 */
    hmd5_md5_t              hash_result;    /**< 结果 */
    hmd5_message_block_t    buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_md5_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_md5_starts(hhmac_md5_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_md5_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_md5_update(hhmac_md5_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_md5_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_md5_finish(hhmac_md5_context_t  *ctx);


typedef struct hhmac_sha1_context hhmac_sha1_context_t;
struct hhmac_sha1_context
{
    hsha1_context_t          ctx;            /**< 上下文 */
    hsha1_sha_t              hash_result;    /**< 结果 */
    hsha1_message_block_t    buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sha1_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sha1_starts(hhmac_sha1_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sha1_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sha1_update(hhmac_sha1_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sha1_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sha1_finish(hhmac_sha1_context_t  *ctx);


typedef struct hhmac_sha2_sha224_context hhmac_sha2_sha224_context_t;
struct hhmac_sha2_sha224_context
{
    hsha2_sha224_context_t          ctx;            /**< 上下文 */
    hsha2_sha224_t                  hash_result;    /**< 结果 */
    hsha2_message_block_t           buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sha2_sha224_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha224_starts(hhmac_sha2_sha224_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sha2_sha224_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha224_update(hhmac_sha2_sha224_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sha2_sha224_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha224_finish(hhmac_sha2_sha224_context_t  *ctx);


typedef struct hhmac_sha2_sha256_context hhmac_sha2_sha256_context_t;
struct hhmac_sha2_sha256_context
{
    hsha2_sha256_context_t          ctx;            /**< 上下文 */
    hsha2_sha256_t                  hash_result;    /**< 结果 */
    hsha2_message_block_t           buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sha2_sha256_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha256_starts(hhmac_sha2_sha256_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sha2_sha256_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha256_update(hhmac_sha2_sha256_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sha2_sha256_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha256_finish(hhmac_sha2_sha256_context_t  *ctx);

typedef struct hhmac_sha2_sha384_context hhmac_sha2_sha384_context_t;
struct hhmac_sha2_sha384_context
{
    hsha2_sha384_context_t          ctx;            /**< 上下文 */
    hsha2_sha384_t                  hash_result;    /**< 结果 */
    hsha2_message2_block_t           buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sha2_sha384_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha384_starts(hhmac_sha2_sha384_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sha2_sha384_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha384_update(hhmac_sha2_sha384_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sha2_sha384_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha384_finish(hhmac_sha2_sha384_context_t  *ctx);

typedef struct hhmac_sha2_sha512_context hhmac_sha2_sha512_context_t;
struct hhmac_sha2_sha512_context
{
    hsha2_sha512_context_t          ctx;            /**< 上下文 */
    hsha2_sha512_t                  hash_result;    /**< 结果 */
    hsha2_message2_block_t           buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sha2_sha512_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha512_starts(hhmac_sha2_sha512_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sha2_sha512_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha512_update(hhmac_sha2_sha512_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sha2_sha512_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sha2_sha512_finish(hhmac_sha2_sha512_context_t  *ctx);

typedef struct hhmac_sm3_context hhmac_sm3_context_t;
struct hhmac_sm3_context
{
    hsm3_context_t          ctx;            /**< 上下文 */
    hsm3_sm3_t              hash_result;    /**< 结果 */
    hsm3_message_block_t    buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_sm3_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_sm3_starts(hhmac_sm3_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_sm3_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_sm3_update(hhmac_sm3_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_sm3_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_sm3_finish(hhmac_sm3_context_t  *ctx);

typedef struct hhmac_ripemd160_context hhmac_ripemd160_context_t;
struct hhmac_ripemd160_context
{
    hripemd160_context_t          ctx;            /**< 上下文 */
    hripemd160_ripemd160_t              hash_result;    /**< 结果 */
    hripemd160_message_block_t    buffer;
};

/** \brief HMAC起始
 *
 * \param ctx hhmac_ripemd160_context_t* HMAC上下文
 * \param key const uint8_t* 密钥
 * \param key_len size_t 密钥长度
 * \return int 0=未出错
 *
 */
int hhmac_ripemd160_starts(hhmac_ripemd160_context_t *ctx,const uint8_t *key,size_t key_len);

/** \brief HMAC更新
 *
 * \param ctx hhmac_ripemd160_context_t* HMAC上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hhmac_ripemd160_update(hhmac_ripemd160_context_t *ctx,const uint8_t *message,size_t message_len);


/** \brief HMAC完成
 *
 * \param ctx hhmac_ripemd160_context_t* HMAC上下文,输出见hash_result成员
 * \return int 0=未出错
 *
 */
int hhmac_ripemd160_finish(hhmac_ripemd160_context_t  *ctx);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHMAC_H__
