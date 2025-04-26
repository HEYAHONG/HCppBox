/***************************************************************
 * Name:      hobject.c
 * Purpose:   实现hobject接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hobject.h"

void hobject_cleanup(hobject_base_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->type==HOBJECT_BASE_TYPE_MANAGED_PTR)
        {
            hobject_managed_ptr_cleanup((hobject_managed_ptr_t *)(void*)obj_ptr);
        }
        if(obj_ptr->type==HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR)
        {
            hobject_managed_array_ptr_cleanup((hobject_managed_array_ptr_t *)(void *)obj_ptr);
        }
        if(obj_ptr->type==HOBJECT_BASE_TYPE_MANAGED_STRUCT)
        {
            hobject_managed_struct_cleanup((hobject_managed_struct_t *)(void *)obj_ptr);
        }
    }
}

void hobject_simple_ptr_init(hobject_simple_ptr_t *obj_ptr,uint16_t usr_type,void *ptr)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_SIMPLE_PTR;
        base->usr_type=usr_type;
        obj_ptr->ptr=ptr;
    }
}

hobject_base_t * hobject_simple_ptr_base(hobject_simple_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_simple_ptr_t * hobject_simple_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_SIMPLE_PTR)
        {
            return (hobject_simple_ptr_t *)(void*)base;
        }
    }
    return NULL;
}

void hobject_array_ptr_init(hobject_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_ARRAY_PTR;
        base->usr_type=usr_type;
        obj_ptr->array_header=array_header;
        obj_ptr->array_size=array_size;
    }
}

hobject_base_t *hobject_array_ptr_base(hobject_array_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_array_ptr_t *hobject_array_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_ARRAY_PTR)
        {
            return (hobject_array_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_managed_ptr_init(hobject_managed_ptr_t *obj_ptr,uint16_t usr_type,void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr))
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_MANAGED_PTR;
        base->usr_type=usr_type;
        obj_ptr->ptr=ptr;
        obj_ptr->onfree=onfree;
    }
}

void hobject_managed_ptr_cleanup(hobject_managed_ptr_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->onfree!=NULL)
        {
            obj_ptr->onfree(obj_ptr);
        }
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INVALID;
    }
}

hobject_base_t *hobject_managed_ptr_base(hobject_managed_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_managed_ptr_t *hobject_managed_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_MANAGED_PTR)
        {
            return (hobject_managed_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_managed_array_ptr_init(hobject_managed_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr))
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR;
        base->usr_type=usr_type;
        obj_ptr->array_header=array_header;
        obj_ptr->array_size=array_size;
        obj_ptr->onfree=onfree;
    }
}

void hobject_managed_array_ptr_cleanup(hobject_managed_array_ptr_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->onfree!=NULL)
        {
            obj_ptr->onfree(obj_ptr);
        }
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INVALID;
    }
}

hobject_base_t *hobject_managed_array_ptr_base(hobject_managed_array_ptr_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_managed_array_ptr_t *hobject_managed_array_ptr(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR)
        {
            return (hobject_managed_array_ptr_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_double_init(hobject_double_t *obj_ptr,uint16_t usr_type,double data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_DOUBLE;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_double_base(hobject_double_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_double_t * hobject_double(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_DOUBLE)
        {
            return (hobject_double_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_int8_init(hobject_int8_t *obj_ptr,uint16_t usr_type,int8_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INT8;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_int8_base(hobject_int8_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_int8_t * hobject_int8(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_INT8)
        {
            return (hobject_int8_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_uint8_init(hobject_uint8_t *obj_ptr,uint16_t usr_type,uint8_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_UINT8;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_uint8_base(hobject_uint8_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_uint8_t * hobject_uint8(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_UINT8)
        {
            return (hobject_uint8_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_int16_init(hobject_int16_t *obj_ptr,uint16_t usr_type,int16_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INT16;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_int16_base(hobject_int16_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_int16_t * hobject_int16(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_INT16)
        {
            return (hobject_int16_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_uint16_init(hobject_uint16_t *obj_ptr,uint16_t usr_type,uint16_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_UINT16;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_uint16_base(hobject_uint16_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_uint16_t * hobject_uint16(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_UINT16)
        {
            return (hobject_uint16_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_int32_init(hobject_int32_t *obj_ptr,uint16_t usr_type,int32_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INT32;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_int32_base(hobject_int32_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_int32_t * hobject_int32(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_INT32)
        {
            return (hobject_int32_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_uint32_init(hobject_uint32_t *obj_ptr,uint16_t usr_type,uint32_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_UINT32;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_uint32_base(hobject_uint32_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_uint32_t * hobject_uint32(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_UINT32)
        {
            return (hobject_uint32_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_int64_init(hobject_int64_t *obj_ptr,uint16_t usr_type,int64_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INT64;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_int64_base(hobject_int64_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_int64_t * hobject_int64(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_INT64)
        {
            return (hobject_int64_t *)(void *)base;
        }
    }
    return NULL;
}

void hobject_uint64_init(hobject_uint64_t *obj_ptr,uint16_t usr_type,uint64_t data)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_UINT64;
        base->usr_type=usr_type;
        obj_ptr->data=data;
    }
}

hobject_base_t * hobject_uint64_base(hobject_uint64_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_uint64_t * hobject_uint64(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_UINT64)
        {
            return (hobject_uint64_t *)(void *)base;
        }
    }
    return NULL;
}

size_t hobject_struct_get_size(size_t struct_size)
{
    return struct_size+sizeof(hobject_struct_t);
}

void * hobject_struct_get_struct_ptr(hobject_struct_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        return (void *)obj_ptr->o_struct;
    }
    return NULL;
}

void hobject_struct_init(hobject_struct_t *obj_ptr,uint16_t usr_type)
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_STRUCT;
        base->usr_type=usr_type;
    }
}

hobject_base_t * hobject_struct_base(hobject_struct_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_struct_t * hobject_struct(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_STRUCT)
        {
            return (hobject_struct_t *)(void *)base;
        }
    }
    return NULL;
}

size_t hobject_managed_struct_get_size(size_t struct_size)
{
    return struct_size+sizeof(hobject_managed_struct_t);
}

void * hobject_managed_struct_get_struct_ptr(hobject_managed_struct_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        return (void *)obj_ptr->o_struct;
    }
    return NULL;
}


void hobject_managed_struct_cleanup(hobject_managed_struct_t *obj_ptr)
{
    if(obj_ptr!=NULL)
    {
        if(obj_ptr->onfree!=NULL)
        {
            obj_ptr->onfree(obj_ptr);
        }
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_INVALID;
    }
}

void hobject_managed_struct_init(hobject_managed_struct_t *obj_ptr,uint16_t usr_type,void (*onfree)(hobject_managed_struct_t *obj_ptr))
{
    if(obj_ptr!=NULL)
    {
        hobject_base_t *base=HOBJECT_BASE(obj_ptr);
        base->type=HOBJECT_BASE_TYPE_MANAGED_STRUCT;
        base->usr_type=usr_type;
        obj_ptr->onfree=onfree;
    }
}

hobject_base_t * hobject_managed_struct_base(hobject_managed_struct_t *obj_ptr)
{
    return HOBJECT_BASE(obj_ptr);
}

hobject_managed_struct_t * hobject_managed_struct(hobject_base_t *base)
{
    if(base!=NULL)
    {
        if(base->type==HOBJECT_BASE_TYPE_MANAGED_STRUCT)
        {
            return (hobject_managed_struct_t *)(void *)base;
        }
    }
    return NULL;
}
