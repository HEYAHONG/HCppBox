/***************************************************************
 * Name:      hendian.c
 * Purpose:   实现hendian接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-06-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hendian.h"
#include "hdefaults.h"

#if defined(HDEFAULTS_LIBC_GLIBC) || defined(HDEFAULTS_LIBC_MUSL) || defined(HDEFAULTS_LIBC_UCLIBC) || defined (HAVE_ENDIAN_H)
#include "endian.h"
#endif

static inline void  hlibc_endian_bswap(uint8_t *bytes,size_t len)
{
    if(bytes!=NULL && len > 0)
    {
        for(size_t i=0; i<len/2; i++)
        {
            uint8_t tmp=bytes[i];
            bytes[i]=bytes[len-1-i];
            bytes[len-1-i]=tmp;
        }
    }
}

static inline uint16_t hlibc_endian_bswap16(uint16_t data)
{
    uint16_t ret=data;
#if defined(__clang__) || (defined(__GNUC__) && !defined(__CC_ARM))
    ret=__builtin_bswap16(ret);
#elif defined(HDEFAULTS_LIBC_MSVC)
    ret = _byteswap_ushort(ret);
#else
    hlibc_endian_bswap((uint8_t *)&ret,sizeof(ret));
#endif
    return ret;
}

uint16_t hhtobe16(uint16_t host_16bits)
{
#if defined(htobe16)
    return htobe16(host_16bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap16(host_16bits);
#else
    return host_16bits;
#endif
}
uint16_t hhtole16(uint16_t host_16bits)
{
#if defined(htole16)
    return htole16(host_16bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap16(host_16bits);
#else
    return host_16bits;
#endif
}
uint16_t hbe16toh(uint16_t big_endian_16bits)
{
#if defined(be16toh)
    return be16toh(big_endian_16bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap16(big_endian_16bits);
#else
    return big_endian_16bits;
#endif
}
uint16_t hle16toh(uint16_t little_endian_16bits)
{
#if defined(le16toh)
    return le16toh(little_endian_16bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap16(little_endian_16bits);
#else
    return little_endian_16bits;
#endif
}


static inline uint32_t hlibc_endian_bswap32(uint32_t data)
{
    uint32_t ret=data;
#if defined(__clang__) || (defined(__GNUC__) && !defined(__CC_ARM))
    ret=__builtin_bswap32(ret);
#elif defined(HDEFAULTS_LIBC_MSVC)
    ret = _byteswap_ulong(ret);
#else
    hlibc_endian_bswap((uint8_t *)&ret,sizeof(ret));
#endif
    return ret;
}

uint32_t hhtobe32(uint32_t host_32bits)
{
#if defined(htobe32)
    return htobe32(host_32bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap32(host_32bits);
#else
    return host_32bits;
#endif
}
uint32_t hhtole32(uint32_t host_32bits)
{
#if defined(htole32)
    return htole32(host_32bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap32(host_32bits);
#else
    return host_32bits;
#endif
}
uint32_t hbe32toh(uint32_t big_endian_32bits)
{
#if defined(be32toh)
    return be32toh(big_endian_32bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap32(big_endian_32bits);
#else
    return big_endian_32bits;
#endif
}
uint32_t hle32toh(uint32_t little_endian_32bits)
{
#if defined(le32toh)
    return le32toh(little_endian_32bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap32(little_endian_32bits);
#else
    return little_endian_32bits;
#endif
}

static inline uint64_t hlibc_endian_bswap64(uint64_t data)
{
    uint64_t ret=data;
#if defined(__clang__) || (defined(__GNUC__) && !defined(__CC_ARM))
    ret=__builtin_bswap64(ret);
#elif defined(HDEFAULTS_LIBC_MSVC)
    ret = _byteswap_uint64(ret);
#else
    hlibc_endian_bswap((uint8_t *)&ret,sizeof(ret));
#endif
    return ret;
}

uint64_t hhtobe64(uint64_t host_64bits)
{
#if defined(htobe64)
    return htobe64(host_64bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap64(host_64bits);
#else
    return host_64bits;
#endif
}
uint64_t hhtole64(uint64_t host_64bits)
{
#if defined(htole64)
    return htole64(host_64bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap64( host_64bits);
#else
    return  host_64bits;
#endif
}
uint64_t hbe64toh(uint64_t big_endian_64bits)
{
#if defined(be64toh)
    return be64toh(big_endian_64bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)!=(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap64(big_endian_64bits);
#else
    return big_endian_64bits;
#endif
}
uint64_t hle64toh(uint64_t little_endian_64bits)
{
#if defined(le64toh)
    return le64toh(little_endian_64bits);
#elif ((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
    return hlibc_endian_bswap64(little_endian_64bits);
#else
    return little_endian_64bits;
#endif
}



