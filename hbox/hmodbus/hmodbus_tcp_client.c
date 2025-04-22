/***************************************************************
 * Name:      tcp_client.c
 * Purpose:   modbus tcp 客户端
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"


bool modbus_tcp_client_request(modbus_tcp_client_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    return modbus_io_interface_request(MODBUS_IO_INTERFACE_TCP_CLIENT,(modbus_io_interface_t *)io,function_code,context,context_length);
}


bool modbus_tcp_client_request_gateway(modbus_tcp_client_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    return modbus_io_interface_request(MODBUS_IO_INTERFACE_TCP_GATEWAY_CLIENT,(modbus_io_interface_t *)io,function_code,context,context_length);
}


