/***************************************************************
 * Name:      HCPPThread.h
 * Purpose:   HCPPThread,为可执行的HCPPObject对象提供执行的线程，实际使用时只需要将HCPPThread对象设置为父对象并将HCPPObject对象设置为可执行。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-06-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPTHREAD_H__
#define __HCPPTHREAD_H__

/*
*线程类，当一个可执行的对象的父对象是线程类时，将自动执行.线程类不可用作子对象
*注意：最好不要直接使用线程调用HCPPObject的函数,若使用其接口设置某些内部标志将导致运行异常
*/

#include <thread>
#include <chrono>
#include "HCPPObject.h"

/** \brief 线程类，封装std::thread的部分功能
 */
class HCPPThread:public HCPPObject,protected std::thread
{
private:
    //空闲时睡眠时间
    std::chrono::milliseconds m_idle_sleep_time;
    //不允许用户直接创建对象,使用New函数
    HCPPThread();
    //不允许用户直接删除对象,使用DeleteThread函数
    virtual ~HCPPThread();
    O_HCPPOBJECT
public:
    //不允许复制构造
    HCPPThread(HCPPThread &oths) = delete;
    //不允许移动构造
    HCPPThread(HCPPThread &&oths) = delete;
    //不允许赋值操作
    HCPPThread &operator =(HCPPThread &oths) = delete;
    HCPPThread &operator =(HCPPThread &&oths) = delete;

    //创建线程
    /** \brief 创建线程，线程只可以通过HCPPThread::New()创建，不能使用new HCPPThread创建。
     *
     * \return HCPPThread* 线程指针
     *
     */
    static HCPPThread *New();

    //删除线程，通知删除
    /** \brief 通知删除，调用此函数后不要在访问线程指针（其随时都可能被删除）
     *
     *
     */
    void DeleteThread();

    //设置是否删除子对象
    /** \brief 设置是否删除子对象
     *
     * \param childdelete bool 是否在线程被删除时删除其子对象。
     *
     */
    void SetChildDelete(bool childdelete=true) override;

    /** \brief 获取类型
     *
     * \return Type 类型
     *
     */
    virtual Type GetType() override
    {
        return HCPPOBJECT_TYPE_THREAD;
    }

    /** \brief 设置线程空闲时睡眠时间
     *
     * \param idle_sleep_time std::chrono::milliseconds 空闲时睡眠时间
     *
     */
    void SetIdleSleepTime(std::chrono::milliseconds idle_sleep_time)
    {
        m_idle_sleep_time = idle_sleep_time;
    }

private:
    //运行线程
    void RunThread();
};

#endif // __HCPPTHREAD_H__
