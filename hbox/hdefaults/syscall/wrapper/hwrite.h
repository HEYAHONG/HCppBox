/***************************************************************
 * Name:      hwrite.h
 * Purpose:   声明hwrite接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HWRITE_H__
#define __HWRITE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef intptr_t hwrite_ssize_t;

/** \brief 向文件描述符中写入数据
 *
 * \param fd int 文件描述符
 * \param buff const void* 缓冲区
 * \param buff_count size_t 缓冲区长度
 * \return hwrite_ssize_t 返回值,已经写入的字节数,-1表示出错,
 *
 */
hwrite_ssize_t hwrite(int fd,const void *buff,size_t buff_count);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HWRITE_H__
