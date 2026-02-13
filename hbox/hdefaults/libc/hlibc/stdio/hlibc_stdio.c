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

enum
{
    HLIBC_STDIO_STREAM_OPEN_MODE_READ=          (1UL << (0)),
    HLIBC_STDIO_STREAM_OPEN_MODE_WRITE=         (1UL << (1)),
    HLIBC_STDIO_STREAM_OPEN_MODE_APPEND=        (1UL << (2)),
    HLIBC_STDIO_STREAM_OPEN_MODE_BINARY=        (1UL << (3)),
    HLIBC_STDIO_STREAM_OPEN_MODE_CREATE=        (1UL << (4)),
};

struct hfile
{
    int magic;
    hatomic_int_t flag;
    int open_mode;
    int fd;
    hmtx_t lock;
};

static const char hlibc_stdio_file_maigc[]="FILEPTR";
static int hlibc_stdio_file_magic_number(void)
{
    int ret=0;
    memcpy(&ret,hlibc_stdio_file_maigc,sizeof(ret) > sizeof(hlibc_stdio_file_maigc) ? (sizeof(hlibc_stdio_file_maigc)):(sizeof(ret)));
    return ret;
}
static hfile_t * hlibc_stdio_file_new(void)
{
    hfile_t * ret=(hfile_t *)hmalloc(sizeof(hfile_t));

    if(ret==NULL)
    {
        return ret;
    }

    hmemset(ret,0,sizeof(hfile_t));

    hmtx_init(&ret->lock,hmtx_recursive | hmtx_timed);

    ret->magic=hlibc_stdio_file_magic_number();

    return ret;
}

static void hlibc_stdio_file_delete(hfile_t * fp)
{
    if(fp==NULL)
    {
        return;
    }

    hmtx_destroy(&fp->lock);

    free(fp);
}

static bool hlibc_stdio_file_check(hfile_t *fp)
{
    if(fp==NULL)
    {
        return false;
    }

    if(fp->magic!=hlibc_stdio_file_magic_number())
    {
        return false;
    }

    if(fp->fd < 0)
    {
        return false;
    }

    return true;
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

hfile_t * hlibc_fopen( const char* filename, const char* mode )
{
    if(filename==NULL || mode == NULL)
    {
        return NULL;
    }
    hfile_t *ret=hlibc_stdio_file_new();
    if(ret==NULL)
    {
        return ret;
    }

    int open_mode=0;
    while((*mode)!='\0')
    {
        switch(*mode)
        {
        case 'r':
        {
            ret->open_mode |= HLIBC_STDIO_STREAM_OPEN_MODE_READ;
            open_mode=O_RDONLY;
        }
        break;
        case 'w':
        {
            ret->open_mode |= (HLIBC_STDIO_STREAM_OPEN_MODE_WRITE | HLIBC_STDIO_STREAM_OPEN_MODE_CREATE);
            open_mode=O_WRONLY | O_CREAT;
        }
        break;
        case '+':
        {
            ret->open_mode |= (HLIBC_STDIO_STREAM_OPEN_MODE_WRITE | HLIBC_STDIO_STREAM_OPEN_MODE_READ);
            open_mode&=(~(O_RDONLY | O_WRONLY | O_RDWR));
            open_mode|=O_RDWR;
            if((ret->open_mode & HLIBC_STDIO_STREAM_OPEN_MODE_CREATE)!=0)
            {
                open_mode|=O_CREAT;
            }
        }
        break;
        case 'a':
        {
            ret->open_mode |= HLIBC_STDIO_STREAM_OPEN_MODE_APPEND;
            open_mode |= O_APPEND;
        }
        break;
        case 'b':
        {
            ret->open_mode |= HLIBC_STDIO_STREAM_OPEN_MODE_BINARY;
            open_mode |= O_BINARY;
        }
        break;
        default:
        {

        }
        break;
        }
        mode++;
    }

    ret->fd=hopen(filename,open_mode,00777);

    if(ret->fd < 0)
    {
        hlibc_stdio_file_delete(ret);
        ret=NULL;
        return ret;
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

    if(hlibc_stdio_file_check(fp))
    {
        if((fp->open_mode & HLIBC_STDIO_STREAM_OPEN_MODE_WRITE)==0)
        {
            return ret;
        }
        hmtx_lock(&fp->lock);
        const uint8_t *w_buffer=(uint8_t *)buffer;
        for(size_t i=0;i < obj_count;i++)
        {
            if(hwrite(fp->fd,&w_buffer[i*obj_size],obj_size)==obj_size)
            {
                ret=i;
                hlibc_stdio_stream_flag_clear(&fp->flag,HLIBC_STDIO_STREAM_FLAG_EOF);
            }
            else
            {
                hlibc_stdio_stream_flag_set(&fp->flag,HLIBC_STDIO_STREAM_FLAG_EOF);
                break;
            }
        }
        hmtx_unlock(&fp->lock);
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

    if(hlibc_stdio_file_check(fp))
    {
        if((fp->open_mode & HLIBC_STDIO_STREAM_OPEN_MODE_READ)==0)
        {
            return ret;
        }
        hmtx_lock(&fp->lock);
        uint8_t *r_buffer=(uint8_t *)buffer;
        for(size_t i=0;i < obj_count;i++)
        {
            if(hread(fp->fd,&r_buffer[i*obj_size],obj_size)==obj_size)
            {
                ret=i;
                hlibc_stdio_stream_flag_clear(&fp->flag,HLIBC_STDIO_STREAM_FLAG_EOF);
            }
            else
            {
                hlibc_stdio_stream_flag_set(&fp->flag,HLIBC_STDIO_STREAM_FLAG_EOF);
                break;
            }
        }
        hmtx_unlock(&fp->lock);
    }

    return ret;
}

int hlibc_fclose(hfile_t * fp)
{
    int ret=-1;

    if(hlibc_stdio_file_check(fp))
    {
        hclose(fp->fd);
        hlibc_stdio_file_delete(fp);
        ret=0;
    }

    return ret;
}

int64_t hlibc_fseek(hfile_t * fp, int64_t offset, int whence)
{
    int64_t ret=-1;
    if(hlibc_stdio_file_check(fp))
    {
        return hlseek(fp->fd,offset,whence);
    }
    return ret;
}
int64_t hlibc_ftell(hfile_t * fp)
{
    return hlibc_fseek(fp,0,SEEK_CUR);
}
void hlibc_rewind(hfile_t * fp)
{
    if(fp!=NULL)
    {
        hlibc_fseek(fp,0,SEEK_SET);
    }
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

    if(hlibc_stdio_file_check(fp))
    {
        hatomic_int_store(&fp->flag,0);
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
    if(hlibc_stdio_file_check(fp))
    {
        ret=hatomic_int_load(&fp->flag);
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


