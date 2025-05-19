/***************************************************************
 * Name:      hmunmap.h
 * Purpose:   声明hmunmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMUNMAP_H__
#define __HMUNMAP_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
#include "hmmap.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 取消mmap映射
 *
 * \param addr void* 使用mmap映射的地址
 * \param len size_t 期望取消的长度
 * \return int 成功返回0,失败返回-1
 *
 */
int hmunmap(void *addr, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMUNMAP_H__
