/***************************************************************
 * Name:      hmodbus.c
 * Purpose:   实现hmodbus接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hmodbus.h"


#include "modbus/hmodbus_crc.c"
#include "modbus/hmodbus_data.c"
#include "modbus/hmodbus_io_interface.c"
#include "modbus/hmodbus_rtu.c"
#include "modbus/hmodbus_rtu_master.c"
#include "modbus/hmodbus_rtu_slave_tiny.c"
#include "modbus/hmodbus_tcp.c"
#include "modbus/hmodbus_tcp_client.c"
#include "modbus/hmodbus_tcp_gateway_server.c"
