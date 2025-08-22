/***************************************************************
 * Name:      hrsa4096.h
 * Purpose:   声明hrsa4096接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRSA4096_H__
#define __HRSA4096_H__
#include "stdint.h"
#include "../hbignum.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HRSA4096_CIPHER_MESSAGE_BYTES               (sizeof(huint4096_t))

typedef uint8_t hrsa4096_cipher_message_t[HRSA4096_CIPHER_MESSAGE_BYTES];       /**< 密文 */

#define HRSA4096_DATA_BLOCK_BYTES                (HRSA4096_CIPHER_MESSAGE_BYTES)

typedef uint8_t hrsa4096_data_block_t[HRSA4096_DATA_BLOCK_BYTES];        /**< 明文数据块（明文经过填充得到的数据） */


typedef struct hrsa4096_public_key hrsa4096_public_key_t;
struct hrsa4096_public_key
{
    huint4096_t n;
    huint4096_t e;
};

void hrsa4096_public_key_load_n(hrsa4096_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa4096_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa4096_public_key_t *key);
void hrsa4096_public_key_load_e(hrsa4096_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa4096_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa4096_public_key_t *key);
bool hrsa4096_public_key_is_ok(const hrsa4096_public_key_t *key);

typedef struct hrsa4096_private_key hrsa4096_private_key_t;
struct hrsa4096_private_key
{
    huint4096_t n;
    huint4096_t d;
};

void hrsa4096_private_key_load_n(hrsa4096_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa4096_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa4096_private_key_t *key);
void hrsa4096_private_key_load_d(hrsa4096_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa4096_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa4096_private_key_t *key);
bool hrsa4096_private_key_is_ok(const hrsa4096_private_key_t *key);

typedef struct hrsa4096_context hrsa4096_context_t;
struct hrsa4096_context
{
    huint8192_state_t state;
};

/** \brief RSA4096加密
 *
 * \param ctx hrsa4096_context_t* 上下文
 * \param cipher_message hrsa4096_cipher_message_t 密文
 * \param data_block const hrsa4096_data_block_t 明文数据块（明文经过填充得到的数据），注意：需要保证明文数据块存储的数据小于n
 * \param key const hrsa4096_private_key_t * 私钥
 * \return bool 是否成功
 *
 */
bool hrsa4096_encipher(hrsa4096_context_t *ctx,hrsa4096_cipher_message_t cipher_message,const hrsa4096_data_block_t data_block,const hrsa4096_private_key_t *key);

/** \brief RSA4096解密
 *
 * \param ctx hrsa4096_context_t* 上下文
 * \param data_block hrsa4096_data_block_t 明文数据块（明文经过填充得到的数据）,注意：需要保证明文数据块存储的数据小于n
 * \param cipher_message const hrsa4096_cipher_message_t 密文
 * \param key const hrsa4096_public_key_t* 公钥
 * \return bool 是否成功
 *
 */
bool hrsa4096_decipher(hrsa4096_context_t *ctx,hrsa4096_data_block_t data_block,const hrsa4096_cipher_message_t cipher_message,const hrsa4096_public_key_t *key);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRSA4096_H__
