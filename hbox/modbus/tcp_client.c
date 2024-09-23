#include "modbus.h"


bool modbus_tcp_client_request(modbus_tcp_client_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    return modbus_io_interface_request(MODBUS_IO_INTERFACE_TCP_CLIENT,(modbus_io_interface_t *)io,function_code,context,context_length);
}


bool modbus_tcp_client_request_gateway(modbus_tcp_client_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    return modbus_io_interface_request(MODBUS_IO_INTERFACE_TCP_GATEWAY_CLIENT,(modbus_io_interface_t *)io,function_code,context,context_length);
}


