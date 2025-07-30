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

#define HMEMORYHASH_JEN_MIX(a,b,c)                                               \
do {                                                                             \
  a -= b; a -= c; a ^= ( c >> 13 );                                              \
  b -= c; b -= a; b ^= ( a << 8 );                                               \
  c -= a; c -= b; c ^= ( b >> 13 );                                              \
  a -= b; a -= c; a ^= ( c >> 12 );                                              \
  b -= c; b -= a; b ^= ( a << 16 );                                              \
  c -= a; c -= b; c ^= ( b >> 5 );                                               \
  a -= b; a -= c; a ^= ( c >> 3 );                                               \
  b -= c; b -= a; b ^= ( a << 10 );                                              \
  c -= a; c -= b; c ^= ( b >> 15 );                                              \
} while (0)


uint64_t hmemoryhash_jenkins(const uint8_t *data,size_t length)
{
    uint64_t hashv=0;
    if(data!=NULL && length > 0)
    {
        unsigned i=0,j=0,k=0;
        hashv = 0xfeedbeefu;
        i = j = 0x9e3779b9u;
        k = (unsigned)(length);
        const uint8_t *dataptr=data;
        while (k >= 12U)
        {
            i +=    (dataptr[0] + ( (unsigned)dataptr[1] << 8 ) + ( (unsigned)dataptr[2] << 16 ) + ( (unsigned)dataptr[3] << 24 ) );
            j +=    (dataptr[4] + ( (unsigned)dataptr[5] << 8 ) + ( (unsigned)dataptr[6] << 16 ) + ( (unsigned)dataptr[7] << 24 ) );
            hashv += (dataptr[8] + ( (unsigned)dataptr[9] << 8 )+ ( (unsigned)dataptr[10] << 16 )+ ( (unsigned)dataptr[11] << 24 ) );
            HMEMORYHASH_JEN_MIX(i, j, hashv);
            dataptr += 12;
            k -= 12U;
        }
        hashv += (unsigned)(length);
        switch ( k )
        {
        case 11:
        {
            hashv += ( (unsigned)dataptr[10] << 24 );
            hashv += ( (unsigned)dataptr[9] << 16 );
            hashv += ( (unsigned)dataptr[8] << 8 );
            j += ( (unsigned)dataptr[7] << 24 );
            j += ( (unsigned)dataptr[6] << 16 );
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 10:
        {
            hashv += ( (unsigned)dataptr[9] << 16 );
            hashv += ( (unsigned)dataptr[8] << 8 );
            j += ( (unsigned)dataptr[7] << 24 );
            j += ( (unsigned)dataptr[6] << 16 );
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 9:
        {
            hashv += ( (unsigned)dataptr[8] << 8 );
            j += ( (unsigned)dataptr[7] << 24 );
            j += ( (unsigned)dataptr[6] << 16 );
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 8:
        {
            j += ( (unsigned)dataptr[7] << 24 );
            j += ( (unsigned)dataptr[6] << 16 );
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 7:
        {
            j += ( (unsigned)dataptr[6] << 16 );
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 6:
        {
            j += ( (unsigned)dataptr[5] << 8 );
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 5:
        {
            j += dataptr[4];
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 4:
        {
            i += ( (unsigned)dataptr[3] << 24 );
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 3:
        {
            i += ( (unsigned)dataptr[2] << 16 );
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 2:
        {
            i += ( (unsigned)dataptr[1] << 8 );
            i += dataptr[0];
        }
        break;
        case 1:
        {
            i += dataptr[0];
        }
        break;
        default:
        {

        }
        break;
        }
        HMEMORYHASH_JEN_MIX(i, j, hashv);
    }
    return hashv;
}

uint64_t hmemoryhash_murmurhash2(const uint8_t *data,size_t length)
{
    uint32_t hashv=length;
    if(data!=NULL && length > 0)
    {
        while(length > 4)
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
