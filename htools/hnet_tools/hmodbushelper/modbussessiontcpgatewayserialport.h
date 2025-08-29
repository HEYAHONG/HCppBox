#ifndef MODBUSSESSIONTCPGATEWAYSERIALPORT_H
#define MODBUSSESSIONTCPGATEWAYSERIALPORT_H

#include "libserialport.h"
#include "stdint.h"
#include "stdlib.h"
#include "HCPPBox.h"
#include "hsocketserver.h"

#ifdef __cplusplus
#include <string>

class ModbusSessionTCPGatewaySerialport:public HSocketServer
{
public:
    ModbusSessionTCPGatewaySerialport();
    virtual ~ModbusSessionTCPGatewaySerialport();
    virtual struct sp_port_config & GetSerialportConfig();
    virtual std::string & GetSerialportName();
    std::string ModbusRTURequest(std::string request,unsigned int timeout_ms=100);
protected:
    struct sp_port_config *serialport_config;
    std::string serialport_name;
    virtual void LocalLog(wxString format,...);
    int ModbusRTURequestTimeout;
    virtual void OnModbusRTUBegin();
    virtual void OnModbusRTUSend(std::string request);
    virtual void OnModbusRTUReceive(std::string reply);
    virtual void OnModbusRTUEnd();
};

#endif

#endif // MODBUSSESSIONTCPGATEWAYSERIALPORT_H
