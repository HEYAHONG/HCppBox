#ifndef __HCPPTHREAD_H__
#define __HCPPTHREAD_H__

/*
*线程类，当一个可执行的对象的父对象是线程类时，将自动执行.线程类不可用作子对象
*注意：最好不要直接使用线程调用HCPPObject的函数,若使用其接口设置某些内部标志将导致运行异常
*/

#include <thread>
#include "HCPPObject.h"

class HCPPThread:public HCPPObject,protected std::thread
{
private:
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
    static HCPPThread *New();

    //删除线程，通知删除
    void DeleteThread();

    //设置是否删除子对象
    void SetChildDelete(bool childdelete=true);

private:
    //运行线程
    void RunThread();
};

#endif // __HCPPTHREAD_H__
