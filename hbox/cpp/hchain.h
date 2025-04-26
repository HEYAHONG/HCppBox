/***************************************************************
 * Name:      hchain.h
 * Purpose:   hchain
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HCHAIN_H
#define HCHAIN_H

#ifdef __cplusplus

#include "hcpprt.h"
#include "hevent.h"


template<class lock=hlock,class cmemory=hcmemory>
class hchain
{
    lock &m_lock;
    cmemory &m_cmemory;
    heventchain_t * m_chain;
    bool is_vaild()
    {
        return m_chain!=NULL;
    }
    void create()
    {
        hlockguard<lock> s_lock(m_lock);
        if(!is_vaild())
        {
            m_chain=heventchain_new_with_memmang_and_lock(this,
                    [](size_t size,void *usr) ->void *
            {
                if(usr!=NULL)
                {
                    hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
                    return obj.m_cmemory.malloc(size);
                }
                return NULL;
            },[](void *ptr,void *usr) ->void
            {
                if(usr!=NULL)
                {
                    hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
                    obj.m_cmemory.free(ptr);
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
                    obj.m_lock.lock();
                }
            },[](void *usr)->void
            {
                if(usr!=NULL)
                {
                    hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
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
            heventchain_free(m_chain);
            m_chain=NULL;
        }
    }
public:
    hchain(lock &_lock=hcpprt_global_lock(),cmemory &_cmemory=hcpprt_global_cmemory()):m_lock(_lock),m_cmemory(_cmemory),m_chain(NULL)
    {
    }

    //注意:拷贝构造不会复制已安装的钩子,即不允许多个对象使用同一个heventchain
    hchain(hchain & oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_chain(NULL)
    {
    }
    hchain(hchain && oths):m_lock(oths.m_lock),m_cmemory(oths.m_cmemory),m_chain(NULL)
    {
    }

    //不允许赋值操作
    hchain &operator =(hchain & oths) =delete;
    hchain &operator =(hchain && oths) =delete;

    virtual ~hchain()
    {
        destory();
    }

    //heventchain接口包装
    bool start(void *parameter)
    {
        create();
        if(is_vaild())
        {
            return heventchain_start(m_chain,parameter);
        }
        return false;
    }

    uint32_t install_hook(bool (*hook)(void *,void *),void (*onfree)(void *)=NULL,uint32_t pro=0,void *hook_usr=NULL)
    {
        create();
        if(is_vaild())
        {
            return heventchain_install_hook(m_chain,pro,hook_usr,hook,onfree);
        }
        return 0;
    }

    void uninstall_hook(uint32_t id)
    {
        create();
        if(is_vaild())
        {
            heventchain_uninstall_hook(m_chain,id);
        }
    }

    void uninstall_all_hook()
    {
        create();
        if(is_vaild())
        {
            heventchain_uninstall_all_hook(m_chain);
        }
    }

};

#endif // __cplusplus

#endif // HCHAIN_H
