/***************************************************************
 * Name:      htlsf.h
 * Purpose:   声明htlsf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HTLSF_H_INCLUDE__
#define __HTLSF_H_INCLUDE__
#include "h3rdparty_port_tlsf.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef void* htlsf_t;
typedef void* htlsf_pool_t;

void htlsf_set_lock(void (*lock)(void),void (*unlock)(void));

htlsf_t htlsf_create(void* mem);
htlsf_t htlsf_create_with_pool(void* mem, size_t bytes);
void htlsf_destroy(htlsf_t tlsf);
htlsf_pool_t htlsf_get_pool(htlsf_t tlsf);

htlsf_pool_t htlsf_add_pool(htlsf_t tlsf, void* mem, size_t bytes);
void htlsf_remove_pool(htlsf_t tlsf, htlsf_pool_t pool);


void* htlsf_malloc(htlsf_t tlsf, size_t bytes);
void* htlsf_memalign(htlsf_t tlsf, size_t align, size_t bytes);
void* htlsf_realloc(htlsf_t tlsf, void* ptr, size_t size);
void htlsf_free(htlsf_t tlsf, void* ptr);


size_t htlsf_block_size(void* ptr);

size_t htlsf_size(void);
size_t htlsf_align_size(void);
size_t htlsf_block_size_min(void);
size_t htlsf_block_size_max(void);
size_t htlsf_pool_overhead(void);
size_t htlsf_alloc_overhead(void);


typedef void (*htlsf_walker)(void* ptr, size_t size, int used, void* user);
void htlsf_walk_pool(htlsf_pool_t pool, htlsf_walker walker, void* user);

int htlsf_check(htlsf_t tlsf);
int htlsf_check_pool(htlsf_pool_t pool);


htlsf_t htlsfheap_set_default(htlsf_t tlsf_heap);
htlsf_t htlsfheap_get_default(void);

void * htlsfheap_malloc(size_t bytes);
void   htlsfheap_free(void *ptr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HTLSF_H_INCLUDE__
