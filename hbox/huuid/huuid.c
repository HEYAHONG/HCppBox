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

void huuid_random_uuid_format(huuid_t random_uuid)
{
    if(random_uuid==NULL)
    {
        return;
    }
    huuid_base_version_t uuid;
    huuid_unpack(random_uuid,&uuid);
    uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x4000;
    huuid_pack(&uuid,random_uuid);
}

void huuid_unparse(huuid_string_t out,const huuid_t uuid)
{
    huuid_unparse_upper(out,uuid);
}

#include "h3rdparty.h"

void huuid_unparse_upper(huuid_string_t out,const huuid_t uuid)
{
    huuid_base_version_t l_uuid;
    huuid_unpack(uuid,&l_uuid);
    hsprintf(out,"%08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X",(int32_t)l_uuid.time_low,(int)l_uuid.time_mid,(int)l_uuid.time_hi_and_version,(int)l_uuid.clock_seq,(int)l_uuid.node[0],(int)l_uuid.node[1],(int)l_uuid.node[2],(int)l_uuid.node[3],(int)l_uuid.node[4],(int)l_uuid.node[5]);
}

void huuid_unparse_lower(huuid_string_t out,const huuid_t uuid)
{
    huuid_base_version_t l_uuid;
    huuid_unpack(uuid,&l_uuid);
    hsprintf(out,"%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",(int32_t)l_uuid.time_low,(int)l_uuid.time_mid,(int)l_uuid.time_hi_and_version,(int)l_uuid.clock_seq,(int)l_uuid.node[0],(int)l_uuid.node[1],(int)l_uuid.node[2],(int)l_uuid.node[3],(int)l_uuid.node[4],(int)l_uuid.node[5]);
}
