/***************************************************************
 * Name:      data.c
 * Purpose:   modbus 数据处理
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmodbus.h"

/** \brief 从数据帧中获取uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \return uint16_t 读取的数据
 *
 */
uint16_t modbus_data_get_uint16_t(const uint8_t *data,size_t offset,size_t data_length)
{
    uint16_t ret=0;
    if(data!=NULL && ((offset + 2) <= data_length))
    {
        ret=data[offset];
        ret<<=8;
        ret+=data[offset+1];
    }
    return ret;
}

/** \brief 向数据帧中获取设置uint16_t数据
 *
 * \param data uint8_t* 数据帧指针
 * \param offset size_t 数据在数据帧中的偏移
 * \param data_length size_t 数据帧长度
 * \param val uint16_t 设置的数据
 *
 */
void modbus_data_set_uint16_t(uint8_t *data,size_t offset,size_t data_length,uint16_t val)
{
    if(data!=NULL && ((offset + 2) <= data_length))
    {
        data[offset]=(val>>8);
        data[offset+1]=(val&0xFF);
    }
}


