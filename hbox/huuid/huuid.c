/***************************************************************
 * Name:      huuid.c
 * Purpose:   实现huuid接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "huuid.h"
#include "string.h"

HUUID_DEFINE_GLOBAL(huuid_null_uuid,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

void huuid_clear(huuid_t uu)
{
    memcpy(uu,huuid_null_uuid,sizeof(huuid_null_uuid));
}

int huuid_compare(const huuid_t uu1, const huuid_t uu2)
{
    return memcmp(uu1,uu2,sizeof(huuid_null_uuid));
}

void huuid_copy(huuid_t dst, const huuid_t src)
{
    memcpy(dst,src,sizeof(huuid_null_uuid));
}

int huuid_is_null(const huuid_t uu)
{
    return huuid_compare(uu,huuid_null_uuid);
}

void huuid_pack(const huuid_base_version_t *uu, huuid_t ptr)
{
    if(uu==NULL)
    {
        return;
    }

    if(ptr==NULL)
    {
        return;
    }

    uint32_t	tmp=0;
    uint8_t	*out = (uint8_t *)ptr;

    tmp = uu->time_low;
    out[3] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[2] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[1] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[0] = (uint8_t) (tmp&0xFF);

    tmp = uu->time_mid;
    out[5] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[4] = (uint8_t) (tmp&0xFF);

    tmp = uu->time_hi_and_version;
    out[7] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[6] = (uint8_t) (tmp&0xFF);

    tmp = uu->clock_seq;
    out[9] = (uint8_t) (tmp&0xFF);
    tmp >>= 8;
    out[8] = (uint8_t) (tmp&0xFF);

    memcpy(&out[10], uu->node, sizeof(uu->node));
}

void huuid_unpack(const huuid_t in, huuid_base_version_t *uu)
{
    if(uu==NULL)
    {
        return;
    }

    if(in==NULL)
    {
        return;
    }

    const uint8_t	*ptr = (const uint8_t *)in;
    uint32_t		tmp=0;

    tmp = (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    uu->time_low = tmp;

    tmp = (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    uu->time_mid = tmp;

    tmp = (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    uu->time_hi_and_version = tmp;

    tmp = (*ptr++);
    tmp = (tmp << 8) | (*ptr++);
    uu->clock_seq = tmp;

    memcpy(uu->node, ptr, sizeof(uu->node));
}
