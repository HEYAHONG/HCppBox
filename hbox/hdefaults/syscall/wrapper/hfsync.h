/***************************************************************
 * Name:      hfsync.h
 * Purpose:   声明hfsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFSYNC_H__
#define __HFSYNC_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 清空文件缓存
 *
 * \param fd int 文件描述符
 * \return int 返回值,0表示成功，-1表示出错
 *
 */
int hfsync(int fd);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFSYNC_H__
