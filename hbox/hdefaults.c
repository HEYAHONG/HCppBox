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


#ifdef __RTTHREAD__
#include "rtthread.h"
#endif // __RTTHREAD__
#ifdef WIN32
#include "windows.h"
static CRITICAL_SECTION g_mutex_lock;
static bool g_is_mutex_lock_init=false;
void check_mutex_lock()
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
void check_mutex_lock()
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
    {
        hdefaults_tick_t ret=0;
        struct timeval tv= {0};
        struct timezone tz= {0};
        gettimeofday(&tv, &tz);
        ret+=tv.tv_sec*1000;
        ret+=tv.tv_usec/1000;
        return ret;
    }
#else
    return 0;//默认永远返回0
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

const hdefaults_api_table_t defalut_table=
{
    do_hdefaults_tick_get,
    do_hdefaults_malloc,
    do_hdefaults_free,
    do_hdefaults_mutex_lock,
    do_hdefaults_mutex_unlock,
};
const hdefaults_api_table_t * hdefaults_get_api_table(void)
{
    return &defalut_table;
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

