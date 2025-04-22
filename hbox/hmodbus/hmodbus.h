/***************************************************************
 * Name:      hmodbus.h
 * Purpose:   hmodbus接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_H_INCLUDED__
#define __MODBUS_HMODBUS_H_INCLUDED__

#include "hmodbus_common.h"
#include "hmodbus_crc.h"
#include "hmodbus_data.h"
#include "hmodbus_rtu.h"
#include "hmodbus_tcp.h"
#include "hmodbus_rtu_slave_tiny.h"
#include "hmodbus_tcp_gateway_server.h"
#include "hmodbus_io_interface.h"
#include "hmodbus_rtu_master.h"
#include "hmodbus_tcp_client.h"

//导入用户扩展定义(由标准文档保留但被本库使用的定义)头文件
#include "hmodbus_user_extend.h"

#endif // HMODBUS_H_INCLUDED
