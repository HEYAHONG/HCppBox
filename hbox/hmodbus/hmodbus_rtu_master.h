/***************************************************************
 * Name:      hmodbus_rtu_master.h
 * Purpose:   hmodbus_rtu_master
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_RTU_MASTER_H__
#define __MODBUS_HMODBUS_RTU_MASTER_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "hmodbus_io_interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef modbus_io_interface_t modbus_rtu_master_io_interface_t;

/** \brief modbus rtu主机请求
 *          注意：此函数对栈的要求较高，需要保证栈足够大
 *
 * \param io modbus_rtu_master_io_interface_t* IO接口
 * \param function_code uint8_t 功能码，见MODBUS_FC_*
 * \param context void* 上下文，根据功能码的不同而不同
 * \param context_length size_t 上下文长度
 * \return bool 是否成功请求
 *
 */
bool modbus_rtu_master_request(modbus_rtu_master_io_interface_t *io,uint8_t function_code,void *context,size_t context_length);

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_RTU_MASTER_H__



