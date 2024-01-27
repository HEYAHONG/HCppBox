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
#include "hdefaults.h"

#ifndef HCPPRT_NO_ATOMIC
#include <atomic>
#endif // HCPPRT_NO_ATOMIC

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief  hcpprt初始化，若环境不支持进行一些初始化操作,可调用此函数初始化。
 *
 *
 */
void hcpprt_init();

#ifdef __cplusplus
}
#endif // __cplusplus
#ifdef __cplusplus

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

/*
简易自旋锁(不区分加锁顺序),利用原子操作实现,注意:不支持递归,不能被HBox中的应用直接使用
*/
#ifndef HCPPRT_NO_ATOMIC
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

#endif // __cplusplus
#endif // HCPPRT_H
