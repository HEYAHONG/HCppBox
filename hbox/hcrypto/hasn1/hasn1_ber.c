/***************************************************************
 * Name:      hasn1_ber.c
 * Purpose:   实现hasn1_ber接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hasn1_ber.h"

hasn1_ber_type_class_t hasn1_ber_type_class_get(const hasn1_ber_type_t *type)
{
    if(type!=NULL)
    {
        return (hasn1_ber_type_class_t)(type->type & (3 << 6));
    }
    return HASN1_BER_TYPE_CLASS_UNIVERSAL;
}

void hasn1_ber_type_class_set(hasn1_ber_type_t *type,const hasn1_ber_type_class_t type_class)
{
    if(type!=NULL)
    {
        type->type &= (~(3 <<6));
        type->type |= type_class;
    }
}


hasn1_ber_type_p_c_t hasn1_ber_type_p_c_get(const hasn1_ber_type_t *type)
{
    if(type!=NULL)
    {
        return (hasn1_ber_type_p_c_t)(type->type & (1 << 5));
    }
    return HASN1_BER_TYPE_PRIMITIVE;
}

void hasn1_ber_type_p_c_set(hasn1_ber_type_t *type,const hasn1_ber_type_p_c_t type_p_c)
{
    if(type!=NULL)
    {
        type->type &= (~(1 <<5));
        type->type |= type_p_c;
    }
}

uint64_t hasn1_ber_type_tag_get(const hasn1_ber_type_t *type)
{
    uint64_t ret=0;
    if(type!=NULL)
    {
        ret=(type->type&0x1F);
        if(ret==HASN1_BER_TYPE_TAG_FOLLOW_BYTES)
        {
            ret=0;
            for(size_t i=0; i<sizeof(type->tag); i++)
            {
                ret<<=7;
                ret+=(type->tag[i]&0x7f);
                if((type->tag[i]&0x80)==0)
                {
                    break;
                }
            }
        }
    }
    return ret;
}

void hasn1_ber_type_tag_set(hasn1_ber_type_t *type,uint64_t tag)
{
    if(type!=NULL)
    {
        if(tag <= 0x1F)
        {
            type->type &= (~(0x1f));
            type->type |= tag;
        }
        else
        {
            //type低五位全部置1
            type->type |= 0x1f;
            size_t clz=0;
            for(size_t i=0; i<8*sizeof(tag); i++)
            {
                if(((1ULL << (8*sizeof(tag)-1-i)) & tag )!=0)
                {
                    clz=i;
                    break;
                }
            }
            size_t bytes_need=(8*sizeof(tag)-clz)/8 + (((8*sizeof(tag)-clz)%8!=0)?1:0);
            if(bytes_need > sizeof(type->tag))
            {
                //不能超过结构体存储的上限
                bytes_need=sizeof(type->tag);
            }
            for(size_t i=0; i<bytes_need; i++)
            {
                type->tag[i]=(0x80 | (0x7F &(tag >> (((bytes_need-1-i)*7)))));
                if(i==(bytes_need-1))
                {
                    //最后一个字节的最高位为0
                    type->tag[i] &= 0x7f;
                }
            }
        }
    }
}

size_t hasn1_ber_type_get(hasn1_ber_type_t *type,const uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(data==NULL || data_length==0)
    {
        return ret;
    }
    uint8_t type_field=data[0];
    ret=1;
    if((type_field&0x1F)==HASN1_BER_TYPE_TAG_FOLLOW_BYTES)
    {
        //长度在后跟的字节中
        bool tag_ok=false;
        for(size_t i=1; i < data_length ; i++)
        {
            if((data[i] & 0x80)==0)
            {
                ret=(i+1);
                tag_ok=true;
                break;
            }
        }
        if(!tag_ok)
        {
            //tag过长，不能存储在hasn1_ber_type_t结构中
            ret=0;
        }
    }
    if(type!=NULL)
    {
        type->type=type_field;
        if(ret > 1)
        {
            memcpy(type->tag,&data[1],ret-1);
        }
    }
    return ret;
}

size_t hasn1_ber_type_set(const hasn1_ber_type_t *type,uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(type==NULL || data==NULL || data_length==0)
    {
        return ret;
    }

    data[ret++]=type->type;
    if((type->type&0x1F)==HASN1_BER_TYPE_TAG_FOLLOW_BYTES)
    {
        for(size_t i=0; i<sizeof(type->tag); i++)
        {
            if(i+1 < data_length)
            {
                data[ret++]=type->tag[i];
                if((type->tag[i]&0x80)==0)
                {
                    break;
                }
            }
            else
            {
                //写入失败
                return ret;
            }
        }
    }

    return ret;
}


size_t hasn1_ber_length_get( hasn1_ber_length_t *length,const uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(data==NULL && data_length ==0)
    {
        return ret;
    }
    {
        //跳过类型数据
        size_t type_len=hasn1_ber_type_get(NULL,data,data_length);
        if(type_len > 0)
        {
            data+=type_len;
            data_length-=type_len;
        }
        else
        {
            return ret;
        }
    }
    if(data[0] < 0x80 )
    {
        //长度小于127
        ret = 1;
        if(length!=NULL)
        {
            length->length=(data[0]&0x7f);
        }
    }
    if(data[0]==0x80)
    {
        //长度动态(在发送时不确定，且需要立即发送数据时使用)，值（V）以两个00结尾（不计入长度字节与值字节）
        ret=1;
        bool value_end_found=false;
        for(size_t i=0; i<(data_length-1); i++)
        {
            if(data[1+i]==0x00 && data[1+i+1]==0x00)
            {
                if(length!=NULL)
                {
                    length->length=i;
                }
                break;
            }
        }
        if(!value_end_found)
        {
            ret=0;
        }
    }
    if(data[0] > 0x80)
    {
        //长度值字节数在第一个字节的低7位给出
        size_t len=(data[0]&0x7f);
        ret=len+1;
        if(len > sizeof(length->length) || ret > data_length)
        {
            //长度所占空间过大
            ret=0;
        }
        if(length!=NULL)
        {
            length->length=0;
            for(size_t i=0; i<len; i++)
            {
                length->length <<= 8;
                length->length += data[1+i];
            }
        }
    }
    return ret;
}

size_t hasn1_ber_length_set(const hasn1_ber_length_t *length,uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(data == NULL || data_length==0)
    {
        return ret;
    }
    {
        //跳过类型数据
        size_t type_len=hasn1_ber_type_get(NULL,data,data_length);
        if(type_len > 0)
        {
            data+=type_len;
            data_length-=type_len;
        }
        else
        {
            return ret;
        }
    }


    if(length!=NULL)
    {
        if(length->length > 0x80)
        {
            size_t bytes_need=0;
            {
                size_t clz=0;
                for(size_t i=0; i<8*sizeof(length->length); i++)
                {
                    if(((1ULL << (8*sizeof(length->length)-1-i)) & length->length )!=0)
                    {
                        clz=i;
                        break;
                    }
                }
                bytes_need=(8*sizeof(length->length)-clz)/8 + (((8*sizeof(length->length)-clz)%8!=0)?1:0);
            }
            if(bytes_need + 1 > data_length )
            {
                data[0]=(0x80 & bytes_need);
                for(size_t i=0; i<bytes_need; i++)
                {
                    data[1+i]=(0xff &(length->length >> ((bytes_need-1-i)*8)));
                }
                ret=bytes_need+1;
            }
        }
        else
        {
            if(data_length >= 1)
            {
                data[0]=(length->length&0x7f);
                ret=1;
            }
        }
    }
    else
    {
        if(data_length >= 1)
        {
            //动态长度，在值末尾添两个00结尾
            data[0]=0x80;
            ret=1;
        }
    }

    return ret;
}

size_t hasn1_ber_value_get( hasn1_ber_value_t *value,const uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(data==NULL || data_length ==0)
    {
        return ret;
    }
    const uint8_t *   original_data=data;
    size_t      original_data_length=data_length;
    {
        //跳过类型数据
        size_t type_len=hasn1_ber_type_get(NULL,original_data,original_data_length);
        if(type_len > 0)
        {
            data+=type_len;
            data_length-=type_len;
        }
        else
        {
            return ret;
        }
    }
    hasn1_ber_length_t length;
    bool is_dynamic_length=(data[0]==0x80);
    {
        //跳过长度数据
        size_t length_len=hasn1_ber_length_get(&length,original_data,original_data_length);
        if(length_len > 0)
        {
            data+=length_len;
            data_length-=length_len;
        }
        else
        {
            return ret;
        }
    }
    if(data_length >= length.length)
    {
        ret=length.length;
        if(is_dynamic_length)
        {
            //末尾00计入字节数
            ret+=2;
        }
        if(value!=NULL)
        {
            value->value=data;
            value->length=length.length;
        }
    }
    return ret;
}

size_t hasn1_ber_value_set(const hasn1_ber_value_t *value,uint8_t *data,size_t data_length)
{
    size_t ret=0;
    if(data==NULL || data_length ==0 || value == NULL)
    {
        return ret;
    }
    uint8_t *   original_data=data;
    size_t      original_data_length=data_length;
    {
        //跳过类型数据
        size_t type_len=hasn1_ber_type_get(NULL,original_data,original_data_length);
        if(type_len > 0)
        {
            data+=type_len;
            data_length-=type_len;
        }
        else
        {
            return ret;
        }
    }
    bool is_dynamic_length=(data[0]==0x80);
    if(is_dynamic_length)
    {
        data+=1;
        data_length-=1;
        if(data_length >= (value->length+2))
        {
            memcpy(data,value->value,value->length);
            //末尾添加两个00字节
            memset(&data[value->length],0,2);
            ret=(value->length+2);
        }
    }
    else
    {
        hasn1_ber_length_t length;
        length.length=value->length;
        size_t length_len=hasn1_ber_length_set(&length,original_data,original_data_length);
        if(length_len > 0)
        {
            data+=length_len;
            data_length-=length_len;
            if(data_length >= value->length)
            {
                memcpy(data,value->value,value->length);
                ret=value->length;
            }
        }
    }

    return ret;
}

bool hasn1_ber_parse(hasn1_ber_parse_callback_t callback,void *usr,size_t depth,const uint8_t *data,size_t data_length)
{
    bool ret=false;
    if(callback == NULL || data == NULL || data_length == 0)
    {
        return ret;
    }
    depth++;
    ret=true;
    size_t index=0;
    while(data_length > 0)
    {
        size_t current_length=0;
        hasn1_ber_type_t type;
        current_length+=hasn1_ber_type_get(&type,data,data_length);
        hasn1_ber_length_t length;
        current_length+=hasn1_ber_length_get(&length,data,data_length);
        hasn1_ber_value_t value;
        current_length+=hasn1_ber_value_get(&value,data,data_length);
        if(current_length <= data_length)
        {
            callback(usr,depth,index,&type,&value);
            if(hasn1_ber_type_p_c_get(&type)!=HASN1_BER_TYPE_PRIMITIVE)
            {
                if(!hasn1_ber_parse(callback,usr,depth,value.value,value.length))
                {
                    ret=false;
                    break;
                }
            }
            data+=current_length;
            data_length-=current_length;
        }
        else
        {
            ret=false;
            break;
        }
        index++;
    }
    return ret;
}
