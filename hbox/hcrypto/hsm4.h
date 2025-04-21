/***************************************************************
 * Name:      hsm4.h
 * Purpose:   声明hsm4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSM4_H__
#define __HSM4_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
 * 本文件提供简易的SM4加密解密功能。
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HSM4_BLOCK_SIZE
/*
 * SM4是分组加密算法，一个块的大小为16字节
 */
#define HSM4_BLOCK_SIZE 16
#endif // HSM4_BLOCK_SIZE

typedef struct hsm4_key hsm4_key_t;
struct hsm4_key
{
    uint32_t rkey[32];
};

/** \brief sm4初始化加密密钥
 *
 * \param userKey const uint8_t* 密钥数据(16字节)
 * \param key hsm4_key_t* 密钥
 * \return int 0等于成功
 *
 */
int hsm4_set_encrypt_key(const uint8_t *userKey,hsm4_key_t *key);

/** \brief sm4初始化解密密钥
 *
 * \param userKey const uint8_t* 密钥数据(16字节)
 * \param key hsm4_key_t* 密钥
 * \return int 0等于成功
 *
 */
int hsm4_set_decrypt_key(const uint8_t *userKey, hsm4_key_t *key);

/** \brief sm4加密单个分组（16字节）
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t* 输出数据
 * \param key const hsm4_key_t* 加密密钥
 *
 */
void hsm4_encrypt(const uint8_t *in, uint8_t *out,const hsm4_key_t *key);

/** \brief sm4解密单个分组（16字节）
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t* 输出数据
 * \param key const hsm4_key_t* 解密密钥
 *
 */
void hsm4_decrypt(const uint8_t *in, uint8_t *out,const hsm4_key_t *key);


/** \brief sm4 cbc加密
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t*  输出数据
 * \param length const size_t 数据长度（输入输出数据长度一致）
 * \param key const hsm4_key_t* 加密密钥
 * \param ivec uint8_t* 初始向量（长度为16字节），必须可读写
 * \param enc bool 是否为加密
 *
 */
void hsm4_cbc_encrypt(const uint8_t *in, uint8_t *out,const size_t length, const hsm4_key_t *key, uint8_t *ivec, bool enc);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSM4_H__
