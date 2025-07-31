#ifndef MODBUSSESSIONTCPCLIENT_H
#define MODBUSSESSIONTCPCLIENT_H
#include "modbussessionbase.h"

#ifdef __cplusplus

class ModbusSessionTCPClient:public ModbusSessionBase
{
public:
    ModbusSessionTCPClient();
    virtual ~ModbusSessionTCPClient();

protected:

private:
};

#endif // __cplusplus

#endif // MODBUSSESSIONTCPCLIENT_H
