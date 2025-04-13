/***************************************************************
 * Name:      hlrc.h
 * Purpose:   声明hlrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLRC_H__
#define __HLRC_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
/*
 * LRC校验,通常用于Modbus Ascii模式，其校验算法为将所有字节求和，再取其补码
 */

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief lrc 计算
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return uint8_t 校验结果
 *
 */
uint8_t hlrc_calculate(const uint8_t *data,size_t datalen);

/** \brief lrc 校验
 *
 * \param data const uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \param check uint8_t 校验值
 * \return bool 是否校验成功
 *
 */
bool hlrc_check(const uint8_t *data,size_t datalen,uint8_t check);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLRC_H__
