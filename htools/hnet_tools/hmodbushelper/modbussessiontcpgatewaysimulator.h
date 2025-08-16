#ifndef MODBUSSESSIONTCPGATEWAYSIMULATOR_H
#define MODBUSSESSIONTCPGATEWAYSIMULATOR_H
#ifdef __cplusplus
#include "modbussessionbase.h"
#include "hsocketserver.h"
#include "HCPPBox.h"

class ModbusSessionTCPGatewaySimulator:public ModbusSessionBase,public HSocketServer
{
public:
    ModbusSessionTCPGatewaySimulator();
    virtual ~ModbusSessionTCPGatewaySimulator();
protected:
    virtual bool  OnModbusReadCoil(modbus_data_address_t addr);
    virtual bool  OnModbusReadDiscreteInput(modbus_data_address_t addr);
    virtual modbus_data_register_t  OnModbusReadHoldingRegister(modbus_data_address_t addr);
    virtual modbus_data_register_t  OnModbusReadInputRegister(modbus_data_address_t addr);
    virtual void    OnModbusWriteCoil(modbus_data_address_t addr,bool value);
    virtual void    OnModbusHoldingRegister(modbus_data_address_t addr,modbus_data_register_t value);
    virtual bool    OnModbusCheckAnycastCondition();
    bool SimulatorDataUpdate;
private:
    void ModbusContextInit(modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t * ctx);
    static bool  ModbusReadCoil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);
    static bool  ModbusReadDiscreteInput(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);
    static modbus_data_register_t  ModbusReadHoldingRegister(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);
    static modbus_data_register_t  ModbusReadInputRegister(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);
    static void    ModbusWriteCoil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,bool value);
    static void    ModbusWriteHoldingRegister(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,modbus_data_register_t value);
    static bool    ModbusCheckAnycastCondition(modbus_rtu_slave_tiny_context_t* ctx);
};

#endif // __cplusplus

#endif // MODBUSSESSIONTCPGATEWAYSIMULATOR_H
