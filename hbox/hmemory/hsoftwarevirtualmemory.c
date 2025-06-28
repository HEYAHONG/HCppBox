/***************************************************************
 * Name:      hsoftwarevirtualmemory.c
 * Purpose:   实现hsoftwarevirtualmemory接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftwarevirtualmemory.h"

size_t hsoftwarevirtualmemory_read(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,uint8_t *data,size_t length)
{
    size_t ret=0;
    if(map_table==NULL || length == 0)
    {
        return ret;
    }

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
        if(((uintptr_t)data) >= map_table->map_address && ((uintptr_t)data) < (map_table->map_address+map_table->map_size))
        {
            if(map_table->read_callback!=NULL)
            {
                ret=map_table->read_callback(map_table,address,data,length);
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
    if(map_table==NULL || length == 0)
    {
        return ret;
    }

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
        if(((uintptr_t)data) >= map_table->map_address && ((uintptr_t)data) < (map_table->map_address+map_table->map_size))
        {
            if(map_table->write_callback!=NULL)
            {
                ret=map_table->write_callback(map_table,address,data,length);
            }
            break;
        }
        map_table++;
    }
    return ret;
}
