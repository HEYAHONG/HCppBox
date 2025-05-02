/***************************************************************
 * Name:      hmodbus_tcp_gateway_server.h
 * Purpose:   hmodbus_tcp_gateway_server
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __MODBUS_HMODBUS_TCP_GATEWAY_SERVER_H__
#define __MODBUS_HMODBUS_TCP_GATEWAY_SERVER_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  modbus tcp gateway server,特点如下`：
 *      -用于将modbus tcp server转化为modbus rtu salve.
 *  注意：
 *      -对未使用的结构体成员一定要初始化为0或NULL
 *      -多线程使用时需要加锁
 */

struct modbus_tcp_gateway_server_context;
typedef struct modbus_tcp_gateway_server_context modbus_tcp_gateway_server_context_t;
struct modbus_tcp_gateway_server_context
{
    uint16_t TId;//发送标识，需要以此区分是否为同一对请求,由内部使用，用户设置无效
    uint8_t *tcp_buffer;//TCP缓冲，长度为MODBUS_TCP_MAX_ADU_LENGTH，当缓冲为NULL时，采用栈作为缓冲，此时栈要足够大。
    uint8_t *rtu_buffer;//RTU缓冲，长度为MODBUS_RTU_MAX_ADU_LENGTH，当缓冲为NULL时，采用栈作为缓冲，此时栈要足够大。
    void    *usr;//用户指针，由用户确定使用场景
    void    (*reply)(modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length);//数据输出，通常直接输出TCP数据，此函数的ADU主要指modbus tcp的ADU
    bool    (*rtu_request)(modbus_tcp_gateway_server_context_t *ctx,const uint8_t *adu,size_t adu_length,bool (*rtu_reply)(modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length));//modbus rtu请求,不可为NULL,此函数的ADU主要指modbus rtu的ADU
};

/** \brief 默认modbus_tcp_gateway_server上下文,用于初始化上下文
 *          注意：默认情况下使用栈作为缓冲，需要确保栈足够大
 *
 * \return modbus_tcp_gateway_server_context_t modbus_tcp_gateway_server上下文
 *
 */
modbus_tcp_gateway_server_context_t modbus_tcp_gateway_server_context_default(void);


/** \brief  modbus gateway server解析输入并返回
 *          注意:本操作未加锁，应当避免在多个线程中使用同一个上下文调用此函数。
 *
 * \param ctx modbus_tcp_gateway_server_context_t*上下文指针
 * \param adu uint8_t* modbus tcp请求数据包地址
 * \param adu_length size_t modbus tcp请求数据包长度
 * \return bool 是否成功处理
 *
 */
bool modbus_tcp_gateway_server_parse_input(modbus_tcp_gateway_server_context_t* ctx,uint8_t *adu,size_t adu_length);

/*
 *  modbus tcp gateway server(使用精简modbus协议),特点如下`：
 *      -用于间接实现简易tcp server,用于资源较为有限的TCP网络环境
 *  注意：
 *      -对未使用的结构体成员一定要初始化为0或NULL,结构体
 *      -多线程使用时需要加锁
 */
struct modbus_tcp_gateway_server_context_with_modbus_rtu_tiny;
typedef struct modbus_tcp_gateway_server_context_with_modbus_rtu_tiny modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t;
struct modbus_tcp_gateway_server_context_with_modbus_rtu_tiny
{
    modbus_tcp_gateway_server_context_t gateway;/**< 网关，此变量需要放在第一位，注意：内部的变量(如usr、reply、rtu_request)被库使用，用户的设置无效，其余变量仍需用户设置*/
    modbus_rtu_slave_tiny_context_t     slave;/**<  从机，注意：内部的变量(如usr、reply)被库使用，用户的设置无效,其余变量仍需用户设置*/
    void    *usr;/**< 用户指针，用户可使用此指针传递用户数据 */
    void    (*reply)(modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx,const uint8_t *adu,size_t adu_length);//数据输出，通常直接输出TCP数据，此函数的ADU主要指modbus tcp的ADU
};

/** \brief modbus tcp gateway server(使用精简modbus协议)默认上下文，用于初始化上下文
 *
 * \return modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t modbus tcp gateway server(使用精简modbus协议)上下文
 *
 */
modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_context_default(void);

/** \brief  modbus gateway server（(使用精简modbus协议)）解析输入并返回
 *          注意:本操作未加锁，应当避免在多个线程中使用同一个上下文调用此函数。
 *
 * \param ctx modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t*上下文指针
 * \param adu uint8_t* modbus tcp请求数据包地址
 * \param adu_length size_t modbus tcp请求数据包长度
 * \return bool 是否成功处理
 *
 */
bool modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_parse_input(modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx,uint8_t *adu,size_t adu_length);



#ifdef __cplusplus
}
#endif

#endif // __MODBUS_HMODBUS_TCP_H__



