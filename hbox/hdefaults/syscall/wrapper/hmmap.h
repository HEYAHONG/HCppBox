/***************************************************************
 * Name:      hmmap.h
 * Purpose:   声明hmmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMMAP_H__
#define __HMMAP_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#ifdef HAVE_SYS_MMAN_H
#define HMMAN_PROT_NONE       PROT_NONE
#define HMMAN_PROT_READ       PROT_READ
#define HMMAN_PROT_WRITE      PROT_WRITE
#define HMMAN_PROT_EXEC       PROT_EXEC

#define HMMAN_MAP_FILE        MAP_FILE
#define HMMAN_MAP_SHARED      MAP_SHARED
#define HMMAN_MAP_PRIVATE     MAP_PRIVATE
#define HMMAN_MAP_TYPE        MAP_TYPE
#define HMMAN_MAP_FIXED       MAP_FIXED
#define HMMAN_MAP_ANONYMOUS   MAP_ANONYMOUS
#define HMMAN_MAP_ANON        MAP_ANON

#define HMMAN_MAP_FAILED      MAP_FAILED
#else
#define HMMAN_PROT_NONE       0
#define HMMAN_PROT_READ       1
#define HMMAN_PROT_WRITE      2
#define HMMAN_PROT_EXEC       4

#define HMMAN_MAP_FILE        0
#define HMMAN_MAP_SHARED      1
#define HMMAN_MAP_PRIVATE     2
#define HMMAN_MAP_TYPE        0x0F
#define HMMAN_MAP_FIXED       0x10
#define HMMAN_MAP_ANONYMOUS   0x20
#define HMMAN_MAP_ANON        MAP_ANONYMOUS

#define HMMAN_MAP_FAILED      ((void *)-1)
#endif // HAVE_SYS_MMAN_H

/** \brief 将文件或者其它对象映射进内存中
 *
 * \param addr void* 期望的内存地址，若未启用HMMAN_MAP_FIXED，为NULL时由系统分配
 * \param len size_t 期望的长度
 * \param prot int 内存保护标志
 * \param flags int 标志
 * \param fildes int 文件描述符
 * \param off uint64_t 文件偏移，注意：此处使用的是64位偏移
 * \return void* 失败返回HMMAN_MAP_FAILED，成功时返回已分配的地址
 *
 */
void *hmmap(void *addr, size_t len, int prot, int flags, int fildes, uint64_t off);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMMAP_H__
