/***************************************************************
 * Name:      HCPPTimer.h
 * Purpose:   HCPPTimer定时器接口，依赖HCPPThread,通过轮询实现。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPTIMER_H__
#define __HCPPTIMER_H__

#include "HCPPObject.h"
#include "HCPPThread.h"
#include <chrono>

/** \brief 定时器(软件定时器,非精确定时),由于定时器不可用作父对象，故采用保护继承(HCPPTimer指针无法在类外转换为HCPPObject指针)。当定时器失效时将自动被删除。
 */
class HCPPTimer:protected HCPPObject
{
    O_HCPPOBJECT
protected:
    bool m_IsOneShot;
    std::chrono::steady_clock::time_point m_start;
    std::chrono::steady_clock::time_point m_end;
    std::chrono::microseconds m_interval;
    std::function<void(HCPPTimer *timer)> m_timeout;
public:
    HCPPTimer(HCPPObject *parent,std::chrono::microseconds interval,std::function<void(HCPPTimer *timer)> timeout=NULL,bool IsOneShot=false);
    virtual ~HCPPTimer();

    HCPPTimer(const HCPPTimer& other)=delete;
    HCPPTimer& operator=(const HCPPTimer& other)=delete;
    HCPPTimer(const HCPPTimer&& other)=delete;
    HCPPTimer& operator=(const HCPPTimer&& other)=delete;

    //定时器超时,在子类重载此函数时需要在函数末尾调用HCPPTimer::timeout()
    /** \brief
     *
     *
     */
    virtual void timeout();

    /** \brief 获取类型
    *
    * \return Type 类型
    *
    */
    virtual Type GetType() override
    {
        return HCPPOBJECT_TYPE_TIMER;
    }

    //设置父对象
    /** \brief 设置父对象
     *
     * \param parent HCPPObject* 父对象指针，可为NULL
     *
     * \param force_update bool 是否强制更新，为false时当原有父指针不为空时不更新指针，反之直接更新指针。
     * \return bool 是否成功
     *
     */
    bool SetParent(HCPPObject * parent,bool force_update=false);

    //删除定时器,替代直接使用delete(具有危险性)
    /** \brief 通知删除，调用此函数后不要在访问定时器指针（其随时都可能被删除）
     *
     *
     */
    void DeleteTimer();

private:

    void InvokeInit() override;
    void InvokeUpdate() override;
};

#endif // __HCPPTIMER_H__
