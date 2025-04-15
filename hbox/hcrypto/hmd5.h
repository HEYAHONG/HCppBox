/***************************************************************
 * Name:      hmd5.h
 * Purpose:   声明hmd5接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMD5_H__
#define __HMD5_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * MD5
 */
typedef uint8_t hmd5_md5_t[16];


/*
 * MD5 消息块
 */
typedef uint8_t hmd5_message_block_t[64];

/*
 * MD5上下文
 */
typedef struct hmd5_context hmd5_context_t;
struct hmd5_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    hmd5_message_block_t buffer;
};


/** \brief md5 开始
 *
 * \param ctx hmd5_context_t* md5上下文指针
 * \return int 0=未出错
 *
 */
int hmd5_starts(hmd5_context_t *ctx);

/** \brief md5 更新
 *
 * \param ctx hmd5_context_t* md5上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hmd5_update(hmd5_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief md5 完成
 *
 * \param ctx hmd5_context_t* md5上下文指针
 * \param output hmd5_md5_t 输出MD5
 * \return int 0=未出错
 *
 */
int hmd5_finish(hmd5_context_t *ctx,hmd5_md5_t output);

/** \brief md5计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hmd5_md5_t 输出MD5
 * \return int 0=未出错
 *
 */
int hmd5_md5(const uint8_t *input,size_t ilen,hmd5_md5_t output);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMD5_H__
