#ifndef __HCPPOBJECT_H__
#define __HCPPOBJECT_H_


#ifdef __cplusplus
#include <thread>
#include "stdlib.h"
#include "stdint.h"
namespace HCPPObject
{

class HCPPObjectInfo
{
    std::thread::id _tid;
public:
    HCPPObjectInfo()
    {
        _tid=std::this_thread::get_id();
    }
    HCPPObjectInfo(HCPPObjectInfo &&other):_tid(other._tid)
    {

    }
    HCPPObjectInfo &operator =(HCPPObjectInfo &&other)
    {
        if(this==&other)
        {
            return *this;
        }

        _tid=other._tid;

        return *this;
    }
    HCPPObjectInfo(HCPPObjectInfo &other):_tid(other._tid)
    {

    }
    HCPPObjectInfo &operator =(HCPPObjectInfo &other)
    {
        if(this==&other)
        {
            return *this;
        }

        _tid=other._tid;

        return *this;
    }
    std::thread::id GetThreadId()
    {
        return _tid;
    }

};


class HCPPObject
{
public:
    //重载new
    void *operator new(std::size_t count);
    //重载delete
    void operator delete(void *ptr);

    //是否在heap上，判断是否为栈变量或堆变量
    bool IsInHeap();

    //若为堆上的变量，当前是否在创建它的线程
    bool IsInThread();

    //获取此类的void指针
    void * GetVoidPtr();

protected:
    //必须在子类重载此函数
    virtual void * getthis()=0;

};


}
#endif // __cplusplus

/*
HCPPObject的子类必须包含以下结构
class A:public HCPPObject::HCPPObject
{
    ...//其它声明或定义
    O_HCPPOBJECT
    ...//其它声明或定义(注意：后面的权限变为public)
}
*/
#define O_HCPPOBJECT \
                    protected:\
                    void *getthis()\
                    {\
                        return static_cast<void *>(this);\
                    }\
                    public:\
                    void *operator new(std::size_t count)\
                    {\
                        return HCPPObject::operator new(count);\
                    }\
                    void operator delete(void *ptr)\
                    {\
                        return HCPPObject::operator delete(ptr);\
                    }\



#endif // __CPPOBJECT__

