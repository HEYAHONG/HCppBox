/***************************************************************
 * Name:      io_interface.c
 * Purpose:   io接口，通常用于主机
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "modbus.h"

modbus_io_interface_t modbus_io_interface_default()
{
    modbus_io_interface_t io= {0};
    return io;
}

modbus_io_interface_context_base_t modbus_io_interface_context_base_default()
{
    modbus_io_interface_context_base_t ctx= {0};
    ctx.slave_addr=MODBUS_NODE_ADDRESS_DEFAULT;
    return ctx;
}

modbus_io_interface_context_read_coils_t modbus_io_interface_context_read_coils_default()
{
    modbus_io_interface_context_read_coils_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_coils=1;
    return ctx;
}

modbus_io_interface_context_read_discrete_inputs_t modbus_io_interface_context_read_discrete_inputs_default()
{
    modbus_io_interface_context_read_discrete_inputs_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_inputs=1;
    return ctx;
}

static bool modbus_io_interface_is_serialline_only_function_code(uint8_t function_code)
{
    bool ret=false;
    switch(function_code)
    {
    case MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS:
    case MODBUS_FC_SERIAL_DIAGNOSTICS:
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER:
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG:
    case MODBUS_FC_SERIAL_REPORT_SERVER_ID:
    {
        ret=true;
    }
    break;
    default:
        break;
    }
    return ret;
}

static uint16_t modbus_io_interface_get_Tid_and_increase(modbus_io_interface_context_base_t *ctx)
{
    if(ctx!=NULL)
    {
        return ctx->Tid++;
    }
    return 0;
}


static bool read_coils_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_coils_t *fc_ctx=(modbus_io_interface_context_read_coils_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if(function_code!=MODBUS_FC_READ_COILS)
        {
            return false;
        }
        if(function_code>MODBUS_FC_EXCEPTION_BASE)
        {
            if(pdu_length>=2)
            {
                uint8_t exception=pdu[1];
                if(fc_ctx->base.on_exception!=NULL)
                {
                    fc_ctx->base.on_exception(&fc_ctx->base,function_code,exception);
                    //用户处理了异常，视为成功
                    return true;
                }
            }
        }
        else
        {
            modbus_data_address_t addr=fc_ctx->starting_address;
            modbus_data_register_t length=fc_ctx->quantity_of_coils;
            if(length>MODBUS_MAX_READ_BITS)
            {
                length=MODBUS_MAX_READ_BITS;
            }
            if(pdu_length>=(1+length/8))
            {
                const uint8_t *data=&pdu[2];
                if(fc_ctx->on_read_coils!=NULL)
                {
                    for(size_t i=0; i<length; i++)
                    {
                        if((data[i/8]&(1<<(i%8))!=0))
                        {
                            fc_ctx->on_read_coils(fc_ctx,addr+i,true);
                        }
                        else
                        {
                            fc_ctx->on_read_coils(fc_ctx,addr+i,false);
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool read_coils_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return read_coils_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool read_discrete_inputs_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_discrete_inputs_t *fc_ctx=(modbus_io_interface_context_read_discrete_inputs_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if(function_code!=MODBUS_FC_READ_DISCRETE_INPUTS)
        {
            return false;
        }
        if(function_code>MODBUS_FC_EXCEPTION_BASE)
        {
            if(pdu_length>=2)
            {
                uint8_t exception=pdu[1];
                if(fc_ctx->base.on_exception!=NULL)
                {
                    fc_ctx->base.on_exception(&fc_ctx->base,function_code,exception);
                    //用户处理了异常，视为成功
                    return true;
                }
            }
        }
        else
        {
            modbus_data_address_t addr=fc_ctx->starting_address;
            modbus_data_register_t length=fc_ctx->quantity_of_inputs;
            if(length>MODBUS_MAX_READ_BITS)
            {
                length=MODBUS_MAX_READ_BITS;
            }
            if(pdu_length>=(1+length/8))
            {
                const uint8_t *data=&pdu[2];
                if(fc_ctx->on_read_discrete_inputs!=NULL)
                {
                    for(size_t i=0; i<length; i++)
                    {
                        if((data[i/8]&(1<<(i%8))!=0))
                        {
                            fc_ctx->on_read_discrete_inputs(fc_ctx,addr+i,true);
                        }
                        else
                        {
                            fc_ctx->on_read_discrete_inputs(fc_ctx,addr+i,false);
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool read_discrete_inputs_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return read_discrete_inputs_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool modbus_io_interface_request_rtu(modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    modbus_io_interface_context_base_t *ctx=(modbus_io_interface_context_base_t *)context;
    modbus_io_interface_get_Tid_and_increase(ctx);
    uint8_t buffer[MODBUS_RTU_MAX_ADU_LENGTH];
    uint8_t *pdu=&buffer[1];
    switch(function_code)
    {
    case MODBUS_FC_READ_COILS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_coils_t))
        {
            return false;
        }
        modbus_io_interface_context_read_coils_t *fc_ctx=(modbus_io_interface_context_read_coils_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_coils);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,read_coils_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_discrete_inputs_t))
        {
            return false;
        }
        modbus_io_interface_context_read_discrete_inputs_t *fc_ctx=(modbus_io_interface_context_read_discrete_inputs_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_inputs);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,read_discrete_inputs_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    default:
        break;
    }
    return false;
}

static bool modbus_io_interface_request_tcp(modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length,bool is_gateway)
{
    if(!is_gateway)
    {
        if(modbus_io_interface_is_serialline_only_function_code(function_code))
        {
            //modbus tcp默认不响应串口专用功能码
            return false;
        }
    }
    modbus_io_interface_context_base_t *ctx=(modbus_io_interface_context_base_t *)context;
    uint16_t Tid=modbus_io_interface_get_Tid_and_increase(ctx);
    uint8_t buffer[MODBUS_TCP_MAX_ADU_LENGTH];
    uint8_t *pdu=&buffer[7];
    switch(function_code)
    {
    case MODBUS_FC_READ_COILS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_coils_t))
        {
            return false;
        }
        modbus_io_interface_context_read_coils_t *fc_ctx=(modbus_io_interface_context_read_coils_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_coils);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,read_coils_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_discrete_inputs_t))
        {
            return false;
        }
        modbus_io_interface_context_read_discrete_inputs_t *fc_ctx=(modbus_io_interface_context_read_discrete_inputs_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_inputs);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,read_discrete_inputs_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    default:
        break;
    }
    return false;
}

bool modbus_io_interface_request(modbus_io_interface_request_t type,modbus_io_interface_t *io,uint8_t function_code,void *context,size_t context_length)
{
    if(context == NULL || context_length < sizeof(modbus_io_interface_context_base_t))
    {
        //传入的不是上下文指针
        return false;
    }
    if(io==NULL || io->send == NULL || io->recv==NULL)
    {
        return false;
    }
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



