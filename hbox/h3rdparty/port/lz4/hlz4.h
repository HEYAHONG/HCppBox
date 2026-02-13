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

/** \brief LZ4压缩(高压缩率模式，解压方式同hlz4_compress)
 *
 * \param src const char* 源数据
 * \param dst char* 目标缓冲区
 * \param src_length int 源长度
 * \param dst_length int 目标缓冲区长度
 * \param level int 压缩级别
 * \return int 若成功，返回已使用的目标缓冲区长度
 *
 */
int hlz4_compress_hc(const char* src, char* dst, int src_length, int dst_length,int level);

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


#include "stdio.h"
struct hlz4_readfile;
typedef struct hlz4_readfile hlz4_readfile_t;
int hlz4f_readopen(hlz4_readfile_t** lz4f, FILE* fp);
size_t hlz4f_read(hlz4_readfile_t* lz4f, void* buf, size_t length);
int hlz4f_readclose(hlz4_readfile_t* lz4f);
struct hlz4_writefile;
typedef struct hlz4_writefile hlz4_writefile_t;
/*
 * hlz4f_preferences相关选项需手动包含H3RDPARTY_LZ4FILE_HEADER头文件，并参考LZ4F_preferences_t结构体
 */
struct hlz4f_preferences;
typedef struct hlz4f_preferences hlz4f_preferences_t;
const hlz4f_preferences_t* hlz4f_preferences_fast(void);
const hlz4f_preferences_t* hlz4f_preferences_best(void);
int hlz4f_writeopen(hlz4_writefile_t** lz4f, FILE* fp, const hlz4f_preferences_t* prefsPtr);
size_t hlz4f_write(hlz4_writefile_t* lz4f, const void* buf, size_t length);
int hlz4f_writeclose(hlz4_writefile_t* lz4f);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLZ4_H_INCLUDE__
