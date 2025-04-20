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
#include "ctype.h"
#include "stdlib.h"
#include "hcrypto.h"

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

void huuid_node_format(huuid_node_t node)
{
    if(node==NULL)
    {
        return;
    }

    node[0] |= 0x01; /**<  防止与MAC地址冲突 */
}

uint64_t huuid_timestamp_convert(uint64_t unix_ts_us)
{
    return unix_ts_us*10+( (((uint64_t) 0x01B21DD2) << 32) + 0x13814000);
}

void huuid_time_uuid_format(huuid_t time_uuid,uint64_t timestamp,uint16_t clock_seq,huuid_node_t node)
{
    if(time_uuid == NULL || node == NULL)
    {
        return;
    }

    huuid_base_version_t uuid;
    memcpy(uuid.node,node,sizeof(huuid_node_t));
    uuid.clock_seq=clock_seq;
    uuid.time_low=((timestamp >> 0)&0xFFFFFFFF);
    uuid.time_mid=((timestamp >> 32)&0xFFFF);
    uuid.time_hi_and_version=((timestamp >> 48)&0xFFFF);
    uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x1000;
    huuid_pack(&uuid,time_uuid);
}

HUUID_DEFINE_GLOBAL(huuid_dns_uuid,0x6b,0xa7,0xb8,0x10,0x9d,0xad,0x11,0xd1,0x80,0xb4,0x00,0xc0,0x4f,0xd4,0x30,0xc8);
HUUID_DEFINE_GLOBAL(huuid_url_uuid,0x6b,0xa7,0xb8,0x11,0x9d,0xad,0x11,0xd1,0x80,0xb4,0x00,0xc0,0x4f,0xd4,0x30,0xc8);
HUUID_DEFINE_GLOBAL(huuid_oid_uuid,0x6b,0xa7,0xb8,0x12,0x9d,0xad,0x11,0xd1,0x80,0xb4,0x00,0xc0,0x4f,0xd4,0x30,0xc8);
HUUID_DEFINE_GLOBAL(huuid_x500_uuid,0x6b,0xa7,0xb8,0x14,0x9d,0xad,0x11,0xd1,0x80,0xb4,0x00,0xc0,0x4f,0xd4,0x30,0xc8);

bool huuid_md5_uuid_generate(huuid_t output,const uint8_t *name,size_t name_len,const huuid_t namespace_uuid)
{
    if(output==NULL || namespace_uuid==NULL)
    {
        return false;
    }

    {
        hmd5_md5_t md5;
        hmd5_context_t ctx;
        hmd5_starts(&ctx);
        hmd5_update(&ctx,(uint8_t *)namespace_uuid,sizeof(huuid_t));
        if(name != NULL && name_len > 0)
        {
            hmd5_update(&ctx,name,name_len);
        }
        hmd5_finish(&ctx,md5);
        memcpy(output,md5,sizeof(huuid_t));
    }

    {
        huuid_base_version_t uuid;
        huuid_unpack(output,&uuid);
        uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
        uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x3000;
        huuid_pack(&uuid,output);
    }

    return true;
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

bool huuid_sha1_uuid_generate(huuid_t output,const uint8_t *name,size_t name_len,const huuid_t namespace_uuid)
{
    if(output==NULL || namespace_uuid==NULL)
    {
        return false;
    }

    {
        hsha1_sha_t sha1;
        hsha1_context_t ctx;
        hsha1_starts(&ctx);
        hsha1_update(&ctx,(uint8_t *)namespace_uuid,sizeof(huuid_t));
        if(name != NULL && name_len > 0)
        {
            hsha1_update(&ctx,name,name_len);
        }
        hsha1_finish(&ctx,sha1);
        memcpy(output,sha1,sizeof(huuid_t));
    }

    {
        huuid_base_version_t uuid;
        huuid_unpack(output,&uuid);
        uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
        uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x5000;
        huuid_pack(&uuid,output);
    }

    return true;
}

void huuid_time_be_uuid_format(huuid_t time_uuid,uint64_t timestamp,uint16_t clock_seq,huuid_node_t node)
{
    if(time_uuid == NULL || node == NULL)
    {
        return;
    }

    huuid_base_version_t uuid;
    memcpy(uuid.node,node,sizeof(huuid_node_t));
    uuid.clock_seq=clock_seq;
    /*
     * V6与V1的区别在于时间戳的高低位的分布不同
     */
    uuid.time_low=((timestamp >> 28)&0xFFFFFFFF);
    uuid.time_mid=((timestamp >> 12)&0xFFFF);
    uuid.time_hi_and_version=((timestamp >> 0)&0xFFFF);
    uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x6000;
    huuid_pack(&uuid,time_uuid);
}

void huuid_time_ordered_random_uuid_format(huuid_t time_ordered_random_uuid,uint64_t unix_ts_ms)
{
    if(time_ordered_random_uuid==NULL)
    {
        return;
    }
    huuid_base_version_t uuid;
    huuid_unpack(time_ordered_random_uuid,&uuid);
    uuid.time_low=(unix_ts_ms>>16);
    uuid.time_mid=(unix_ts_ms&0xFFFF);
    uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x7000;
    huuid_pack(&uuid,time_ordered_random_uuid);
}

void huuid_custom_uuid_format(huuid_t custom_uuid)
{
    if(custom_uuid==NULL)
    {
        return;
    }
    huuid_base_version_t uuid;
    huuid_unpack(custom_uuid,&uuid);
    uuid.clock_seq = (uuid.clock_seq & 0x3FFF) | 0x8000;
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0FFF) | 0x8000;
    huuid_pack(&uuid,custom_uuid);
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

bool huuid_parse(huuid_t out,const char *uuid_string)
{
    if(out == NULL || uuid_string == NULL)
    {
        return false;
    }
    if(strlen(uuid_string) != (sizeof(huuid_t)*2+4))
    {
        return false;
    }
    for(size_t i=0; i<((sizeof(huuid_t)*2+4)); i++)
    {
        if((i==8)|| (i==13) || (i==18) || (i==23))
        {
            if(uuid_string[i]!='-')
            {
                return false;
            }
        }
        else
        {
            if(!isxdigit((int)uuid_string[i]))
            {
                return false;
            }
        }
    }
    huuid_base_version_t uuid;
    uuid.time_low=strtoul(uuid_string,NULL,16);
    uuid.time_mid=strtoull(&uuid_string[9],NULL,16);
    uuid.time_hi_and_version=strtoul(&uuid_string[14],NULL,16);
    uuid.clock_seq=strtoul(&uuid_string[19],NULL,16);
    for(size_t i=0; i<sizeof(uuid.node); i++)
    {
        char buff[3]= {0};
        buff[0]=uuid_string[24+i*2];
        buff[1]=uuid_string[24+i*2+1];
        uuid.node[i]=strtoul(buff,NULL,16);
    }
    huuid_pack(&uuid,out);
    return true;
}
