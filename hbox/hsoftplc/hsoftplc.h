/***************************************************************
 * Name:      hsoftplc.h
 * Purpose:   声明hsoftplc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTPLC_H__
#define __HSOFTPLC_H__
#include "stdlib.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 软件PLC初始化。可由hruntime自动调用。
 *
 *
 */
void hsoftplc_init(void);

/** \brief 软件PLC循环。可由hruntime自动调用。
 *
 *
 */
void hsoftplc_loop(void);


enum hsoftplc_callback_type
{
    HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_BEGIN  =   0,            /**<  组态初始化开始 */
    HSOFTPLC_CALLBACK_TYPE_CONFIG_INIT_END    =   1,            /**<  组态初始化结束 */
    HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_BEGIN   =   2,            /**<  组态运行开始,通常情况下需要更新变量值 */
    HSOFTPLC_CALLBACK_TYPE_CONFIG_RUN_END     =   3,            /**<  组态运行结束,通常情况下需要变量变化进行操作 */
};
typedef enum hsoftplc_callback_type hsoftplc_callback_type_t;

typedef void (*hsoftplc_callback_t)(hsoftplc_callback_type_t cb_type);

/** \brief 软件PLC设置回调
 *
 * \param cb hsoftplc_callback_t 待设置的回调
 * \return hsoftplc_callback_t  原回调
 *
 */
hsoftplc_callback_t hsoftplc_set_callback(hsoftplc_callback_t cb);

/*
 * 基本变量头文件，通过include使用。
 * #include HSOFTPLC_IEC_BASE_TYPE_HEADER
 */
#define HSOFTPLC_IEC_BASE_TYPE_HEADER "hsoftplc/hsoftplc_iec_base_type.h"

/** \brief 软件PLC获取变量指针
 *
 * \param variable-name const char* 变量名称
 * \return void * 变量指针
 *
 */
void *hsoftplc_get_located_variables(const char *variable_name);

/** \brief 软件PLC变量枚举回调
 *
 * \param variable_name const char* 变量名称
 * \param variable void* 变量
 * \param usr void* 用户参数
 *
 */
typedef void (*hsoftplc_located_variable_enum_callback_t)(const char *variable_name,void *variable,void *usr);

/** \brief 软件PLC变量枚举
 *
 * \param cb hsoftplc_located_variable_enum_callback_t 变量枚举回调
 * \param usr void* 用户参数
 * \return size_t 变量个数
 *
 */
size_t hsoftplc_get_located_all_variables(hsoftplc_located_variable_enum_callback_t cb,void *usr);


/** \brief 变量名称类型
 */
typedef char hsoftplc_variable_name_t[32];

/** \brief 软件PLC通过变量名称获取IEC地址
 *
 * \param iec_addr hsoftplc_variable_name_t IEC地址
 * \param variable_name const char *  C语言变量名称
 * \return bool 是否成功
 *
 */
bool hsoftplc_get_iec_addr_from_variable_name(hsoftplc_variable_name_t iec_addr,const char * variable_name);


/** \brief 软件PLC通过IEC地址获取变量名称
 *
 * \param variable_name hsoftplc_variable_name_t C语言变量名称
 * \param iec_addr const char* IEC地址
 * \return bool 是否成功
 *
 */
bool hsoftplc_get_variable_name_from_iec_addr(hsoftplc_variable_name_t variable_name,const char *iec_addr);

typedef struct hsoftplc_variable_symbol hsoftplc_variable_symbol_t;
struct hsoftplc_variable_symbol
{
    hsoftplc_variable_name_t buffer;                /**< 存储数据的缓冲 */
    char            variable_location;              /**< 位置：通常为I（输入）、Q（输出）、M（内存） */
    char            variable_size;                  /**< 大小：通常为X（1位）、B（8位）、W（16位）、D（32位）、L（64位） */
    const char *    variable_address[3];            /**< 地址：通常为数字字符串 */
};


/** \brief 软件PLC解析变量符号
 *
 * \param variable_symbol hsoftplc_variable_symbol_t * 变量符号，不可为NULL
 * \param variable_name_or_iec_addr const char* C语言变量名称或者IEC地址
 * \return hsoftplc_variable_symbol_t * 变量符号，当失败时返回NULL
 *
 */
hsoftplc_variable_symbol_t * hsoftplc_parse_variable_symbol(hsoftplc_variable_symbol_t * variable_symbol,const char *variable_name_or_iec_addr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTPLC_H__
