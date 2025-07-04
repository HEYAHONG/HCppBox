/***************************************************************
 * Name:      hsoftwarevirtualmemory.c
 * Purpose:   实现hsoftwarevirtualmemory接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftwarevirtualmemory.h"

static size_t hsoftwarevirtualmemory_default_read(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length)
{
    (void)map_item;
    if(data==NULL)
    {
        return 0;
    }
    for(size_t i=0; i<length; i++)
    {
        data[i]=(*((const volatile uint8_t *)(address+i)));
    }
    return length;
};

static size_t hsoftwarevirtualmemory_default_write(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length)
{
    (void)map_item;
    if(data==NULL)
    {
        return 0;
    }
    for(size_t i=0; i<length; i++)
    {
        (*((volatile uint8_t *)(address+i)))=data[i];
    }
    return length;
};


static const hsoftwarevirtualmemory_map_item_t hsoftwarevirtualmemory_default_map_table[]=
{
    {0,0,hsoftwarevirtualmemory_default_read,hsoftwarevirtualmemory_default_write,NULL}
};

const hsoftwarevirtualmemory_map_item_t * hsoftwarevirtualmemory_global_map_table=NULL;

size_t hsoftwarevirtualmemory_read(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,uint8_t *data,size_t length)
{
    size_t ret=0;
    if(length == 0)
    {
        return ret;
    }
    if(map_table==NULL)
    {
        if(hsoftwarevirtualmemory_global_map_table!=NULL)
        {
            map_table=hsoftwarevirtualmemory_global_map_table;
        }
        else
        {
            map_table=hsoftwarevirtualmemory_default_map_table;
        }
    }
    const hsoftwarevirtualmemory_map_item_t * map_table_start=map_table;
    while(true)
    {
        if(map_table->map_address == 0 && map_table->map_size==0)
        {
            if(map_table->read_callback!=NULL)
            {
                ret=map_table->read_callback(map_table,address,data,length);
            }
            break;
        }
        if(((uintptr_t)address) >= map_table->map_address && ((uintptr_t)address) < (map_table->map_address+map_table->map_size))
        {
            if(map_table->read_callback!=NULL)
            {
                size_t datalength=length;
                if(address+datalength > (map_table->map_address+map_table->map_size))
                {
                    datalength=(map_table->map_address+map_table->map_size)-address;
                }
                ret=map_table->read_callback(map_table,address,data,datalength);
                if(datalength < length)
                {
                    ret+=hsoftwarevirtualmemory_read(map_table_start,address+datalength,&data[datalength],length-datalength);
                }
            }
            break;
        }
        map_table++;
    }
    return ret;
}

size_t hsoftwarevirtualmemory_write(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,const uint8_t *data,size_t length)
{
    size_t ret=0;
    if(length == 0)
    {
        return ret;
    }
    if(map_table==NULL)
    {
        if(hsoftwarevirtualmemory_global_map_table!=NULL)
        {
            map_table=hsoftwarevirtualmemory_global_map_table;
        }
        else
        {
            map_table=hsoftwarevirtualmemory_default_map_table;
        }
    }
    const hsoftwarevirtualmemory_map_item_t * map_table_start=map_table;
    while(true)
    {
        if(map_table->map_address == 0 && map_table->map_size==0)
        {
            if(map_table->write_callback!=NULL)
            {
                ret=map_table->write_callback(map_table,address,data,length);
            }
            break;
        }
        if(((uintptr_t)address) >= map_table->map_address && ((uintptr_t)address) < (map_table->map_address+map_table->map_size))
        {
            if(map_table->write_callback!=NULL)
            {
                size_t datalength=length;
                if(address+datalength > (map_table->map_address+map_table->map_size))
                {
                    datalength=(map_table->map_address+map_table->map_size)-address;
                }
                ret=map_table->write_callback(map_table,address,data,datalength);
                if(datalength < length)
                {
                    ret+=hsoftwarevirtualmemory_write(map_table_start,address+datalength,&data[datalength],length-datalength);
                }
            }
            break;
        }
        map_table++;
    }
    return ret;
}

typedef struct hsoftwarevirtualmemory_ringbuf_internal hsoftwarevirtualmemory_ringbuf_internal_t;
struct hsoftwarevirtualmemory_ringbuf_internal
{
    hsoftwarevirtualmemory_ringbuf_t base;
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
};

hsoftwarevirtualmemory_ringbuf_t hsoftwarevirtualmemory_ringbuf_get(const hsoftwarevirtualmemory_map_item_t * map_table,uintptr_t  ringbuf_base,size_t ringbuf_size)
{
    hsoftwarevirtualmemory_ringbuf_t ret= {map_table,ringbuf_base,ringbuf_size};
    if(ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        /*
         * 将大小置0
         */
        ret.ringbuf_size=0;
        return ret;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(map_table,ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        /*
         * 将大小置0
         */
        ret.ringbuf_size=0;
        return ret;
    }
    if((ret.map_table !=rbuf_internal.base.map_table) || (ret.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (ret.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        /*
         * 环形缓冲区未初始化
         */
        memset(&rbuf_internal,0,sizeof(rbuf_internal));
        rbuf_internal.base=ret;
        rbuf_internal.buff_length=rbuf_internal.base.ringbuf_size-sizeof(rbuf_internal);
        rbuf_internal.data_start=0;
        rbuf_internal.data_end=0;
        if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_write(map_table,ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
        {
            /*
             * 将大小置0
             */
            ret.ringbuf_size=0;
            return ret;
        }
        /*
         * 设置为默认锁
         */
        hsoftwarevirtualmemory_ringbuf_set_lock(ret,NULL,hdefaults_get_api_table()->mutex_lock,hdefaults_get_api_table()->mutex_unlock);
    }

    return ret;
}

void hsoftwarevirtualmemory_ringbuf_set_lock(hsoftwarevirtualmemory_ringbuf_t buff,void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return;
    }
    rbuf_internal.usr=usr;
    rbuf_internal.mutex_lock=mutex_lock;
    rbuf_internal.mutex_unlock=mutex_unlock;
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_write(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return;
    }

}

void * hsoftwarevirtualmemory_ringbuf_get_usr_ptr(hsoftwarevirtualmemory_ringbuf_t buff)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return NULL;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return NULL;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return NULL;
    }
    return rbuf_internal.usr;
}

size_t hsoftwarevirtualmemory_ringbuf_get_length(hsoftwarevirtualmemory_ringbuf_t buff)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return 0;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return 0;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return 0;
    }

    //加锁
    if(rbuf_internal.mutex_lock!=NULL)
    {
        rbuf_internal.mutex_lock(rbuf_internal.usr);
    }

    size_t data_start=rbuf_internal.data_start;
    size_t data_end=rbuf_internal.data_end;
    if(data_end < data_start)
    {
        data_end+=rbuf_internal.buff_length;
    }

    //解锁
    if(rbuf_internal.mutex_unlock!=NULL)
    {
        rbuf_internal.mutex_unlock(rbuf_internal.usr);
    }

    return data_end-data_start;
}

size_t hsoftwarevirtualmemory_ringbuf_get_max_length(hsoftwarevirtualmemory_ringbuf_t buff)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return 0;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return 0;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return 0;
    }

    return rbuf_internal.buff_length-1;
}

size_t hsoftwarevirtualmemory_ringbuf_input(hsoftwarevirtualmemory_ringbuf_t buff,const uint8_t *data,size_t data_length)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return 0;
    }
    if(data==NULL || data_length==0)
    {
        return 0;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return 0;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return 0;
    }

    //加锁
    if(rbuf_internal.mutex_lock!=NULL)
    {
        rbuf_internal.mutex_lock(rbuf_internal.usr);
    }

    size_t count=data_length;

    if(count > hsoftwarevirtualmemory_ringbuf_get_max_length(buff)-hsoftwarevirtualmemory_ringbuf_get_length(buff))
    {
        count =  hsoftwarevirtualmemory_ringbuf_get_max_length(buff)-hsoftwarevirtualmemory_ringbuf_get_length(buff);
    }

    size_t ret=0;

    {
        /*
         * 第一次写入
         */
        size_t bytes_to_write=count;
        if(bytes_to_write > rbuf_internal.buff_length-rbuf_internal.data_end)
        {
            bytes_to_write=rbuf_internal.buff_length-rbuf_internal.data_end;
        }
        ret+=hsoftwarevirtualmemory_write(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_end,(uint8_t *)data,bytes_to_write);
        rbuf_internal.data_end+=ret;
        data+=ret;
        count-=ret;
    }

    if(rbuf_internal.data_end >= rbuf_internal.buff_length)
    {
        rbuf_internal.data_end-=rbuf_internal.buff_length;
    }

    if(ret!=0 && count > 0)
    {
        /*
         * 第二次写入
         */
        size_t bytes_to_write=count;
        if(bytes_to_write > rbuf_internal.buff_length-rbuf_internal.data_end)
        {
            bytes_to_write=rbuf_internal.buff_length-rbuf_internal.data_end;
        }
        ret+=hsoftwarevirtualmemory_write(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_end,(uint8_t *)data,bytes_to_write);
        rbuf_internal.data_end+=ret;
        data+=ret;
        count-=ret;
    }


    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_write(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        //更改缓冲区失败，返回0
        ret=0;
    }

    //解锁
    if(rbuf_internal.mutex_unlock!=NULL)
    {
        rbuf_internal.mutex_unlock(rbuf_internal.usr);
    }

    return ret;

}

size_t hsoftwarevirtualmemory_ringbuf_output(hsoftwarevirtualmemory_ringbuf_t buff,uint8_t *data,size_t data_length)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return 0;
    }
    if(data==NULL || data_length==0)
    {
        return 0;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return 0;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return 0;
    }

    //加锁
    if(rbuf_internal.mutex_lock!=NULL)
    {
        rbuf_internal.mutex_lock(rbuf_internal.usr);
    }

    size_t count=data_length;

    if(count > hsoftwarevirtualmemory_ringbuf_get_length(buff))
    {
        count =  hsoftwarevirtualmemory_ringbuf_get_length(buff);
    }

    size_t ret=0;

    {
        /*
         * 第一次读取
         */
        size_t bytes_to_read=count;
        if(bytes_to_read > rbuf_internal.buff_length-rbuf_internal.data_start)
        {
            bytes_to_read=rbuf_internal.buff_length-rbuf_internal.data_start;
        }
        ret+=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_start,(uint8_t *)data,bytes_to_read);
        rbuf_internal.data_start+=ret;
        data+=ret;
        count-=ret;
    }

    if(rbuf_internal.data_start >= rbuf_internal.buff_length)
    {
        rbuf_internal.data_start-=rbuf_internal.buff_length;
    }

    if(ret!=0 && count > 0)
    {
        /*
         * 第二次读取
         */
        size_t bytes_to_read=count;
        if(bytes_to_read > rbuf_internal.buff_length-rbuf_internal.data_start)
        {
            bytes_to_read=rbuf_internal.buff_length-rbuf_internal.data_start;
        }
        ret+=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_start,(uint8_t *)data,bytes_to_read);
        rbuf_internal.data_start+=ret;
        data+=ret;
        count-=ret;
    }


    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_write(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        //更改缓冲区失败，返回0
        ret=0;
    }

    //解锁
    if(rbuf_internal.mutex_unlock!=NULL)
    {
        rbuf_internal.mutex_unlock(rbuf_internal.usr);
    }

    return ret;
}

size_t hsoftwarevirtualmemory_ringbuf_output_no_clear(hsoftwarevirtualmemory_ringbuf_t buff,uint8_t *data,size_t data_length)
{
    if(buff.ringbuf_size < sizeof(hsoftwarevirtualmemory_ringbuf_internal_t))
    {
        return 0;
    }
    if(data==NULL || data_length==0)
    {
        return 0;
    }
    hsoftwarevirtualmemory_ringbuf_internal_t rbuf_internal= {0};
    if(sizeof(rbuf_internal)!=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base,(uint8_t *)&rbuf_internal,sizeof(rbuf_internal)))
    {
        return 0;
    }
    if((buff.map_table !=rbuf_internal.base.map_table) || (buff.ringbuf_base !=rbuf_internal.base.ringbuf_base) ||  (buff.ringbuf_size !=rbuf_internal.base.ringbuf_size))
    {
        return 0;
    }

    //加锁
    if(rbuf_internal.mutex_lock!=NULL)
    {
        rbuf_internal.mutex_lock(rbuf_internal.usr);
    }

    size_t count=data_length;

    if(count > hsoftwarevirtualmemory_ringbuf_get_length(buff))
    {
        count =  hsoftwarevirtualmemory_ringbuf_get_length(buff);
    }

    size_t ret=0;

    {
        /*
         * 第一次读取
         */
        size_t bytes_to_read=count;
        if(bytes_to_read > rbuf_internal.buff_length-rbuf_internal.data_start)
        {
            bytes_to_read=rbuf_internal.buff_length-rbuf_internal.data_start;
        }
        ret+=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_start,(uint8_t *)data,bytes_to_read);
        rbuf_internal.data_start+=ret;
        data+=ret;
        count-=ret;
    }

    if(rbuf_internal.data_start >= rbuf_internal.buff_length)
    {
        rbuf_internal.data_start-=rbuf_internal.buff_length;
    }

    if(ret!=0 && count > 0)
    {
        /*
         * 第二次读取
         */
        size_t bytes_to_read=count;
        if(bytes_to_read > rbuf_internal.buff_length-rbuf_internal.data_start)
        {
            bytes_to_read=rbuf_internal.buff_length-rbuf_internal.data_start;
        }
        ret+=hsoftwarevirtualmemory_read(buff.map_table,buff.ringbuf_base+sizeof(rbuf_internal)+rbuf_internal.data_start,(uint8_t *)data,bytes_to_read);
        rbuf_internal.data_start+=ret;
        data+=ret;
        count-=ret;
    }


    //解锁
    if(rbuf_internal.mutex_unlock!=NULL)
    {
        rbuf_internal.mutex_unlock(rbuf_internal.usr);
    }

    return ret;
}
