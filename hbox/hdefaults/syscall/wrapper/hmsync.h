/***************************************************************
 * Name:      hmsync.h
 * Purpose:   声明hmsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMSYNC_H__
#define __HMSYNC_H__
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

#ifdef HAVE_SYS_MMAN_H
#define HMMAN_MS_ASYNC            MS_ASYNC
#define HMMAN_MS_SYNC             MS_SYNC
#define HMMAN_MS_INVALIDATE       MS_INVALIDATE
#else
#define HMMAN_MS_ASYNC            1
#define HMMAN_MS_SYNC             2
#define HMMAN_MS_INVALIDATE       4
#endif

/** \brief 同步映射的文件(或其它对象)
 *
 * \param addr void* 通过mmap映射的地址
 * \param len size_t 期望同步的长度
 * \param flags int 标志位
 * \return int 成功返回0,失败返回-1
 *
 */
int hmsync(void *addr, size_t len, int flags);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMSYNC_H__
