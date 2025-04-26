/***************************************************************
 * Name:      hloop.h
 * Purpose:   hloop
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HLOOP_H
#define HLOOP_H

#ifdef __cplusplus

#include "hcpprt.h"
#include "hevent.h"

template<class lock=hlock,class cmemory=hcmemory>
class hloop
{
    lock &m_lock;
    cmemory &m_cmemory;
    heventloop_t * m_loop;
    bool is_vaild()
    {
        return m_loop!=NULL;
    }
    void create()
    {
        hlockguard<lock> s_lock(m_lock);
        if(!is_vaild())
        {
            m_loop=heventloop_new_with_memmang_and_lock(this,
                    [](size_t size,void *usr) ->void *
            {
                if(usr!=NULL)
                {
                    hloop<lock,cmemory> &obj=*(hloop<lock,cmemory> *)usr;
                    return obj.m_cmemory.malloc(size);
                }
                return NULL;
            },[](void *ptr,void *usr) ->void
            {
                if(usr!=NULL)
                {
                    hloop<lock,cmemory> &obj=*(hloop<lock,cmemory> *)usr;
                    obj.m_cmemory.free(ptr);
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hloop<lock,cmemory> &obj=*(hloop<lock,cmemory> *)usr;
                    obj.m_lock.lock();
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hloop<lock,cmemory> &obj=*(hloop<lock,cmemory> *)usr;
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
            heventloop_free(m_loop);
            m_loop=NULL;
        }
    }
public:
    hloop(lock &_lock=hcpprt_global_lock(),cmemory &_cmemory=hcpprt_global_cmemory()):m_lock(_lock),m_cmemory(_cmemory),m_loop(NULL)
    {
    }

    //注意:拷贝构造不会复制事件循环,即不允许多个对象使用同一个heventloop
    hloop(hloop & oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_loop(NULL)
    {
    }
    hloop(hloop && oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_loop(NULL)
    {
    }

    //不允许赋值操作
    hloop &operator =(hloop & oths) =delete;
    hloop &operator =(hloop && oths) =delete;

    virtual ~hloop()
    {
        destory();
    }

    //heventloop接口包装
    bool has_events()
    {
        create();
        if(is_vaild())
        {
            return heventloop_has_events(m_loop);
        }
        return false;
    }

    void process_event()
    {
        if(has_events())
        {
            heventloop_process_event(m_loop);
        }
    }

    bool add_event(void (*event_process)(void *),void (*event_free)(void *)=NULL,void *event_usr=NULL)
    {
        create();
        if(is_vaild())
        {
            return heventloop_add_event(m_loop,event_usr,event_process,event_free);
        }
        return false;
    }

    bool add_event_ex1(void (*event_process)(void *,heventloop_t *),void (*event_free)(void *,heventloop_t *)=NULL,void *event_usr=NULL)
    {
        create();
        if(is_vaild())
        {
            return heventloop_add_event_ex1(m_loop,event_usr,event_process,event_free);
        }
        return false;
    }

    uint32_t get_events_number()
    {
        create();
        if(is_vaild())
        {
            return heventloop_get_events_number(m_loop);
        }
        return 0;
    }

    uint32_t get_max_events_number()
    {
        create();
        if(is_vaild())
        {
            return heventloop_get_max_events_number(m_loop);
        }
        return 0;
    }

    void set_max_events_number(uint32_t max_event_number=(uint32_t)-1)
    {
        create();
        if(is_vaild())
        {
            return heventloop_set_max_events_number(m_loop,max_event_number);
        }
    }
};

#endif // __cplusplus

#endif // HLOOP_H
