#ifndef __CPPOBJECT_H__
#define __CPPOBJECT_H_


#ifdef __cplusplus
#include <thread>
#include "stdlib.h"
#include "stdint.h"
namespace CPPObject
{

class CPPObjectInfo
{
    std::thread::id _tid;
public:
    CPPObjectInfo()
    {
        _tid=std::this_thread::get_id();
    }
    CPPObjectInfo(CPPObjectInfo &&other):_tid(other._tid)
    {

    }
    CPPObjectInfo &operator =(CPPObjectInfo &&other)
    {
        if(this==&other)
        {
            return *this;
        }

        _tid=other._tid;

        return *this;
    }
    CPPObjectInfo(CPPObjectInfo &other):_tid(other._tid)
    {

    }
    CPPObjectInfo &operator =(CPPObjectInfo &other)
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


class CPPObject
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

protected:
    //必须在子类重载此函数
    virtual void * getthis()=0;

};


}
#endif // __cplusplus

/*
CPPObject的子类必须包含以下结构
class A:public CPPObject::CPPObject
{
    ...//其它声明或定义
protected:
    O_CPPOBJECT
    ...//其它声明或定义(需要重新声明权限)
}
*/
#define O_CPPOBJECT \
                    void *getthis()\
                    {\
                        return static_cast<void *>(this);\
                    }\
                    public:\
                    void *operator new(std::size_t count)\
                    {\
                        return CPPObject::operator new(count);\
                    }\
                    void operator delete(void *ptr)\
                    {\
                        return CPPObject::operator delete(ptr);\
                    }\
                    protected:



#endif // __CPPOBJECT__

