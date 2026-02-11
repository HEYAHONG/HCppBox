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

#if defined(Z_SOLO)
hgzfile_t *  hgzdopen(int fd, const char *mode)
{
    return NULL;
}
int  hgzbuffer(hgzfile_t * file, unsigned length)
{
    return -1;
}
int  hgzsetparams(hgzfile_t * file, int level, int strategy)
{
    return -1;
}
int  hgzread(hgzfile_t * file, void * buf, unsigned len)
{
    return -1;
}
size_t  hgzfread(void * buf, size_t length, size_t nitems,hgzfile_t * file)
{
    return -1;
}
int  hgzwrite(hgzfile_t * file, const void * buf, unsigned len)
{
    return -1;
}
size_t  hgzfwrite(const void * buf, size_t length,size_t nitems, hgzfile_t * file)
{
    return -1;
}
int  hgzputs(hgzfile_t * file, const char *s)
{
    return -1;
}
char *  hgzgets(hgzfile_t * file, char *buf, int len)
{
    return "";
}
int  hgzputc(hgzfile_t * file, int c)
{
    return -1;
}
int  hgzgetc(hgzfile_t * file)
{
    return -1;
}
int  hgzungetc(int c, hgzfile_t * file)
{
    return -1;
}
int  hgzflush(hgzfile_t * file, int flush)
{
    return -1;
}
int  hgzrewind(hgzfile_t * file)
{
    return -1;
}
int  hgzeof(hgzfile_t * file)
{
    return -1;
}
int  hgzdirect(hgzfile_t * file)
{
    return -1;
}
int  hgzclose(hgzfile_t * file)
{
    return -1;
}
int  hgzclose_r(hgzfile_t * file)
{
    return -1;
}
int  hgzclose_w(hgzfile_t * file)
{
    return -1;
}
const char *  hgzerror(hgzfile_t * file, int *errnum)
{
    return "";
}
void  hgzclearerr(hgzfile_t * file)
{
    return;
}
#else
hgzfile_t *  hgzdopen(int fd, const char *mode)
{
    return (hgzfile_t *)gzdopen(fd,mode);
}
int  hgzbuffer(hgzfile_t * file, unsigned length)
{
    return gzbuffer((gzFile)file,length);
}
int  hgzsetparams(hgzfile_t * file, int level, int strategy)
{
    return gzsetparams((gzFile)file,level,strategy);
}
int  hgzread(hgzfile_t * file, void * buf, unsigned len)
{
    return gzread((gzFile)file,buf,len);
}
size_t  hgzfread(void * buf, size_t length, size_t nitems,hgzfile_t * file)
{
    return gzfread(buf,length,nitems,(gzFile)file);
}
int  hgzwrite(hgzfile_t * file, const void * buf, unsigned len)
{
    return gzwrite((gzFile)file,buf,len);
}
size_t  hgzfwrite(const void * buf, size_t length,size_t nitems, hgzfile_t * file)
{
    return gzfwrite(buf,length,nitems,(gzFile)file);
}
int  hgzputs(hgzfile_t * file, const char *s)
{
    return gzputs((gzFile)file,s);
}
char *  hgzgets(hgzfile_t * file, char *buf, int len)
{
    return gzgets((gzFile)file,buf,len);
}
int  hgzputc(hgzfile_t * file, int c)
{
    return gzputc((gzFile)file,c);
}
int  hgzgetc(hgzfile_t * file)
{
    return gzgetc((gzFile)file);
}
int  hgzungetc(int c, hgzfile_t * file)
{
    return gzungetc(c,(gzFile)file);
}
int  hgzflush(hgzfile_t * file, int flush)
{
    return gzflush((gzFile)file,flush);
}
int  hgzrewind(hgzfile_t * file)
{
    return gzrewind((gzFile)file);
}
int  hgzeof(hgzfile_t * file)
{
    return gzeof((gzFile)file);
}
int  hgzdirect(hgzfile_t * file)
{
    return gzdirect((gzFile)file);
}
int  hgzclose(hgzfile_t * file)
{
    return gzclose((gzFile)file);
}
int  hgzclose_r(hgzfile_t * file)
{
    return gzclose_r((gzFile)file);
}
int  hgzclose_w(hgzfile_t * file)
{
    return gzclose_w((gzFile)file);
}
const char *  hgzerror(hgzfile_t * file, int *errnum)
{
    return gzerror((gzFile)file,errnum);
}
void  hgzclearerr(hgzfile_t * file)
{
    gzclearerr((gzFile)file);
}
#endif
