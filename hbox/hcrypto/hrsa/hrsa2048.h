/***************************************************************
 * Name:      hrsa2048.h
 * Purpose:   声明hrsa2048接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRSA2048_H__
#define __HRSA2048_H__
#include "stdint.h"
#include "../hbignum.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HRSA2048_CIPHER_MESSAGE_BYTES               (sizeof(huint2048_t))

typedef uint8_t hrsa2048_cipher_message_t[HRSA2048_CIPHER_MESSAGE_BYTES];       /**< 密文 */

#define HRSA2048_DATA_BLOCK_BYTES                (HRSA2048_CIPHER_MESSAGE_BYTES)

typedef uint8_t hrsa2048_data_block_t[HRSA2048_DATA_BLOCK_BYTES];        /**< 明文数据块（明文经过填充得到的数据） */


typedef struct hrsa2048_public_key hrsa2048_public_key_t;
struct hrsa2048_public_key
{
    huint2048_t n;
    huint2048_t e;
};

void hrsa2048_public_key_load_n(hrsa2048_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa2048_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa2048_public_key_t *key);
void hrsa2048_public_key_load_e(hrsa2048_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa2048_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa2048_public_key_t *key);
bool hrsa2048_public_key_is_ok(const hrsa2048_public_key_t *key);
bool hrsa2048_public_key_load_from_asn1_public_key(hrsa2048_public_key_t *key,const uint8_t *asn1_bytes,size_t bytes_len);
bool hrsa2048_public_key_load_from_asn1_private_key(hrsa2048_public_key_t *key,const uint8_t *asn1_bytes,size_t bytes_len);

typedef struct hrsa2048_private_key hrsa2048_private_key_t;
struct hrsa2048_private_key
{
    huint2048_t n;
    huint2048_t d;
};

void hrsa2048_private_key_load_n(hrsa2048_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa2048_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa2048_private_key_t *key);
void hrsa2048_private_key_load_d(hrsa2048_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa2048_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa2048_private_key_t *key);
bool hrsa2048_private_key_is_ok(const hrsa2048_private_key_t *key);
bool hrsa2048_private_key_load_from_asn1_private_key(hrsa2048_private_key_t *key,const uint8_t *asn1_bytes,size_t bytes_len);


typedef struct hrsa2048_context hrsa2048_context_t;
struct hrsa2048_context
{
    huint4096_state_t state;
};

/** \brief RSA2048加密
 *
 * \param ctx hrsa2048_context_t* 上下文
 * \param cipher_message hrsa2048_cipher_message_t 密文
 * \param data_block const hrsa2048_data_block_t 明文数据块（明文经过填充得到的数据），注意：需要保证明文数据块存储的数据小于n
 * \param key const hrsa2048_private_key_t * 私钥
 * \return bool 是否成功
 *
 */
bool hrsa2048_encipher(hrsa2048_context_t *ctx,hrsa2048_cipher_message_t cipher_message,const hrsa2048_data_block_t data_block,const hrsa2048_private_key_t *key);

/** \brief RSA2048解密
 *
 * \param ctx hrsa2048_context_t* 上下文
 * \param data_block hrsa2048_data_block_t 明文数据块（明文经过填充得到的数据）,注意：需要保证明文数据块存储的数据小于n
 * \param cipher_message const hrsa2048_cipher_message_t 密文
 * \param key const hrsa2048_public_key_t* 公钥
 * \return bool 是否成功
 *
 */
bool hrsa2048_decipher(hrsa2048_context_t *ctx,hrsa2048_data_block_t data_block,const hrsa2048_cipher_message_t cipher_message,const hrsa2048_public_key_t *key);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRSA2048_H__
