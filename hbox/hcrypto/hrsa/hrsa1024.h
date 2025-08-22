/***************************************************************
 * Name:      hrsa1024.h
 * Purpose:   声明hrsa1024接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRSA1024_H__
#define __HRSA1024_H__
#include "stdint.h"
#include "../hbignum.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HRSA1024_CIPHER_MESSAGE_BYTES               (sizeof(huint1024_t))

typedef uint8_t hrsa1024_cipher_message_t[HRSA1024_CIPHER_MESSAGE_BYTES];       /**< 密文 */

#define HRSA1024_DATA_BLOCK_BYTES                (HRSA1024_CIPHER_MESSAGE_BYTES)

typedef uint8_t hrsa1024_data_block_t[HRSA1024_DATA_BLOCK_BYTES];        /**< 明文数据块（明文经过填充得到的数据） */


typedef struct hrsa1024_public_key hrsa1024_public_key_t;
struct hrsa1024_public_key
{
    huint1024_t n;
    huint1024_t e;
};

void hrsa1024_public_key_load_n(hrsa1024_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa1024_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa1024_public_key_t *key);
void hrsa1024_public_key_load_e(hrsa1024_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa1024_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa1024_public_key_t *key);
bool hrsa1024_public_key_is_ok(const hrsa1024_public_key_t *key);

typedef struct hrsa1024_private_key hrsa1024_private_key_t;
struct hrsa1024_private_key
{
    huint1024_t n;
    huint1024_t d;
};

void hrsa1024_private_key_load_n(hrsa1024_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa1024_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa1024_private_key_t *key);
void hrsa1024_private_key_load_d(hrsa1024_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa1024_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa1024_private_key_t *key);
bool hrsa1024_private_key_is_ok(const hrsa1024_private_key_t *key);

typedef struct hrsa1024_context hrsa1024_context_t;
struct hrsa1024_context
{
    huint2048_state_t state;
};

/** \brief RSA1024加密
 *
 * \param ctx hrsa1024_context_t* 上下文
 * \param cipher_message hrsa1024_cipher_message_t 密文
 * \param data_block const hrsa1024_data_block_t 明文数据块（明文经过填充得到的数据），注意：需要保证明文数据块存储的数据小于n
 * \param key const hrsa1024_private_key_t * 私钥
 * \return bool 是否成功
 *
 */
bool hrsa1024_encipher(hrsa1024_context_t *ctx,hrsa1024_cipher_message_t cipher_message,const hrsa1024_data_block_t data_block,const hrsa1024_private_key_t *key);

/** \brief RSA1024解密
 *
 * \param ctx hrsa1024_context_t* 上下文
 * \param data_block hrsa1024_data_block_t 明文数据块（明文经过填充得到的数据）,注意：需要保证明文数据块存储的数据小于n
 * \param cipher_message const hrsa1024_cipher_message_t 密文
 * \param key const hrsa1024_public_key_t* 公钥
 * \return bool 是否成功
 *
 */
bool hrsa1024_decipher(hrsa1024_context_t *ctx,hrsa1024_data_block_t data_block,const hrsa1024_cipher_message_t cipher_message,const hrsa1024_public_key_t *key);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRSA1024_H__
