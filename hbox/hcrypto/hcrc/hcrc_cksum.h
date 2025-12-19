/***************************************************************
 * Name:      hcrc_cksum.h
 * Purpose:   声明hcrc_cksum接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCRC_CKSUM_H__
#define __HCRC_CKSUM_H__
#include "crcgen/hcrc_crc32_cksum.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hcrc_cksum_context;
typedef struct hcrc_cksum_context hcrc_cksum_context_t;
struct hcrc_cksum_context
{
    uint32_t crc_ctx;
    uint64_t byte_count;
};

/** \brief cksum开始(查表法)
 *
 * \return hcksum_context_t cksum上下文
 *
 */
hcrc_cksum_context_t hcrc_cksum_starts(void);


/** \brief cksum更新(查表法)
 *
 * \param ctx hcrc_cksum_context_t  cksum上下文
 * \param data const uint32_t* 数据
 * \param datalen size_t 数据长度
 * \return hcrc_cksum_context_t 更新后的上下文
 *
 */
hcrc_cksum_context_t hcrc_cksum_update(hcrc_cksum_context_t ctx,const uint8_t *data,size_t datalen);


/** \brief cksum完成(查表法)
 *
 * \param ctx hcrc_cksum_context_t cksum上下文
 * \return uint32_t cksum
 *
 */
uint32_t hcrc_cksum_finish(hcrc_cksum_context_t ctx);


/** \brief cksum 计算(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint32_t 校验结果
 *
 */
uint32_t hcrc_cksum_calculate(const uint8_t *data,size_t datalen);

/** \brief cksum 校验(查表法)
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint32_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hcrc_cksum_check(const uint8_t *data,size_t datalen,uint32_t check);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCRC_CKSUM_H__
