/***************************************************************
 * Name:      hmtx_stdatomic.h
 * Purpose:   声明hmtx_stdatomic接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMTX_STDATOMIC_H__
#define __HMTX_STDATOMIC_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hthrd.h"

#include "hmtx.h"

/*
 × 非标实现
 * 采用原子变量实现的互斥锁
 × 注意：本文中的互斥锁不可与其它头文件混用
 */
int hmtx_stdatomic_init(hmtx_t* __mutex, int type);
int hmtx_stdatomic_lock(hmtx_t* __mutex);
int hmtx_stdatomic_timedlock(hmtx_t * __mutex,const htimespec_t * time_point);
int hmtx_stdatomic_trylock(hmtx_t *__mutex);
int hmtx_stdatomic_unlock( hmtx_t *__mutex);
void hmtx_stdatomic_destroy(hmtx_t *__mutex );


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMTX_STDATOMIC_H__
