/***************************************************************
 * Name:      hatomic_int.h
 * Purpose:   声明hatomic_int接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HATOMIC_INT_H__
#define __HATOMIC_INT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "../../stdatomic/hstdatomic_common.h"
#include "../../stdatomic/hatomic_flag.h"
#include "hdefaults/hdefaults_config.h"

#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
typedef int         hatomic_int_t;
#else
typedef _Atomic int hatomic_int_t;
#endif

void hatomic_int_init(volatile hatomic_int_t* obj, int desired);

void hatomic_int_store_explicit( volatile hatomic_int_t* obj,int desired, hmemory_order_t order);
static inline void hatomic_int_store(volatile hatomic_int_t* obj,int desired)
{
    hatomic_int_store_explicit(obj,desired,hmemory_order_seq_cst);
}

int hatomic_int_load_explicit( const volatile hatomic_int_t* obj, hmemory_order_t order);
static inline int hatomic_int_load( const volatile hatomic_int_t* obj )
{
    return hatomic_int_load_explicit(obj,hmemory_order_seq_cst);
}


int hatomic_int_exchange_explicit( volatile hatomic_int_t* obj, int desired, hmemory_order_t order);
static inline int hatomic_int_exchange( volatile hatomic_int_t* obj, int desired )
{
    return hatomic_int_exchange_explicit(obj,desired,hmemory_order_seq_cst);
}

bool hatomic_int_compare_exchange_strong_explicit( volatile hatomic_int_t* obj,int* expected, int desired,hmemory_order_t succ,hmemory_order_t fail );
static bool  hatomic_int_compare_exchange_strong( volatile hatomic_int_t* obj,int* expected, int desired)
{
    return hatomic_int_compare_exchange_strong_explicit(obj,expected,desired,hmemory_order_seq_cst,hmemory_order_seq_cst);
}

bool hatomic_int_compare_exchange_weak_explicit( volatile hatomic_int_t* obj,int* expected, int desired,hmemory_order_t succ,hmemory_order_t fail );
static bool  hatomic_int_compare_exchange_weak( volatile hatomic_int_t* obj,int* expected, int desired)
{
    return hatomic_int_compare_exchange_weak_explicit(obj,expected,desired,hmemory_order_seq_cst,hmemory_order_seq_cst);
}

int hatomic_int_fetch_add_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order );
static inline int hatomic_int_fetch_add( volatile hatomic_int_t* obj, int arg)
{
    return hatomic_int_fetch_add_explicit(obj,arg,hmemory_order_seq_cst);
}

int hatomic_int_fetch_sub_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order );
static inline int hatomic_int_fetch_sub( volatile hatomic_int_t* obj, int arg)
{
    return hatomic_int_fetch_sub_explicit(obj,arg,hmemory_order_seq_cst);
}

int hatomic_int_fetch_or_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order );
static inline int hatomic_int_fetch_or( volatile hatomic_int_t* obj, int arg)
{
    return hatomic_int_fetch_or_explicit(obj,arg,hmemory_order_seq_cst);
}

int hatomic_int_fetch_xor_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order );
static inline int hatomic_int_fetch_xor( volatile hatomic_int_t* obj, int arg)
{
    return hatomic_int_fetch_xor_explicit(obj,arg,hmemory_order_seq_cst);
}

int hatomic_int_fetch_and_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order );
static inline int hatomic_int_fetch_and( volatile hatomic_int_t* obj, int arg)
{
    return hatomic_int_fetch_and_explicit(obj,arg,hmemory_order_seq_cst);
}

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HATOMIC_INT_H__
