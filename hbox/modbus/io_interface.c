#include "modbus.h"

modbus_io_interface_t modbus_io_interface_default()
{
    modbus_io_interface_t io= {0};
    return io;
}

static bool modbus_io_interface_request_rtu(modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    if(io==NULL || io->send == NULL || io->recv==NULL)
    {
        return false;
    }
    switch(function_code)
    {
    default:
        break;
    }
    return false;
}

static bool modbus_io_interface_request_tcp(modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length,bool is_gateway)
{
    if(io==NULL || io->send == NULL || io->recv==NULL)
    {
        return false;
    }
    switch(function_code)
    {
    default:
        break;
    }
    return false;
}

bool modbus_io_interface_request(modbus_io_interface_request_t type,modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    switch(type)
    {
    case    MODBUS_IO_INTERFACE_TCP_CLIENT:
    {
        return modbus_io_interface_request_tcp(io,function_code,context,context_length,false);
    }
    break;
    case    MODBUS_IO_INTERFACE_RTU_MASTER:
    {
        return modbus_io_interface_request_rtu(io,function_code,context,context_length);
    }
    break;
    case MODBUS_IO_INTERFACE_TCP_GATEWAY_CLIENT:
    {
        return modbus_io_interface_request_tcp(io,function_code,context,context_length,true);
    }
    break;
    default:
        break;
    }
    return false;
}



