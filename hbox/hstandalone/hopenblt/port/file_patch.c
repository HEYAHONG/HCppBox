/***************************************************************
 * Name:      file.c
 * Purpose:   实现file接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HOPENBLT_FILE_IMPLEMENTATION  1

#include "boot.h"
#include "hbox.h"

#if !defined(HOPENBLT_FILE_FUNCTION_ATTRIBUTE)
#define HOPENBLT_FILE_FUNCTION_ATTRIBUTE __WEAK
#endif

#if !defined(HOPENBLT_FILE_NO_IMPLEMENTATION)

#include "patch/ff.h"

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
FRESULT f_mount (FATFS* fs, const char* path, uint8_t opt)
{
    /*
     * 文件系统挂载
     */
    return FR_OK;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
FRESULT f_open (FIL* fp, const char* path, uint8_t mode)
{
    /*
     * openblt只需要只读打开
     */

    if(fp != NULL && path !=NULL )
    {
        FILE * fp_ptr=hfopen(path, "r");
        if(fp_ptr==NULL)
        {
            return FR_NO_FILE;
        }

        fp->ptr[0]=(uintptr_t)fp_ptr;

    }
    return FR_OK;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
FRESULT f_close (FIL* fp)
{
    if(fp!=NULL)
    {
        return (hfclose((FILE *)fp->ptr[0]) != 0)?FR_INVALID_OBJECT:FR_OK;
    }
    return FR_INVALID_OBJECT;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
FRESULT f_lseek (FIL* fp, intptr_t ofs)
{
    if(fp!=NULL)
    {
        return (hfseek((FILE *)fp->ptr[0],SEEK_SET,ofs)<0)?FR_INVALID_OBJECT:FR_OK;
    }
    return FR_INVALID_OBJECT;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
char*   f_gets (char* buff, intptr_t len, FIL* fp)
{
    if(fp!=NULL &&buff !=NULL&&len>0)
    {
        size_t index=0;
        char ch=-1;
        hfread(&ch,sizeof(ch),1,(FILE *)fp->ptr[0]);
        if(ch > 0)
        {
            buff[index++]=ch;
            if(index==len)
            {
                return buff;
            }
            if(ch == '\n')
            {
                return buff;
            }
        }
        else
        {
            return buff;
        }
    }
    return buff;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
int     f_error(FIL* fp)
{
    if(fp!=NULL)
    {
        return hferror((FILE *)fp->ptr[0])!=0?1:0;
    }
    return 1;
}

HOPENBLT_FILE_FUNCTION_ATTRIBUTE
int     f_eof(FIL* fp)
{
    if(fp!=NULL)
    {
        return hfeof((FILE *)fp->ptr[0])!=0?1:0;
    }
    return 1;
}



#endif

