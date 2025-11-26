/***************************************************************
 * Name:      hed25519_file.h
 * Purpose:   声明hed25519_file接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_FILE_H__
#define __HED25519_FILE_H__
#include "stdlib.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 本文件与openwrt的usign工具兼容,源代码可见https://git.openwrt.org/project/usign.git
 */


/*
 * 公钥文件
 */
struct hed25519_public_key_file_struct;
typedef struct hed25519_public_key_file_struct hed25519_public_key_file_struct_t;
struct hed25519_public_key_file_struct
{
    char    pkalg[2];           /**< 算法名称，通常为 Ed */
    uint8_t fingerprint[8];     /**< 指纹，私钥、公钥与签名的指纹应相同,生成时可随机填充 */
    uint8_t pubkey[32];         /**< Ed25519公钥 */
};

/*
 * 私钥文件
 */
struct hed25519_private_key_file_struct;
typedef struct hed25519_private_key_file_struct hed25519_private_key_file_struct_t;
struct hed25519_private_key_file_struct
{
    char        pkalg[2];               /**< 算法名称，通常为 Ed */
    char        kdfalg[2];              /**< KDF算法，若不进行私钥加密，应当指定为BK */
    uint32_t    kdfrounds;              /**< KDF轮数，若不进行私钥加密，应当指定为0 */
    uint8_t     salt[16];               /**< 盐，生成时可随机填充 */
    uint8_t     checksum[8];            /**< 校验和，seckey成员的sha512的前8字节 */
    uint8_t     fingerprint[8];         /**< 指纹，私钥、公钥与签名的指纹应相同,生成时可随机填充 */
    uint8_t     seckey[64];             /**< Ed25519私钥，可根据KDF算法进行一定变换,若不进行私钥加密，后32字节应当填充Ed25519公钥 */
};

/*
 * 签名文件
 */
struct hed25519_sign_file_struct;
typedef struct hed25519_sign_file_struct hed25519_sign_file_struct_t;
struct hed25519_sign_file_struct
{
    char    pkalg[2];                       /**< 算法名称，通常为 Ed */
    uint8_t fingerprint[8];                 /**< 指纹，私钥、公钥与签名的指纹应相同,生成时可随机填充 */
    uint8_t sig[64];                        /**< Ed25519签名 */
};

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_FILE_H__
