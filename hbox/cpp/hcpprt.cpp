/***************************************************************
 * Name:      hcpprt.cpp
 * Purpose:   hcpprt实现,实现常用的C++移植。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcpprt.h"
#include "hcompiler.h"
#include "hdefaults.h"
#include "h3rdparty.h"
#include "hevent.h"
#include "hruntime.h"


/*
通过此类判断构造函数是否被执行。
*/
static class ctors_state
{
    int state;
    int GetMagicNumber()
    {
        return 0x5A5AAA55;
    }
public:
    ctors_state():state(0)
    {
        state=GetMagicNumber();
    }
    ~ctors_state()
    {

    }
    bool IsOk()
    {
        /*
        当未构造时，内存数据一般不为魔数
        */
        return state==GetMagicNumber();
    }
} g_ctors_state;

#ifdef HCPPRT_USE_CTORS

extern "C"
{
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
    //GCC环境
    typedef void(*ctors_func)();
#ifdef WIN32
    //Windows一般不需要手动进行构造函数调用
    ctors_func *__init_array_start=NULL;
    ctors_func *__init_array_end=NULL;
#else
    //由链接脚本提供
    extern ctors_func __init_array_start[];
    //由链接脚本提供
    extern ctors_func __init_array_end[];
#endif
#endif
}

#endif

static void ctors_execute()
{
#ifdef HCPPRT_USE_CTORS

    if(!g_ctors_state.IsOk())
    {
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
        /*
        GCC环境中，某些SDK不提供C++构造函数支持，需要手动添加构造函数,需要链接脚本提供___init_array_start与___init_array_end符号。
        */
        {
            ctors_func *p=NULL;

            for (p = __init_array_start; p < __init_array_end; p++)
            {
                (*p)();
            }
        }
#endif
    }
#endif
}

void hcpprt_init()
{
    /*
     * C语言组件初始化(底层部分)
     */
    hruntime_init_lowlevel();

    /*
     * 系统循环
     */
    {
        heventslots_t *slots_loop=heventslots_get_slots_from_table(HEVENTSLOTS_SYSTEM_SLOTS_LOOP);
        if(slots_loop==NULL)
        {
#ifndef HCPPRT_SYSTEM_LOOP_NO_AUTOINIT
            heventslots_set_slots_to_table(HEVENTSLOTS_SYSTEM_SLOTS_LOOP,NULL);
#endif // HCPPRT_SYSTEM_LOOP_NO_AUTOINIT
        }
    }

    /*
     * 工作队列
     */
    {
        heventloop_t *loop_workqueue=heventloop_get_loop_from_table(HEVENTLOOP_SYSTEM_LOOP_WORKQUEUE);
        if(loop_workqueue==NULL)
        {
#ifndef HCPPRT_SYSTEM_WORKQUEUE_NO_AUTOINIT
            heventloop_set_loop_to_table(HEVENTLOOP_SYSTEM_LOOP_WORKQUEUE,NULL);
#endif // HCPPRT_SYSTEM_WORKQUEUE_NO_AUTOINIT
        }
    }

    /*
     * 执行构造函数
     */
    ctors_execute();

    /*
     * C语言组件初始化
     */
    hruntime_init();
}

HSTACKLESSCOROUTINE_DECLARE_COROUTINE(hsoftdog);
void hcpprt_loop(void)
{
    //hsoftdog组件
    HSTACKLESSCOROUTINE_ENTRY(hsoftdog);

    //C语言组件循环
    hruntime_loop();
}


#ifndef HCPPRT_NO_NEW_AND_DELETE_OVERRIDE

//重载全局new与delete,如需进一步移植请参考hdefaults组件
void *operator new(size_t size)
{
    return hdefaults_malloc(size,NULL);
}

void *operator new[](size_t size)
{
    return hdefaults_malloc(size,NULL);
}

#ifdef _GLIBCXX_USE_NOEXCEPT
#define DELETE_USE_NOEXCEPT  _GLIBCXX_USE_NOEXCEPT
#endif // _GLIBCXX_USE_NOEXCEPT

#ifdef __clang__
#ifndef DELETE_USE_NOEXCEPT
#define DELETE_USE_NOEXCEPT throw()
#endif // DELETE_USE_NOEXCEPT
#endif

void operator delete(void *ptr)
#ifdef DELETE_USE_NOEXCEPT
DELETE_USE_NOEXCEPT
#endif
{
    hdefaults_free(ptr,NULL);
}

void operator delete[](void *ptr)
#ifdef DELETE_USE_NOEXCEPT
DELETE_USE_NOEXCEPT
#endif
{
    hdefaults_free(ptr,NULL);
}

#endif // HCPPRT_NO_NEW_AND_DELETE_OVERRIDE

static hcmemory g_cmemory;
hcmemory &hcpprt_global_cmemory()
{
    return g_cmemory;
}

static hlock g_lock;
hlock &hcpprt_global_lock()
{
    return g_lock;
}
