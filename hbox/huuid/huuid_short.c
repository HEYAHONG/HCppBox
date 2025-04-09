/***************************************************************
 * Name:      huuid_short.c
 * Purpose:   实现huuid_short接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "huuid_short.h"
#include "string.h"

bool huuid_is_uuid_short32(const huuid_t in,const huuid_t base)
{
    if(in==NULL || base == NULL)
    {
        return false;
    }
    huuid_t in1;
    memcpy(in1,in,sizeof(in1));
    memset(in1,0,sizeof(huuid_short32_t));
    huuid_t base1;
    memcpy(base1,base,sizeof(base1));
    memset(base1,0,sizeof(huuid_short32_t));
    return huuid_compare(in1,base1)==0;
}

huuid_short32_t huuid_uuid_short32_get(const huuid_t in)
{
    huuid_short32_t ret=0;
    if(in!=NULL)
    {
        for(size_t i=0; i<sizeof(ret); i++)
        {
            ret<<=8;
            ret+=in[i];
        }
    }
    return ret;
}

void huuid_uuid_short32_set(huuid_t out,const huuid_short32_t short_uuid,const huuid_t base)
{
    if(out==NULL || base==NULL)
    {
        return;
    }

    memcpy(out,base,sizeof(huuid_t));

    for(size_t i=0; i<sizeof(short_uuid); i++)
    {
        out[i]=((short_uuid >> (8*(sizeof(short_uuid)-1-i)))&0xFF);
    }
}

bool huuid_is_uuid_short16(const huuid_t in,const huuid_t base)
{
    if(in==NULL || base == NULL)
    {
        return false;
    }
    huuid_t in1;
    memcpy(in1,in,sizeof(in1));
    memset(&in1[sizeof(huuid_short32_t)-sizeof(huuid_short16_t)],0,sizeof(huuid_short16_t));
    huuid_t base1;
    memcpy(base1,base,sizeof(base1));
    memset(&base1[sizeof(huuid_short32_t)-sizeof(huuid_short16_t)],0,sizeof(huuid_short16_t));
    return huuid_compare(in1,base1)==0;
}

huuid_short16_t huuid_uuid_short16_get(const huuid_t in)
{
    huuid_short32_t ret=0;
    if(in!=NULL)
    {
        for(size_t i=0; i<sizeof(ret); i++)
        {
            ret<<=8;
            ret+=in[i];
        }
    }
    return (huuid_short16_t)ret;
}

void huuid_uuid_short16_set(huuid_t out,const huuid_short16_t short_uuid,const huuid_t base)
{
    if(out==NULL || base==NULL)
    {
        return;
    }

    memcpy(out,base,sizeof(huuid_t));

    for(size_t i=0; i<sizeof(short_uuid); i++)
    {
        out[i+sizeof(huuid_short32_t)-sizeof(huuid_short16_t)]=((short_uuid >> (8*(sizeof(short_uuid)-1-i)))&0xFF);
    }
}

HUUID_DEFINE_GLOBAL(huuid_ble_base128_uuid,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb);

bool huuid_is_uuid_ble(const huuid_t in)
{
    return huuid_is_uuid_short32(in,huuid_ble_base128_uuid);
}

huuid_short32_t huuid_uuid_ble_get(const huuid_t in)
{
    return huuid_uuid_short32_get(in);
}

void huuid_uuid_ble_set(huuid_t out,const huuid_short32_t ble_uuid)
{
    huuid_uuid_short32_set(out,ble_uuid,huuid_ble_base128_uuid);
}
