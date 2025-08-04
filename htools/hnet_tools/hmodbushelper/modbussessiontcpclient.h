#ifndef MODBUSSESSIONTCPCLIENT_H
#define MODBUSSESSIONTCPCLIENT_H
#include "modbussessionbase.h"
#include <wx/socket.h>
#include "common_wxwidgets.h"

#ifdef __cplusplus

class ModbusSessionTCPClient:public ModbusSessionBase,public wxSocketClient
{
public:
    ModbusSessionTCPClient();
    virtual ~ModbusSessionTCPClient();

    bool ModbusReadCoils(uint16_t addr,size_t length);
    bool ModbusWriteCoils(uint16_t addr,size_t length);
    bool ModbusWriteSingleCoil(uint16_t addr);
    bool ModbusReadDiscreteInput(uint16_t addr,size_t length);

protected:
    virtual void OnModbusException(uint8_t function_code,uint8_t exception_code);
    virtual void OnModbusWrite(const uint8_t *adu,size_t adu_length);
    virtual void OnModbusRead(const uint8_t *buffer,size_t buffer_length);
    modbus_io_interface_t GetModbusIoInterface();

private:
    static size_t modbus_io_send(modbus_io_interface_t *io,const uint8_t *adu,size_t adu_length);
    static size_t modbus_io_recv(modbus_io_interface_t *io,uint8_t *buffer,size_t buffer_length);
};

#endif // __cplusplus

#endif // MODBUSSESSIONTCPCLIENT_H
