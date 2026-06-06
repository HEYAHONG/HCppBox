/***************************************************************
 * Name:      hendian.h
 * Purpose:   声明hendian接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-06-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HENDIAN_H__
#define __HENDIAN_H__
#include "stdint.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 主机字节序转大端序
 *
 * \param host_16bits uint16_t 主机字节序
 * \return uint16_t 大端序
 *
 */
uint16_t hhtobe16(uint16_t host_16bits);
/** \brief 主机字节序转小端序
 *
 * \param host_16bits uint16_t 主机字节序
 * \return uint16_t 小端序
 *
 */
uint16_t hhtole16(uint16_t host_16bits);
/** \brief 大端序转主机字节序
 *
 * \param big_endian_16bits uint16_t 大端序
 * \return uint16_t 主机字节序
 *
 */
uint16_t hbe16toh(uint16_t big_endian_16bits);
/** \brief 小端序转主机字节序
 *
 * \param little_endian_16bits uint16_t 小端序
 * \return uint16_t 主机字节序
 *
 */
uint16_t hle16toh(uint16_t little_endian_16bits);

/** \brief 主机字节序转大端序
 *
 * \param host_32bits uint32_t 主机字节序
 * \return uint32_t 大端序
 *
 */
uint32_t hhtobe32(uint32_t host_32bits);
/** \brief 主机字节序转小端序
 *
 * \param host_32bits uint32_t 主机字节序
 * \return uint32_t 小端序
 *
 */
uint32_t hhtole32(uint32_t host_32bits);
/** \brief 大端序转主机字节序
 *
 * \param big_endian_32bits uint32_t 大端序
 * \return uint32_t 主机字节序
 *
 */
uint32_t hbe32toh(uint32_t big_endian_32bits);
/** \brief 小端序转主机字节序
 *
 * \param little_endian_32bits uint32_t 小端序
 * \return uint32_t 主机字节序
 *
 */
uint32_t hle32toh(uint32_t little_endian_32bits);

/** \brief 主机字节序转大端序
 *
 * \param host_64bits uint64_t 主机字节序
 * \return uint64_t 大端序
 *
 */
uint64_t hhtobe64(uint64_t host_64bits);
/** \brief 主机字节序转小端序
 *
 * \param host_64bits uint64_t 主机字节序
 * \return uint64_t 小端序
 *
 */
uint64_t hhtole64(uint64_t host_64bits);
/** \brief 大端序转主机字节序
 *
 * \param big_endian_64bits uint64_t 大端序
 * \return uint64_t 主机字节序
 *
 */
uint64_t hbe64toh(uint64_t big_endian_64bits);
/** \brief 小端序转主机字节序
 *
 * \param little_endian_64bits uint64_t 小端序
 * \return uint64_t 主机字节序
 *
 */
uint64_t hle64toh(uint64_t little_endian_64bits);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HENDIAN_H__
