/***************************************************************
 * Name:      hmodbus_rtu_slave_tiny.h
 * Purpose:   hmodbus_rtu_slave_tiny
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_RTU_SLAVE_TINY_H__
#define __MODBUS_HMODBUS_RTU_SLAVE_TINY_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  精简的modbus rtu协议,特点如下：
 *      -无异常处理，失败直接返回(不发送任何信息,可用于多种协议兼容（如当modbus协议失败后可执行其它协议,如AT协议）)
 *      -只支持0x01、0x02、0x03、0x04、0x06、0x0F、0x10、0x16、0x17功能码。
 *  适用场景如下：
 *      -资源极其有限的单片机。
 *  注意：
 *      -对未使用的结构体成员一定要初始化为0或NULL
 *      -多线程使用时需要加锁
 */
struct modbus_rtu_slave_tiny_context;
typedef struct modbus_rtu_slave_tiny_context modbus_rtu_slave_tiny_context_t;
struct modbus_rtu_slave_tiny_context
{
    uint8_t addr;//从机地址，当地址小于MODBUS_NODE_ADDRESS_MIN或大于MODBUS_NODE_ADDRESS_MAX时无效
    uint8_t *buffer;// 发送缓冲，长度为MODBUS_RTU_MAX_ADU_LENGTH，当发送缓冲为NULL时，采用栈作为发送缓冲，此时栈要足够大。
    void    *usr;//用户指针，由用户确定使用场景
    void    (*reply)(modbus_rtu_slave_tiny_context_t* ctx,const uint8_t *adu,size_t adu_length);//数据输出，当无数据输出时不能进行回复
    bool    (*read_coil)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);//读线圈
    bool    (*read_discrete_input)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);//读离散输入
    modbus_data_register_t  (*read_holding_register)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);//读保持寄存器
    modbus_data_register_t  (*read_input_register)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr);//读输入寄存器
    void    (*write_coil)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,bool value);//写线圈
    void    (*write_holding_register)(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,modbus_data_register_t value);//写保持寄存器
    bool    (*check_anycast_condition)(modbus_rtu_slave_tiny_context_t* ctx);//检查任播条件，标准modbus无此功能，由本库扩展
};


/** \brief  默认精简modbus rtu上下文，可用于初始化上下文。
 *          注意：默认情况下使用栈作为缓冲，需要确保栈足够大
 *          默认行为中，各个回调函数将返回假数据，在真实设备上需要替换掉相关回调函数
 *
 * \return modbus_rtu_slave_tiny_context_t 上下文，可用于初始化精简modubus rtu上下文
 *
 */
modbus_rtu_slave_tiny_context_t modbus_rtu_slave_tiny_context_default(void);


/** \brief  modbus rtu解析输入并返回
 *          注意:本操作未加锁，应当避免在多个线程中使用同一个上下文调用此函数。
 *
 * \param ctx modbus_rtu_slave_tiny_context_t*上下文指针
 * \param adu uint8_t* 请求数据包地址
 * \param adu_length size_t 请求数据包长度
 * \return bool 是否成功处理
 *
 */
bool modbus_rtu_slave_tiny_parse_input(modbus_rtu_slave_tiny_context_t* ctx,uint8_t *adu,size_t adu_length);


#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_RTU_SLAVE_TINY_H__



