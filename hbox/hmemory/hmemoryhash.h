/***************************************************************
 * Name:      hmemoryhash.h
 * Purpose:   声明hmemoryhash接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMEMORYHASH_H__
#define __HMEMORYHASH_H__
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief hash函数
 * 注意：无论是32位的算法还是64位算法，均采用64位返回值
 */
typedef uint64_t (*hmemoryhash_function_t)(const uint8_t *data,size_t length);

/** \brief bernstein算法(times 33)
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_bernstein_times_33(const uint8_t *data,size_t length);

/** \brief SAX算法
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_sax(const uint8_t *data,size_t length);

/** \brief FNV-1a算法(32位)
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_fnv_1a_32(const uint8_t *data,size_t length);

/** \brief FNV-1a算法(64位)
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_fnv_1a_64(const uint8_t *data,size_t length);

/** \brief OAT算法
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_oat(const uint8_t *data,size_t length);


/** \brief JENKINS算法
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_jenkins(const uint8_t *data,size_t length);

/** \brief MURMURHASH2算法
 *
 * \param data const uint8_t* 数据
 * \param length size_t 数据长度
 * \return uint64_t hash值
 *
 */
uint64_t hmemoryhash_murmurhash2(const uint8_t *data,size_t length);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMEMORYHASH_H__
