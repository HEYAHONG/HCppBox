
#include "HCPPObject.h"
#include <map>
#include <mutex>
#include <algorithm>

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
    std::thread::id &GetThreadId()
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

void HCPPObject::RemoveFromChildList(HCPPObject * child)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    auto it=std::find(m_child_list.begin(),m_child_list.end(),child);
    if(it!=m_child_list.end())
    {
        m_child_list.erase(it);
    }
}

void HCPPObject::AddToChildList(HCPPObject * child)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    m_child_list.push_back(child);
}

HCPPObject::HCPPObject(HCPPObject *parent):m_parent(parent),m_lock(new std::recursive_mutex)
{
    if(m_parent!=NULL)
    {
        //添加到父对象
        m_parent->AddToChildList(this);
    }
}

HCPPObject::~HCPPObject()
{
    if(m_parent!=NULL)
    {
        //从原有父对象删除
        m_parent->RemoveFromChildList(this);
        m_parent=NULL;
    }
    {
        //清理子对象(从子对象列表中删除子对象)
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        for(std::list<HCPPObject*>::iterator it=m_child_list.begin(); it!=m_child_list.end(); it++)
        {
            HCPPObject *child=(*it);
            if(child!=NULL && child->IsInHeap())
            {
                //删除在堆上分配的子对象
                delete child;
            }
        }
        m_child_list.clear();
    }
    //删除锁
    if(m_lock!=NULL)
    {
        delete m_lock;
    }
}

HCPPObject *HCPPObject::GetParent()
{
    return m_parent;
}

bool HCPPObject::SetParent(HCPPObject * parent,bool force_update)
{
    if(!force_update && m_parent!=NULL)
    {
        return false;
    }
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(m_parent!=NULL)
    {
        //从原有父对象删除
        m_parent->RemoveFromChildList(this);
    }
    m_parent=parent;
    if(m_parent!=NULL)
    {
        //添加到新父对象
        m_parent->AddToChildList(this);
    }
    return true;
}

void HCPPObject::Lock()
{
    m_lock->lock();
}
void HCPPObject::UnLock()
{
    m_lock->unlock();
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

std::thread::id *HCPPObject::GetThreadId()
{
    if(HasHCPPObject(GetVoidPtr()))
    {
        return &CPPHeapObjPool[GetVoidPtr()].GetThreadId();
    }
    return NULL;
}

void * HCPPObject::GetVoidPtr()
{
    return (void *)getthis();
}
