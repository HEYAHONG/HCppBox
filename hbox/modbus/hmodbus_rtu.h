/***************************************************************
 * Name:      hmodbus_rtu.h
 * Purpose:   hmodbus_rtu
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_RTU_H__
#define __MODBUS_HMODBUS_RTU_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief RTU的PDU数据帧回调
 */
typedef bool (*modbus_rtu_pdu_callback_t)(uint8_t node_address,const uint8_t *pdu,size_t pdu_length,void *usr);

/** \brief 从RTU的ADU数据中提取PDU数据
 *
 * \param adu uint8_t* ADU数据帧
 * \param adu_length size_t ADU数据帧长度
 * \param cb modbus_rtu_pdu_callback_t PDU数据帧回调
 * \param usr void* 用户参数,用于PDU数据帧回调
 * \return bool 是否成功提取
 *
 */
bool modbus_rtu_get_pdu_from_adu(const uint8_t *adu,size_t adu_length,modbus_rtu_pdu_callback_t cb,void *usr);

/** \brief 向RTU的ADU数据中设置PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param node_address uint8_t 节点地址
 * \param pdu const uint8_t* PDU数据指针
 * \param pdu_length size_t PDU长度
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_rtu_set_pdu_to_adu(uint8_t *adu,size_t adu_length,uint8_t node_address,const uint8_t *pdu,size_t pdu_length);


/** \brief 向RTU的ADU数据中设置异常PDU数据
 *
 * \param adu uint8_t* ADU数据指针
 * \param adu_length size_t ADU长度
 * \param TId uint16_t 传输ID，用于区分请求与响应是否为一对
 * \param node_address uint8_t 节点地址
 * \param function_code uint8_t 功能码
 * \param exception_code uint8_t 异常代码
 * \return size_t 实际ADU长度
 *
 */
size_t modbus_rtu_set_exception_pdu_to_adu(uint8_t *adu,size_t adu_length,uint8_t node_address,uint8_t function_code,uint8_t exception_code);



#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_RTU_H__



