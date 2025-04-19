/***************************************************************
 * Name:      hmd4.h
 * Purpose:   声明hmd4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMD4_H__
#define __HMD4_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * MD4
 */
typedef uint8_t hmd4_md4_t[16];


/*
 * MD4 消息块
 */
typedef uint8_t hmd4_message_block_t[64];

/*
 * MD4上下文
 */
typedef struct hmd4_context hmd4_context_t;
struct hmd4_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    hmd4_message_block_t buffer;
};


/** \brief md4 开始
 *
 * \param ctx hmd4_context_t* md4上下文指针
 * \return int 0=未出错
 *
 */
int hmd4_starts(hmd4_context_t *ctx);

/** \brief md4 更新
 *
 * \param ctx hmd4_context_t* md4上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hmd4_update(hmd4_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief md4 完成
 *
 * \param ctx hmd4_context_t* md4上下文指针
 * \param output hmd4_md4_t 输出MD4
 * \return int 0=未出错
 *
 */
int hmd4_finish(hmd4_context_t *ctx,hmd4_md4_t output);

/** \brief md4计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hmd4_md4_t 输出MD4
 * \return int 0=未出错
 *
 */
int hmd4_md4(const uint8_t *input,size_t ilen,hmd4_md4_t output);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMD4_H__
