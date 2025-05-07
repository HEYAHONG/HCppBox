/***************************************************************
 * Name:      huuid_short.h
 * Purpose:   声明huuid_short接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUUID_SHORT_H_INCLUDED__
#define __HUUID_SHORT_H_INCLUDED__
#include "huuid_short.h"
/*
 * 本文件主要用于缩短uuid，主要用于BLE等场景。
 * 一般情况下，短uuid是长uuid的一部分。
 * 由于uuid v1采用的60位时间戳的起点早于更常用的Unix时间戳的起始时间，因此，60位时间戳小于0x01B21DD213814000的uuid v1一般可手工分配（不会与自动生成的uuid冲突）。
 * 经典的例子就是蓝牙的128位基础uuid(00000000-0000-1000-8000-00805F9B34FB)
 */
#include "huuid.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 定义32位uuid(占用128位uuid的前32位)
 */
typedef uint32_t huuid_short32_t;


/** \brief uuid是否为短uuid
 *
 * \param in const huuid_t uuid
 * \param base const huuid_t 基础uuid（用于比较除短uuid所占位的其它位）
 * \return bool 是否是32位短uuid
 *
 */
bool huuid_is_uuid_short32(const huuid_t in,const huuid_t base);


/** \brief 获取32位短uuid
 *
 * \param in const huuid_t uuid
 * \return huuid_short32_t 32位短uuid
 *
 */
huuid_short32_t huuid_uuid_short32_get(const huuid_t in);

/** \brief 设置32位短uuid
 *
 * \param out huuid_t uuid
 * \param short_uuid const huuid_short32_t 32位短uuid
 * \param base const huuid_t 基础uuid（用于除短uuid所占位的其它位）
 *
 */
void huuid_uuid_short32_set(huuid_t out,const huuid_short32_t short_uuid,const huuid_t base);

/*
 * 定义16位uuid(占用128位uuid的[位16 位31])
 */
typedef uint16_t huuid_short16_t;

/** \brief uuid是否为短uuid
 *
 * \param in const huuid_t uuid
 * \param base const huuid_t 基础uuid（用于比较除短uuid所占位的其它位）
 * \return bool 是否是16位短uuid
 *
 */
bool huuid_is_uuid_short16(const huuid_t in,const huuid_t base);


/** \brief 获取16位短uuid
 *
 * \param in const huuid_t uuid
 * \return huuid_short16_t 16位短uuid
 *
 */
huuid_short16_t huuid_uuid_short16_get(const huuid_t in);

/** \brief 设置16位短uuid
 *
 * \param out huuid_t uuid
 * \param short_uuid const huuid_short16_t 16位短uuid
 * \param base const huuid_t 基础uuid（用于除短uuid所占位的其它位）
 *
 */
void huuid_uuid_short16_set(huuid_t out,const huuid_short16_t short_uuid,const huuid_t base);


/*
 * 声明蓝牙基础uuid
 */
HUUID_DECLARE_GLOBAL(huuid_ble_base128_uuid);

/** \brief uuid是否为蓝牙uuid
 *
 * \param in const huuid_t uuid
 * \return bool 是否是蓝牙uuid(兼容32位与16位)
 *
 */
bool huuid_is_uuid_ble(const huuid_t in);


/** \brief 获取32位蓝牙uuid
 *
 * \param in const huuid_t uuid
 * \return huuid_short32_t 蓝牙uuid(兼容32位与16位)
 *
 */
huuid_short32_t huuid_uuid_ble_get(const huuid_t in);

/** \brief 设置32位蓝牙uuid
 *
 * \param out huuid_t uuid
 * \param short_uuid const huuid_short32_t 蓝牙uuid(兼容32位与16位)
 *
 */
void huuid_uuid_ble_set(huuid_t out,const huuid_short32_t ble_uuid);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HUUID_SHORT_H_INCLUDED__
