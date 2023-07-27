
#include "HCPPObject.h"
#include <map>
#include <mutex>

//对象池操作
extern void HCPPObjectPool_ObjectDelete(void *ptr);

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
static std::map<void *,HCPPObjectInfo> CPPHeapObjPool;
static std::mutex CPPHeapObjPoolLock;
static void AddHCPPObject(void *ptr)
{
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    CPPHeapObjPool[ptr]=HCPPObjectInfo();
}

static void RemoveHCPPObject(void *ptr)
{
    HCPPObjectPool_ObjectDelete(ptr);
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    auto it=CPPHeapObjPool.find(ptr);
    if(it!=CPPHeapObjPool.end())
    {
        CPPHeapObjPool.erase(it);
    }
}
static void UpdateHCPPObject(void *ptr,HCPPObjectInfo &info)
{
    RemoveHCPPObject(ptr);
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    CPPHeapObjPool[ptr]=info;
}

static bool HasHCPPObject(void *ptr)
{
    return CPPHeapObjPool.find(ptr)!=CPPHeapObjPool.end();
}

void *HCPPObject::operator new(std::size_t count)
{
    //调用全局的new
    void *ptr=::operator new(count);

    //记录此对象
    AddHCPPObject(ptr);

    return ptr;
}

void HCPPObject::operator delete(void *ptr)
{
    //删除记录
    RemoveHCPPObject(ptr);

    //调用全局的delete
    ::operator delete(ptr);
}

bool HCPPObject::IsInHeap()
{
    return HasHCPPObject(GetVoidPtr());
}

bool HCPPObject::IsInThread()
{
    if(HasHCPPObject(GetVoidPtr()))
    {
        return CPPHeapObjPool[GetVoidPtr()].GetThreadId()==std::this_thread::get_id();
    }
    else
    {
        return false;
    }
}

void * HCPPObject::GetVoidPtr()
{
    return (void *)getthis();
}
