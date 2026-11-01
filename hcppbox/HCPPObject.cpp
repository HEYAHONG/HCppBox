/***************************************************************
 * Name:      HCPPObject.cpp
 * Purpose:   实现HCPPObject接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-08
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HCPPOBJECT_IMPLEMENTATION  1

#include "HCPPObject.h"
#include <map>
#include <set>
#include <mutex>
#include <algorithm>


//对象池操作
extern void HCPPObjectPool_ObjectDelete(void *ptr);
namespace HCPPObjectGlobal
{
class HCPPObjectInfo
{
    std::thread::id _tid;
public:
    HCPPObjectInfo();
    HCPPObjectInfo(const HCPPObjectInfo&& other);
    HCPPObjectInfo& operator =(const HCPPObjectInfo&& other);
    HCPPObjectInfo(const HCPPObjectInfo& other);
    HCPPObjectInfo& operator =(const HCPPObjectInfo& other);
    std::thread::id& GetThreadId();
    bool SetThreadId(std::thread::id _id, bool force_update = false);
};
extern  std::map<void *,HCPPObjectInfo> CPPHeapObjPool;
extern  std::recursive_mutex CPPHeapObjPoolLock;
}

static void AddHCPPObject(void *ptr)
{
    std::lock_guard<std::recursive_mutex> lock(HCPPObjectGlobal::CPPHeapObjPoolLock);
    HCPPObjectGlobal::CPPHeapObjPool[ptr]=HCPPObjectGlobal::HCPPObjectInfo();
}

static void RemoveHCPPObject(void *ptr)
{
    HCPPObjectPool_ObjectDelete(ptr);
    std::lock_guard<std::recursive_mutex> lock(HCPPObjectGlobal::CPPHeapObjPoolLock);
    auto it=HCPPObjectGlobal::CPPHeapObjPool.find(ptr);
    if(it!=HCPPObjectGlobal::CPPHeapObjPool.end())
    {
        HCPPObjectGlobal::CPPHeapObjPool.erase(it);
    }
}
static void UpdateHCPPObject(void *ptr,HCPPObjectGlobal::HCPPObjectInfo &info)
{
    RemoveHCPPObject(ptr);
    std::lock_guard<std::recursive_mutex> lock(HCPPObjectGlobal::CPPHeapObjPoolLock);
    HCPPObjectGlobal::CPPHeapObjPool[ptr]=info;
}

static bool HasHCPPObject(void *ptr)
{
    return HCPPObjectGlobal::CPPHeapObjPool.find(ptr)!=HCPPObjectGlobal::CPPHeapObjPool.end();
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
    for(size_t i=0; i<sizeof(flags)/sizeof(flags[0]); i++)
    {
        flags[i]=0;
    }
    if(m_parent!=NULL)
    {
        //添加到父对象
        m_parent->AddToChildList(this);
    }
}

HCPPObject::HCPPObject(HCPPObject &other):m_parent(other.m_parent),m_lock(new std::recursive_mutex)
{
    for(size_t i=0; i<sizeof(flags)/sizeof(flags[0]); i++)
    {
        flags[i]=0;
    }
    if(m_parent!=NULL)
    {
        //添加到父对象
        m_parent->AddToChildList(this);
    }
}

HCPPObject::HCPPObject(HCPPObject &&other):m_parent(other.m_parent),m_lock(new std::recursive_mutex)
{
    for(size_t i=0; i<sizeof(flags)/sizeof(flags[0]); i++)
    {
        flags[i]=0;
    }
    if(m_parent!=NULL)
    {
        //添加到父对象
        m_parent->AddToChildList(this);
    }

    {
        std::lock(*m_lock,*other.m_lock);
        std::lock_guard<std::recursive_mutex> lock(*m_lock,std::adopt_lock);
        std::lock_guard<std::recursive_mutex> lock_other(*other.m_lock,std::adopt_lock);
        auto m_child_list_copy=other.m_child_list;//由于过程中会修改m_child_list，故使用副本进行遍历
        for(std::list<HCPPObject*>::iterator it=m_child_list_copy.begin(); it!=m_child_list_copy.end(); it++)
        {
            HCPPObject *child=(*it);
            if(child!=NULL)
            {
                child->SetParent(this,true);
            }
        }
    }

    {
        //移动标志位
        for(size_t i=0; i<sizeof(flags); i++)
        {
            flags[i]=other.flags[i];
        }
    }
}

HCPPObject & HCPPObject::operator =(HCPPObject &other)
{
    if(&other==this)
    {
        return *this;
    }

    SetParent(other.m_parent,true);

    return *this;
}

HCPPObject & HCPPObject::operator =(HCPPObject &&other)
{
    if(&other==this)
    {
        return *this;
    }

    SetParent(other.m_parent,true);

    {
        std::lock(*m_lock,*other.m_lock);
        std::lock_guard<std::recursive_mutex> lock(*m_lock,std::adopt_lock);
        std::lock_guard<std::recursive_mutex> lock_other(*other.m_lock,std::adopt_lock);
        auto m_child_list_copy=other.m_child_list;//由于过程中会修改m_child_list，故使用副本进行遍历
        for(std::list<HCPPObject*>::iterator it=m_child_list_copy.begin(); it!=m_child_list_copy.end(); it++)
        {
            HCPPObject *child=(*it);
            if(child!=NULL)
            {
                child->SetParent(this,true);
            }
        }
    }

    {
        //移动标志位
        for(size_t i=0; i<sizeof(flags); i++)
        {
            flags[i]=other.flags[i];
        }
    }

    return *this;
}


HCPPObject::~HCPPObject()
{
    SetRunable(false);

    if(m_parent!=NULL)
    {
        //从原有父对象删除
        m_parent->RemoveFromChildList(this);
        m_parent=NULL;
    }
    {
        //清理子对象(从子对象列表中删除子对象)
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        auto m_child_list_copy=m_child_list;//由于删除子对象时会修改m_child_list，故使用副本进行遍历
        for(std::list<HCPPObject*>::iterator it=m_child_list_copy.begin(); it!=m_child_list_copy.end(); it++)
        {
            HCPPObject *child=(*it);
            if(child!=NULL)
            {
                if(child->IsInHeap() && !child->HasFlag(HCPPOBJECT_FLAG_NO_DELETE) && !HasFlag(HCPPOBJECT_FLAG_NO_CHILD_DELETE))
                {
                    //删除在堆上分配的子对象
                    delete child;
                }
                else
                {
                    //当非堆上分配的对象或不移除子对象，直接移除其父对象指针
                    std::lock_guard<std::recursive_mutex> lock(*child->m_lock);
                    child->SetParent(NULL,true);
                }
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

HCPPObject *HCPPObject::GetTopHCPPObject()
{
    std::set<HCPPObject *> visited;
    HCPPObject *ret=this;
    HCPPObject *p=m_parent;
    while(p!=NULL)
    {
        if(!visited.insert(p).second)
        {
            //父子关系成环，返回当前对象，避免无限循环
            return this;
        }
        ret=p;
        p=p->m_parent;
    }
    return ret;
}

bool HCPPObject::SetParent(HCPPObject * parent,bool force_update)
{
    if(!force_update && m_parent!=NULL)
    {
        return false;
    }

    //检查父子关系是否成环，避免GetTopHCPPObject/Run/GC无限循环
    {
        std::set<HCPPObject *> ancestors;
        for(HCPPObject *p=parent; p!=NULL; p=p->m_parent)
        {
            if(p==this || !ancestors.insert(p).second)
            {
                return false;
            }
        }
    }

    //先在锁外操作父对象，避免同时持有“子对象锁”与“父对象锁”造成锁顺序反转
    HCPPObject *old_parent=NULL;
    {
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        if(!force_update && m_parent!=NULL)
        {
            return false;
        }
        old_parent=m_parent;
    }
    if(old_parent!=NULL)
    {
        //从原有父对象删除
        old_parent->RemoveFromChildList(this);
    }
    {
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        m_parent=parent;
    }
    if(parent!=NULL)
    {
        //添加到新父对象
        parent->AddToChildList(this);
    }
    return true;
}

void HCPPObject::EnumChild(std::function<void(const HCPPObject * const)> OnEnum)
{
    //先进行垃圾回收
    GC();

    if(OnEnum==NULL)
    {
        return;
    }
    {
        std::lock_guard<std::recursive_mutex> lock(*m_lock);
        std::list<HCPPObject*> children=m_child_list;//使用副本，避免回调中删除子对象导致迭代器失效
        for(std::list<HCPPObject*>::iterator it=children.begin(); it!=children.end(); it++)
        {
            HCPPObject *child=(*it);
            if(child!=NULL)
            {
                OnEnum(child);
            }
        }
    }
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
        return HCPPObjectGlobal::CPPHeapObjPool[GetVoidPtr()].GetThreadId()==std::this_thread::get_id();
    }
    else
    {
        return false;
    }
}

std::thread::id const * const HCPPObject::GetThreadId()
{
    if(HasHCPPObject(GetVoidPtr()))
    {
        return &HCPPObjectGlobal::CPPHeapObjPool[GetVoidPtr()].GetThreadId();
    }
    return NULL;
}

bool HCPPObject::SetThreadId(std::thread::id _id,bool force_update)
{
    if(HasHCPPObject(GetVoidPtr()))
    {
        return HCPPObjectGlobal::CPPHeapObjPool[GetVoidPtr()].SetThreadId(_id,force_update);
    }
    return false;
}

void * HCPPObject::GetVoidPtr()
{
    return (void *)getthis();
}

void HCPPObject::SetFlag(Flag flag)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(sizeof(flags)*8 > static_cast<size_t>(flag))
    {
        flags[static_cast<size_t>(flag)/8] |= (1UL << (static_cast<size_t>(flag)%8));
    }
}

void HCPPObject::ClearFlag(Flag flag)
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(sizeof(flags)*8 > static_cast<size_t>(flag))
    {
        flags[static_cast<size_t>(flag)/8] &= (~(1UL << (static_cast<size_t>(flag)%8)));
    }
}

bool HCPPObject::HasFlag(Flag flag)
{
    if(sizeof(flags)*8 > static_cast<size_t>(flag))
    {
        return (flags[static_cast<size_t>(flag)/8] & (1UL << (static_cast<size_t>(flag)%8)))!=0;
    }
    return false;
}

void HCPPObject::GC()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    auto m_child_list_copy=m_child_list;//由于删除子对象时会修改m_child_list，故使用副本进行遍历
    for(std::list<HCPPObject*>::iterator it=m_child_list_copy.begin(); it!=m_child_list_copy.end(); it++)
    {
        HCPPObject *child=(*it);
        if(child->HasFlag(HCPPOBJECT_FLAG_TO_BE_DELETED))
        {
            if (child->IsInHeap())
            {
                //删除子对象
                delete child;
            }
            else
            {
                //对于非在堆上分配的对象,不能进行删除
                child->ClearFlag(HCPPOBJECT_FLAG_TO_BE_DELETED);
                //设置对象为不可执行(确保不可执行)
                child->SetRunable(false);
            }

        }
        else
        {
            //调用子对象的GC
            child->GC();
        }
    }
}

void HCPPObject::Run()
{
    if(!HasFlag(HCPPOBJECT_FLAG_RUNABLE))
    {
        //不可执行直接返回
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    if(HasFlag(HCPPOBJECT_FLAG_RUNNING))
    {
        //防止Run重入导致无限递归
        return;
    }
    SetFlag(HCPPOBJECT_FLAG_RUNNING);
    if(!HasFlag(HCPPOBJECT_FLAG_RUN_INIT))
    {
        //执行初始化
        InvokeInit();
        //设置已初始化标志
        SetFlag(HCPPOBJECT_FLAG_RUN_INIT);
    }
    else
    {
        //执行更新
        InvokeUpdate();
    }
    {
        //调用子对象的Run()
        EnumChild([](const HCPPObject *obj)
        {
            if(obj != NULL)
            {
                ((HCPPObject *)obj)->Run();
            }
        }
                 );
    }
    ClearFlag(HCPPOBJECT_FLAG_RUNNING);
}

