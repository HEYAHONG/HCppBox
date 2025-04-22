/***************************************************************
 * Name:      hmodbus_crc.h
 * Purpose:   hmodbus_crc
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_CRC_H__
#define __MODBUS_HMODBUS_CRC_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief 检查一帧数据的crc
 *
 * \param adu 整帧数据(包含CRC)的指针
 * \param adu_length 长度(包含CRC)
 * \return CRC是否通过
 *
 */
bool modbus_rtu_adu_check_crc(const uint8_t *adu,size_t adu_length);



/** \brief 在数据帧末尾的添加crc校验
 *
 * \param adu 整帧数据的指针，需要在末尾留两个字节以填写CRC
 * \param adu_length 长度(包含CRC)
 * \return 是否调用成功
 *
 */
bool modbus_rtu_adu_append_crc(uint8_t *adu,size_t adu_length);


#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_CRC_H__



