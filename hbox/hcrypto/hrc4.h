/***************************************************************
 * Name:      hrc4.h
 * Purpose:   声明hrc4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRC4_H__
#define __HRC4_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
 * 本文件提供简易的RC4加密解密功能。
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef     HRC4_INT
#define     HRC4_INT uint8_t
#endif // __HRC4_H__

typedef struct hrc4_context hrc4_context_t;
struct hrc4_context
{
    HRC4_INT x;
    HRC4_INT y;
    HRC4_INT data[256];
};

/** \brief RC4设置密钥
 *
 * \param context hrc4_context_t* RC4上下文
 * \param key const uint8_t* 密钥
 * \param len size_t 密钥长度
 *
 */
void hrc4_set_key(hrc4_context_t *ctx,const uint8_t *key,size_t len);


/** \brief RC4加密/解密,由于RC4是对明文进行XOR,所以加密解密操作相同
 *
 * \param ctx hrc4_context_t* RC4上下文
 * \param len size_t 数据长度
 * \param indata const uint8_t* 输入数据
 * \param outdata uint8_t* 输出数据,长度同输入数据
 *
 */
void hrc4_rc4(hrc4_context_t *ctx, size_t len, const uint8_t *indata,uint8_t *outdata);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRC4_H__
