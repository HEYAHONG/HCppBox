/***************************************************************
 * Name:      hrsa3072.h
 * Purpose:   声明hrsa3072接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRSA3072_H__
#define __HRSA3072_H__
#include "stdint.h"
#include "../hbignum.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HRSA3072_CIPHER_MESSAGE_BYTES               (sizeof(huint3072_t))

typedef uint8_t hrsa3072_cipher_message_t[HRSA3072_CIPHER_MESSAGE_BYTES];       /**< 密文 */

#define HRSA3072_DATA_BLOCK_BYTES                (HRSA3072_CIPHER_MESSAGE_BYTES)

typedef uint8_t hrsa3072_data_block_t[HRSA3072_DATA_BLOCK_BYTES];        /**< 明文数据块（明文经过填充得到的数据） */


typedef struct hrsa3072_public_key hrsa3072_public_key_t;
struct hrsa3072_public_key
{
    huint3072_t n;
    huint3072_t e;
};

void hrsa3072_public_key_load_n(hrsa3072_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa3072_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa3072_public_key_t *key);
void hrsa3072_public_key_load_e(hrsa3072_public_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa3072_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa3072_public_key_t *key);
bool hrsa3072_public_key_is_ok(const hrsa3072_public_key_t *key);

typedef struct hrsa3072_private_key hrsa3072_private_key_t;
struct hrsa3072_private_key
{
    huint3072_t n;
    huint3072_t d;
};

void hrsa3072_private_key_load_n(hrsa3072_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa3072_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa3072_private_key_t *key);
void hrsa3072_private_key_load_d(hrsa3072_private_key_t *key,const uint8_t *bytes,size_t bytes_len);
void hrsa3072_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa3072_private_key_t *key);
bool hrsa3072_private_key_is_ok(const hrsa3072_private_key_t *key);

typedef struct hrsa3072_context hrsa3072_context_t;
struct hrsa3072_context
{
    huint6144_state_t state;
};

/** \brief RSA3072加密
 *
 * \param ctx hrsa3072_context_t* 上下文
 * \param cipher_message hrsa3072_cipher_message_t 密文
 * \param data_block const hrsa3072_data_block_t 明文数据块（明文经过填充得到的数据），注意：需要保证明文数据块存储的数据小于n
 * \param key const hrsa3072_private_key_t * 私钥
 * \return bool 是否成功
 *
 */
bool hrsa3072_encipher(hrsa3072_context_t *ctx,hrsa3072_cipher_message_t cipher_message,const hrsa3072_data_block_t data_block,const hrsa3072_private_key_t *key);

/** \brief RSA3072解密
 *
 * \param ctx hrsa3072_context_t* 上下文
 * \param data_block hrsa3072_data_block_t 明文数据块（明文经过填充得到的数据）,注意：需要保证明文数据块存储的数据小于n
 * \param cipher_message const hrsa3072_cipher_message_t 密文
 * \param key const hrsa3072_public_key_t* 公钥
 * \return bool 是否成功
 *
 */
bool hrsa3072_decipher(hrsa3072_context_t *ctx,hrsa3072_data_block_t data_block,const hrsa3072_cipher_message_t cipher_message,const hrsa3072_public_key_t *key);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRSA3072_H__
