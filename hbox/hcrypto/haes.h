/***************************************************************
 * Name:      haes.h
 * Purpose:   声明haes接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HAES_H__
#define __HAES_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
 * 本文件提供简易的AES加密解密功能。
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HAES_MAXNR
#define HAES_MAXNR      14
#endif // HAES_MAXNR
#ifndef HAES_BLOCK_SIZE
/*
 * AES是分组加密算法，一个块的大小为16字节
 */
#define HAES_BLOCK_SIZE 16
#endif // HAES_BLOCK_SIZE

typedef struct haes_key haes_key_t;
struct haes_key
{
    uint32_t rd_key[4 *(HAES_MAXNR + 1)];
    int rounds;
};

/** \brief aes初始化加密密钥
 *
 * \param userKey const uint8_t* 密钥数据
 * \param bits const int 128/192/256 分别对应aes128、aes192、aes256
 * \param key haes_key_t* 密钥
 * \return int 0等于成功
 *
 */
int haes_set_encrypt_key(const uint8_t *userKey, const int bits,haes_key_t *key);

/** \brief aes初始化解密密钥
 *
 * \param userKey const uint8_t* 密钥数据
 * \param bits const int 128/192/256 分别对应aes128、aes192、aes256
 * \param key haes_key_t* 密钥
 * \return int 0等于成功
 *
 */
int haes_set_decrypt_key(const uint8_t *userKey, const int bits,haes_key_t *key);

/** \brief aes加密单个分组（16字节）
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t* 输出数据
 * \param key const haes_key_t* 加密密钥
 *
 */
void haes_encrypt(const uint8_t *in, uint8_t *out,const haes_key_t *key);

/** \brief aes解密单个分组（16字节）
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t* 输出数据
 * \param key const haes_key_t* 解密密钥
 *
 */
void haes_decrypt(const uint8_t *in, uint8_t *out,const haes_key_t *key);


/** \brief aes cbc加密
 *
 * \param in const uint8_t* 输入数据
 * \param out uint8_t*  输出数据
 * \param length const size_t 数据长度（输入输出数据长度一致）
 * \param key const haes_key_t* 加密密钥
 * \param ivec uint8_t* 初始向量（长度为16字节），必须可读写
 * \param enc bool 是否为加密
 *
 */
void haes_cbc_encrypt(const uint8_t *in, uint8_t *out,const size_t length, const haes_key_t *key, uint8_t *ivec, bool enc);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HAES_H__
