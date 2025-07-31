#include "modbussessionbase.h"

ModbusSessionBase::ModbusSessionBase():m_lock(new std::recursive_mutex())
{

}

ModbusSessionBase::~ModbusSessionBase()
{
    if(m_lock!=NULL)
    {
        delete m_lock;
    }
}

ModbusSessionBase::ModbusSessionBase(const ModbusSessionBase& other):m_lock(new std::recursive_mutex())
{
    CopyDataFromOther(other);
}

ModbusSessionBase& ModbusSessionBase::operator=(const ModbusSessionBase& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    CopyDataFromOther(rhs);

    return *this;
}

std::map<uint16_t,bool> ModbusSessionBase::GetAllCoils()
{
    std::lock_guard<std::recursive_mutex> lock_self(*m_lock);
    return m_coils;
}
std::map<uint16_t,bool> ModbusSessionBase::GetAllDiscreteInputs()
{
    std::lock_guard<std::recursive_mutex> lock_self(*m_lock);
    return m_discrete_inputs;
}
std::map<uint16_t,uint16_t> ModbusSessionBase::GetAllHoldingRegisters()
{
    std::lock_guard<std::recursive_mutex> lock_self(*m_lock);
    return m_holding_registers;
}
std::map<uint16_t,uint16_t> ModbusSessionBase::GetAllInputRegisters()
{
    std::lock_guard<std::recursive_mutex> lock_self(*m_lock);
    return m_input_registers;
}

void ModbusSessionBase::CopyDataFromOther(const ModbusSessionBase &other)
{
    std::lock_guard<std::recursive_mutex> lock_self(*m_lock);
    std::lock_guard<std::recursive_mutex> lock_other(*other.m_lock);
    /*
     * 复制数据
     */
    m_coils=other.m_coils;
    m_discrete_inputs=other.m_discrete_inputs;
    m_holding_registers=other.m_holding_registers;
    m_input_registers=other.m_input_registers;
}
