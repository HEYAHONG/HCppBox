/***************************************************************
 * Name:      hpkcs1.c
 * Purpose:   实现hpkcs1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hpkcs1.h"

#ifndef HPKCS1_RAND
#define HPKCS1_RAND hpkcs1_rand
#include "hdefaults.h"
static int hpkcs1_rand(void)
{
    int ret=0;
    hgetrandom(&ret,sizeof(ret),0);
    return ret;
}
#endif // HPKCS1_RAND

size_t hpkcs1_data_block_max_data_length(size_t data_block_length)
{
    if(data_block_length <= (3+HPKCS1_PS_MIN_LENGTH))
    {
        return 0;
    }
    return data_block_length- (3+HPKCS1_PS_MIN_LENGTH);
}

bool hpkcs1_padding(uint8_t *data_block,size_t data_block_length,const uint8_t *data,size_t data_length,uint8_t bt_type)
{
    bool ret=false;
    if(data_block==NULL || data==NULL || data_length==0 || data_length > hpkcs1_data_block_max_data_length(data_block_length))
    {
        return ret;
    }
    ret=true;
    data_block[0]=0;
    data_block[1]=bt_type;
    size_t ps_length=data_block_length-3-data_length;
    switch(bt_type)
    {
    case HPKCS1_BT_TYPE_0:
    {
        memset(&data_block[2],0,ps_length);
        if(data[0]==0)
        {
            /*
             * 第一个字节为0,填充将失败
             */
            ret=false;
            break;
        }
    }
    break;
    case HPKCS1_BT_TYPE_1:
    {
        for(size_t i=0;i<ps_length;i++)
        {
            data_block[2+i]=0xFF;
        }
    }
    break;
    case HPKCS1_BT_TYPE_2:
    default:
    {
        for(size_t i=0;i<ps_length;i++)
        {
            uint8_t temp=0;
            while(0==(temp=(uint8_t)HPKCS1_RAND()));
            data_block[2+i]=temp;
        }
    }
    break;
    }
    data_block[2+ps_length]=0;
    memcpy(&data_block[3+ps_length],data,data_length);

    return ret;
}

bool hpkcs1_check_padding(const uint8_t *data_block,size_t data_block_length,const uint8_t **data_ptr,size_t *length_ptr)
{
    bool ret=false;
    if(data_block==NULL || hpkcs1_data_block_max_data_length(data_block_length)==0 )
    {
        return ret;
    }

    if(data_block[0]!=0)
    {
        return ret;
    }

    if(!(data_block[1]==HPKCS1_BT_TYPE_0 || data_block[1]==HPKCS1_BT_TYPE_1 || data_block[1]==HPKCS1_BT_TYPE_2))
    {
        return ret;
    }

    switch(data_block[1])
    {
    case HPKCS1_BT_TYPE_0:
    {
        for(size_t i=2;i<data_block_length;i++)
        {
            if(data_block[i]!=0)
            {
                if(data_ptr!=NULL)
                {
                    (*data_ptr)=&data_block[i];
                }
                if(length_ptr!=NULL)
                {
                    (*length_ptr)=data_block_length-i;
                }
                ret=true;
                break;
            }
        }
    }
    break;
    case HPKCS1_BT_TYPE_1:
    case HPKCS1_BT_TYPE_2:
    default:
    {
        for(size_t i=2;i<data_block_length;i++)
        {
            if(i==data_block_length-1)
            {
                break;
            }

            if(data_block[i]==0)
            {
                /*
                 * 找到0x00作为填充内容的结束
                 */
                i++;
                if(data_ptr!=NULL)
                {
                    (*data_ptr)=&data_block[i];
                }
                if(length_ptr!=NULL)
                {
                    (*length_ptr)=data_block_length-i;
                }
                ret=true;
                break;
            }
        }
    }
    break;
    }

    return ret;
}
