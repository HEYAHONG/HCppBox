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
#include "heventchain.h"

template<class lock=hlock,class cmemory=hcmemory>
class hchain
{
    lock &m_lock;
    cmemory &m_cmemory;
    static void *mem_alloc(size_t,void *);
    static void mem_free(void *,void *);
    static void mutex_lock(void *);
    static void mutex_unlock(void *);
    heventchain_t * m_chain;
    friend heventchain_t * heventchain_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *));
    bool is_vaild()
    {
        return m_chain!=NULL;
    }
    void create()
    {
        hlockguard<lock> s_lock(m_lock);
        if(!is_vaild())
        {
            m_chain=heventchain_new_with_memmang_and_lock(this,mem_alloc,mem_free,mutex_lock,mutex_unlock);
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
    hchain(lock &_lock,cmemory &_cmemory):m_lock(_lock),m_cmemory(_cmemory)
    {
    }
    //此用法仅用于默认C内存分配（全局内存分配），不推荐用户有自己定义的C内存分配时使用
    hchain(lock &_lock,cmemory &&_cmemory=cmemory()):m_lock(_lock),m_cmemory(_cmemory)
    {
    }
    //此用法仅用于默认锁(全局锁)及C内存分配（全局内存分配），不推荐用户有自己定义的锁及C内存分配时使用
    hchain(lock &&_lock=lock(),cmemory &&_cmemory=cmemory()):m_lock(_lock),m_cmemory(_cmemory)
    {
    }
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
