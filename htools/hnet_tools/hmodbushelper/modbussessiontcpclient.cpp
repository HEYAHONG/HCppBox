#include "modbussessiontcpclient.h"

ModbusSessionTCPClient::ModbusSessionTCPClient():wxSocketClient(wxSOCKET_NOWAIT_READ)
{

}

bool ModbusSessionTCPClient::ModbusReadCoils(uint16_t addr,size_t length)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_read_coils_t ctx=modbus_io_interface_context_read_coils_default();
    ctx.usr=this;
    ctx.quantity_of_coils=length;
    ctx.starting_address=addr;
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.on_read_coils=[](modbus_io_interface_context_read_coils_t *ctx,modbus_data_address_t addr,bool value)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        std::lock_guard<std::recursive_mutex> lock(*client.m_lock);
        client.m_coils[addr]=value;
    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_COILS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusWriteCoils(uint16_t addr,size_t length)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_write_multiple_coils_t ctx=modbus_io_interface_context_write_multiple_coils_default();
    ctx.usr=this;
    ctx.starting_address=addr;
    ctx.quantity_of_output=length;
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.coil_value=[](modbus_io_interface_context_write_multiple_coils_t *ctx,modbus_data_address_t addr,bool *value)
    {
        if(ctx==NULL || ctx->usr==NULL || value==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        std::lock_guard<std::recursive_mutex> lock(*client.m_lock);
        (*value)=client.m_coils[addr];
    };
    ctx.on_write_multiple_coils=[](modbus_io_interface_context_write_multiple_coils_t *ctx,modbus_data_address_t addr,modbus_data_register_t length)
    {

    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_MULTIPLE_COILS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusWriteSingleCoil(uint16_t addr)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_write_single_coil_t ctx=modbus_io_interface_context_write_single_coil_default();
    ctx.usr=this;
    ctx.output_address=addr;
    {
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        ctx.output_value=m_coils[addr];
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.on_write_single_coil=[](modbus_io_interface_context_write_single_coil_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {

    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_SINGLE_COIL,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusReadDiscreteInput(uint16_t addr,size_t length)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_read_discrete_inputs_t ctx=modbus_io_interface_context_read_discrete_inputs_default();
    ctx.usr=this;
    ctx.quantity_of_inputs=length;
    ctx.starting_address=addr;
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.on_read_discrete_inputs=[](modbus_io_interface_context_read_discrete_inputs_t *ctx,modbus_data_address_t addr,bool value)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        std::lock_guard<std::recursive_mutex> lock(*client.m_lock);
        client.m_discrete_inputs[addr]=value;
    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_DISCRETE_INPUTS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusReadHoldingRegisters(uint16_t addr,size_t length)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_read_holding_registers_t ctx=modbus_io_interface_context_read_holding_registers_default();
    ctx.usr=this;
    ctx.quantity_of_registers=length;
    ctx.starting_address=addr;
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.on_read_holding_registers=[](modbus_io_interface_context_read_holding_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        std::lock_guard<std::recursive_mutex> lock(*client.m_lock);
        client.m_holding_registers[addr]=value;
    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_HOLDING_REGISTERS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusWriteHoldingRegisters(uint16_t addr,size_t length)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_write_multiple_registers_t ctx=modbus_io_interface_context_write_multiple_registers_default();
    ctx.usr=this;
    ctx.starting_address=addr;
    ctx.quantity_of_output=length;
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.register_value=[](modbus_io_interface_context_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t *value)
    {
        if(ctx==NULL || ctx->usr==NULL || value==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        std::lock_guard<std::recursive_mutex> lock(*client.m_lock);
        (*value)=client.m_holding_registers[addr];
    };
    ctx.on_write_multiple_registers=[](modbus_io_interface_context_write_multiple_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t length)
    {

    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_MULTIPLE_REGISTERS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

bool ModbusSessionTCPClient::ModbusWriteSingleHoldingRegister(uint16_t addr)
{
    modbus_io_interface_t io=GetModbusIoInterface();
    modbus_io_interface_context_write_single_register_t ctx=modbus_io_interface_context_write_single_register_default();
    ctx.usr=this;
    ctx.output_address=addr;
    {
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        ctx.output_value=m_holding_registers[addr];
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        if(ctx==NULL || ctx->usr==NULL)
        {
            return;
        }
        ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)ctx->usr;
        client.OnModbusException(function_code,exception_code);
    };
    ctx.on_write_single_register=[](modbus_io_interface_context_write_single_register_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {

    };
    return modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_SINGLE_REGISTER,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx));
}

void ModbusSessionTCPClient::OnModbusException(uint8_t function_code,uint8_t exception_code)
{

}

void ModbusSessionTCPClient::OnModbusWrite(const uint8_t *adu,size_t adu_length)
{

}

void ModbusSessionTCPClient::OnModbusRead(const uint8_t *buffer,size_t buffer_length)
{

}

modbus_io_interface_t ModbusSessionTCPClient::GetModbusIoInterface()
{
    modbus_io_interface_t io= {this,modbus_io_send,modbus_io_recv};
    return io;
}

size_t ModbusSessionTCPClient::modbus_io_send(modbus_io_interface_t *io,const uint8_t *adu,size_t adu_length)
{
    size_t ret=0;
    if(io==NULL || io->usr == NULL|| adu==NULL || adu_length==0)
    {
        return ret;
    }

    ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)io->usr;
    if(!client.IsConnected())
    {
        return ret;
    }

    if(!client.WaitForWrite(3))
    {
        return ret;
    }

    client.Discard();

    client.Write(adu,adu_length);

    ret=client.LastCount();

    client.OnModbusWrite(adu,ret);

    return ret;
}

size_t ModbusSessionTCPClient::modbus_io_recv(modbus_io_interface_t *io,uint8_t *buffer,size_t buffer_length)
{
    size_t ret=0;
    if(io==NULL || io->usr ==NULL  || buffer==NULL || buffer_length==0)
    {
        return ret;
    }

    ModbusSessionTCPClient &client=*(ModbusSessionTCPClient *)io->usr;
    if(!client.IsConnected())
    {
        return ret;
    }

    if(!client.WaitForRead(3))
    {
        return ret;
    }


    client.Read(buffer,buffer_length);

    ret=client.LastCount();

    client.OnModbusRead(buffer,ret);

    return ret;
}

ModbusSessionTCPClient::~ModbusSessionTCPClient()
{

}
