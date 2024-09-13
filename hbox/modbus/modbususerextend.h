#ifndef __MODBUS_MODBUSUSEREXTEND_H__
#define __MODBUS_MODBUSUSEREXTEND_H__
#include "modbus.h"
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

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_MODBUSUSEREXTEND_H__



