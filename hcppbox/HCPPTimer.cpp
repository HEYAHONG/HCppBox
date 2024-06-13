#include "HCPPTimer.h"
#include <mutex>

namespace HCPPTimerGlobal
{
extern class TimerThread
{
    HCPPThread *m_thread;
    std::recursive_mutex m_lock;
public:
    TimerThread();
    ~TimerThread();
    HCPPThread* GetThread();
} g_timerthread;

}


HCPPTimer::HCPPTimer(HCPPObject *parent,std::chrono::microseconds interval,std::function<void(HCPPTimer *timer)> timeout,bool IsOneShot):HCPPObject(parent),m_interval(interval),m_timeout(timeout),m_IsOneShot(IsOneShot),m_end(std::chrono::steady_clock::time_point::min())
{
    m_start=std::chrono::steady_clock::now();
    m_end=m_start+m_interval;
    SetParent(parent,true);
    SetRunable();
}

HCPPTimer::~HCPPTimer()
{
    SetRunable(false);
}

void HCPPTimer::timeout()
{
    if(m_timeout!=NULL)
    {
        try
        {
            m_timeout(this);
        }
        catch(...)
        {

        }
    }
}

bool HCPPTimer::SetParent(HCPPObject * parent,bool force_update)
{
    //定时器必须要求顶层父对象为线程
    if(parent==NULL || parent->GetTopHCPPObject()->GetType() != HCPPOBJECT_TYPE_THREAD)
    {
        parent=HCPPTimerGlobal::g_timerthread.GetThread();
    }
    return HCPPObject::SetParent(parent,force_update);
}

void HCPPTimer::DeleteTimer()
{
    DeleteLatter();
}

void HCPPTimer::InvokeInit()
{

}

void HCPPTimer::InvokeUpdate()
{
    if(m_end!=std::chrono::steady_clock::time_point::min() && std::chrono::steady_clock::now()>m_end)
    {
        //定时器超时
        if(m_IsOneShot)
        {
            timeout();
            m_end=std::chrono::steady_clock::time_point::min();
            //稍后删除
            DeleteLatter();
        }
        else
        {
            timeout();
            m_end+=m_interval;
        }
    }
}
