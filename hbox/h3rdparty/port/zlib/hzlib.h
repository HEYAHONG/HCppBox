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


struct hgzfile;
typedef struct hgzfile hgzfile_t;
hgzfile_t *  hgzdopen(int fd, const char *mode);
int  hgzbuffer(hgzfile_t * file, unsigned length);
int  hgzsetparams(hgzfile_t * file, int level, int strategy);
int  hgzread(hgzfile_t * file, void * buf, unsigned len);
size_t  hgzfread(void * buf, size_t length, size_t nitems,hgzfile_t * file);
int  hgzwrite(hgzfile_t * file, const void * buf, unsigned len);
size_t  hgzfwrite(const void * buf, size_t length,size_t nitems, hgzfile_t * file);
int  hgzputs(hgzfile_t * file, const char *s);
char *  hgzgets(hgzfile_t * file, char *buf, int len);
int  hgzputc(hgzfile_t * file, int c);
int  hgzgetc(hgzfile_t * file);
int  hgzungetc(int c, hgzfile_t * file);
int  hgzflush(hgzfile_t * file, int flush);
int  hgzrewind(hgzfile_t * file);
int  hgzeof(hgzfile_t * file);
int  hgzdirect(hgzfile_t * file);
int  hgzclose(hgzfile_t * file);
int  hgzclose_r(hgzfile_t * file);
int  hgzclose_w(hgzfile_t * file);
const char *  hgzerror(hgzfile_t * file, int *errnum);
void  hgzclearerr(hgzfile_t * file);




#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HZLIB_H_INCLUDE__
