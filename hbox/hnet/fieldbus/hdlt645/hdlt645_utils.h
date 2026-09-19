/***************************************************************
 * Name:      hdlt645_utils.h
 * Purpose:   声明hdlt645_utils接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_UTILS_H_INCLUDED__
#define __HDLT645_UTILS_H_INCLUDED__

#include "hdlt645_common.h"
#include "math.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 数字转BCD码
 *
 * \param data uint64_t 数字
 * \return uint64_t BCD码
 *
 */
uint64_t hdlt645_uint64_to_bcd(uint64_t data);

/** \brief 数字转BCD码
 *
 * \param bcd uint64_t BCD码
 * \return uint64_t 数字
 *
 */
uint64_t hdlt645_bcd_to_uint64(uint64_t bcd);


/** \brief 设置BCD码（小端）
 *
 * \param bcd uint8_t* BCD码指针
 * \param bcd_length size_t BCD码长度
 * \param data uint64_t 数字（普通）
 *
 */
void hdlt645_bcd_le_set(uint8_t *bcd,size_t bcd_length,uint64_t data);

/** \brief 读取BCD码（小端）
 *
 * \param bcd uint8_t* BCD码指针
 * \param bcd_length size_t BCD码长度
 * \return uint64_t 数字（普通）
 *
 */
uint64_t hdlt645_bcd_le_get(const uint8_t *bcd,size_t bcd_length);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_UTILS_H_INCLUDED
