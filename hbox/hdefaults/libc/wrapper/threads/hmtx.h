/***************************************************************
 * Name:      hmtx.h
 * Purpose:   声明hmtx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMTX_H__
#define __HMTX_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hthrd.h"

#if !defined(HMTX_SIZE)
#define HMTX_SIZE (8)
#endif
#if (HMTX_SIZE) < (1)
#undef  HMTX_SIZE
#define HMTX_SIZE (1)
#endif

struct hmtx
{
    /*
     * 存储mtx信息,默认应当清零。
     */
    uintptr_t storage[HMTX_SIZE];
};

typedef struct hmtx hmtx_t;

int hmtx_init(hmtx_t* __mutex, int type);
int hmtx_lock(hmtx_t* __mutex);
int hmtx_timedlock(hmtx_t * __mutex,const htimespec_t * time_point);
int hmtx_trylock(hmtx_t *__mutex);
int hmtx_unlock( hmtx_t *__mutex);
void hmtx_destroy(hmtx_t *__mutex );


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMTX_H__
