/***************************************************************
 * Name:      hobj.h
 * Purpose:   hobj
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HOBJ_H__
#define __HOBJ_H__
#include "hcompiler.h"
#include "hdefaults.h"
#include "hmemory.h"
#include "hcpprt.h"
#ifdef __cplusplus

/*
 * hobj公共基类，hobj.h中的静态类均由此派生
 */
class hstaticobjectbase:virtual public hlock
{
public:
    hstaticobjectbase()
    {

    }
    virtual ~hstaticobjectbase()
    {

    }
    /*
     * 获取当前引用计数，0表示无任何引用（此时可以删除对象）
     */
    virtual size_t ref_count()
    {
        return 1;
    }
    /*
     * 引用计数加
     */
    virtual void  ref_inc()
    {

    }
    /*
     * 引用计数减
     */
    virtual void ref_dec()
    {

    }
};

/*
 * T主要用于C语言的结构体，C++类需要支持赋值构造函数与析构函数。
 */
template<typename T>
class hstaticobjectext:public hstaticobjectbase
{
private:
    //结构体数据存储
    uint8_t obj_data[sizeof(T)+sizeof(hobject_managed_struct_t)];
    hobject_managed_struct_t *get_hobject_managed_struct_t()
    {
        return reinterpret_cast<hobject_managed_struct_t *>(obj_data);
    }
public:
    hstaticobjectext(T & data,uint16_t usr_type=0,void (*onfree)(struct  __hobject_managed_struct *obj_ptr)=NULL)
    {
        memset(obj_data,0,sizeof(obj_data));
        hobject_managed_struct_init(get_hobject_managed_struct_t(),usr_type,onfree);
        //(*(T *)((*get_hobject_managed_struct_t()).o_struct))=data;
        new(((*get_hobject_managed_struct_t()).o_struct)) T(data);
    }
    hstaticobjectext(T && data,uint16_t usr_type=0,void (*onfree)(struct  __hobject_managed_struct *obj_ptr)=NULL)
    {
        memset(obj_data,0,sizeof(obj_data));
        hobject_managed_struct_init(get_hobject_managed_struct_t(),usr_type,onfree);
        //(*(T *)((*get_hobject_managed_struct_t()).o_struct))=data;
        new(((*get_hobject_managed_struct_t()).o_struct)) T((T &&)data);
    }
    hstaticobjectext(hstaticobjectext & oths)=delete;
    hstaticobjectext(hstaticobjectext && oths)=delete;
    hstaticobjectext &operator =(hstaticobjectext & oths)=delete;
    hstaticobjectext &operator =(hstaticobjectext && oths)=delete;
    virtual ~hstaticobjectext()
    {
        hobject_cleanup(HOBJECT_BASE(get_hobject_managed_struct_t()));
        //支持C++析构
        ((T *)((*get_hobject_managed_struct_t()).o_struct))->~T();
    }
    uint16_t obj_type()
    {
        return (HOBJECT_BASE(get_hobject_managed_struct_t()))->type;
    }
    uint16_t obj_usr_type()
    {
        return (HOBJECT_BASE(get_hobject_managed_struct_t()))->usr_type;
    }
    /*
     * 通过hobject*指针获取this指针，用于onfree回调。
     */
    static const hstaticobjectext *get_this(const struct  __hobject_managed_struct *obj_ptr)
    {
        return static_cast<const hstaticobjectext *>(GET_STRUCT_PTR_BY_MEMBER_PTR(obj_ptr,hstaticobjectext,obj_data));
    }
    operator hobject_managed_struct_t *()
    {
        return get_hobject_managed_struct_t();
    }
    operator hobject_base_t *()
    {
        return HOBJECT_BASE(get_hobject_managed_struct_t());
    }
    operator T *()
    {
        return (T *)((*get_hobject_managed_struct_t()).o_struct);
    }
    operator T &()
    {
        return *(T *)(*this);
    }
    T *operator->()
    {
        return (T *)(*this);
    }
    /*
     * 获取当前引用计数，0表示无任何引用（此时可以删除对象）
     */
    virtual size_t ref_count()
    {
        return ((hobject_base_t *)(*this))->ref_cnt;
    }
    /*
     * 引用计数加
     */
    virtual void  ref_inc()
    {
        hlockguard<hlock> m_lock(*this);
        ((hobject_base_t *)(*this))->ref_cnt++;
    }
    /*
     * 引用计数减
     */
    virtual void ref_dec()
    {
        hlockguard<hlock> m_lock(*this);
        if(ref_count()>0)
        {
            ((hobject_base_t *)(*this))->ref_cnt--;
        }
    }
};

class hstaticobject:public hstaticobjectbase
{
private:
    union
    {
        hobject_simple_ptr_t            simple_ptr;
        hobject_array_ptr_t             array_ptr;
        hobject_managed_ptr_t           managed_ptr;
        hobject_managed_array_ptr_t     managed_array_ptr;
        hobject_double_t                double_val;
        hobject_int8_t                  int8_val;
        hobject_uint8_t                 uint8_val;
        hobject_int16_t                 int16_val;
        hobject_uint16_t                uint16_val;
        hobject_int32_t                 int32_val;
        hobject_uint32_t                uint32_val;
        hobject_int64_t                 int64_val;
        hobject_uint64_t                uint64_val;
    } obj_data;
public:
    hstaticobject(void *ptr,uint16_t usr_type=0)
    {
        obj_data.simple_ptr=HOBJECT_SIMPLE_PTR_INITIALIZER;
        hobject_simple_ptr_init((hobject_simple_ptr_t *)&obj_data,usr_type,ptr);
    }
    hstaticobject(void *array_ptr,size_t array_size,uint16_t usr_type=0)
    {
        obj_data.array_ptr=HOBJECT_ARRAY_PTR_INITIALIZER;
        hobject_array_ptr_init((hobject_array_ptr_t *)&obj_data,usr_type,array_ptr,array_size);
    }
    hstaticobject(void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr),uint16_t usr_type=0)
    {
        obj_data.managed_ptr=HOBJECT_MANAGED_PTR_INITIALIZER;
        hobject_managed_ptr_init((hobject_managed_ptr_t *)&obj_data,usr_type,ptr,onfree);
    }
    hstaticobject(void *array_ptr,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr),uint16_t usr_type=0)
    {
        obj_data.managed_array_ptr=HOBJECT_MANAGED_ARRAY_PTR_INITIALIZER;
        hobject_managed_array_ptr_init((hobject_managed_array_ptr_t *)&obj_data,usr_type,array_ptr,array_size,onfree);
    }
    hstaticobject(double val,uint16_t usr_type=0)
    {
        obj_data.double_val=HOBJECT_DOUBLE_INITIALIZER;
        hobject_double_init((hobject_double_t *)&obj_data,usr_type,val);
    }
    hstaticobject(int8_t val,uint16_t usr_type=0)
    {
        obj_data.int8_val=HOBJECT_INT8_INITIALIZER;
        hobject_int8_init((hobject_int8_t *)&obj_data,usr_type,val);
    }
    hstaticobject(uint8_t val,uint16_t usr_type=0)
    {
        obj_data.uint8_val=HOBJECT_UINT8_INITIALIZER;
        hobject_uint8_init((hobject_uint8_t *)&obj_data,usr_type,val);
    }
    hstaticobject(int16_t val,uint16_t usr_type=0)
    {
        obj_data.int16_val=HOBJECT_INT16_INITIALIZER;
        hobject_int16_init((hobject_int16_t *)&obj_data,usr_type,val);
    }
    hstaticobject(uint16_t val,uint16_t usr_type=0)
    {
        obj_data.uint16_val=HOBJECT_UINT16_INITIALIZER;
        hobject_uint16_init((hobject_uint16_t *)&obj_data,usr_type,val);
    }
    hstaticobject(int32_t val,uint16_t usr_type=0)
    {
        obj_data.int32_val=HOBJECT_INT32_INITIALIZER;
        hobject_int32_init((hobject_int32_t *)&obj_data,usr_type,val);
    }
    hstaticobject(uint32_t val,uint16_t usr_type=0)
    {
        obj_data.uint32_val=HOBJECT_UINT32_INITIALIZER;
        hobject_uint32_init((hobject_uint32_t *)&obj_data,usr_type,val);
    }
    hstaticobject(int64_t val,uint16_t usr_type=0)
    {
        obj_data.int64_val=HOBJECT_INT64_INITIALIZER;
        hobject_int64_init((hobject_int64_t *)&obj_data,usr_type,val);
    }
    hstaticobject(uint64_t val,uint16_t usr_type=0)
    {
        obj_data.uint64_val=HOBJECT_UINT64_INITIALIZER;
        hobject_uint64_init((hobject_uint64_t *)&obj_data,usr_type,val);
    }
    hstaticobject(hstaticobject & oths)=delete;
    hstaticobject(hstaticobject && oths)=delete;
    hstaticobject &operator =(hstaticobject & oths)=delete;
    hstaticobject &operator =(hstaticobject && oths)=delete;
    virtual ~hstaticobject()
    {
        hobject_cleanup((hobject_base_t *)&obj_data);
    }
    /*
     * 通过hobject*指针获取this指针，用于onfree回调（若有）。
     */
    static const hstaticobject *get_this(const hobject_base_t *obj_ptr)
    {
        return static_cast<const hstaticobject *>(GET_STRUCT_PTR_BY_MEMBER_PTR(obj_ptr,hstaticobject,obj_data));
    }
    operator hobject_base_t *()
    {
        return HOBJECT_BASE(&obj_data);
    }
    operator hobject_simple_ptr_t *()
    {
        return hobject_simple_ptr((hobject_base_t *)&obj_data);
    }
    operator hobject_array_ptr_t *()
    {
        return hobject_array_ptr((hobject_base_t *)&obj_data);
    }
    operator hobject_managed_ptr_t *()
    {
        return hobject_managed_ptr((hobject_base_t *)&obj_data);
    }
    operator hobject_managed_array_ptr_t *()
    {
        return hobject_managed_array_ptr((hobject_base_t *)&obj_data);
    }
    uint16_t obj_type()
    {
        return ((hobject_base_t *)&obj_data)->type;
    }
    uint16_t obj_usr_type()
    {
        return ((hobject_base_t *)&obj_data)->usr_type;
    }
    operator hobject_double_t *()
    {
        return hobject_double((hobject_base_t *)&obj_data);
    }
    operator hobject_int8_t *()
    {
        return hobject_int8((hobject_base_t *)&obj_data);
    }
    operator hobject_uint8_t *()
    {
        return hobject_uint8((hobject_base_t *)&obj_data);
    }
    operator hobject_int16_t *()
    {
        return hobject_int16((hobject_base_t *)&obj_data);
    }
    operator hobject_uint16_t *()
    {
        return hobject_uint16((hobject_base_t *)&obj_data);
    }
    operator hobject_int32_t *()
    {
        return hobject_int32((hobject_base_t *)&obj_data);
    }
    operator hobject_uint32_t *()
    {
        return hobject_uint32((hobject_base_t *)&obj_data);
    }
    operator hobject_int64_t *()
    {
        return hobject_int64((hobject_base_t *)&obj_data);
    }
    operator hobject_uint64_t *()
    {
        return hobject_uint64((hobject_base_t *)&obj_data);
    }
    /*
     * 获取当前引用计数，0表示无任何引用（此时可以删除对象）
     */
    virtual size_t ref_count()
    {
        return ((hobject_base_t *)(*this))->ref_cnt;
    }
    /*
     * 引用计数加
     */
    virtual void  ref_inc()
    {
        hlockguard<hlock> m_lock(*this);
        ((hobject_base_t *)(*this))->ref_cnt++;
    }
    /*
     * 引用计数减
     */
    virtual void ref_dec()
    {
        hlockguard<hlock> m_lock(*this);
        if(ref_count()>0)
        {
            ((hobject_base_t *)(*this))->ref_cnt--;
        }
    }
};

/*
 * 空类，用于占位
 */
class hdymaicobjdata
{

};

template<class T=hdymaicobjdata>
class hdymaicobj:virtual public hlock
{
private:
    struct
    {
        uint8_t  ext_obj:1; //是否是ext_obj
    } flags;
    hstaticobjectbase *obj;
    /*
     * 释放obj
     */
    void release_obj()
    {
        hlockguard<hlock> lock(*this);
        if(obj!=NULL)
        {
            obj->ref_dec();
            if(obj->ref_count()==0)
            {
                delete obj;
                obj=NULL;
            }
        }
    }
    /*
     *  获取obj，一般由另外一个实例调用
     */
    hstaticobjectbase *acquire_obj()
    {
        hlockguard<hlock> lock(*this);
        if(obj!=NULL)
        {
            obj->ref_inc();
        }
        return obj;
    }
public:
    hdymaicobj():flags{0},obj{NULL}
    {

    }
    hdymaicobj(T & data,uint16_t usr_type=0,void (*onfree)(struct  __hobject_managed_struct *obj_ptr)=NULL):flags{0},obj{NULL}
    {
        obj=new hstaticobjectext<T>(data,usr_type,onfree);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=1;
    }
    hdymaicobj(T && data,uint16_t usr_type=0,void (*onfree)(struct  __hobject_managed_struct *obj_ptr)=NULL):flags{0},obj{NULL}
    {
        obj=new hstaticobjectext<T>((T &&)data,usr_type,onfree);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=1;
    }
    explicit hdymaicobj(void *ptr,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(ptr,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    hdymaicobj(void *array_ptr,size_t array_size,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(array_ptr,array_size,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr),uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(ptr,onfree,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    hdymaicobj(void *array_ptr,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr),uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(array_ptr,array_size,onfree,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(double val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(int8_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(uint8_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(int16_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(uint16_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(int32_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(uint32_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(int64_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    explicit hdymaicobj(uint64_t val,uint16_t usr_type=0):flags{0},obj{NULL}
    {
        obj=new hstaticobject(val,usr_type);
        if(obj!=NULL)
        {
            if(obj->ref_count()==0)
            {
                obj->ref_inc();
            }
        }
        flags.ext_obj=0;
    }
    hdymaicobj(const hdymaicobj & oths):flags{0},obj{NULL}
    {
        flags=oths.flags;
        release_obj();
        obj=oths.acquire_obj();
    }
    hdymaicobj(const hdymaicobj && oths):flags{0},obj{NULL}
    {
        flags=oths.flags;
        release_obj();
        obj=oths.acquire_obj();
    }
    hdymaicobj &operator =(const hdymaicobj & oths)
    {
        if(&oths == this)
        {
            return *this;
        }
        flags=oths.flags;
        release_obj();
        obj=oths.acquire_obj();
        return *this;
    }
    hdymaicobj &operator =(const hdymaicobj && oths)
    {
        if(&oths == this)
        {
            return *this;
        }
        flags=oths.flags;
        release_obj();
        obj=oths.acquire_obj();
        return *this;
    }
    virtual ~hdymaicobj()
    {
        release_obj();
    }

    operator hobject_base_t *()
    {
        if(obj!=NULL)
        {
            if(flags.ext_obj!=0)
            {
                return static_cast<hobject_base_t *>(*(hstaticobjectext<T> *)obj);
            }
            else
            {
                return static_cast<hobject_base_t *>(*(hstaticobject *)obj);
            }
        }
        return NULL;
    }
    uint16_t obj_type()
    {
        if(obj!=NULL)
        {
            if(flags.ext_obj!=0)
            {
                return (*(hstaticobjectext<T> *)obj).obj_type();
            }
            else
            {
                return (*(hstaticobject *)obj).obj_type();
            }
        }
        return 0;
    }
    uint16_t obj_usr_type()
    {
        if(obj!=NULL)
        {
            if(flags.ext_obj!=0)
            {
                return (*(hstaticobjectext<T> *)obj).obj_usr_type();
            }
            else
            {
                return (*(hstaticobject *)obj).obj_usr_type();
            }
        }
        return 0;
    }
    /*
     * 仅hstaticobjectext可使用此函数
     */
    operator T *()
    {
        if(flags.ext_obj==0)
        {
            return NULL;
        }
        return (T *)(*(hstaticobjectext<T> *)obj);
    }
    /*
     * 仅hstaticobjectext可使用此函数
     */
    operator T &()
    {
        return *(T *)(*this);
    }
    /*
     * 仅hstaticobjectext可使用此函数
     */
    T *operator->()
    {
        return (T *)(*this);
    }

    /*
     * 判断是否存储有数据
     */
    operator bool()
    {
        return obj!=NULL;
    }

    hstaticobjectext<T> *  get_hstaticobjectext()
    {
        if(flags.ext_obj!=0)
        {
            return (hstaticobjectext<T> *)obj;
        }
        return NULL;
    }

    hstaticobject *  get_hstaticobject()
    {
        if(flags.ext_obj==0)
        {
            return (hstaticobject *)obj;
        }
        return NULL;
    }
};


#endif // __cplusplus

#endif // __HOBJ_H__
