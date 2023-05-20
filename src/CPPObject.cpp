
#include "CPPObject.h"
#include <map>
#include <mutex>

//对象池操作
extern void CPPObjectPool_ObjectDelete(void *ptr);

namespace CPPObject
{
static std::map<void *,CPPObjectInfo> CPPHeapObjPool;
static std::mutex CPPHeapObjPoolLock;
static void AddCPPObject(void *ptr)
{
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    CPPHeapObjPool[ptr]=CPPObjectInfo();
}

static void RemoveCPPObject(void *ptr)
{
    CPPObjectPool_ObjectDelete(ptr);
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    auto it=CPPHeapObjPool.find(ptr);
    if(it!=CPPHeapObjPool.end())
    {
        CPPHeapObjPool.erase(it);
    }
}
static void UpdateCPPObject(void *ptr,CPPObjectInfo &info)
{
    RemoveCPPObject(ptr);
    std::lock_guard<std::mutex> lock(CPPHeapObjPoolLock);
    CPPHeapObjPool[ptr]=info;
}

static bool HasCPPObject(void *ptr)
{
    return CPPHeapObjPool.find(ptr)!=CPPHeapObjPool.end();
}

void *CPPObject::operator new(std::size_t count)
{
    //调用全局的new
    void *ptr=::operator new(count);

    //记录此对象
    AddCPPObject(ptr);

    return ptr;
}

void CPPObject::operator delete(void *ptr)
{
    //删除记录
    RemoveCPPObject(ptr);

    //调用全局的delete
    ::operator delete(ptr);
}

bool CPPObject::IsInHeap()
{
    return HasCPPObject(GetVoidPtr());
}

bool CPPObject::IsInThread()
{
    if(HasCPPObject(GetVoidPtr()))
    {
        return CPPHeapObjPool[GetVoidPtr()].GetThreadId()==std::this_thread::get_id();
    }
    else
    {
        return false;
    }
}

void * CPPObject::GetVoidPtr()
{
    return (void *)getthis();
}

}
