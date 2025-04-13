/***************************************************************
 * Name:      hbcc.h
 * Purpose:   声明hbcc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HBCC_H__
#define __HBCC_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
/*
 * BCC校验，又称异或检验。将数据段的每一个字节异或可得出校验。
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief bcc 计算
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint8_t 校验结果
 *
 */
uint8_t hbcc_calculate(const uint8_t *data,size_t datalen);

/** \brief bcc 校验
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint8_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hbcc_check(const uint8_t *data,size_t datalen,uint8_t check);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HBCC_H__
