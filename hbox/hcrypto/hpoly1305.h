/***************************************************************
 * Name:      hpoly1305.h
 * Purpose:   声明hpoly1305接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPOLY1305_H__
#define __HPOLY1305_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
 * 本文件提供简易的POLY1305消息认证功能。
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HPOLY1305_BLOCK_SIZE 16

typedef struct hpoly1305_context hpoly1305_context_t;
struct hpoly1305_context
{
    uint32_t r[5];
    uint32_t h[5];
    uint32_t pad[4];
    size_t leftover;
    uint8_t buffer[HPOLY1305_BLOCK_SIZE];
    uint8_t is_final;
};

typedef uint8_t hpoly1305_key_t[32];

/** \brief POLY1305起始
 *
 * \param ctx hpoly1305_context_t* POLY1305上下文
 * \param key  hpoly1305_key_t 密钥
 * \return int 0=未出错
 *
 */
int hpoly1305_starts(hpoly1305_context_t *ctx,hpoly1305_key_t key);

/** \brief POLY1305更新
 *
 * \param ctx hpoly1305_context_t* POLY1305上下文
 * \param message const uint8_t* 消息
 * \param message_len size_t 消息长度
 * \return int 0=未出错
 *
 */
int hpoly1305_update(hpoly1305_context_t *ctx,const uint8_t *message,size_t message_len);


typedef uint8_t hpoly1305_mac_t[16];

/** \brief POLY1305完成
 *
 * \param ctx hpoly1305_context_t* POLY1305上下文
 * \param mac  hpoly1305_mac_t 消息认证码
 * \return int 0=未出错
 *
 */
int hpoly1305_finish(hpoly1305_context_t  *ctx,hpoly1305_mac_t mac);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPOLY1305_H__
