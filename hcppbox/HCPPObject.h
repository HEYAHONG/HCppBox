#ifndef __HCPPOBJECT_H__
#define __HCPPOBJECT_H__
/*
*HCPPObject为基础类，如需添加新功能可修改以下枚举：
*   Type:对象类型，如有新类型需要添加至此枚举
*   Flag:对象标志，如有新标志（需要全局使用）需要添加至此枚举
*
*/

#ifdef __cplusplus
#include <mutex>
#include <list>
#include <thread>
#include <typeinfo>
#include <functional>
#include "stdlib.h"
#include "stdint.h"
class HCPPObject
{
private:
    //对象锁
    std::recursive_mutex *m_lock;
    //父对象指针
    HCPPObject *m_parent;
    //子对象列表
    std::list<HCPPObject *> m_child_list;
    void AddToChildList(HCPPObject * child);
    void RemoveFromChildList(HCPPObject * child);
public:
    HCPPObject(HCPPObject *parent=NULL);
    virtual ~HCPPObject();

    //拷贝构造函数,注意：不复制子对象列表与标志
    HCPPObject(HCPPObject &other);
    //移动构造函数,注意:将移动子对象列表与标志。当需要移动一个对象时请使用std::move将左值引用转化右值引用。
    HCPPObject(HCPPObject &&other);
    //赋值操作符(左值),注意：不赋值子对象列表与标志
    HCPPObject & operator =(HCPPObject &other);
    //赋值操作符(右值),注意:将移动子对象列表与标志。当需要移动一个对象时请使用std::move将左值引用转化右值引用。
    HCPPObject & operator =(HCPPObject &&other);

    //获取父对象
    HCPPObject *GetParent();

    //获取最顶层对象,对于没有父对象的对象而言，顶层对象就是他们自身。
    HCPPObject *GetTopHCPPObject();

    //设置父对象
    bool SetParent(HCPPObject * parent,bool force_update=false);

    //子对象枚举
    void EnumChild(std::function<void(const HCPPObject * const)> OnEnum);

    //锁定对象
    void Lock();

    //解锁对象
    void UnLock();

    typedef enum
    {
        HCPPOBJECT_TYPE_BASE=0, /**< HCPPObject基类 */
        HCPPOBJECT_TYPE_SIMPLE, /**< HCPPObjectSimple模板类 */
        HCPPOBJECT_TYPE_CUSTOM_BASE /**< 自定义类型基址，可使用类似HCPPOBJECT_TYPE_CUSTOM_BASE+0,HCPPOBJECT_TYPE_CUSTOM_BASE+1形式自定义类*/
    } Type;

    //重载new
    void *operator new(std::size_t count);
    //重载delete
    void operator delete(void *ptr);

    //是否在heap上，判断是否为栈变量或堆变量
    bool IsInHeap();

    //若为堆上的变量，当前是否在创建它的线程
    bool IsInThread();

    //获取线程ID
    std::thread::id const * const GetThreadId();

    //设置线程ID，默认情况下只有对象所属线程可以更新
    bool SetThreadId(std::thread::id _id,bool force_update=false);

    //获取此类的void指针
    void * GetVoidPtr();

    //获取类型
    virtual Type GetType()
    {
        return HCPPOBJECT_TYPE_BASE;
    }

    //获取typeid
    virtual const std::type_info & GetTypeId()
    {
        return typeid(*this);
    }

    typedef enum
    {
        HCPPOBJECT_FLAG_START=0,//起始,无其它作用，其余标志需在此标志之后
        HCPPOBJECT_FLAG_NO_CHILD_DELETE,//不删除子对象,注意：当此标志被设定后，一旦此对象被删除，其子对象将不存在父对象
        HCPPOBJECT_FLAG_TO_BE_DELETED,//准备被删除，当此标志位被被置位时，父对象（或其它对象）不应该使用此类的资源,在合适的时机将删除此对象。
        HCPPOBJECT_FLAG_RUNABLE,//对象是否可执行,当无此标志时，调用Run成员函数将无效果，当此标志被置位时，将执行内部操作，如InvokeInit与InvokeUpdate
        HCPPOBJECT_FLAG_RUN_INIT,//对象是否已执行INIT,若置位表示已执行INIT(不再执行InvokeInit函数)
        HCPPOBJECT_FLAG_END,//结束,无其它作用,其余标志需在此标志之前
    } Flag;//标志位

    //设置标志位,在子类重载此函数时需要在函数末尾调用HCPPObject::SetFlag
    virtual void SetFlag(Flag flag);
    //清除标志位,在子类重载此函数时需要在函数末尾调用HCPPObject::ClearFlag
    virtual void ClearFlag(Flag flag);
    //判断标志位,在子类重载此函数时需要在函数末尾调用HCPPObject::HasFlag
    virtual bool HasFlag(Flag flag);

    //设置是否自动删除子对象，在子类重载此函数时需要在函数末尾调用HCPPObject::SetChildDelete
    virtual void SetChildDelete(bool childdelete=true)
    {
        if(childdelete)
        {
            ClearFlag(HCPPOBJECT_FLAG_NO_CHILD_DELETE);
        }
        else
        {
            SetFlag(HCPPOBJECT_FLAG_NO_CHILD_DELETE);
        }
    }

    //稍后删除，在子类重载此函数时需要在函数末尾调用HCPPObject::DeleteLatter()
    //一般用于多线程，当在一个线程想要删除由另一个线程管理的对象时，可调用此函数通知其管理线程删除此对象
    virtual void DeleteLatter()
    {
        //默认只设置标志位,
        SetFlag(HCPPOBJECT_FLAG_TO_BE_DELETED);
    }

    //垃圾回收(清理需要删除的子对象)，一般配合DeleteLatter()使用，在子类重载此函数时需要在函数末尾调用HCPPObject::GC()
    virtual void GC();

    //执行初始化，由Run调用，通常需要在子类中重载,不可长时间阻塞
    virtual void InvokeInit()
    {

    }


    //执行更新，由Run调用，通常需要在子类中重载，不可长时间阻塞
    virtual void InvokeUpdate()
    {

    }

    //运行此对象（内部不阻塞），可多次调用，在子类重载此函数时需要在函数末尾调用HCPPObject::Run()
    virtual void Run();

    //设置是否可运行，在子类重载此函数时需要在函数末尾调用HCPPObject::SetRunable
    virtual void SetRunable(bool runable=true)
    {
        if(runable)
        {
            SetFlag(HCPPOBJECT_FLAG_RUNABLE);
        }
        else
        {
            ClearFlag(HCPPOBJECT_FLAG_RUNABLE);
        }
    }

protected:
    //必须在子类重载此函数
    virtual void * getthis()=0;

private:
    //标志位，不同的标志有不同的行为
    uint8_t flags[(static_cast<size_t>(HCPPOBJECT_FLAG_END)/(8*sizeof(uint8_t)))+1];


};
#endif // __cplusplus

/*
HCPPObject的子类必须包含以下结构
class A:public HCPPObject
{
    ...//其它声明或定义
    O_HCPPOBJECT
    ...//其它声明或定义(注意：后面的权限变为public)
}
*/
#define O_HCPPOBJECT \
                    protected:\
                    void *getthis() override\
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
                    const std::type_info & GetTypeId() override\
                    {\
                        return typeid(*this);\
                    }\

#ifdef __cplusplus
/*
简易的C++对象模板
*/
template<typename T>
class HCPPObjectSimple:public HCPPObject
{
    T data;
    O_HCPPOBJECT
public:
    HCPPObjectSimple(T &_data,HCPPObject *parent=NULL):data(_data),HCPPObject(parent)
    {

    }
    HCPPObjectSimple(T &&_data,HCPPObject *parent=NULL):data(_data),HCPPObject(parent)
    {

    }
    virtual ~HCPPObjectSimple()
    {

    }
    //获取类型
    Type GetType() override
    {
        return HCPPOBJECT_TYPE_SIMPLE;
    }
    //获取数据,失败返回NULL
    template<typename DataType=T>
    DataType *GetDataPtr()
    {
        return dynamic_cast<DataType*>(&data);
    }
};

/*
判断简易的C++对象模板数据类型是否为某类型
*/
template<typename T>
bool IsHCPPObjectSimpleDataType(HCPPObject *obj)
{
    if((obj!=NULL) && (obj->GetType()==HCPPObject::HCPPOBJECT_TYPE_SIMPLE))
    {
        return typeid(HCPPObjectSimple<T>)==obj->GetTypeId();
    }
    return false;
}
#endif // __cplusplus

#endif // __CPPOBJECT__

