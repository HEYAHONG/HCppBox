/***************************************************************
 * Name:      hmemoryhash.c
 * Purpose:   实现hmemoryhash接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmemoryhash.h"

uint64_t hmemoryhash_bernstein_times_33(const uint8_t *data,size_t length)
{
    uint64_t hashv=0;
    if(data!=NULL && length > 0)
    {
        for(size_t i=0; i<length; i++)
        {
            /*
             * x*33=(x<<5)+x
             */
            hashv=((hashv<<5)+hashv)+data[i];
        }
    }
    return hashv;
}

uint64_t hmemoryhash_sax(const uint8_t *data,size_t length)
{
    uint64_t hashv=0;
    if(data!=NULL && length > 0)
    {
        for(size_t i=0; i<length; i++)
        {
            hashv ^= (hashv<<5) + (hashv >> 2) +data[i];
        }
    }
    return hashv;
}

uint64_t hmemoryhash_fnv_1a_32(const uint8_t *data,size_t length)
{
    uint32_t hashv=2166136261U;
    if(data!=NULL && length > 0)
    {
        for(size_t i=0; i<length; i++)
        {
            hashv = hashv^data[i];
            hashv = hashv*16777619U;
        }
    }
    return hashv;
}

uint64_t hmemoryhash_fnv_1a_64(const uint8_t *data,size_t length)
{
    uint64_t hashv=14695981039346656037ULL;
    if(data!=NULL && length > 0)
    {
        for(size_t i=0; i<length; i++)
        {
            hashv = hashv^data[i];
            hashv = hashv*1099511628211ULL;
        }
    }
    return hashv;
}

uint64_t hmemoryhash_oat(const uint8_t *data,size_t length)
{
    uint64_t hashv=0;
    if(data!=NULL && length > 0)
    {
        for(size_t i=0; i<length; i++)
        {
            hashv += data[i];
            hashv += (hashv << 10);
            hashv ^= (hashv >> 6);
        }

        hashv+= (hashv << 3);
        hashv^= (hashv>>11);
        hashv+= (hashv << 15);
    }
    return hashv;
}

#define HMEMORYHASH_JEN_ROT(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define HMEMORYHASH_JEN_MIX(a,b,c)                                               \
do {                                                                             \
    a -= c; a ^= HMEMORYHASH_JEN_ROT(c,4); c += b;                               \
    b -= a; b ^= HMEMORYHASH_JEN_ROT(a,6); a += c;                               \
    c -= b; c ^= HMEMORYHASH_JEN_ROT(b,8); b += a;                               \
    a -= c; a ^= HMEMORYHASH_JEN_ROT(c,16); c += b;                              \
    b -= a; b ^= HMEMORYHASH_JEN_ROT(a,19); a += c;                              \
    c -= b; c ^= HMEMORYHASH_JEN_ROT(b,4); b += a;                               \
} while (0)

#define HMEMORYHASH_JEN_FINAL(a,b,c)                                             \
do {                                                                             \
    c ^= b; c -= HMEMORYHASH_JEN_ROT(b,14);                                      \
    a ^= c; a -= HMEMORYHASH_JEN_ROT(c,11);                                      \
    b ^= a; b -= HMEMORYHASH_JEN_ROT(a,25);                                      \
    c ^= b; c -= HMEMORYHASH_JEN_ROT(b,16);                                      \
    a ^= c; a -= HMEMORYHASH_JEN_ROT(c,4);                                       \
    b ^= a; b -= HMEMORYHASH_JEN_ROT(a,14);                                      \
    c ^= b; c -= HMEMORYHASH_JEN_ROT(b,24);                                      \
} while (0)

uint64_t hmemoryhash_jenkins(const uint8_t *data,size_t length)
{
    //标准lookup3算法,低32位为标准jenkins(initval=0),高32位为第二个32位结果
    if(data==NULL)
    {
        length=0;
    }
    uint32_t a=0xdeadbeefU+(uint32_t)length;
    uint32_t b=a;
    uint32_t c=a;
    if(length==0)
    {
        return ((uint64_t)b<<32)|c;
    }

    const uint8_t *data_ptr=data;
    size_t remaining=length;
    while(remaining>12)
    {
        a+=(uint32_t)data_ptr[0]+((uint32_t)data_ptr[1]<<8)+((uint32_t)data_ptr[2]<<16)+((uint32_t)data_ptr[3]<<24);
        b+=(uint32_t)data_ptr[4]+((uint32_t)data_ptr[5]<<8)+((uint32_t)data_ptr[6]<<16)+((uint32_t)data_ptr[7]<<24);
        c+=(uint32_t)data_ptr[8]+((uint32_t)data_ptr[9]<<8)+((uint32_t)data_ptr[10]<<16)+((uint32_t)data_ptr[11]<<24);
        HMEMORYHASH_JEN_MIX(a,b,c);
        data_ptr+=12;
        remaining-=12;
    }

    for(size_t index=0;index<remaining;index++)
    {
        if(index<4)
        {
            a+=(uint32_t)data_ptr[index]<<(8*index);
        }
        else if(index<8)
        {
            b+=(uint32_t)data_ptr[index]<<(8*(index-4));
        }
        else
        {
            c+=(uint32_t)data_ptr[index]<<(8*(index-8));
        }
    }

    HMEMORYHASH_JEN_FINAL(a,b,c);
    return ((uint64_t)b<<32)|c;
}

uint64_t hmemoryhash_murmurhash2(const uint8_t *data,size_t length)
{
    if(data==NULL)
    {
        return 0;
    }
    uint32_t hashv=length;
    if(data!=NULL && length > 0)
    {
        while(length >= 4)
        {
            uint32_t k=data[0]+((uint32_t)data[1] << 8)+((uint32_t)data[2] << 16)+((uint32_t)data[3] << 24);
            k *= 0x5bd1e995;
            k ^= (k>>24);
            k *= 0x5bd1e995;
            hashv *= 0x5bd1e995;
            hashv ^= k;
            data += 4;
            length -= 4;
        }

        switch(length)
        {
        case 3:
        {
            hashv ^= ((uint32_t)data[2]<<16);
            hashv ^= ((uint32_t)data[1]<<8);
            hashv ^= data[0];
            hashv *= 0x5bd1e995;
        }
        break;
        case 2:
        {
            hashv ^= ((uint32_t)data[1]<<8);
            hashv ^= data[0];
            hashv *= 0x5bd1e995;
        }
        break;
        case 1:
        {
            hashv ^= data[0];
            hashv *= 0x5bd1e995;
        }
        break;
        default:
        {

        }
        break;
        }

        hashv ^= (hashv >> 13);
        hashv *= 0x5bd1e995;
        hashv ^= (hashv >>15);
    }
    return hashv;
}
