/***************************************************************
 * Name:      hatomic_int.c
 * Purpose:   实现hatomic_int接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hatomic_int.h"
#include "stdlib.h"

void hatomic_int_init(volatile hatomic_int_t* obj, int desired)
{
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        (*obj)=desired;
        hdefaults_mutex_unlock(NULL);
#else
        atomic_init(obj,desired);
#endif
    }
}

void hatomic_int_store_explicit( volatile hatomic_int_t* obj,int desired, hmemory_order_t order)
{
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        (*obj)=desired;
        hdefaults_mutex_unlock(NULL);
#else
        atomic_store_explicit(obj,desired,(memory_order)hmemory_order_cstd(order));
#endif
    }
}

int hatomic_int_load_explicit( const volatile hatomic_int_t* obj, hmemory_order_t order)
{
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        int ret=0;
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        hdefaults_mutex_unlock(NULL);
        return ret;
#else
        return atomic_load_explicit(obj,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return 0;
}

int hatomic_int_exchange_explicit( volatile hatomic_int_t* obj, int desired, hmemory_order_t order)
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)=desired;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_exchange_explicit(obj,desired,hmemory_order_cstd(order));
#endif
    }
    return ret;
}

bool hatomic_int_compare_exchange_strong_explicit( volatile hatomic_int_t* obj,int* expected, int desired,hmemory_order_t succ,hmemory_order_t fail )
{
    bool ret=false;
    if(obj!=NULL && expected!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=((*obj)==(*expected));
        if(ret)
        {
            (*obj)=desired;
        }
        else
        {
            (*expected)=(*obj);
        }
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_compare_exchange_strong_explicit(obj,expected,desired,(memory_order)hmemory_order_cstd(succ),(memory_order)hmemory_order_cstd(fail));
#endif
    }
    return ret;
}

bool hatomic_int_compare_exchange_weak_explicit( volatile hatomic_int_t* obj,int* expected, int desired,hmemory_order_t succ,hmemory_order_t fail )
{
    bool ret=false;
    if(obj!=NULL && expected!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=((*obj)==(*expected));
        {
            (*expected)=(*obj);
        }
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_compare_exchange_weak_explicit(obj,expected,desired,(memory_order)hmemory_order_cstd(succ),(memory_order)hmemory_order_cstd(fail));
#endif
    }
    return ret;
}

int hatomic_int_fetch_add_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order )
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)+=arg;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_fetch_add_explicit(obj,arg,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return ret;
}

int hatomic_int_fetch_sub_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order )
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)-=arg;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_fetch_sub_explicit(obj,arg,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return ret;
}

int hatomic_int_fetch_or_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order )
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)|=arg;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_fetch_or_explicit(obj,arg,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return ret;
}

int hatomic_int_fetch_xor_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order )
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)^=arg;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_fetch_xor_explicit(obj,arg,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return ret;
}

int hatomic_int_fetch_and_explicit( volatile hatomic_int_t* obj, int arg, hmemory_order_t order )
{
    int ret=0;
    if(obj!=NULL)
    {
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
        hdefaults_mutex_lock(NULL);
        ret=(*obj);
        (*obj)&=arg;
        hdefaults_mutex_unlock(NULL);
#else
        ret=atomic_fetch_and_explicit(obj,arg,(memory_order)hmemory_order_cstd(order));
#endif
    }
    return ret;
}

