/***************************************************************
 * Name:      hmprotect.h
 * Purpose:   声明hmprotect接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMPROTECT_H__
#define __HMPROTECT_H__
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


/** \brief  修改内存的保护标志(读、写、执行)
 *
 * \param addr void* 通过mmap映射的地址
 * \param len size_t 期望同步的长度
 * \param prot int 内存保护标志位
 * \return int 成功返回0,失败返回-1
 *
 */
int hmprotect(void *addr, size_t len, int prot);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMPROTECT_H__
