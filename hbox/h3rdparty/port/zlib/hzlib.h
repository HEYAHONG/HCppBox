/***************************************************************
 * Name:      hzlib.h
 * Purpose:   声明hzlib接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HZLIB_H_INCLUDE__
#define __HZLIB_H_INCLUDE__
#include "h3rdparty_port_zlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief zlib默认压缩
 *
 * \param dest uint8_t* 目标缓冲区
 * \param dest_length unsigned long* 传入目标缓冲区长度，传出目标数据长度
 * \param src const uint8_t* 源缓冲区
 * \param src_length unsigned long 源缓冲区长度
 * \return bool 是否成功
 *
 */
bool hzlib_compress(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length);

/** \brief zlib压缩
 *
 * \param dest uint8_t* 目标缓冲区
 * \param dest_length unsigned long* 传入目标缓冲区长度，传出目标数据长度
 * \param src const uint8_t* 源缓冲区
 * \param src_length unsigned long 源缓冲区长度
 * \param level int 压缩级别，0=不压缩，-1=默认压缩，1～9=压缩级别1～压缩级别9
 * \return bool 是否成功
 *
 */
bool hzlib_compress2(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length,int level);


/** \brief zlib解压
 *
 * \param dest uint8_t* 目标缓冲区
 * \param dest_length unsigned long* 传入目标缓冲区长度，传出目标数据长度
 * \param src const uint8_t* 源缓冲区
 * \param src_length unsigned long 源缓冲区长度
 * \return bool 是否成功
 *
 */
bool hzlib_uncompress(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length);

/** \brief zlib解压
 *
 * \param dest uint8_t* 目标缓冲区
 * \param dest_length unsigned long* 传入目标缓冲区长度，传出目标数据长度
 * \param src const uint8_t* 源缓冲区
 * \param src_length unsigned long * 传入源缓冲区长度,传出已使用的数据长度
 * \return bool 是否成功
 *
 */
bool hzlib_uncompress2(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long *src_length);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HZLIB_H_INCLUDE__
