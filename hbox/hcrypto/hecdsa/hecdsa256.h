/***************************************************************
 * Name:      hecdsa256.h
 * Purpose:   声明hecdsa256接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HECDSA256_H__
#define __HECDSA256_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HECDSA256_BYTES                                 (32)
#define HECDSA256_SHARED_KEY_SIZE                       (HECDSA256_BYTES)
#define HECDSA256_PRIVATE_KEY_SIZE                      (HECDSA256_BYTES)
#define HECDSA256_PUBLIC_KEY_SIZE                       (HECDSA256_BYTES + 1)
#define HECDSA256_PUBLIC_DECOMPRESS_KEY_SIZE            (HECDSA256_BYTES * 2)
#define HECDSA256_SIGNATURE_SIZE                        (HECDSA256_BYTES * 2)

typedef uint8_t hecdsa256_hash_t[HECDSA256_BYTES];                 /**< 哈希类型，通常采用SHA256 */
typedef uint8_t hecdsa256_shared_key_t[HECDSA256_SHARED_KEY_SIZE];
typedef uint8_t hecdsa256_private_key_t[HECDSA256_PRIVATE_KEY_SIZE];
typedef uint8_t hecdsa256_public_key_t[HECDSA256_PUBLIC_KEY_SIZE];
typedef uint8_t hecdsa256_public_decompress_key_t[HECDSA256_PUBLIC_DECOMPRESS_KEY_SIZE];
typedef uint8_t hecdsa256_signature_t[HECDSA256_SIGNATURE_SIZE];


typedef struct hecdsa256_curve  hecdsa256_curve_t;
struct hecdsa256_curve
{
    uint64_t p[HECDSA256_BYTES/sizeof(uint64_t)];           /**< p为大素数 */
    uint64_t a_inv[HECDSA256_BYTES/sizeof(uint64_t)];       /**< a的相反数,a为公式y*y=x*x*x + a*x +b 中的系数 */
    uint64_t b[HECDSA256_BYTES/sizeof(uint64_t)];           /**< b, a为公式y*y=x*x*x + a*x +b 中的系数 */
    uint64_t n[HECDSA256_BYTES/sizeof(uint64_t)];           /**< n,n为阶数,私钥的最大值 */
    uint64_t x[HECDSA256_BYTES/sizeof(uint64_t)];           /**< G点x坐标 */
    uint64_t y[HECDSA256_BYTES/sizeof(uint64_t)];           /**< G点y坐标 */
};

extern const hecdsa256_curve_t hecdsa256_curve_secp256r1;


/** \brief  生成ecdsa256共享密钥
 *
 * \param curve const hecdsa256_curve_t* 曲线参数
 * \param public_key const hecdsa256_public_key_t 公钥
 * \param private_key const hecdsa256_private_key_t 私钥,注意：此处的私钥并非公钥对应的私钥，而是对方的公钥与自己的私钥
 * \param shared_key hecdsa256_shared_key_t 共享密钥
 * \return int 是否生成
 *
 */
int hecdsa256_ecdh256_keygen(const hecdsa256_curve_t *curve,const hecdsa256_public_key_t public_key, const hecdsa256_private_key_t private_key, hecdsa256_shared_key_t shared_key);


/** \brief 生成ecdsa256密钥对
 *
 * \param curve const hecdsa256_curve_t* 曲线参数
 * \param public_key hecdsa256_public_key_t 公钥
 * \param private_key hecdsa256_private_key_t 私钥,注意：私钥通过HECDSA256_RAND定义的函数(默认定义为rand)产生。
 * \return int 是否生成
 *
 */
int hecdsa256_keygen(const hecdsa256_curve_t *curve,hecdsa256_public_key_t public_key, hecdsa256_private_key_t private_key);

/** \brief ecdsa256解压公钥
 *
 * \param curve const hecdsa256_curve_t* 曲线参数
 * \param public_key const hecdsa256_public_key_t 公钥
 * \param public_key_decompress hecdsa256_public_decompress_key_t 解压后的公钥
 * \return int 是否解压
 *
 */
int hecdsa256_public_key_decompress(const hecdsa256_curve_t *curve,const hecdsa256_public_key_t public_key,hecdsa256_public_decompress_key_t public_key_decompress);

/** \brief 签名ecdsa256
 *
 * \param curve const hecdsa256_curve_t* 曲线参数
 * \param private_key const hecdsa256_private_key_t 私钥
 * \param hash256 const hecdsa256_hash_t hash值，256位
 * \param signature const hecdsa256_signature_t 签名
 * \return int 是否成功签名
 *
 */
int hecdsa256_sign(const hecdsa256_curve_t *curve,const hecdsa256_private_key_t private_key, const hecdsa256_hash_t hash256, hecdsa256_signature_t signature);

/** \brief 验证ecdsa256
 *
 * \param curve const hecdsa256_curve_t* 曲线参数
 * \param public_key const hecdsa256_public_key_t 公钥
 * \param hash256 const hecdsa256_hash_t hash值，256位
 * \param signature const hecdsa256_signature_t 签名
 * \return int 是否成功验证
 *
 */
int hecdsa256_verify(const hecdsa256_curve_t *curve,const hecdsa256_public_key_t public_key, const hecdsa256_hash_t hash256, const hecdsa256_signature_t signature);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HECDSA256_H__
