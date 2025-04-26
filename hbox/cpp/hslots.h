/***************************************************************
 * Name:      hslots.h
 * Purpose:   hslots
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HSLOTS_H
#define HSLOTS_H

#ifdef __cplusplus

#include "hcpprt.h"
#include "hevent.h"

template<class lock=hlock,class cmemory=hcmemory>
class hslots
{
    lock &m_lock;
    cmemory &m_cmemory;
    heventslots_t * m_slots;
    bool is_vaild()
    {
        return m_slots!=NULL;
    }
    void create()
    {
        hlockguard<lock> s_lock(m_lock);
        if(!is_vaild())
        {
            m_slots=heventslots_new_with_memmang_and_lock(this,
                    [](size_t size,void *usr) ->void *
            {
                if(usr!=NULL)
                {
                    hslots<lock,cmemory> &obj=*(hslots<lock,cmemory> *)usr;
                    return obj.m_cmemory.malloc(size);
                }
                return NULL;
            },[](void *ptr,void *usr) ->void
            {
                if(usr!=NULL)
                {
                    hslots<lock,cmemory> &obj=*(hslots<lock,cmemory> *)usr;
                    obj.m_cmemory.free(ptr);
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hslots<lock,cmemory> &obj=*(hslots<lock,cmemory> *)usr;
                    obj.m_lock.lock();
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hslots<lock,cmemory> &obj=*(hslots<lock,cmemory> *)usr;
                    obj.m_lock.unlock();
                }
            });
        }

    }
    void destory()
    {
        hlockguard<lock> s_lock(m_lock);
        if(is_vaild())
        {
            heventslots_free(m_slots);
            m_slots=NULL;
        }
    }
public:
    hslots(lock &_lock=hcpprt_global_lock(),cmemory &_cmemory=hcpprt_global_cmemory()):m_lock(_lock),m_cmemory(_cmemory),m_slots(NULL)
    {
    }

    //注意:拷贝构造不会复制槽,即不允许多个对象使用同一个heventslots
    hslots(hslots & oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_slots(NULL)
    {
    }
    hslots(hslots && oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_slots(NULL)
    {
    }

    //不允许赋值操作
    hslots &operator =(hslots & oths) =delete;
    hslots &operator =(hslots && oths) =delete;

    virtual ~hslots()
    {
        destory();
    }

    //heventslots接口包装
    void emit_signal(void *signal)
    {
        create();
        if(is_vaild())
        {
            heventslots_emit_signal(m_slots,signal);
        }
    }

    uint32_t register_slot(void (*slot)(void *,void *),void (*onfree)(void *)=NULL,void *slot_usr=NULL)
    {
        create();
        if(is_vaild())
        {
            return heventslots_register_slot(m_slots,slot_usr,slot,onfree);
        }
        return 0;
    }

    void unregister_slot(uint32_t id)
    {
        create();
        if(is_vaild())
        {
            return heventslots_unregister_slot(m_slots,id);
        }
    }

    void unregister_all_slot()
    {
        create();
        if(is_vaild())
        {
            return heventslots_unregister_all_slot(m_slots);
        }
    }

};

#endif // __cplusplus


#endif // HSLOTS_H
