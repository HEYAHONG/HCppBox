/***************************************************************
 * Name:      hevent.c
 * Purpose:   实现hevent接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hevent.h"

#ifdef  internal_heap_mem_alloc
#undef  internal_heap_mem_alloc
#endif // internal_heap_mem_alloc
#define  internal_heap_mem_alloc heventloop_internal_heap_mem_alloc
#ifdef  internal_heap_mem_free
#undef  internal_heap_mem_free
#endif // internal_heap_mem_free
#define  internal_heap_mem_free heventloop_internal_heap_mem_free
#include "hevent/heventloop.c"


#ifdef  internal_heap_mem_alloc
#undef  internal_heap_mem_alloc
#endif // internal_heap_mem_alloc
#define  internal_heap_mem_alloc heventslots_internal_heap_mem_alloc
#ifdef  internal_heap_mem_free
#undef  internal_heap_mem_free
#endif // internal_heap_mem_free
#define  internal_heap_mem_free heventslots_internal_heap_mem_free
#include "hevent/heventslots.c"


#ifdef  internal_heap_mem_alloc
#undef  internal_heap_mem_alloc
#endif // internal_heap_mem_alloc
#define  internal_heap_mem_alloc heventchain_internal_heap_mem_alloc
#ifdef  internal_heap_mem_free
#undef  internal_heap_mem_free
#endif // internal_heap_mem_free
#define  internal_heap_mem_free heventchain_internal_heap_mem_free
#include "hevent/heventchain.c"

