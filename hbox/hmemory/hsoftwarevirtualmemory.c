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

size_t hsoftwarevirtualmemory_read(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,uint8_t *data,size_t length)
{
    size_t ret=0;
    if(length == 0)
    {
        return ret;
    }
    if(map_table==NULL)
    {
        map_table=hsoftwarevirtualmemory_default_map_table;
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
        map_table=hsoftwarevirtualmemory_default_map_table;
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
