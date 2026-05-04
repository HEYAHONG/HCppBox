/***************************************************************
 * Name:      hsoftplc_database.h
 * Purpose:   声明hsoftplc_database接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTPLC_DATABASE_H__
#define __HSOFTPLC_DATABASE_H__
#include "hsoftplc.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
/*
 * 数据库采用Key-Value模式
 */


/** \brief 软件PLC数据库采用64位数字作为软件PLC数据库的键(包含位置、大小、地址0～2)
 *
 *   地址2   地址1     地址0     大小      位置
 *  /     \ /      \  /      \  /      \  /     \
 *  0 ... 7 8 ... 15 16 ... 47 48 ... 55 56 .. 63
 */
typedef uint64_t hsoftplc_database_key_t;


/** \brief 软件PLC数据库
 *
 * \param variable_name const char* 变量名称
 * \return hsoftplc_database_key_t 键，失败返回0
 *
 */
hsoftplc_database_key_t hsoftplc_database_key_get_from_variable_name(const char *variable_name);


/** \brief 软件PLC数据库值句柄（可通过 软件PLC变量枚举 或者 软件PLC获取变量指针 获得）
 */
typedef void *   hsoftplc_database_value_handle_t;

/** \brief 软件PLC数据库值(无论位宽多少与是否为无符号整数，均采用64位无符号整数进行包装)
 */
typedef uint64_t hsoftplc_database_value_t;

/** \brief 软件PLC数据库读取值
 *
 * \param key hsoftplc_database_key_t 键
 * \param handle hsoftplc_database_value_handle_t 值句柄
 * \param value hsoftplc_database_value_t* 值指针
 * \return bool 是否成功读取
 *
 */
bool hsoftplc_database_get_value(hsoftplc_database_key_t key,hsoftplc_database_value_handle_t handle,hsoftplc_database_value_t *value);


/** \brief 软件PLC数据库写入值
 *
 * \param key hsoftplc_database_key_t 键
 * \param handle hsoftplc_database_value_handle_t 值句柄
 * \param value hsoftplc_database_value_t 值
 * \return bool 是否成功写入
 *
 */
bool hsoftplc_database_set_value(hsoftplc_database_key_t key,hsoftplc_database_value_handle_t handle,hsoftplc_database_value_t value);


struct  hsoftplc_database_value_cache;
typedef struct  hsoftplc_database_value_cache hsoftplc_database_value_cache_t;
struct  hsoftplc_database_value_cache
{
    hsoftplc_database_value_handle_t handle;    /**< 句柄 */
    hsoftplc_database_value_t        value1;    /**< 上次的值 */
};


/** \brief 软件PLC数据库值缓存初始化，通常在组态初始化结束时运行
 *
 * \param cache hsoftplc_database_value_cache_t* 软件PLC数据库值缓存
 * \param variable_name const char* 变量名称
 * \param handle hsoftplc_database_value_handle_t 值句柄
 * \return bool 是否成功初始化
 *
 */
bool hsoftplc_database_value_cache_init(hsoftplc_database_value_cache_t *cache,const char *variable_name,hsoftplc_database_value_handle_t handle);


/** \brief 软件PLC数据库值缓存更新回调
 *
 * \param cache const hsoftplc_database_value_cache_t* 软件PLC数据库值缓存（更新后）
 * \param variable_name const char* 变量名称
 * \param usr void* 用户参数
 *
 */
typedef void (*hsoftplc_database_value_cache_on_update_callback_t)(const hsoftplc_database_value_cache_t *cache,const char *variable_name,void *usr);

/** \brief 软件PLC数据库值缓存更新，通常在组态运行结束时运行
 *
 * \param cache hsoftplc_database_value_cache_t* 软件PLC数据库
 * \param variable_name const char* 变量名称
 * \param cb hsoftplc_database_value_cache_on_update_callback_t 更新回调
 * \param usr void* 用户参数
 * \return bool 是否成功更新
 *
 */
bool hsoftplc_database_value_cache_update(hsoftplc_database_value_cache_t *cache,const char *variable_name,hsoftplc_database_value_cache_on_update_callback_t cb,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTPLC_DATABASE_H__
