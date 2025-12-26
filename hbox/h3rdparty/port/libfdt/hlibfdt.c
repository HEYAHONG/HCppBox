/***************************************************************
 * Name:      hlibfdt.c
 * Purpose:   实现hlibfdt接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibfdt.h"
#include H3RDPARTY_LIBFDT_HEADER

bool hlibfdt_check(const void *fdt)
{
    if(fdt_check_header(fdt)==0 && fdt_check_full(fdt,fdt_totalsize(fdt))==0)
    {
        return true;
    }
    return false;
}

static bool hlibfdt_traverse_node_internal(const void *fdt,int offset,int depth,hlibfdt_traverse_on_node_t on_node,void *usr)
{
    const char *node_name=fdt_get_name(fdt,offset,NULL);
    if(node_name==NULL)
    {
        return false;
    }

    if(on_node!=NULL)
    {
        on_node(fdt,offset,node_name,depth,usr);
    }

    /*
     * 遍历子节点
     */
    {
        if(offset >= 0)
        {
            int child_offset=0;
            fdt_for_each_subnode(child_offset,fdt,offset)
            {
                if(!hlibfdt_traverse_node_internal(fdt,child_offset,depth+1,on_node,usr))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool hlibfdt_traverse_node(const void *fdt,hlibfdt_traverse_on_node_t on_node,void *usr)
{
    if(!hlibfdt_check(fdt))
    {
        return false;
    }

    return hlibfdt_traverse_node_internal(fdt,0,1,on_node,usr);
}

bool hlibfdt_traverse_node_property(const void *fdt,int node_offset,hlibfdt_traverse_on_node_property_t on_property,void *usr)
{
    bool ret=false;
    if(!hlibfdt_check(fdt))
    {
        return ret;
    }
    ret=true;

    {
        int prop_offset=0;
        fdt_for_each_property_offset(prop_offset,fdt,node_offset)
        {
            int prop_size=0;
            const char *prop_name=NULL;
            const uint8_t *prop_value = (const uint8_t *)fdt_getprop_by_offset(fdt, prop_offset, &prop_name, &prop_size);
            if(prop_name==NULL)
            {
                ret=false;
                break;
            }

            if(on_property!=NULL)
            {
                on_property(fdt,prop_offset,prop_name,prop_value,prop_size,usr);
            }
        }
    }

    return ret;
}

uint32_t hlibfdt_get_phandle(const void *fdt, int node_offset)
{
    return fdt_get_phandle(fdt, node_offset);
}


