/***************************************************************
 * Name:      hdlt645_utils.c
 * Purpose:   实现hdlt645_utils接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_utils.h"

uint64_t hdlt645_uint64_to_bcd(uint64_t data)
{
    uint64_t bcd=0;
    /*
     *由于64位bcd码只能显示16位10进制数，因此最大支持16位十进制数
     */
    for(size_t i=1; i<=16; i++)
    {
        uint64_t d=((data)%((uint64_t)pow(10,(i))))/((uint64_t)pow(10,(i-1)));
        bcd+=((d)<<((i-1)*4));
    }
    return bcd;
}

uint64_t hdlt645_bcd_to_uint64(uint64_t bcd)
{
    uint64_t data=0;
    /*
     *由于64位bcd码只能显示16位10进制数，因此最大支持16位十进制数
     */
    for(size_t i=0; i<15; i++)
    {
        uint64_t d=(bcd%(0x01ULL<<((i+1)*4)))/(0x01ULL<<(i*4));
        data+=(d*pow(10,i));
    }
    {
        uint64_t d=bcd/(0x01ULL<<(15*4));
        data+= d*pow(10,15);
    }

    return data;
}

void hdlt645_bcd_le_set(uint8_t *bcd,size_t bcd_length,uint64_t data)
{
    if(bcd == NULL || bcd_length== 0)
    {
        return;
    }

    data = hdlt645_uint64_to_bcd(data);

    for(size_t i=0; i<bcd_length; i++)
    {
        bcd[i]=(data&0xFF);
        data >>= 8;
    }

}

uint64_t hdlt645_bcd_le_get(const uint8_t *bcd,size_t bcd_length)
{

    if(bcd == NULL || bcd_length== 0)
    {
        return 0;
    }

    uint64_t ret=0;

    for(size_t i=0; i<bcd_length; i++)
    {
        ret+=bcd[bcd_length-1-i];
        if(i!=(bcd_length-1))
        {
            ret <<= 8;
        }
    }

    ret = hdlt645_bcd_to_uint64(ret);

    return ret;
}

void hdlt645_bcd_le_signed_set(uint8_t *bcd,size_t bcd_length,int64_t data)
{
    if(bcd == NULL || bcd_length== 0)
    {
        return;
    }

    bool data_signed=false;

    if(data < 0)
    {
        data_signed=true;
        data = -data;
    }

    hdlt645_bcd_le_set(bcd,bcd_length,data);

    if(data_signed)
    {
        bcd[bcd_length-1] |= 0x80;
    }

}


int64_t hdlt645_bcd_le_signed_get(const uint8_t *bcd,size_t bcd_length)
{
    if(bcd == NULL || bcd_length== 0)
    {
        return 0;
    }

    int64_t ret=0;

    bool data_signed=false;

    for(size_t i=0; i<bcd_length; i++)
    {
        ret+=bcd[bcd_length-1-i];
        if(i == 0)
        {
            if((ret & 0x80)!=0)
            {
                data_signed=true;
            }

            ret &= (~(0x80));
        }
        if(i!=(bcd_length-1))
        {
            ret <<= 8;
        }
    }

    ret = hdlt645_bcd_to_uint64(ret);

    if(data_signed)
    {
        ret = -ret;
    }

    return ret;
}
