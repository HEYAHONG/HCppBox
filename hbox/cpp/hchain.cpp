/***************************************************************
 * Name:      hchain.cpp
 * Purpose:   hchain
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hchain.h"

template<class lock,class cmemory>
void *hchain<lock,cmemory>::mem_alloc(size_t size,void *usr)
{
    if(usr!=NULL)
    {
        hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
        return obj.m_cmemory.malloc(size);
    }
}

template<class lock,class cmemory>
void hchain<lock,cmemory>::mem_free(void *ptr,void *usr)
{
    if(usr!=NULL)
    {
        hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
        return obj.m_cmemory.free(ptr);
    }
}

template<class lock,class cmemory>
void hchain<lock,cmemory>::mutex_lock(void *usr)
{
    if(usr!=NULL)
    {
        hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
        obj.m_lock.lock();
    }
}

template<class lock,class cmemory>
void hchain<lock,cmemory>::mutex_unlock(void *usr)
{
    if(usr!=NULL)
    {
        hchain<lock,cmemory> &obj=*(hchain<lock,cmemory> *)usr;
        obj.m_lock.unlock();
    }
}
