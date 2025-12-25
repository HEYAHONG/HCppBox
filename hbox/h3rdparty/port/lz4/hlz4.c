/***************************************************************
 * Name:      hlz4.c
 * Purpose:   实现hlz4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlz4.h"
#include H3RDPARTY_LZ4_HEADER
#include H3RDPARTY_LZ4HC_HEADER

int hlz4_compress(const char* src, char* dst, int src_length, int dst_length)
{
    return LZ4_compress_default(src,dst,src_length,dst_length);
}

int hlz4_compress_hc(const char* src, char* dst, int src_length, int dst_length,int level)
{
     return LZ4_compress_HC(src,dst,src_length,dst_length,level);
}

int hlz4_decompress(const char* src, char* dst, int src_length, int dst_length)
{
    return  LZ4_decompress_safe(src,dst,src_length,dst_length);
}

