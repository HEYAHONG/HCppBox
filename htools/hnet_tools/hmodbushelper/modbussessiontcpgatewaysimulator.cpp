#include "modbussessiontcpgatewaysimulator.h"

typedef struct
{
    modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t mb_ctx;
    wxSharedPtr<wxSocketServer> server;
    wxSharedPtr<wxSocketBase> client;
} ModbusSessionTCPGatewaySimulatorContext_t;

ModbusSessionTCPGatewaySimulator::ModbusSessionTCPGatewaySimulator()
{
    SetOnData([&](HSocketServer&,wxSharedPtr<wxSocketServer> server,wxSharedPtr<wxSocketBase> client)
    {
        if(client==NULL || !client->IsData())
        {
            return;
        }
        ModbusSessionTCPGatewaySimulatorContext_t context= {modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_context_default(),server,client};
        ModbusContextInit(&context.mb_ctx);
        context.mb_ctx.reply=[](modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx,const uint8_t *adu,size_t adu_length)
        {
            const ModbusSessionTCPGatewaySimulatorContext_t *context=GET_STRUCT_PTR_BY_MEMBER_PTR(ctx,ModbusSessionTCPGatewaySimulatorContext_t,mb_ctx);
            if(context==NULL || context->client==NULL)
            {
                return;
            }
            context->client->WaitForWrite();
            context->client->Write(adu,adu_length);
            context->client->WaitForWrite();
        };
        client->WaitForRead(0,5);
        uint8_t adu[MODBUS_TCP_MAX_ADU_LENGTH]= {0};
        client->Read(adu,sizeof(adu));
        modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_parse_input(&context.mb_ctx,adu,client->LastReadCount());
        client->Discard();
        SimulatorDataUpdate=true;
    });
}

ModbusSessionTCPGatewaySimulator::~ModbusSessionTCPGatewaySimulator()
{

}

bool  ModbusSessionTCPGatewaySimulator::OnModbusReadCoil(modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(m_coils.find(addr)!=m_coils.end())
    {
        return m_coils[addr];
    }
    return (addr%2)!=0;
}
bool  ModbusSessionTCPGatewaySimulator::OnModbusReadDiscreteInput(modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(m_discrete_inputs.find(addr)!=m_discrete_inputs.end())
    {
        return m_discrete_inputs[addr];
    }
    return !OnModbusReadCoil(addr);
}
modbus_data_register_t  ModbusSessionTCPGatewaySimulator::OnModbusReadHoldingRegister(modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(m_holding_registers.find(addr)!=m_holding_registers.end())
    {
        return m_holding_registers[addr];
    }
    return 0xDEAD;
}
modbus_data_register_t  ModbusSessionTCPGatewaySimulator::OnModbusReadInputRegister(modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(m_input_registers.find(addr)!=m_input_registers.end())
    {
        return m_input_registers[addr];
    }
    return ~OnModbusReadHoldingRegister(addr);
}

void    ModbusSessionTCPGatewaySimulator::OnModbusWriteCoil(modbus_data_address_t addr,bool value)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    m_coils[addr]=value;
}

void    ModbusSessionTCPGatewaySimulator::OnModbusHoldingRegister(modbus_data_address_t addr,modbus_data_register_t value)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    m_holding_registers[addr]=value;
}
bool    ModbusSessionTCPGatewaySimulator::OnModbusCheckAnycastCondition()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    return false;
}


void ModbusSessionTCPGatewaySimulator::ModbusContextInit(modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx)
{
    if(ctx==NULL)
    {
        return;
    }
    ctx->usr=this;
    ctx->slave.read_coil=ModbusReadCoil;
    ctx->slave.read_discrete_input=ModbusReadDiscreteInput;
    ctx->slave.read_holding_register=ModbusReadHoldingRegister;
    ctx->slave.read_input_register=ModbusReadInputRegister;
    ctx->slave.write_coil=ModbusWriteCoil;
    ctx->slave.write_holding_register=ModbusWriteHoldingRegister;
    ctx->slave.check_anycast_condition=ModbusCheckAnycastCondition;
}

bool  ModbusSessionTCPGatewaySimulator::ModbusReadCoil(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return false;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    return context.OnModbusReadCoil(addr);
}
bool  ModbusSessionTCPGatewaySimulator::ModbusReadDiscreteInput(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return false;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    return context.OnModbusReadDiscreteInput(addr);
}
modbus_data_register_t  ModbusSessionTCPGatewaySimulator::ModbusReadHoldingRegister(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return 0xDEAD;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    return context.OnModbusReadHoldingRegister(addr);
}

modbus_data_register_t  ModbusSessionTCPGatewaySimulator::ModbusReadInputRegister(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return 0xBEEF;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    return context.OnModbusReadInputRegister(addr);
}

void    ModbusSessionTCPGatewaySimulator::ModbusWriteCoil(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr,bool value)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return ;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    context.OnModbusWriteCoil(addr,value);
}
void    ModbusSessionTCPGatewaySimulator::ModbusWriteHoldingRegister(modbus_rtu_slave_tiny_context_t* r_ctx,modbus_data_address_t addr,modbus_data_register_t value)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return ;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    context.OnModbusHoldingRegister(addr,value);
}
bool    ModbusSessionTCPGatewaySimulator::ModbusCheckAnycastCondition(modbus_rtu_slave_tiny_context_t* r_ctx)
{
    const modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx=GET_STRUCT_PTR_BY_MEMBER_PTR(r_ctx,modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t,slave);
    if(ctx==NULL || ctx->usr==NULL)
    {
        return false;
    }
    ModbusSessionTCPGatewaySimulator & context=*(ModbusSessionTCPGatewaySimulator *)ctx->usr;
    return context.OnModbusCheckAnycastCondition();
}

