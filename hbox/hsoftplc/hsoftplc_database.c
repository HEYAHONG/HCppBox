/***************************************************************
 * Name:      hsoftplc_database.c
 * Purpose:   实现hsoftplc_database接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftplc_database.h"
#include "stdlib.h"
#include "stdint.h"

hsoftplc_database_key_t hsoftplc_database_key_get_from_variable_name(const char *variable_name)
{
    hsoftplc_database_key_t ret=0;
    hsoftplc_variable_symbol_t variable_symbol;
    if(hsoftplc_parse_variable_symbol(&variable_symbol,variable_name)!=NULL)
    {
        uint32_t  addr0=0;
        if(variable_symbol.variable_address[0]!=NULL)
        {
            addr0=strtoul(variable_symbol.variable_address[0],NULL,10);
        }
        uint8_t   addr1=0;
        if(variable_symbol.variable_address[1]!=NULL)
        {
            addr1=strtoul(variable_symbol.variable_address[1],NULL,10);
        }
        uint8_t   addr2=0;
        if(variable_symbol.variable_address[2]!=NULL)
        {
            addr2=strtoul(variable_symbol.variable_address[2],NULL,10);
        }
        ret=((uint64_t)addr2)+(((uint64_t)addr1)<<8)+(((uint64_t)addr0)<<16)+(((uint64_t)(uint8_t)variable_symbol.variable_size)<<48)+(((uint64_t)(uint8_t)variable_symbol.variable_location)<<56);
    }
    return ret;
}

bool hsoftplc_database_get_value(hsoftplc_database_key_t key,hsoftplc_database_value_handle_t handle,hsoftplc_database_value_t *value)
{
    bool ret=false;
    if(key==0 || handle == NULL || value == NULL)
    {
        return ret;
    }
    /*
     * 通过键中包含的大小写入值
     */
    ret=true;
    switch(0xFF&(key>>48))
    {
    case 'X':
    case 'B':
    {
        (*value)=(*(uint8_t *)handle);
    }
    break;
    case 'W':
    {
        (*value)=(*(uint16_t *)handle);
    }
    break;
    case 'D':
    {
        (*value)=(*(uint32_t *)handle);
    }
    break;
    case 'L':
    {
        (*value)=(*(uint64_t *)handle);
    }
    break;
    default:
    {
        ret=false;
    }
    break;
    }
    return ret;
}

bool hsoftplc_database_set_value(hsoftplc_database_key_t key,hsoftplc_database_value_handle_t handle,hsoftplc_database_value_t value)
{
    bool ret=false;
    if(key==0 || handle == NULL)
    {
        return ret;
    }
    /*
     * 通过键中包含的大小读取值
     */
    ret=true;
    switch(0xFF&(key>>48))
    {
    case 'X':
    case 'B':
    {
        (*(uint8_t *)handle)=value;
    }
    break;
    case 'W':
    {
        (*(uint16_t *)handle)=value;
    }
    break;
    case 'D':
    {
        (*(uint32_t *)handle)=value;
    }
    break;
    case 'L':
    {
        (*(uint64_t *)handle)=value;
    }
    break;
    default:
    {
        ret=false;
    }
    break;
    }
    return ret;
}

bool hsoftplc_database_value_cache_init(hsoftplc_database_value_cache_t *cache,const char *variable_name,hsoftplc_database_value_handle_t handle)
{
    bool ret=false;
    if(cache==NULL || variable_name ==NULL || handle == NULL)
    {
        return ret;
    }
    hsoftplc_database_key_t key=hsoftplc_database_key_get_from_variable_name(variable_name);
    if(key == 0)
    {
        return ret;
    }
    cache->handle=handle;
    if(hsoftplc_database_get_value(key,handle,&cache->value1))
    {
        ret=true;
    }
    return ret;
}

bool hsoftplc_database_value_cache_update(hsoftplc_database_value_cache_t *cache,const char *variable_name,hsoftplc_database_value_cache_on_update_callback_t cb,void *usr)
{
    bool ret=false;
    if(cache==NULL || variable_name ==NULL || cache->handle == NULL)
    {
        return ret;
    }
    hsoftplc_database_key_t key=hsoftplc_database_key_get_from_variable_name(variable_name);
    if(key == 0)
    {
        return ret;
    }
    hsoftplc_database_value_t value1=cache->value1;
    if(hsoftplc_database_get_value(key,cache->handle,&cache->value1))
    {
        ret=true;
        if(cb!=NULL && value1!=cache->value1)
        {
            cb(cache,variable_name,usr);
        }
    }
    return ret;
}
