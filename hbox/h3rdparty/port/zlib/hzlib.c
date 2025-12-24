/***************************************************************
 * Name:      hzlib.c
 * Purpose:   实现hzlib接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hzlib.h"
#include H3RDPARTY_ZLIB_HEADER

bool hzlib_compress(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length)
{
    bool ret=false;
    if(dest==NULL || dest_length == NULL || src==NULL || src_length==0)
    {
        return ret;
    }
#ifndef Z_SOLO
    ret=(Z_OK==compress(dest,dest_length,src,src_length));
#endif // Z_SOLO
    return ret;
}

bool hzlib_compress2(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length,int level)
{
    bool ret=false;
    if(dest==NULL || dest_length == NULL || src==NULL || src_length==0)
    {
        return ret;
    }
#ifndef Z_SOLO
    ret=(Z_OK==compress2(dest,dest_length,src,src_length,level));
#endif // Z_SOLO
    return ret;
}

bool hzlib_uncompress(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long src_length)
{
    bool ret=false;
    if(dest==NULL || dest_length == NULL || src==NULL || src_length==0)
    {
        return ret;
    }
#ifndef Z_SOLO
    ret=(Z_OK==uncompress(dest,dest_length,src,src_length));
#endif // Z_SOLO
    return ret;
}

bool hzlib_uncompress2(uint8_t *dest,unsigned long *dest_length,const uint8_t *src,unsigned long* src_length)
{
    bool ret=false;
    if(dest==NULL || dest_length == NULL || src==NULL || src_length==NULL)
    {
        return ret;
    }
#ifndef Z_SOLO
    ret=(Z_OK==uncompress2(dest,dest_length,src,src_length));
#endif // Z_SOLO
    return ret;
}
