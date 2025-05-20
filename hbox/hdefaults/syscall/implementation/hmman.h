/***************************************************************
 * Name:      hmman.h
 * Purpose:   声明hmman接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMMAN_H__
#define __HMMAN_H__
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

typedef struct hmman_parameter hmman_parameter_t;
struct hmman_parameter
{
    void * mem;   /**< 未对齐前的地址,通常用于内存释放操作 */
    size_t len;
    int prot;
    int flags;
    int fildes;
    uint64_t offset;
};

#define HMMAN_ALIGNED_SIZE  16

/*
 * 对齐大小的参数
 */
#define HMMAN_ALIGNED_PARAMETER_SIZE (sizeof(hmman_parameter_t)/(HMMAN_ALIGNED_SIZE)*(HMMAN_ALIGNED_SIZE)+((sizeof(hmman_parameter_t)%(HMMAN_ALIGNED_SIZE))!=0?(HMMAN_ALIGNED_SIZE):0))

/*
 * 对齐大小内存块指针
 */
#define HMMAN_ALIGNED_MEM_PTR_SIZE(x)  ((void *)(((uintptr_t)x)/(HMMAN_ALIGNED_SIZE)*(HMMAN_ALIGNED_SIZE)+((((uintptr_t)x)%(HMMAN_ALIGNED_SIZE))!=0?(HMMAN_ALIGNED_SIZE):0)))

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMMAN_H__
