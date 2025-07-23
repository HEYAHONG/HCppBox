/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   实现一些一些默认函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hcompiler.h"
#include "hdefaults.h"
#include "time.h"

#ifdef __RTTHREAD__
#include "rtthread.h"
#endif // __RTTHREAD__
#ifdef WIN32
#include "windows.h"
static CRITICAL_SECTION g_mutex_lock;
static bool g_is_mutex_lock_init=false;
static void check_mutex_lock()
{
    if(!g_is_mutex_lock_init)
    {
        InitializeCriticalSection(&g_mutex_lock);
        g_is_mutex_lock_init=true;
    }
}
#endif // WIN32
#ifdef __unix__
#include "pthread.h"
static pthread_mutex_t g_mutex_lock;
static pthread_mutexattr_t g_mutex_lock_attr;
static bool g_is_mutex_lock_init=false;
static void check_mutex_lock()
{
    if(!g_is_mutex_lock_init)
    {
        pthread_mutexattr_init(&g_mutex_lock_attr);
        pthread_mutexattr_settype(&g_mutex_lock_attr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&g_mutex_lock,&g_mutex_lock_attr);
        g_is_mutex_lock_init=true;
    }
}
#endif // __unix__

#ifdef HDEFAULTS_TICK_GET
extern hdefaults_tick_t HDEFAULTS_TICK_GET(void);
#endif // HDEFAULTS_TICK_GET
static hdefaults_tick_t do_hdefaults_tick_get(void)
{
#ifdef HDEFAULTS_TICK_GET
    return HDEFAULTS_TICK_GET();
#elif defined(HDEFAULTS_OS_RTTHREAD)
    return rt_tick_get_millisecond();
#elif defined(HDEFAULTS_OS_WINDOWS)
    return GetTickCount();
#elif defined(HDEFAULTS_OS_UNIX)
#if defined(CLOCK_MONOTONIC)
    {
        struct timespec ts={0};
        clock_gettime(CLOCK_MONOTONIC,&ts);
        return ts.tv_sec*1000ULL+ts.tv_nsec/1000000ULL;
    }
#else
    {
        hdefaults_tick_t ret=0;
        struct timeval tv= {0};
        struct timezone tz= {0};
        gettimeofday(&tv, &tz);
        ret+=tv.tv_sec*1000;
        ret+=tv.tv_usec/1000;
        return ret;
    }
#endif
#else
    return 1000ULL*clock()/(CLOCKS_PER_SEC);
#endif // HDEFAULTS_TICK_GET
}

#ifdef USING_HMEMORYHEAP
extern void* hmemoryheap_malloc(size_t nBytes);
#endif // USING_HMEMORYHEAP
#ifdef HDEFAULTS_MALLOC
extern void * HDEFAULTS_MALLOC(size_t bytes);
#endif // HDEFAULTS_MALLOC
static void * do_hdefaults_malloc(size_t nBytes,void *usr)
{
    UNUSED(usr);
#ifdef HDEFAULTS_MALLOC
    return HDEFAULTS_MALLOC(nBytes);
#elif defined(__RTTHREAD__)
    return rt_malloc(nBytes);
#elif defined(USING_HMEMORYHEAP)
    return hmemoryheap_malloc(nBytes);
#else
    return malloc(nBytes);
#endif
}

#ifdef USING_HMEMORYHEAP
extern void hmemoryheap_free(void*);
#endif // USING_HMEMORYHEAP
#ifdef HDEFAULTS_FREE
extern void  HDEFAULTS_FREE(void *ptr);
#endif // HDEFAULTS_FREE
static void do_hdefaults_free(void *ptr,void *usr)
{
    UNUSED(usr);
#ifdef HDEFAULTS_FREE
    HDEFAULTS_FREE(ptr);
#elif defined(__RTTHREAD__)
    rt_free(ptr);
#elif defined(USING_HMEMORYHEAP)
    hmemoryheap_free(ptr);
#else
    free(ptr);
#endif

}


#ifdef HDEFAULTS_MUTEX_LOCK
extern void HDEFAULTS_MUTEX_LOCK(void);
#endif // HDEFAULTS_MUTEX_LOCK

static void  do_hdefaults_mutex_lock(void *usr)
{
    UNUSED(usr);
#ifdef HDEFAULTS_MUTEX_LOCK
    HDEFAULTS_MUTEX_LOCK();
#elif defined(__RTTHREAD__)
    rt_enter_critical();
#elif defined(WIN32)
    check_mutex_lock();
    EnterCriticalSection(&g_mutex_lock);
#elif defined(__unix__)
    check_mutex_lock();
    pthread_mutex_lock(&g_mutex_lock);
#else
#warning "hdefaults_mutex_lock is  invalid!"
#endif
}

#ifdef HDEFAULTS_MUTEX_UNLOCK
extern void HDEFAULTS_MUTEX_UNLOCK(void);
#endif // HDEFAULTS_MUTEX_UNLOCK

static void  do_hdefaults_mutex_unlock(void *usr)
{
    UNUSED(usr);
#ifdef  HDEFAULTS_MUTEX_UNLOCK
    HDEFAULTS_MUTEX_UNLOCK();
#elif defined(__RTTHREAD__)
    rt_exit_critical();
#elif defined(WIN32)
    check_mutex_lock();
    LeaveCriticalSection(&g_mutex_lock);
#elif defined(__unix__)
    check_mutex_lock();
    pthread_mutex_unlock(&g_mutex_lock);
#else
#warning "hdefaults_mutex_unlock is  invalid!"
#endif
}


HDEFAULTS_USERCALL_DECLARE(tick);
HDEFAULTS_USERCALL_DECLARE(malloc);
HDEFAULTS_USERCALL_DECLARE(free);
HDEFAULTS_USERCALL_DECLARE(glock);
HDEFAULTS_USERCALL_DECLARE(gunlock);
intptr_t do_hdefaults_usercall(uintptr_t number,...)
{
    intptr_t ret=0;
    va_list va;
    va_start(va,number);
    if(number < HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_END)
    {
        //准备调用syscall
        hdefaults_syscall_function_t func=hdefaults_syscall_function_find(number);
        if(func!=NULL)
        {
            ret=func(number,va);
        }
    }
    else if(number < HDEFAULTS_USERCALL_NUMBER_END)
    {

        switch(number)
        {
        case HDEFAULTS_USERCALL_NUMBER_TICK:
        {
            ret=__hdefaults_usercall_tick(number,va);
        }
        break;
        case HDEFAULTS_USERCALL_NUMBER_MALLOC:
        {
            ret=__hdefaults_usercall_malloc(number,va);
        }
        break;
        case HDEFAULTS_USERCALL_NUMBER_FREE:
        {
            ret=__hdefaults_usercall_free(number,va);
        }
        break;
        case HDEFAULTS_USERCALL_NUMBER_GLOCK:
        {
            ret=__hdefaults_usercall_glock(number,va);
        }
        break;
        case HDEFAULTS_USERCALL_NUMBER_GUNLOCK:
        {
            ret=__hdefaults_usercall_gunlock(number,va);
        }
        break;
        default:
        {

        }
        break;
        }
    }
    va_end(va);
    return ret;
}

intptr_t  __hdefaults_usercall_return_check(uintptr_t usercall_number,intptr_t return_value)
{
    return return_value;
}

HDEFAULTS_USERCALL_DEFINE0(tick,HDEFAULTS_USERCALL_NUMBER_TICK,hdefaults_tick_t)
{
    return hdefaults_tick_get();
}

HDEFAULTS_USERCALL_DEFINE2(malloc,HDEFAULTS_USERCALL_NUMBER_MALLOC,void *,size_t,nBytes,void *,usr)
{
    return hdefaults_malloc(nBytes,usr);
}

HDEFAULTS_USERCALL_DEFINE2(free,HDEFAULTS_USERCALL_NUMBER_FREE,int,void *,ptr,void *,usr)
{
    hdefaults_free(ptr,usr);
    return 0;
}

HDEFAULTS_USERCALL_DEFINE1(glock,HDEFAULTS_USERCALL_NUMBER_GLOCK,int,void *,usr)
{
    hdefaults_mutex_lock(usr);
    return 0;
}

HDEFAULTS_USERCALL_DEFINE1(gunlock,HDEFAULTS_USERCALL_NUMBER_GUNLOCK,int,void *,usr)
{
    hdefaults_mutex_unlock(usr);
    return 0;
}

static const hdefaults_api_table_t default_api_table=
{
    do_hdefaults_tick_get,
    do_hdefaults_malloc,
    do_hdefaults_free,
    do_hdefaults_mutex_lock,
    do_hdefaults_mutex_unlock,
    do_hdefaults_usercall,
};
static const hdefaults_api_table_t * api_table=&default_api_table;
const hdefaults_api_table_t * hdefaults_get_api_table(void)
{
    return api_table;
}

const hdefaults_api_table_t * hdefaults_set_api_table(const hdefaults_api_table_t* new_api_table)
{
    const hdefaults_api_table_t* old_api_table=api_table;
    api_table=new_api_table;
    return old_api_table;
}

hdefaults_tick_t hdefaults_tick_get(void)
{
    const hdefaults_api_table_t *api_table=hdefaults_get_api_table();
    if(api_table!=NULL && api_table->tick_get!=NULL)
    {
        return api_table->tick_get();
    }
    else
    {
        return do_hdefaults_tick_get();
    }
}

void * hdefaults_malloc(size_t nBytes,void *usr)
{
    const hdefaults_api_table_t *api_table=hdefaults_get_api_table();
    if(api_table!=NULL && api_table->mem_alloc!=NULL)
    {
        return api_table->mem_alloc(nBytes,usr);
    }
    else
    {
        return do_hdefaults_malloc(nBytes,usr);
    }
}

void hdefaults_free(void *ptr,void *usr)
{
    const hdefaults_api_table_t *api_table=hdefaults_get_api_table();
    if(api_table!=NULL && api_table->mem_free!=NULL)
    {
        api_table->mem_free(ptr,usr);
    }
    else
    {
        do_hdefaults_free(ptr,usr);
    }
}

void  hdefaults_mutex_lock(void *usr)
{
    const hdefaults_api_table_t *api_table=hdefaults_get_api_table();
    if(api_table!=NULL && api_table->mutex_lock!=NULL)
    {
        api_table->mutex_lock(usr);
    }
    else
    {
        do_hdefaults_mutex_lock(usr);
    }
}

void  hdefaults_mutex_unlock(void *usr)
{
    const hdefaults_api_table_t *api_table=hdefaults_get_api_table();
    if(api_table!=NULL && api_table->mutex_unlock!=NULL)
    {
        api_table->mutex_unlock(usr);
    }
    else
    {
        do_hdefaults_mutex_unlock(usr);
    }
}


#include "syscall/hdefaults_syscall.c"
#include "libc/hdefaults_libc_port.c"
#include "libdl/hdefaults_libdl_port.c"

