#include "HCPPThread.h"

HCPPThread::HCPPThread():HCPPObject(NULL),std::thread(std::bind(&HCPPThread::RunThread,this))
{

}

HCPPThread::~HCPPThread()
{

}

void HCPPThread::RunThread()
{
    //HCPPOBject先于线程初始化，此时可使用HCPPObject的成员。
    //等待初始化
    while(!HasFlag(HCPPOBJECT_FLAG_RUN_INIT))
    {
        Run();
        std::this_thread::yield();
    }

    //HCPPThread类初始化完成,可使用类中的成员变量
    while(!HasFlag(HCPPOBJECT_FLAG_TO_BE_DELETED))
    {
        Run();
        std::this_thread::yield();
    }

    //分离线程与对象
    detach();

    //删除自身
    delete this;
}

//创建线程
HCPPThread *HCPPThread::New()
{
    HCPPThread *ret=new HCPPThread();
    if(ret!=NULL)
    {
        ret->SetRunable();
    }
    return ret;
}

//删除线程
void HCPPThread::DeleteThread()
{
    //设置删除标志
    DeleteLatter();
}

void HCPPThread::SetChildDelete(bool childdelete)
{
    HCPPObject::SetChildDelete(childdelete);
}
