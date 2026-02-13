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
#include H3RDPARTY_LZ4FILE_HEADER

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

int hlz4f_readopen(hlz4_readfile_t** lz4f, FILE* fp)
{
    return (LZ4F_OK_NoError==LZ4F_readOpen((LZ4_readFile_t**)lz4f, (FILE*)fp))?0:-1;
}
size_t hlz4f_read(hlz4_readfile_t* lz4f, void* buf, size_t length)
{
    size_t ret=LZ4F_read((LZ4_readFile_t*) lz4f, buf, length);
    return ret;
}
int hlz4f_readclose(hlz4_readfile_t* lz4f)
{
    return (LZ4F_OK_NoError==LZ4F_readClose((LZ4_readFile_t*)lz4f))?0:-1;
}


const LZ4F_preferences_t   hlz4f_preferences_fast_cfg=
{
    LZ4F_INIT_FRAMEINFO,
    0,
    0u,
    0u,
    { 0u, 0u, 0u }
};
const hlz4f_preferences_t* hlz4f_preferences_fast(void)
{
    return (const hlz4f_preferences_t*)&hlz4f_preferences_fast_cfg;
}
const LZ4F_preferences_t   hlz4f_preferences_best_cfg=
{
    LZ4F_INIT_FRAMEINFO,
    LZ4HC_CLEVEL_MAX,
    0u,
    0u,
    { 0u, 0u, 0u }
};
const hlz4f_preferences_t* hlz4f_preferences_best(void)
{
    return (const hlz4f_preferences_t*)&hlz4f_preferences_best_cfg;
}
int hlz4f_writeopen(hlz4_writefile_t** lz4f, FILE* fp, const hlz4f_preferences_t* prefsPtr)
{
    return (LZ4F_OK_NoError==LZ4F_writeOpen((LZ4_writeFile_t**)lz4f, fp, (const LZ4F_preferences_t*)prefsPtr))?0:-1;
}
size_t hlz4f_write(hlz4_writefile_t* lz4f, const void* buf, size_t length)
{
    size_t ret=LZ4F_write((LZ4_writeFile_t*) lz4f, buf, length);
    return ret;
}
int hlz4f_writeclose(hlz4_writefile_t* lz4f)
{
    return (LZ4F_OK_NoError==LZ4F_writeClose((LZ4_writeFile_t*)lz4f))?0:-1;
}

