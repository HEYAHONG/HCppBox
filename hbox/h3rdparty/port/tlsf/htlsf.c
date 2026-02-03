/***************************************************************
 * Name:      htlsf.c
 * Purpose:   实现htlsf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "htlsf.h"
#include H3RDPARTY_TLSF_HEADER
static void (* htlsf_mutex_lock_cb)(void)=NULL;
static void htlsf_mutex_lock(void)
{
    if(htlsf_mutex_lock_cb==NULL)
    {
        hdefaults_mutex_lock(NULL);
    }
    else
    {
        htlsf_mutex_lock_cb();
    }
}

static void (* htlsf_mutex_unlock_cb)(void)=NULL;
static void htlsf_mutex_unlock(void)
{
    if(htlsf_mutex_unlock_cb==NULL)
    {
        hdefaults_mutex_unlock(NULL);
    }
    else
    {
        htlsf_mutex_unlock_cb();
    }
}

void htlsf_set_lock(void (*lock)(void),void (*unlock)(void))
{
    htlsf_mutex_lock_cb=lock;
    htlsf_mutex_unlock_cb=unlock;
}

htlsf_t htlsf_create(void* mem)
{
    htlsf_t ret=NULL;
    ret=tlsf_create(mem);
    return ret;
}


htlsf_t htlsf_create_with_pool(void* mem, size_t bytes)
{
    htlsf_t ret=NULL;
    ret=tlsf_create_with_pool(mem,bytes);
    return ret;
}
void htlsf_destroy(htlsf_t tlsf)
{
    tlsf_destroy(tlsf);
}
htlsf_pool_t htlsf_get_pool(htlsf_t tlsf)
{
    htlsf_pool_t ret=NULL;
    htlsf_mutex_lock();
    ret=tlsf_get_pool(tlsf);
    htlsf_mutex_unlock();
    return ret;
}

htlsf_pool_t htlsf_add_pool(htlsf_t tlsf, void* mem, size_t bytes)
{
    htlsf_pool_t ret=NULL;
    htlsf_mutex_lock();
    ret=tlsf_add_pool(tlsf,mem,bytes);
    htlsf_mutex_unlock();
    return ret;
}
void htlsf_remove_pool(htlsf_t tlsf, htlsf_pool_t pool)
{
    htlsf_mutex_lock();
    tlsf_remove_pool(tlsf,pool);
    htlsf_mutex_unlock();
}


void* htlsf_malloc(htlsf_t tlsf, size_t bytes)
{
    void *ret=NULL;
    htlsf_mutex_lock();
    ret=tlsf_malloc(tlsf,bytes);
    htlsf_mutex_unlock();
    return ret;
}
void* htlsf_memalign(htlsf_t tlsf, size_t align, size_t bytes)
{
    void *ret=NULL;
    htlsf_mutex_lock();
    ret=tlsf_memalign(tlsf,align,bytes);
    htlsf_mutex_unlock();
    return ret;
}

void* htlsf_realloc(htlsf_t tlsf, void* ptr, size_t size)
{
    void *ret=NULL;
    htlsf_mutex_lock();
    ret=tlsf_realloc(tlsf,ptr,size);
    htlsf_mutex_unlock();
    return ret;
}
void htlsf_free(htlsf_t tlsf, void* ptr)
{
    htlsf_mutex_lock();
    tlsf_free(tlsf,ptr);
    htlsf_mutex_unlock();
}


size_t htlsf_block_size(void* ptr)
{
    size_t ret=0;
    htlsf_mutex_lock();
    ret=tlsf_block_size(ptr);
    htlsf_mutex_unlock();
    return ret;
}

size_t htlsf_size(void)
{
    return tlsf_size();
}
size_t htlsf_align_size(void)
{
    return tlsf_align_size();
}
size_t htlsf_block_size_min(void)
{
    return tlsf_block_size_min();
}
size_t htlsf_block_size_max(void)
{
    return tlsf_block_size_max();
}
size_t htlsf_pool_overhead(void)
{
    return tlsf_pool_overhead();
}
size_t htlsf_alloc_overhead(void)
{
    return tlsf_alloc_overhead();
}


void htlsf_walk_pool(htlsf_pool_t pool, htlsf_walker walker, void* user)
{
    htlsf_mutex_lock();
    tlsf_walk_pool(pool,walker,user);
    htlsf_mutex_unlock();
}

int htlsf_check(htlsf_t tlsf)
{
    int ret=0;
    htlsf_mutex_lock();
    ret=tlsf_check(tlsf);
    htlsf_mutex_unlock();
    return ret;
}
int htlsf_check_pool(htlsf_pool_t pool)
{
    int ret=0;
    htlsf_mutex_lock();
    ret=tlsf_check_pool(pool);
    htlsf_mutex_unlock();
    return ret;
}


static htlsf_t htlsf_default_heap=NULL;
htlsf_t htlsfheap_set_default(htlsf_t tlsf_heap)
{
    htlsf_t ret=htlsf_default_heap;
    htlsf_default_heap=tlsf_heap;
    return ret;
}


/*
 *  TLSF的额外开销在3KB（见TLSF库的说明）左右，推荐至少8KB区域用于TLSF的堆
 */
#ifdef USING_HTLSFHEAP
#ifndef HTLSFHEAP_DEFAULT_POOL_SIZE
#define HTLSFHEAP_DEFAULT_POOL_SIZE 8192
#else
#if     (HTLSFHEAP_DEFAULT_POOL_SIZE) < 8192
#undef  HTLSFHEAP_DEFAULT_POOL_SIZE
#define HTLSFHEAP_DEFAULT_POOL_SIZE  8192
#endif
#endif
#endif // USING_HTLSFHEAP

#ifndef  HTLSFHEAP_DEFAULT_POOL_ATTRIBUTE
#define  HTLSFHEAP_DEFAULT_POOL_ATTRIBUTE
#endif // HTLSFHEAP_DEFAULT_POOL_ATTRIBUTE

#if HTLSFHEAP_DEFAULT_POOL_SIZE >= 8192
HTLSFHEAP_DEFAULT_POOL_ATTRIBUTE
static uint64_t htlsfheap_default_pool_bytes[(HTLSFHEAP_DEFAULT_POOL_SIZE+sizeof(uint64_t)-1)/sizeof(uint64_t)]= {0};
static bool htlsfheap_is_default_pool_init=false;
static void htlsfheap_check_default_pool(void)
{
    if(htlsf_default_heap==NULL)
    {
        if(!htlsfheap_is_default_pool_init)
        {
            htlsf_default_heap=htlsf_create_with_pool( htlsfheap_default_pool_bytes,sizeof( htlsfheap_default_pool_bytes));
            htlsfheap_is_default_pool_init=true;
        }
    }
}

#endif // HTLSFHEAP_DEFAULT_POOL_SIZE

htlsf_t htlsfheap_get_default(void)
{
#if HTLSFHEAP_DEFAULT_POOL_SIZE >= 8192
    htlsfheap_check_default_pool();
#endif // HTLSFHEAP_DEFAULT_POOL_SIZE
    return htlsf_default_heap;
}

void * htlsfheap_malloc(size_t bytes)
{
    htlsf_t tlsf_heap=htlsfheap_get_default();
    if(tlsf_heap==NULL)
    {
        return NULL;
    }
    return htlsf_malloc(tlsf_heap,bytes);
}
void   htlsfheap_free(void *ptr)
{
    htlsf_t tlsf_heap=htlsfheap_get_default();
    if(tlsf_heap==NULL)
    {
        return;
    }
    htlsf_free(tlsf_heap,ptr);
}
