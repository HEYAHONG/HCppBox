/***************************************************************
 * Name:      hread.h
 * Purpose:   声明hread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HREAD_H__
#define __HREAD_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef intptr_t hread_ssize_t;

/** \brief 从文件描述符中读取数据
 *
 * \param fd int 文件描述符
 * \param buff void* 缓冲区
 * \param buff_count size_t 缓冲区长度
 * \return hread_ssize_t 返回值,已经读取的字节数,-1表示出错,
 *
 */
hread_ssize_t hread(int fd,void *buff,size_t buff_count);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HREAD_H__
