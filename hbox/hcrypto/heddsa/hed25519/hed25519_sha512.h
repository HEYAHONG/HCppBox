/***************************************************************
 * Name:      hed25519_sha512.h
 * Purpose:   声明hed25519_sha512接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_SHA512_H__
#define __HED25519_SHA512_H__
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HED25519_SHA512_CONTEXT_SIZE
#include "../../hsha.h"
#define HED25519_SHA512_CONTEXT_SIZE sizeof(hsha2_sha512_context_t)
#endif

typedef struct hed25519_sha512_context hed25519_sha512_context_t;
struct hed25519_sha512_context
{
    uint64_t buffer[HED25519_SHA512_CONTEXT_SIZE/sizeof(uint64_t)+((HED25519_SHA512_CONTEXT_SIZE/sizeof(uint64_t)!=0)?1:0)];
};


typedef uint8_t hed25519_sha512_t[64];

typedef struct hed25519_sha512_config hed25519_sha512_config_t;
struct hed25519_sha512_config
{
    int (*starts)(void *ctx);
    int (*update)(void *ctx,const void *data,size_t datalen);
    int (*finish)(void *ctx,hed25519_sha512_t sha512);
};

extern const hed25519_sha512_config_t hed25519_sha512_default_config;

/** \brief sha512开始
 *
 * \param cfg const hed25519_sha512_config_t* 配置
 * \param ctx hed25519_sha512_context_t* 上下文
 * \return int 0= 未出错
 *
 */
int hed25519_sha512_starts(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx);

/** \brief sha512更新
 *
 * \param cfg const hed25519_sha512_config_t* 配置
 * \param ctx hed25519_sha512_context_t* 上下文
 * \param data const void* 数据
 * \param datalen size_t 数据长度
 * \return int 0= 未出错
 *
 */
int hed25519_sha512_update(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx,const void *data,size_t datalen);

/** \brief sha512完成
 *
 * \param cfg const hed25519_sha512_config_t* 配置
 * \param ctx hed25519_sha512_context_t* 上下文
 * \param sha512 hed25519_sha512_t 输出的sha512值
 * \return int 0= 未出错
 *
 */
int hed25519_sha512_finish(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx,hed25519_sha512_t sha512);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_SHA512_H__
