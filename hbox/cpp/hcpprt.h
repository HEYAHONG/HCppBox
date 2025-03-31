/***************************************************************
 * Name:      hcpprt.h
 * Purpose:   hcpprt接口,实现常用的C++移植。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HCPPRT_H
#define HCPPRT_H
#include "hcompiler.h"
#include "hdefaults.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief  hcpprt初始化，若环境不支持进行一些初始化操作,可调用此函数初始化。
 *
 *
 */
void hcpprt_init(void);


/** \brief hcpprt循环，用户需要较低优先级的任务或者空闲任务中周期性调用此函数以保证内部状态得到正确更新。
 *
 *
 */
void hcpprt_loop(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus

/*
无C++11
*/
#ifndef __HAS_CPP11

#ifndef HCPPRT_NO_ATOMIC
#define HCPPRT_NO_ATOMIC 1
#endif // HCPPRT_NO_ATOMIC

#endif // __HAS_CPP11

#ifndef HCPPRT_NO_ATOMIC
#include <atomic>
#endif // HCPPRT_NO_ATOMIC

/*
包装C语言的内存分配,可通过继承此类并实现相应虚函数实现自己的内存分配
*/
class hcmemory
{
public:
    hcmemory()
    {
    }
    virtual ~hcmemory()
    {
    }
    virtual void *malloc(size_t size)
    {
        return hdefaults_malloc(size,NULL);
    }
    virtual void free(void *ptr)
    {
        hdefaults_free(ptr,NULL);
    }
};

hcmemory &hcpprt_global_cmemory();

/*
锁,可通过继承此类并实现相应虚函数实现自己的锁
*/
class hlock
{
public:
    hlock()
    {
    }
    hlock(hlock & oths) = delete;
    hlock(hlock && oths) = delete;
    hlock & operator = (hlock & oths) = delete;
    hlock & operator = (hlock && oths) = delete;
    virtual ~hlock()
    {
    }
    virtual void lock()
    {
        hdefaults_mutex_lock(NULL);
    }
    virtual void unlock()
    {
        hdefaults_mutex_unlock(NULL);
    }
    virtual bool try_lock()
    {
        lock();
        /*
        由于未实现try_lock(),使用lock()永远返回true
        */
        return true;
    }
};

hlock &hcpprt_global_lock();

#ifndef HCPPRT_NO_ATOMIC
/*
简易自旋锁(不区分加锁顺序),利用原子操作实现,注意:不支持递归,不能被HBox中的应用直接使用
*/
class hspinlock:public hlock
{
    std::atomic_flag m_flag=ATOMIC_FLAG_INIT;
public:
    hspinlock()
    {
    }
    hspinlock(hspinlock & oths) = delete;
    hspinlock(hspinlock && oths) = delete;
    hspinlock & operator = (hspinlock & oths) = delete;
    hspinlock & operator = (hspinlock && oths) = delete;
    virtual ~hspinlock()
    {
    }

    //默认为空函数,在实时操作系统中可尝试进行上下文切换(当然那样也不能称之为一般的自旋锁了)
    virtual void spin()
    {
    }

    virtual void lock() override
    {
        while(m_flag.test_and_set())
        {
            spin();
        }
    }

    virtual void unlock() override
    {
        m_flag.clear();
    }

    virtual bool try_lock() override
    {
        return !m_flag.test_and_set();
    }

};


/*
简易可重入自旋锁(不区分加锁顺序),利用原子操作实现,注意:此类不可直接使用，必须实现相应虚函数
*/
class hreentrantspinlock:public hlock
{
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
    std::atomic_int cnt;
public:
    hreentrantspinlock():cnt(0)
    {
    }
    hreentrantspinlock(hreentrantspinlock & oths) = delete;
    hreentrantspinlock(hreentrantspinlock && oths) = delete;
    hreentrantspinlock & operator = (hreentrantspinlock & oths) = delete;
    hreentrantspinlock & operator = (hreentrantspinlock && oths) = delete;
    virtual ~hreentrantspinlock()
    {
    }

    //是否为所有者,仅当已加锁且处于所有者线程这才返回true
    virtual bool is_currnet_ownner() = 0;
    //设置所有者
    virtual void set_currnet_ownner() = 0;
    //清除所有者
    virtual void clear_currnet_ownner() = 0;

    //旋转函数，等待锁时执行
    virtual void spin() = 0;

    virtual void lock() override
    {
        if(is_currnet_ownner())
        {
            cnt++;
        }
        else
        {
            while(m_flag.test_and_set())
            {
                spin();
            }
            set_currnet_ownner();
            cnt++;
        }

    }

    virtual void unlock() override
    {
        if(is_currnet_ownner())
        {
            cnt--;
            if(cnt == 0)
            {
                clear_currnet_ownner();
                m_flag.clear();
            }
        }
    }

    virtual bool try_lock() override
    {
        if(is_currnet_ownner())
        {
            cnt++;
            return true;
        }
        bool ret=!m_flag.test_and_set();
        if(ret)
        {
            set_currnet_ownner();
            cnt++;
        }
        return ret;
    }

};

#endif // HCPPRT_NO_ATOMIC

/*
提供类似std::locak_guard的功能
*/
template<class lock=hlock>
class hlockguard
{
    lock &m_lock;
public:
    hlockguard(lock &_lock):m_lock(_lock)
    {
        m_lock.lock();
    }
    virtual ~hlockguard()
    {
        m_lock.unlock();
    }
};

/*
 * 利用C++运行库进行初始化的入口（仅适用于C++代码）,效果类似GCC的__attribute__((constructor))属性
 */
typedef void (*hcpprt_init_entry_t)(void);
#define HCPPRT_INIT_EXPORT(NAME,ENTRY) \
    __USED\
    static class hcpprt_init_##NAME\
    {\
        public:\
        hcpprt_init_##NAME()\
        {\
            if((ENTRY)!=NULL)\
            {\
                (ENTRY)();\
            }\
        }\
        \
    } g_hcpprt_init_##NAME

#endif // __cplusplus
#endif // HCPPRT_H
