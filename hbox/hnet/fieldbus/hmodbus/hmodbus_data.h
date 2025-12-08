/***************************************************************
 * Name:      hmodbus_data.h
 * Purpose:   hmodbus_data
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_DATA_H__
#define __MODBUS_HMODBUS_DATA_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief 从数据帧中获取uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \return uint16_t 读取的数据
 *
 */
uint16_t modbus_data_get_uint16_t(const uint8_t *data,size_t offset,size_t data_length);

/** \brief 向数据帧中获取设置uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \param val uint16_t 设置的数据
 *
 */
void modbus_data_set_uint16_t(uint8_t *data,size_t offset,size_t data_length,uint16_t val);



#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_DATA_H__



