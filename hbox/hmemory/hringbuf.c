/***************************************************************
 * Name:      hringbuf.c
 * Purpose:   实现hringbuf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hringbuf.h"
#include "hdefaults.h"

struct hringbuf
{
    //用户参数
    void *usr;
    //锁,参数为用户参数。
    void (*mutex_lock)(void *);
    //解锁,参数为用户参数。
    void (*mutex_unlock)(void *);
    //缓冲区长度
    size_t buff_length;
    //数据起始
    size_t data_start;
    //数据结束
    size_t data_end;
    //数据
    uint8_t buff[0];
};

#ifndef HRINGBUF_ALIGNED_SIZE
#define HRINGBUF_ALIGNED_SIZE sizeof(void *)
#endif // HRINGBUF_ALIGNED_SIZE

hringbuf_t *hringbuf_get(uint8_t *buff,size_t length)
{
    if(buff == NULL || length == 0)
    {
        return NULL;
    }

    if( length < HRINGBUF_ALIGNED_SIZE)
    {
        //空间太小
        return NULL;
    }

    if((((uintptr_t)buff)% HRINGBUF_ALIGNED_SIZE)!=0)
    {
        //指针未对齐，某些平台上可能会出错,在此处进行内存对齐操作
        size_t size_to_align=((((uintptr_t)buff)/HRINGBUF_ALIGNED_SIZE)+1)* HRINGBUF_ALIGNED_SIZE-((uintptr_t)buff);
        buff+=size_to_align;
        length-=size_to_align;
    }

    if(length < (sizeof(hringbuf_t)+1))
    {
        return NULL;
    }

    size_t ringbuf_length=length-sizeof(hringbuf_t);
    hringbuf_t *ptr=(hringbuf_t *)buff;
    if(ptr->buff_length != ringbuf_length)
    {
        ptr->buff_length=ringbuf_length;
        ptr->data_start=0;
        ptr->data_end=0;
        hringbuf_set_lock(ptr,NULL,hdefaults_get_api_table()->mutex_lock,hdefaults_get_api_table()->mutex_unlock);
    }
    return ptr;
}

void hringbuf_set_lock(hringbuf_t * buff,void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    if(buff!=NULL)
    {
        buff->usr=usr;
        buff->mutex_lock=mutex_lock;
        buff->mutex_unlock=mutex_unlock;
    }
}

void * hringbuf_get_usr_ptr(hringbuf_t * buff)
{
    if(buff!=NULL)
    {
        return buff->usr;
    }
    return NULL;
}

size_t hringbuf_get_length(hringbuf_t * buff)
{
    if(buff == NULL)
    {
        return 0;
    }
    //加锁
    if(buff->mutex_lock!=NULL)
    {
        buff->mutex_lock(buff->usr);
    }

    size_t data_start=buff->data_start;
    size_t data_end=buff->data_end;
    if(data_end < data_start)
    {
        data_end+=buff->buff_length;
    }

    //解锁
    if(buff->mutex_unlock!=NULL)
    {
        buff->mutex_unlock(buff->usr);
    }

    return data_end-data_start;
}


size_t hringbuf_get_max_length(hringbuf_t * buff)
{
    if(buff!=NULL)
    {
        return buff->buff_length-1;
    }
    return 0;
}

size_t hringbuf_input(hringbuf_t * buff,const uint8_t *data,size_t data_length)
{
    if(buff == NULL || data ==NULL || data_length == 0)
    {
        return 0;
    }

    //加锁
    if(buff->mutex_lock!=NULL)
    {
        buff->mutex_lock(buff->usr);
    }

    size_t count=data_length;

    if(count > hringbuf_get_max_length(buff)-hringbuf_get_length(buff))
    {
        count =  hringbuf_get_max_length(buff)-hringbuf_get_length(buff);
    }

    for(size_t i=0 ; i< count; i++)
    {
        if(buff->data_end >= buff->buff_length)
        {
            buff->data_end-=buff->buff_length;
        }
        ((uint8_t *)buff->buff)[buff->data_end]=data[i];
        buff->data_end++;
    }

    //解锁
    if(buff->mutex_unlock!=NULL)
    {
        buff->mutex_unlock(buff->usr);
    }

    return count;
}

size_t hringbuf_output(hringbuf_t * buff,uint8_t *data,size_t data_length)
{
    if(buff == NULL || data ==NULL || data_length == 0)
    {
        return 0;
    }

    //加锁
    if(buff->mutex_lock!=NULL)
    {
        buff->mutex_lock(buff->usr);
    }

    size_t count=data_length;

    if(count > hringbuf_get_length(buff))
    {
        count =  hringbuf_get_length(buff);
    }

    for(size_t i=0 ; i< count; i++)
    {
        if(buff->data_start >= buff->buff_length)
        {
            buff->data_start-=buff->buff_length;
        }
        data[i]=((uint8_t *)buff->buff)[buff->data_start];
        buff->data_start++;
    }

    //解锁
    if(buff->mutex_unlock!=NULL)
    {
        buff->mutex_unlock(buff->usr);
    }

    return count;
}

size_t hringbuf_output_no_clear(hringbuf_t * buff,uint8_t *data,size_t data_length)
{
    if(buff == NULL || data ==NULL || data_length == 0)
    {
        return 0;
    }

    //加锁
    if(buff->mutex_lock!=NULL)
    {
        buff->mutex_lock(buff->usr);
    }

    size_t count=data_length;

    if(count > hringbuf_get_length(buff))
    {
        count =  hringbuf_get_length(buff);
    }

    size_t data_start=buff->data_start;

    for(size_t i=0 ; i< count; i++)
    {
        if(data_start >= buff->buff_length)
        {
            data_start-=buff->buff_length;
        }
        data[i]=((uint8_t *)buff->buff)[data_start];
        data_start++;
    }

    //解锁
    if(buff->mutex_unlock!=NULL)
    {
        buff->mutex_unlock(buff->usr);
    }

    return count;
}
