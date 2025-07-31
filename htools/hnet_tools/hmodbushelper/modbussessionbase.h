#ifndef __MODBUSSESSIONBASE_H__
#define __MODBUSSESSIONBASE_H__
#include "stdint.h"
#include "stdlib.h"

#ifdef __cplusplus
#include <map>
#include <mutex>

class ModbusSessionBase
{
protected:
    std::recursive_mutex *m_lock;
    std::map<uint16_t,bool> m_coils;
    std::map<uint16_t,bool> m_discrete_inputs;
    std::map<uint16_t,uint16_t> m_holding_registers;
    std::map<uint16_t,uint16_t> m_input_registers;
public:
    ModbusSessionBase();
    virtual ~ModbusSessionBase();
    ModbusSessionBase(const ModbusSessionBase& other);
    ModbusSessionBase& operator=(const ModbusSessionBase& other);

    virtual std::map<uint16_t,bool> GetAllCoils();
    virtual std::map<uint16_t,bool> GetAllDiscreteInputs();
    virtual std::map<uint16_t,uint16_t> GetAllHoldingRegisters();
    virtual std::map<uint16_t,uint16_t> GetAllInputRegisters();


private:
    void CopyDataFromOther(const ModbusSessionBase& other);
};


#endif // __cplusplus

#endif // __MODBUSSESSIONBASE_H__
