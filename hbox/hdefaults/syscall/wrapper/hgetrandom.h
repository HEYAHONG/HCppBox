/***************************************************************
 * Name:      hgetrandom.h
 * Purpose:   声明hgetrandom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGETRANDOM_H__
#define __HGETRANDOM_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HGETRANDOM_GRND_NONBLOCK    0x01
#define HGETRANDOM_GRND_RANDOM      0x02
#define HGETRANDOM_GRND_INSECURE    0x04

typedef intptr_t hgetrandom_ssize_t;

/** \brief 生成随机数
 *
 * \param buffer void*  缓冲区长度
 * \param length size_t 缓冲区长度
 * \param flags unsigned int 标志位
 * \return hgetrandom_ssize_t 写入的缓冲区大小大小,-1=失败
 *
 */
hgetrandom_ssize_t hgetrandom (void *buffer, size_t length,unsigned int flags);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HGETRANDOM_H__
