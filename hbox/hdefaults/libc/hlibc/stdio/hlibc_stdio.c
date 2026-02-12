/***************************************************************
 * Name:      hlibc_stdio.c
 * Purpose:   实现hlibc_stdio接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibc_stdio.h"

enum
{
    HLIBC_STDIO_STREAM_FLAG_EOF=(1UL << (0))
};
static void hlibc_stdio_stream_flag_set(hatomic_int_t *obj,int flag)
{
    if(obj!=NULL)
    {
        hatomic_int_fetch_or(obj,flag);
    }
}

static void hlibc_stdio_stream_flag_clear(hatomic_int_t *obj,int flag)
{
    if(obj!=NULL)
    {
        hatomic_int_fetch_and(obj,~flag);
    }
}
static hatomic_int_t hlibc_stdio_stdin_flag=0;
static hatomic_int_t hlibc_stdio_stdout_flag=0;
static hatomic_int_t hlibc_stdio_stderr_flag=0;

int hlibc_getchar(void)
{
    int ret=-1;
    uint8_t ch_data=0;
    /*
     * 从标准输入(0)读取数据
     */
    if(hread(0,&ch_data,sizeof(ch_data))==sizeof(ch_data))
    {
        hlibc_stdio_stream_flag_clear(&hlibc_stdio_stdin_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
        ret=ch_data;
    }
    else
    {
        hlibc_stdio_stream_flag_set(&hlibc_stdio_stdin_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
    }
    return ret;
}
int hlibc_putchar(int ch)
{
    int ret=-1;
    /*
     * 从标准输出(1)读取数据
     */
    uint8_t ch_data=(uint8_t)ch;
    if(hwrite(1,&ch_data,sizeof(ch_data))==sizeof(ch_data))
    {
        hlibc_stdio_stream_flag_clear(&hlibc_stdio_stdout_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
        ret=ch_data;
    }
    else
    {
        hlibc_stdio_stream_flag_set(&hlibc_stdio_stdout_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
    }
    return ret;
}

size_t hlibc_fwrite(const void*buffer,size_t obj_size,size_t obj_count,hfile_t * fp)
{
    size_t ret=0;

    if(hlibc_is_stdout(fp))
    {
        const uint8_t *w_buffer=(const uint8_t *)buffer;
        for(size_t i=0;i < obj_count;i++)
        {
            /*
             * 从标准输出(1)写入数据
             */
            if(hwrite(1,&w_buffer[i*obj_size],obj_size)==obj_size)
            {
                ret=i;
                hlibc_stdio_stream_flag_clear(&hlibc_stdio_stdout_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
            }
            else
            {
                hlibc_stdio_stream_flag_set(&hlibc_stdio_stdout_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
                break;
            }
        }
    }

    if(hlibc_is_stderr(fp))
    {
        const uint8_t *w_buffer=(const uint8_t *)buffer;
        for(size_t i=0;i < obj_count;i++)
        {
            /*
             * 从标准错误(2)写入数据
             */
            if(hwrite(2,&w_buffer[i*obj_size],obj_size)==obj_size)
            {
                ret=i;
                hlibc_stdio_stream_flag_clear(&hlibc_stdio_stderr_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
            }
            else
            {
                hlibc_stdio_stream_flag_set(&hlibc_stdio_stderr_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
                break;
            }
        }
    }

    return ret;
}

size_t hlibc_fread(void*buffer,size_t obj_size,size_t obj_count,hfile_t * fp)
{
    size_t ret=0;
    if(buffer==NULL || obj_size == 0 || obj_count == 0)
    {
        return ret;
    }

    if(hlibc_is_stdin(fp))
    {
        uint8_t *r_buffer=(uint8_t *)buffer;
        for(size_t i=0;i < obj_count;i++)
        {
            /*
             * 从标准输入(0)读取数据
             */
            if(hread(0,&r_buffer[i*obj_size],obj_size)==obj_size)
            {
                ret=i;
                hlibc_stdio_stream_flag_clear(&hlibc_stdio_stdin_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
            }
            else
            {
                hlibc_stdio_stream_flag_set(&hlibc_stdio_stdin_flag,HLIBC_STDIO_STREAM_FLAG_EOF);
                break;
            }
        }
    }

    return ret;
}

void hlibc_clearerr(hfile_t *fp)
{
    if(hlibc_is_stdin(fp))
    {
        hatomic_int_store(&hlibc_stdio_stdin_flag,0);
    }
    if(hlibc_is_stdout(fp))
    {
        hatomic_int_store(&hlibc_stdio_stdout_flag,0);
    }
    if(hlibc_is_stderr(fp))
    {
        hatomic_int_store(&hlibc_stdio_stderr_flag,0);
    }
}

int hlibc_ferror(hfile_t *fp)
{
    int ret=0;
    if(hlibc_is_stdin(fp))
    {
        ret=hatomic_int_load(&hlibc_stdio_stdin_flag);
    }
    if(hlibc_is_stdout(fp))
    {
        ret=hatomic_int_load(&hlibc_stdio_stdout_flag);
    }
    if(hlibc_is_stderr(fp))
    {
        ret=hatomic_int_load(&hlibc_stdio_stderr_flag);
    }
    return ret;
}
int hlibc_feof(hfile_t *fp)
{
    return (HLIBC_STDIO_STREAM_FLAG_EOF&hlibc_ferror(fp));
}
hfile_t *hlibc_stdin(void)
{
    return (hfile_t *)stdin;
}
hfile_t *hlibc_stdout(void)
{
    return (hfile_t *)stdout;
}
hfile_t *hlibc_stderr(void)
{
    return (hfile_t *)stderr;
}
bool hlibc_is_stdin(void *fp)
{
    return fp==(void *)hlibc_stdin();
}
bool hlibc_is_stdout(void *fp)
{
    return fp==(void *)hlibc_stdout();
}
bool hlibc_is_stderr(void *fp)
{
    return fp==(void *)hlibc_stderr();
}


