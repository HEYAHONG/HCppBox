/***************************************************************
 * Name:      io_interface.c
 * Purpose:   io接口，通常用于主机
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"

modbus_io_interface_t modbus_io_interface_default(void)
{
    modbus_io_interface_t io= {0};
    return io;
}

modbus_io_interface_context_base_t modbus_io_interface_context_base_default(void)
{
    modbus_io_interface_context_base_t ctx= {0};
    ctx.slave_addr=MODBUS_NODE_ADDRESS_DEFAULT;
    return ctx;
}

modbus_io_interface_context_read_coils_t modbus_io_interface_context_read_coils_default(void)
{
    modbus_io_interface_context_read_coils_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_coils=1;
    return ctx;
}

modbus_io_interface_context_read_discrete_inputs_t modbus_io_interface_context_read_discrete_inputs_default(void)
{
    modbus_io_interface_context_read_discrete_inputs_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_inputs=1;
    return ctx;
}

modbus_io_interface_context_read_holding_registers_t modbus_io_interface_context_read_holding_registers_default(void)
{
    modbus_io_interface_context_read_holding_registers_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_registers=1;
    return ctx;
}

modbus_io_interface_context_read_input_registers_t modbus_io_interface_context_read_input_registers_default(void)
{
    modbus_io_interface_context_read_input_registers_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_registers=1;
    return ctx;
}

modbus_io_interface_context_write_single_coil_t modbus_io_interface_context_write_single_coil_default(void)
{
    modbus_io_interface_context_write_single_coil_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_write_single_register_t modbus_io_interface_context_write_single_register_default(void)
{
    modbus_io_interface_context_write_single_register_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_read_exception_status_t modbus_io_interface_context_read_exception_status_default(void)
{
    modbus_io_interface_context_read_exception_status_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_diagnostics_t modbus_io_interface_context_diagnostics_default(void)
{
    modbus_io_interface_context_diagnostics_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_get_comm_event_counter_t modbus_io_interface_context_get_comm_event_counter_default(void)
{
    modbus_io_interface_context_get_comm_event_counter_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_get_comm_event_log_t modbus_io_interface_context_get_comm_event_log_default(void)
{
    modbus_io_interface_context_get_comm_event_log_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_write_multiple_coils_t modbus_io_interface_context_write_multiple_coils_default(void)
{
    modbus_io_interface_context_write_multiple_coils_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_output=1;
    return ctx;
}

modbus_io_interface_context_write_multiple_registers_t modbus_io_interface_context_write_multiple_registers_default(void)
{
    modbus_io_interface_context_write_multiple_registers_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_of_output=1;
    return ctx;
}

modbus_io_interface_context_report_server_id_t modbus_io_interface_context_report_server_id_default(void)
{
    modbus_io_interface_context_report_server_id_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_read_file_record_t modbus_io_interface_context_read_file_record_default(void)
{
    modbus_io_interface_context_read_file_record_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.x_max=1;
    return ctx;
}

modbus_io_interface_context_write_file_record_t modbus_io_interface_context_write_file_record_default(void)
{
    modbus_io_interface_context_write_file_record_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.x_max=1;
    return ctx;
}

modbus_io_interface_context_mask_write_register_t modbus_io_interface_context_mask_write_register_default(void)
{
    modbus_io_interface_context_mask_write_register_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_read_write_multiple_registers_t modbus_io_interface_context_read_write_multiple_registers_default(void)
{
    modbus_io_interface_context_read_write_multiple_registers_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    ctx.quantity_to_read=1;
    ctx.quantity_to_write=1;
    return ctx;
}

modbus_io_interface_context_read_fifo_queue_t modbus_io_interface_context_read_fifo_queue_default(void)
{
    modbus_io_interface_context_read_fifo_queue_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
    return ctx;
}

modbus_io_interface_context_encapsulated_interface_transport_t modbus_io_interface_context_encapsulated_interface_transport_default(void)
{
    modbus_io_interface_context_encapsulated_interface_transport_t ctx= {0};
    ctx.base=modbus_io_interface_context_base_default();
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


static bool hmodbus_read_coils_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_coils_t *fc_ctx=(modbus_io_interface_context_read_coils_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_COILS)
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
                        if((data[i/8]&(1<<(i%8)))!=0)
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

static bool hmodbus_read_coils_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_coils_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_read_discrete_inputs_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_discrete_inputs_t *fc_ctx=(modbus_io_interface_context_read_discrete_inputs_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_DISCRETE_INPUTS)
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
                        if((data[i/8]&(1<<(i%8)))!=0)
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

static bool hmodbus_read_discrete_inputs_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_discrete_inputs_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_read_holding_registers_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_holding_registers_t *fc_ctx=(modbus_io_interface_context_read_holding_registers_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_HOLDING_REGISTERS)
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
            modbus_data_register_t length=fc_ctx->quantity_of_registers;
            if(length>MODBUS_MAX_READ_REGISTERS)
            {
                length=MODBUS_MAX_READ_REGISTERS;
            }
            if(pdu_length>=(2+length*2))
            {
                const uint8_t *data=&pdu[2];
                if(fc_ctx->on_read_holding_registers!=NULL)
                {
                    for(size_t i=0; i<length; i++)
                    {
                        fc_ctx->on_read_holding_registers(fc_ctx,addr+i,modbus_data_get_uint16_t(data,i*2,pdu_length-2));
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_read_holding_registers_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_holding_registers_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_read_input_registers_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_input_registers_t *fc_ctx=(modbus_io_interface_context_read_input_registers_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_INPUT_REGISTERS)
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
            modbus_data_register_t length=fc_ctx->quantity_of_registers;
            if(length>MODBUS_MAX_READ_REGISTERS)
            {
                length=MODBUS_MAX_READ_REGISTERS;
            }
            if(pdu_length>=(2+length*2))
            {
                const uint8_t *data=&pdu[2];
                if(fc_ctx->on_read_input_registers!=NULL)
                {
                    for(size_t i=0; i<length; i++)
                    {
                        fc_ctx->on_read_input_registers(fc_ctx,addr+i,modbus_data_get_uint16_t(data,i*2,pdu_length-2));
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_read_input_registers_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_input_registers_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_write_single_coil_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_write_single_coil_t *fc_ctx=(modbus_io_interface_context_write_single_coil_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_SINGLE_COIL)
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
            if(pdu_length>=5)
            {
                uint16_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
                if(value==0xFF00)
                {
                    fc_ctx->output_value=true;
                }
                if(value==0x0000)
                {
                    fc_ctx->output_value=false;
                }
                if(fc_ctx->on_write_single_coil!=NULL)
                {
                    fc_ctx->on_write_single_coil(fc_ctx,addr,value);
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_write_single_coil_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_write_single_coil_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}


static bool hmodbus_write_single_register_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_write_single_register_t *fc_ctx=(modbus_io_interface_context_write_single_register_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_SINGLE_REGISTER)
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
            if(pdu_length>=5)
            {
                uint16_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t value=modbus_data_get_uint16_t(pdu,3,pdu_length);
                fc_ctx->output_value=value;
                if(fc_ctx->on_write_single_register!=NULL)
                {
                    fc_ctx->on_write_single_register(fc_ctx,addr,value);
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_write_single_register_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_write_single_register_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_read_exception_status_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_exception_status_t *fc_ctx=(modbus_io_interface_context_read_exception_status_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS)
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
            if(pdu_length>=2)
            {
                uint8_t exception_code=pdu[1];
                if(fc_ctx->on_read_exception_status!=NULL)
                {
                    fc_ctx->on_read_exception_status(fc_ctx,exception_code);
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_read_exception_status_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_exception_status_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_diagnostics_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_diagnostics_t *fc_ctx=(modbus_io_interface_context_diagnostics_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_SERIAL_DIAGNOSTICS)
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
            if(pdu_length>=3)
            {
                fc_ctx->sub_function=modbus_data_get_uint16_t(pdu,1,pdu_length);
                const uint8_t *data=&pdu[3];
                size_t data_length=pdu_length-3;
                if(fc_ctx->response_data!=NULL)
                {
                    return fc_ctx->response_data(fc_ctx,data,data_length);
                }
            }
        }
    }
    return false;
}

static bool hmodbus_diagnostics_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_diagnostics_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_get_comm_event_counter_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_get_comm_event_counter_t *fc_ctx=(modbus_io_interface_context_get_comm_event_counter_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER)
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
            if(pdu_length>=5)
            {
                uint16_t status=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t event_count=modbus_data_get_uint16_t(pdu,3,pdu_length);
                if(fc_ctx->on_get_comm_event_counter!=NULL)
                {
                    fc_ctx->on_get_comm_event_counter(fc_ctx,status,event_count);
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_get_comm_event_counter_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_get_comm_event_counter_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}


static bool hmodbus_get_comm_event_log_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_get_comm_event_log_t *fc_ctx=(modbus_io_interface_context_get_comm_event_log_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG)
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
            if(pdu_length>=8)
            {
                uint8_t  byte_count=pdu[1];
                uint16_t status=modbus_data_get_uint16_t(pdu,2,pdu_length);
                uint16_t event_count=modbus_data_get_uint16_t(pdu,4,pdu_length);
                uint16_t message_count=modbus_data_get_uint16_t(pdu,6,pdu_length);
                if(fc_ctx->on_get_comm_event_log!=NULL)
                {
                    fc_ctx->on_get_comm_event_log(fc_ctx,status,event_count,message_count,&pdu[8],byte_count-6);
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_get_comm_event_log_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_get_comm_event_log_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_write_multiple_coils_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_write_multiple_coils_t *fc_ctx=(modbus_io_interface_context_write_multiple_coils_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_MULTIPLE_COILS)
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
            if(pdu_length>=5)
            {
                uint16_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t length=modbus_data_get_uint16_t(pdu,3,pdu_length);
                if(fc_ctx->on_write_multiple_coils!=NULL)
                {
                    fc_ctx->on_write_multiple_coils(fc_ctx,addr,length);
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_write_multiple_coils_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_write_multiple_coils_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_write_multiple_registers_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_write_multiple_registers_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_MULTIPLE_REGISTERS)
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
            if(pdu_length>=5)
            {
                uint16_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t length=modbus_data_get_uint16_t(pdu,3,pdu_length);
                if(fc_ctx->on_write_multiple_registers!=NULL)
                {
                    fc_ctx->on_write_multiple_registers(fc_ctx,addr,length);
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_write_multiple_registers_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_write_multiple_registers_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_report_server_id_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_report_server_id_t *fc_ctx=(modbus_io_interface_context_report_server_id_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_SERIAL_REPORT_SERVER_ID)
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
            if(pdu_length>=2)
            {
                size_t server_id_length=pdu[1];
                const uint8_t *server_id=&pdu[2];
                if(fc_ctx->on_report_server_id!=NULL)
                {
                    fc_ctx->on_report_server_id(fc_ctx,server_id,server_id_length);
                }
            }
        }
    }
    return false;
}

static bool hmodbus_report_server_id_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_report_server_id_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}


static bool hmodbus_read_file_record_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_file_record_t *fc_ctx=(modbus_io_interface_context_read_file_record_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_FILE_RECORD)
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
            if(pdu_length>=2 && fc_ctx->on_read_file_record!=NULL)
            {
                int byte_count=pdu[1];
                int byte_read=0;
                size_t x=0;
                const uint8_t *sub_req_data=&pdu[2];
                while((byte_count-byte_read)>0)
                {
                    uint8_t sub_req_len=sub_req_data[0];
                    uint8_t sub_req_reference_type=sub_req_data[1];
                    fc_ctx->on_read_file_record(fc_ctx,x,sub_req_reference_type,&sub_req_data[2],sub_req_len-1);
                    x++;
                    byte_read+=(sub_req_len+1);
                    sub_req_data=&sub_req_data[sub_req_len+1];
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_read_file_record_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_file_record_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}


static bool hmodbus_write_file_record_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_write_file_record_t *fc_ctx=(modbus_io_interface_context_write_file_record_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_FILE_RECORD)
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
            if(pdu_length>=2)
            {
                int byte_count=pdu[1];
                int byte_read=0;
                if(fc_ctx->on_write_file_record!=NULL)
                {
                    size_t x=0;
                    while(byte_count>byte_read)
                    {
                        const uint8_t *sub_req=&pdu[2+byte_read];
                        uint8_t reference_type=sub_req[0];
                        uint16_t file_number=modbus_data_get_uint16_t(sub_req,1,byte_count-byte_read);
                        uint16_t record_number=modbus_data_get_uint16_t(sub_req,3,byte_count-byte_read);
                        uint16_t record_length=modbus_data_get_uint16_t(sub_req,5,byte_count-byte_read);
                        fc_ctx->on_write_file_record(fc_ctx,x,reference_type,file_number,record_number,&sub_req[7],record_length*2);
                        byte_read+=(7+record_length*2);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_write_file_record_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_write_file_record_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_mask_write_register_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_mask_write_register_t *fc_ctx=(modbus_io_interface_context_mask_write_register_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_MASK_WRITE_REGISTER)
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
            if(pdu_length>=7)
            {
                uint16_t addr=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t and_mask=modbus_data_get_uint16_t(pdu,3,pdu_length);
                uint16_t or_mask=modbus_data_get_uint16_t(pdu,5,pdu_length);
                if(fc_ctx->on_mask_write_register!=NULL)
                {
                    fc_ctx->on_mask_write_register(fc_ctx,addr,and_mask,or_mask);
                }
                return true;
            }
        }
    }
    return false;
}

static bool hmodbus_mask_write_register_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_mask_write_register_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}


static bool hmodbus_read_write_multiple_registers_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_read_write_multiple_registers_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_WRITE_AND_READ_REGISTERS)
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
            if(pdu_length>=2)
            {
                uint8_t byte_count=pdu[1];
                if(fc_ctx->on_read_multiple_registers!=NULL)
                {
                    for(size_t i=0; i<byte_count/2; i++)
                    {
                        uint16_t value=modbus_data_get_uint16_t(pdu,2+i*2,pdu_length);
                        fc_ctx->on_read_multiple_registers(fc_ctx,fc_ctx->read_starting_address+i,value);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_read_write_multiple_registers_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_write_multiple_registers_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_read_fifo_queue_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_read_fifo_queue_t *fc_ctx=(modbus_io_interface_context_read_fifo_queue_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_READ_FIFO_QUEUE)
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
            if(pdu_length>=5)
            {
                uint16_t byte_count=modbus_data_get_uint16_t(pdu,1,pdu_length);
                uint16_t fifo_count=modbus_data_get_uint16_t(pdu,3,pdu_length);
                const uint8_t *data=&pdu[5];
                if(fc_ctx->on_read_fifo_queue!=NULL && byte_count==(fifo_count*2+2))
                {
                    fc_ctx->on_read_fifo_queue(fc_ctx,data,fifo_count*2);
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_read_fifo_queue_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_read_fifo_queue_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
}

static bool hmodbus_encapsulated_interface_transport_tcp_pdu_callback(uint16_t TId,uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    modbus_io_interface_context_encapsulated_interface_transport_t *fc_ctx=(modbus_io_interface_context_encapsulated_interface_transport_t*)usr;
    if(pdu!=NULL && pdu_length > 1)
    {
        uint8_t function_code=pdu[0];
        if((function_code&0x7F)!=MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT)
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
            if(pdu_length>=2)
            {
                uint8_t mei_type=pdu[1];
                fc_ctx->mei_type=mei_type;
                if(fc_ctx->mei_response!=NULL)
                {
                    fc_ctx->mei_response(fc_ctx,&pdu[2],pdu_length-2);
                    return true;
                }
            }
        }
    }
    return false;
}

static bool hmodbus_encapsulated_interface_transport_rtu_pdu_callback(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr)
{
    return hmodbus_encapsulated_interface_transport_tcp_pdu_callback(0,node_address,pdu,pdu_length,usr);
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
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_coils_rtu_pdu_callback,fc_ctx);
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
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_discrete_inputs_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_holding_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_holding_registers_t *fc_ctx=(modbus_io_interface_context_read_holding_registers_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_registers);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_holding_registers_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_INPUT_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_input_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_input_registers_t *fc_ctx=(modbus_io_interface_context_read_input_registers_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_registers);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_input_registers_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_COIL:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_single_coil_t))
        {
            return false;
        }
        modbus_io_interface_context_write_single_coil_t *fc_ctx=(modbus_io_interface_context_write_single_coil_t*)ctx;
        size_t pdu_length=5;//1字节功能码+地址+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->output_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->output_value?0xFF00:0x0000);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_write_single_coil_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_single_register_t))
        {
            return false;
        }
        modbus_io_interface_context_write_single_register_t *fc_ctx=(modbus_io_interface_context_write_single_register_t*)ctx;
        size_t pdu_length=5;//1字节功能码+地址+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->output_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->output_value);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_write_single_register_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_exception_status_t))
        {
            return false;
        }
        modbus_io_interface_context_read_exception_status_t *fc_ctx=(modbus_io_interface_context_read_exception_status_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_exception_status_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_DIAGNOSTICS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_diagnostics_t))
        {
            return false;
        }
        modbus_io_interface_context_diagnostics_t *fc_ctx=(modbus_io_interface_context_diagnostics_t*)ctx;
        if(fc_ctx->request_data==NULL)
        {
            return false;
        }
        size_t pdu_length=3;//1字节功能码+2字节子功能码
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->sub_function);
        {
            //添加请求数据
            pdu_length+=fc_ctx->request_data(fc_ctx,&pdu[pdu_length],MODBUS_MAX_PDU_LENGTH-pdu_length);
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_diagnostics_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_get_comm_event_counter_t))
        {
            return false;
        }
        modbus_io_interface_context_get_comm_event_counter_t *fc_ctx=(modbus_io_interface_context_get_comm_event_counter_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_get_comm_event_counter_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG:
    {
        if(context_length!=sizeof(modbus_io_interface_context_get_comm_event_log_t))
        {
            return false;
        }
        modbus_io_interface_context_get_comm_event_log_t *fc_ctx=(modbus_io_interface_context_get_comm_event_log_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_get_comm_event_log_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_multiple_coils_t))
        {
            return false;
        }
        modbus_io_interface_context_write_multiple_coils_t *fc_ctx=(modbus_io_interface_context_write_multiple_coils_t*)ctx;
        if(fc_ctx->coil_value==NULL || fc_ctx->quantity_of_output == 0 )
        {
            return false;
        }
        if(fc_ctx->quantity_of_output > MODBUS_MAX_WRITE_BITS)
        {
            fc_ctx->quantity_of_output=MODBUS_MAX_WRITE_BITS;
        }
        size_t byte_count=fc_ctx->quantity_of_output/8+((fc_ctx->quantity_of_output%8!=0)?1:0);
        size_t pdu_length=6+byte_count;//1字节功能码+2字节起始地址+2字节数量+1字节字节长度+数据
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_output);
        pdu[5]=byte_count;
        {
            uint8_t *data=&pdu[6];
            for(size_t i=0; i<fc_ctx->quantity_of_output; i++)
            {
                if(i%8==0)
                {
                    //0填充
                    data[i/8]=0x0;
                }
                bool value=false;
                fc_ctx->coil_value(fc_ctx,fc_ctx->starting_address+i,&value);
                if(value)
                {
                    data[i/8] |= (1<<(i%8));
                }
                else
                {
                    data[i/8] &= (~(1<<(i%8)));
                }
            }
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_write_multiple_coils_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_multiple_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_write_multiple_registers_t*)ctx;
        if(fc_ctx->register_value==NULL || fc_ctx->quantity_of_output == 0 )
        {
            return false;
        }
        if(fc_ctx->quantity_of_output > MODBUS_MAX_WRITE_REGISTERS)
        {
            fc_ctx->quantity_of_output=MODBUS_MAX_WRITE_REGISTERS;
        }
        size_t byte_count=fc_ctx->quantity_of_output*2;
        size_t pdu_length=6+byte_count;//1字节功能码+2字节起始地址+2字节数量+1字节字节长度+数据
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_output);
        pdu[5]=byte_count;
        {
            uint8_t *data=&pdu[6];
            for(size_t i=0; i<fc_ctx->quantity_of_output; i++)
            {
                modbus_data_register_t value=0;
                fc_ctx->register_value(fc_ctx,fc_ctx->starting_address+i,&value);
                modbus_data_set_uint16_t(data,i*2,byte_count,value);
            }
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_write_multiple_registers_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_REPORT_SERVER_ID:
    {
        if(context_length!=sizeof(modbus_io_interface_context_report_server_id_t))
        {
            return false;
        }
        modbus_io_interface_context_report_server_id_t *fc_ctx=(modbus_io_interface_context_report_server_id_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_report_server_id_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_FILE_RECORD:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_file_record_t))
        {
            return false;
        }
        modbus_io_interface_context_read_file_record_t *fc_ctx=(modbus_io_interface_context_read_file_record_t*)ctx;
        if(fc_ctx->file_record==NULL || fc_ctx->x_max==0)
        {
            return false;
        }
        if(fc_ctx->x_max>35)
        {
            fc_ctx->x_max=35;
        }
        uint8_t byte_count=fc_ctx->x_max*7;
        size_t pdu_length=2+byte_count;//1字节功能码+1字节字节长度+子请求
        pdu[0]=function_code;
        pdu[1]=byte_count;
        for(size_t i=0; i<fc_ctx->x_max; i++)
        {
            uint8_t *sub_req=&pdu[2+7*i];
            uint8_t reference_type=0x06;
            uint16_t file_number=0;
            uint16_t record_number=0;
            uint16_t record_length=0;
            fc_ctx->file_record(fc_ctx,i,&reference_type,&file_number,&record_number,&record_length);
            sub_req[0]=reference_type;
            modbus_data_set_uint16_t(sub_req,1,7,file_number);
            modbus_data_set_uint16_t(sub_req,3,7,record_number);
            modbus_data_set_uint16_t(sub_req,5,7,record_length);
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_file_record_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_FILE_RECORD:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_file_record_t))
        {
            return false;
        }
        modbus_io_interface_context_write_file_record_t *fc_ctx=(modbus_io_interface_context_write_file_record_t*)ctx;
        if(fc_ctx->file_record==NULL || fc_ctx->x_max==0)
        {
            return false;
        }
        uint8_t byte_count=0;
        for(size_t i=0; i<fc_ctx->x_max; i++)
        {
            if(byte_count+2+7 > MODBUS_MAX_PDU_LENGTH)
            {
                //超过最大PDU长度
                break;
            }
            uint8_t *sub_req=&pdu[2+byte_count];
            uint8_t reference_type=0x06;
            uint16_t file_number=0;
            uint16_t record_number=0;
            size_t len=fc_ctx->file_record(fc_ctx,i,&reference_type,&file_number,&record_number,&sub_req[7],MODBUS_MAX_PDU_LENGTH-7-(2+byte_count));
            sub_req[0]=reference_type;
            modbus_data_set_uint16_t(sub_req,1,7,file_number);
            modbus_data_set_uint16_t(sub_req,3,7,record_number);
            modbus_data_set_uint16_t(sub_req,5,7,len/2);
            byte_count+=(7+len/2*2);//增加的长度为7+寄存器长度的整倍数
        }
        size_t pdu_length=2+byte_count;//1字节功能码+1字节字节长度+子请求
        pdu[0]=function_code;
        pdu[1]=byte_count;
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_write_file_record_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_MASK_WRITE_REGISTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_mask_write_register_t))
        {
            return false;
        }
        modbus_io_interface_context_mask_write_register_t *fc_ctx=(modbus_io_interface_context_mask_write_register_t*)ctx;
        size_t pdu_length=7;//1字节功能码+地址+and_mask+or_mask
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->reference_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->and_mask);
        modbus_data_set_uint16_t(pdu,5,pdu_length,fc_ctx->or_mask);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_mask_write_register_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_write_multiple_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_read_write_multiple_registers_t*)ctx;
        if(fc_ctx->write_register_value==NULL || fc_ctx->quantity_to_read==0 || fc_ctx->quantity_to_write==0)
        {
            return false;
        }
        if(fc_ctx->quantity_to_read>MODBUS_MAX_WR_READ_REGISTERS)
        {
            fc_ctx->quantity_to_read=MODBUS_MAX_WR_READ_REGISTERS;
        }
        if(fc_ctx->quantity_to_write>MODBUS_MAX_WR_WRITE_REGISTERS)
        {
            fc_ctx->quantity_to_write=MODBUS_MAX_WR_WRITE_REGISTERS;
        }
        uint8_t byte_count=fc_ctx->quantity_to_write*2;
        size_t pdu_length=10+byte_count;//1字节功能码+读地址+读数量+写地址+写数量+字节数+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->read_starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_to_read);
        modbus_data_set_uint16_t(pdu,5,pdu_length,fc_ctx->write_starting_address);
        modbus_data_set_uint16_t(pdu,7,pdu_length,fc_ctx->quantity_to_write);
        pdu[9]=byte_count;
        {
            for(size_t i=0; i<fc_ctx->quantity_to_write; i++)
            {
                modbus_data_register_t value=0;
                fc_ctx->write_register_value(fc_ctx,fc_ctx->write_starting_address+i,&value);
                modbus_data_set_uint16_t(pdu,10+2*i,pdu_length,value);
            }
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_write_multiple_registers_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_FIFO_QUEUE:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_fifo_queue_t))
        {
            return false;
        }
        modbus_io_interface_context_read_fifo_queue_t *fc_ctx=(modbus_io_interface_context_read_fifo_queue_t*)ctx;
        size_t pdu_length=3;//1字节功能码+2字节FIFO指针地址
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->fifo_pointer_address);
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_read_fifo_queue_rtu_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT:
    {
        if(context_length!=sizeof(modbus_io_interface_context_encapsulated_interface_transport_t))
        {
            return false;
        }
        modbus_io_interface_context_encapsulated_interface_transport_t *fc_ctx=(modbus_io_interface_context_encapsulated_interface_transport_t*)ctx;
        size_t pdu_length=2;//1字节功能码+1字节mei_type
        pdu[0]=function_code;
        pdu[1]=fc_ctx->mei_type;
        {
            //添加封装数据
            if(fc_ctx->mei_request!=NULL)
            {
                pdu_length+=fc_ctx->mei_request(fc_ctx,&pdu[pdu_length],MODBUS_MAX_PDU_LENGTH-pdu_length);
            }
        }
        size_t req_len=modbus_rtu_set_pdu_to_adu(buffer,sizeof(buffer),ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_rtu_get_pdu_from_adu(buffer,resp_len,hmodbus_encapsulated_interface_transport_rtu_pdu_callback,fc_ctx);
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
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_coils_tcp_pdu_callback,fc_ctx);
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
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_discrete_inputs_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_holding_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_holding_registers_t *fc_ctx=(modbus_io_interface_context_read_holding_registers_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_registers);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_holding_registers_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_INPUT_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_input_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_input_registers_t *fc_ctx=(modbus_io_interface_context_read_input_registers_t*)ctx;
        size_t pdu_length=5;//1字节功能码+起始地址+数量
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_registers);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_input_registers_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_COIL:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_single_coil_t))
        {
            return false;
        }
        modbus_io_interface_context_write_single_coil_t *fc_ctx=(modbus_io_interface_context_write_single_coil_t*)ctx;
        size_t pdu_length=5;//1字节功能码+地址+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->output_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->output_value?0xFF00:0x0000);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_write_single_coil_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_single_register_t))
        {
            return false;
        }
        modbus_io_interface_context_write_single_register_t *fc_ctx=(modbus_io_interface_context_write_single_register_t*)ctx;
        size_t pdu_length=5;//1字节功能码+地址+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->output_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->output_value);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_write_single_register_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_READ_EXCEPTION_STATUS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_exception_status_t))
        {
            return false;
        }
        modbus_io_interface_context_read_exception_status_t *fc_ctx=(modbus_io_interface_context_read_exception_status_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_exception_status_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_DIAGNOSTICS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_diagnostics_t))
        {
            return false;
        }
        modbus_io_interface_context_diagnostics_t *fc_ctx=(modbus_io_interface_context_diagnostics_t*)ctx;
        if(fc_ctx->request_data==NULL)
        {
            return false;
        }
        size_t pdu_length=3;//1字节功能码+2字节子功能码
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->sub_function);
        {
            //添加请求数据
            pdu_length+=fc_ctx->request_data(fc_ctx,&pdu[pdu_length],MODBUS_MAX_PDU_LENGTH-pdu_length);
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_diagnostics_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_COUNTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_get_comm_event_counter_t))
        {
            return false;
        }
        modbus_io_interface_context_get_comm_event_counter_t *fc_ctx=(modbus_io_interface_context_get_comm_event_counter_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_get_comm_event_counter_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_GET_COMM_EVENT_LOG:
    {
        if(context_length!=sizeof(modbus_io_interface_context_get_comm_event_log_t))
        {
            return false;
        }
        modbus_io_interface_context_get_comm_event_log_t *fc_ctx=(modbus_io_interface_context_get_comm_event_log_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_get_comm_event_log_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_multiple_coils_t))
        {
            return false;
        }
        modbus_io_interface_context_write_multiple_coils_t *fc_ctx=(modbus_io_interface_context_write_multiple_coils_t*)ctx;
        if(fc_ctx->coil_value==NULL || fc_ctx->quantity_of_output == 0 )
        {
            return false;
        }
        if(fc_ctx->quantity_of_output > MODBUS_MAX_WRITE_BITS)
        {
            fc_ctx->quantity_of_output=MODBUS_MAX_WRITE_BITS;
        }
        size_t byte_count=fc_ctx->quantity_of_output/8+((fc_ctx->quantity_of_output%8!=0)?1:0);
        size_t pdu_length=6+byte_count;//1字节功能码+2字节起始地址+2字节数量+1字节字节长度+数据
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_output);
        pdu[5]=byte_count;
        {
            uint8_t *data=&pdu[6];
            for(size_t i=0; i<fc_ctx->quantity_of_output; i++)
            {
                if(i%8==0)
                {
                    //0填充
                    data[i/8]=0x0;
                }
                bool value=false;
                fc_ctx->coil_value(fc_ctx,fc_ctx->starting_address+i,&value);
                if(value)
                {
                    data[i/8] |= (1<<(i%8));
                }
                else
                {
                    data[i/8] &= (~(1<<(i%8)));
                }
            }
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_write_multiple_coils_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_multiple_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_write_multiple_registers_t*)ctx;
        if(fc_ctx->register_value==NULL || fc_ctx->quantity_of_output == 0 )
        {
            return false;
        }
        if(fc_ctx->quantity_of_output > MODBUS_MAX_WRITE_REGISTERS)
        {
            fc_ctx->quantity_of_output=MODBUS_MAX_WRITE_REGISTERS;
        }
        size_t byte_count=fc_ctx->quantity_of_output*2;
        size_t pdu_length=6+byte_count;//1字节功能码+2字节起始地址+2字节数量+1字节字节长度+数据
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_of_output);
        pdu[5]=byte_count;
        {
            uint8_t *data=&pdu[6];
            for(size_t i=0; i<fc_ctx->quantity_of_output; i++)
            {
                modbus_data_register_t value=0;
                fc_ctx->register_value(fc_ctx,fc_ctx->starting_address+i,&value);
                modbus_data_set_uint16_t(data,i*2,byte_count,value);
            }
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_write_multiple_registers_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_SERIAL_REPORT_SERVER_ID:
    {
        if(context_length!=sizeof(modbus_io_interface_context_report_server_id_t))
        {
            return false;
        }
        modbus_io_interface_context_report_server_id_t *fc_ctx=(modbus_io_interface_context_report_server_id_t*)ctx;
        size_t pdu_length=1;//1字节功能码
        pdu[0]=function_code;
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_report_server_id_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_FILE_RECORD:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_file_record_t))
        {
            return false;
        }
        modbus_io_interface_context_read_file_record_t *fc_ctx=(modbus_io_interface_context_read_file_record_t*)ctx;
        if(fc_ctx->file_record==NULL || fc_ctx->x_max==0)
        {
            return false;
        }
        if(fc_ctx->x_max>35)
        {
            fc_ctx->x_max=35;
        }
        uint8_t byte_count=fc_ctx->x_max*7;
        size_t pdu_length=2+byte_count;//1字节功能码+1字节字节长度+子请求
        pdu[0]=function_code;
        pdu[1]=byte_count;
        for(size_t i=0; i<fc_ctx->x_max; i++)
        {
            uint8_t *sub_req=&pdu[2+7*i];
            uint8_t reference_type=0x06;
            uint16_t file_number=0;
            uint16_t record_number=0;
            uint16_t record_length=0;
            fc_ctx->file_record(fc_ctx,i,&reference_type,&file_number,&record_number,&record_length);
            sub_req[0]=reference_type;
            modbus_data_set_uint16_t(sub_req,1,7,file_number);
            modbus_data_set_uint16_t(sub_req,3,7,record_number);
            modbus_data_set_uint16_t(sub_req,5,7,record_length);
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_file_record_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_FILE_RECORD:
    {
        if(context_length!=sizeof(modbus_io_interface_context_write_file_record_t))
        {
            return false;
        }
        modbus_io_interface_context_write_file_record_t *fc_ctx=(modbus_io_interface_context_write_file_record_t*)ctx;
        if(fc_ctx->file_record==NULL || fc_ctx->x_max==0)
        {
            return false;
        }
        uint8_t byte_count=0;
        for(size_t i=0; i<fc_ctx->x_max; i++)
        {
            if(byte_count+2+7 > MODBUS_MAX_PDU_LENGTH)
            {
                //超过最大PDU长度
                break;
            }
            uint8_t *sub_req=&pdu[2+byte_count];
            uint8_t reference_type=0x06;
            uint16_t file_number=0;
            uint16_t record_number=0;
            size_t len=fc_ctx->file_record(fc_ctx,i,&reference_type,&file_number,&record_number,&sub_req[7],MODBUS_MAX_PDU_LENGTH-7-(2+byte_count));
            sub_req[0]=reference_type;
            modbus_data_set_uint16_t(sub_req,1,7,file_number);
            modbus_data_set_uint16_t(sub_req,3,7,record_number);
            modbus_data_set_uint16_t(sub_req,5,7,len/2);
            byte_count+=(7+len/2*2);//增加的长度为7+寄存器长度的整倍数
        }
        size_t pdu_length=2+byte_count;//1字节功能码+1字节字节长度+子请求
        pdu[0]=function_code;
        pdu[1]=byte_count;
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_write_file_record_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_MASK_WRITE_REGISTER:
    {
        if(context_length!=sizeof(modbus_io_interface_context_mask_write_register_t))
        {
            return false;
        }
        modbus_io_interface_context_mask_write_register_t *fc_ctx=(modbus_io_interface_context_mask_write_register_t*)ctx;
        size_t pdu_length=7;//1字节功能码+地址+and_mask+or_mask
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->reference_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->and_mask);
        modbus_data_set_uint16_t(pdu,5,pdu_length,fc_ctx->or_mask);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_mask_write_register_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_write_multiple_registers_t))
        {
            return false;
        }
        modbus_io_interface_context_read_write_multiple_registers_t *fc_ctx=(modbus_io_interface_context_read_write_multiple_registers_t*)ctx;
        if(fc_ctx->write_register_value==NULL || fc_ctx->quantity_to_read==0 || fc_ctx->quantity_to_write==0)
        {
            return false;
        }
        if(fc_ctx->quantity_to_read>MODBUS_MAX_WR_READ_REGISTERS)
        {
            fc_ctx->quantity_to_read=MODBUS_MAX_WR_READ_REGISTERS;
        }
        if(fc_ctx->quantity_to_write>MODBUS_MAX_WR_WRITE_REGISTERS)
        {
            fc_ctx->quantity_to_write=MODBUS_MAX_WR_WRITE_REGISTERS;
        }
        uint8_t byte_count=fc_ctx->quantity_to_write*2;
        size_t pdu_length=10+byte_count;//1字节功能码+读地址+读数量+写地址+写数量+字节数+值
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->read_starting_address);
        modbus_data_set_uint16_t(pdu,3,pdu_length,fc_ctx->quantity_to_read);
        modbus_data_set_uint16_t(pdu,5,pdu_length,fc_ctx->write_starting_address);
        modbus_data_set_uint16_t(pdu,7,pdu_length,fc_ctx->quantity_to_write);
        pdu[9]=byte_count;
        {
            for(size_t i=0; i<fc_ctx->quantity_to_write; i++)
            {
                modbus_data_register_t value=0;
                fc_ctx->write_register_value(fc_ctx,fc_ctx->write_starting_address+i,&value);
                modbus_data_set_uint16_t(pdu,10+2*i,pdu_length,value);
            }
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_write_multiple_registers_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_READ_FIFO_QUEUE:
    {
        if(context_length!=sizeof(modbus_io_interface_context_read_fifo_queue_t))
        {
            return false;
        }
        modbus_io_interface_context_read_fifo_queue_t *fc_ctx=(modbus_io_interface_context_read_fifo_queue_t*)ctx;
        size_t pdu_length=3;//1字节功能码+2字节FIFO指针地址
        pdu[0]=function_code;
        modbus_data_set_uint16_t(pdu,1,pdu_length,fc_ctx->fifo_pointer_address);
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_read_fifo_queue_tcp_pdu_callback,fc_ctx);
            }
        }
    }
    break;
    case MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT:
    {
        if(context_length!=sizeof(modbus_io_interface_context_encapsulated_interface_transport_t))
        {
            return false;
        }
        modbus_io_interface_context_encapsulated_interface_transport_t *fc_ctx=(modbus_io_interface_context_encapsulated_interface_transport_t*)ctx;
        size_t pdu_length=2;//1字节功能码+1字节mei_type
        pdu[0]=function_code;
        pdu[1]=fc_ctx->mei_type;
        {
            //添加封装数据
            if(fc_ctx->mei_request!=NULL)
            {
                pdu_length+=fc_ctx->mei_request(fc_ctx,&pdu[pdu_length],MODBUS_MAX_PDU_LENGTH-pdu_length);
            }
        }
        size_t req_len=modbus_tcp_set_pdu_to_adu(buffer,sizeof(buffer),Tid,ctx->slave_addr,pdu,pdu_length);
        if(req_len==io->send(io,buffer,req_len))
        {
            size_t resp_len=io->recv(io,buffer,sizeof(buffer));
            if(resp_len>0)
            {
                return modbus_tcp_get_pdu_from_adu(buffer,resp_len,hmodbus_encapsulated_interface_transport_tcp_pdu_callback,fc_ctx);
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
    bool ret=false;
    switch(type)
    {
    case    MODBUS_IO_INTERFACE_TCP_CLIENT:
    {
        ret = modbus_io_interface_request_tcp(io,function_code,context,context_length,false);
    }
    break;
    case    MODBUS_IO_INTERFACE_RTU_MASTER:
    {
        ret = modbus_io_interface_request_rtu(io,function_code,context,context_length);
    }
    break;
    case MODBUS_IO_INTERFACE_TCP_GATEWAY_CLIENT:
    {
        ret = modbus_io_interface_request_tcp(io,function_code,context,context_length,true);
    }
    break;
    default:
        break;
    }
    return ret;
}



