/***************************************************************
 * Name:      hlz4.h
 * Purpose:   声明hlz4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLZ4_H_INCLUDE__
#define __HLZ4_H_INCLUDE__
#include "h3rdparty_port_lz4.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief LZ4压缩
 *
 * \param src const char* 源数据
 * \param dst char* 目标缓冲区
 * \param src_length int 源长度
 * \param dst_length int 目标缓冲区长度
 * \return int 若成功，返回已使用的目标缓冲区长度
 *
 */
int hlz4_compress(const char* src, char* dst, int src_length, int dst_length);

/** \brief LZ4解压
 *
 * \param src const char* 源数据
 * \param dst char* 目标缓冲区
 * \param src_length int 源长度
 * \param dst_length int 目标缓冲区长度
 * \return int 若成功，返回已使用的目标缓冲区长度
 *
 */
int hlz4_decompress(const char* src, char* dst, int src_length, int dst_length);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLZ4_H_INCLUDE__
