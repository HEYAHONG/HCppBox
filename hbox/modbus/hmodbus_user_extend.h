/***************************************************************
 * Name:      hmodbus_user_extend.h
 * Purpose:   hmodbus用户扩展接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_USER_EXTEND_H__
#define __MODBUS_HMODBUS_USER_EXTEND_H__
#include "hmodbus.h"
/*
 * 此文件用于标准modbus进行扩展，通常使用modbus标准协议保留的功能。
 */
#ifdef __cplusplus
extern "C"
{
#endif


/*
 *  modbus保留地址定义
 */

/*
 *  任播地址，与普通的节点地址(即单播地址)功能类似，唯一区别是节点将不检查请求地址且以自身实际地址响应请求
 *  此地址通常用于一主一从的情况，也可用于从机地址改变后联系从机。
 *  此地址不可用于多从机网络，将出现严重的冲突。
 */
#ifndef MODBUS_ANYCAST_ADDRESS
#define MODBUS_ANYCAST_ADDRESS  (MODBUS_NODE_ADDRESS_RESERVED_MAX)
#endif

/*
 *  带条件的任播地址，与任播地址功能类似.其区别为节点可自行选择是否响应。
 *  此地址可用于多从机网络，其主要用法为当某些保持寄存器满足条件后才选择响应请求。
 *  此地址的主要用法为使用扩展地址（位于保持寄存器中），主要用法如下：
 *      -当主机需要对特定从机进行请求时，需要先通过广播向所有从机的保持寄存器写入扩展地址(位于保持寄存器中)
 *      -然后在使用此地址进行请求操作，这样就突破了modbus从机地址只有1字节的限制。
 */
#ifndef MODBUS_ANYCAST_WITH_CONDITION_ADDRESS
#define MODBUS_ANYCAST_WITH_CONDITION_ADDRESS  (MODBUS_NODE_ADDRESS_RESERVED_MAX-1)
#endif

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_USER_EXTEND_H__



