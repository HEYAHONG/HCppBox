/***************************************************************
 * Name:      hfiledescriptor_common.c
 * Purpose:   实现hfiledescriptor_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "ctype.h"
#include "hfiledescriptor_common.h"
#include "hfiledescriptor_filev1.h"

static hfiledescriptor_std_table_t hfiledescriptor_std_table_object={0};
void hfiledescriptor_std_table_set_read(hfiledescriptor_fd_t fd,hfiledescriptor_ssize_t (*std_read)(hfiledescriptor_fd_t fd, void * buff, hfiledescriptor_size_t buff_length))
{
    if(fd==HFILEDESCRIPTOR_STDIN)
    {
        hfiledescriptor_std_table_object.stdin_read=std_read;
    }
}
void hfiledescriptor_std_table_set_write(hfiledescriptor_fd_t fd,hfiledescriptor_ssize_t (*std_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length))
{
    if(fd==HFILEDESCRIPTOR_STDOUT)
    {
        hfiledescriptor_std_table_object.stdout_write=std_write;
    }
    if(fd==HFILEDESCRIPTOR_STDERR)
    {
        hfiledescriptor_std_table_object.stderr_write=std_write;
    }
}


#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#define HFILEDESCRIPTOR_COMMON_TABLE_SIZE (2048)
#endif
#if !defined(HFILEDESCRIPTOR_COMMON_TABLE_SIZE)
#define HFILEDESCRIPTOR_COMMON_TABLE_SIZE  (4)
#endif
#if (HFILEDESCRIPTOR_COMMON_TABLE_SIZE) < (1)
#undef   HFILEDESCRIPTOR_COMMON_TABLE_SIZE
#define  HFILEDESCRIPTOR_COMMON_TABLE_SIZE (1)
#endif

#ifndef HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE
#define HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE
#endif // HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE

HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE
static hfiledescriptor_common_table_t hfiledescriptor_common_table_object[HFILEDESCRIPTOR_COMMON_TABLE_SIZE]={0};

#ifndef HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE
#define HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE
#endif // HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE

HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE
static hatomic_int_t                  hfiledescriptor_common_table_flag[HFILEDESCRIPTOR_COMMON_TABLE_SIZE]={0};

hfiledescriptor_common_table_t *hfiledescriptor_common_table_get(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_common_table_t *ret=NULL;
    if(fd < 0)
    {
        return ret;
    }
    /*
     * 默认占用文件描述符空间末尾部分
     */
    const hfiledescriptor_fd_t fd_base=hfiledescriptor_fd_min();
    if(fd_base < 0 || fd < fd_base)
    {
        return ret;
    }
    const uintptr_t            table_object_base=(uintptr_t)hfiledescriptor_common_table_object;
    uintptr_t                  table_object_ptr=(table_object_base+(fd-fd_base)*sizeof(hfiledescriptor_common_table_t));
    if((table_object_ptr-table_object_base) < (sizeof(hfiledescriptor_common_table_object)))
    {
        ret=(hfiledescriptor_common_table_t *)table_object_ptr;
    }

    return ret;
}

static hfiledescriptor_fd_t hfiledescriptor_common_table_fd(hfiledescriptor_common_table_t * table)
{
    hfiledescriptor_fd_t ret=-1;
    const uintptr_t            table_object_base=(uintptr_t)hfiledescriptor_common_table_object;
    const uintptr_t            table_object_ptr=(uintptr_t)table;
    if(table_object_ptr < table_object_base || table_object_ptr >= (table_object_base+sizeof(hfiledescriptor_common_table_object)) || ((table_object_ptr-table_object_base)%sizeof(sizeof(hfiledescriptor_common_table_t))!=0))
    {
        return ret;
    }

    /*
     * 默认占用文件描述符空间末尾部分
     */
    const hfiledescriptor_fd_t fd_base=hfiledescriptor_fd_min();
    if(fd_base < 0)
    {
        return ret;
    }

    ret=fd_base+(table_object_ptr-table_object_base)/sizeof(hfiledescriptor_common_table_t);

    return ret;
}

hfiledescriptor_fd_t  hfiledescriptor_common_table_alloc(hfiledescriptor_fd_t reuse_fd,int type)
{
    int ret=reuse_fd;
    hfiledescriptor_common_table_t * obj=hfiledescriptor_common_table_get(ret);
    if(obj!=NULL)
    {
        size_t index=(((uintptr_t)obj)-((uintptr_t)hfiledescriptor_common_table_object))/sizeof(hfiledescriptor_common_table_t);
        int value=0;
        if(hatomic_int_compare_exchange_strong(&hfiledescriptor_common_table_flag[index],&value,type))
        {
            return ret;
        }
    }

    ret=-1;

    for(size_t index=0;index < sizeof(hfiledescriptor_common_table_flag)/sizeof(hfiledescriptor_common_table_flag[0]);index++)
    {
        int value=0;
        if(hatomic_int_compare_exchange_strong(&hfiledescriptor_common_table_flag[index],&value,type))
        {
            ret=hfiledescriptor_common_table_fd(&hfiledescriptor_common_table_object[index]);
            break;
        }
    }

    return ret;
}

int hfiledescriptor_common_table_type(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_common_table_t * obj=hfiledescriptor_common_table_get(fd);
    if(obj==NULL)
    {
        return 0;
    }
    size_t index=(((uintptr_t)obj)-((uintptr_t)hfiledescriptor_common_table_object))/sizeof(hfiledescriptor_common_table_t);
    return hatomic_int_load(&hfiledescriptor_common_table_flag[index]);
}

void hfiledescriptor_common_table_free(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_common_table_t * obj=hfiledescriptor_common_table_get(fd);
    if(obj==NULL)
    {
        return;
    }
    size_t index=(((uintptr_t)obj)-((uintptr_t)hfiledescriptor_common_table_object))/sizeof(hfiledescriptor_common_table_t);
    hatomic_int_store(&hfiledescriptor_common_table_flag[index],0);
}

bool hfiledescriptor_check_fd(hfiledescriptor_fd_t fd)
{
    if(fd==HFILEDESCRIPTOR_STDIN)
    {
        return hfiledescriptor_std_table_object.stdin_read!=NULL;
    }

    if(fd==HFILEDESCRIPTOR_STDOUT)
    {
        return hfiledescriptor_std_table_object.stdout_write!=NULL;
    }

    if(fd==HFILEDESCRIPTOR_STDERR)
    {
        return hfiledescriptor_std_table_object.stderr_write!=NULL;
    }

    if(hfiledescriptor_common_table_get(fd)!=NULL)
    {
        return true;
    }

    return false;
}


hfiledescriptor_fd_t hfiledescriptor_fd_max(void)
{
    return ((1ULL<<(sizeof(hfiledescriptor_fd_t)*8-1))-1);
}

hfiledescriptor_fd_t hfiledescriptor_fd_min(void)
{
    return hfiledescriptor_fd_max()-(sizeof(hfiledescriptor_common_table_object)/sizeof(hfiledescriptor_common_table_object[0]));
}

static int  hfiledescriptor_reserved_file_nul_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len)
{
    (void)fd;
    (void)buff;
    return buff_len;
}
static const hfiledescriptor_filev1_t hfiledescriptor_reserved_file_nul=
{
    NULL,
    NULL,
    hfiledescriptor_reserved_file_nul_write,
    NULL,
    NULL,
    NULL
};

static int  hfiledescriptor_reserved_file_con_read(hfiledescriptor_fd_t fd,void *buff,hfiledescriptor_size_t  buff_len)
{
    (void)fd;
    return hfiledescriptor_read(HFILEDESCRIPTOR_STDIN,buff,buff_len);
}
static int  hfiledescriptor_reserved_file_con_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len)
{
    (void)fd;
    return hfiledescriptor_write(HFILEDESCRIPTOR_STDOUT,buff,buff_len);
}
static const hfiledescriptor_filev1_t hfiledescriptor_reserved_file_con=
{
    NULL,
    hfiledescriptor_reserved_file_con_read,
    hfiledescriptor_reserved_file_con_write,
    NULL,
    NULL,
    NULL
};

static hfiledescriptor_fd_t  hfiledescriptor_open_reserved(hfiledescriptor_fd_t reuse_fd,const char * filename,int oflag,unsigned int mode)
{
    int ret=-1;
    if(filename==NULL)
    {
        return ret;
    }
    if(hstrlen(filename) < 3 || hstrlen(filename) > 7)
    {
        return ret;
    }
    unsigned char filename_temp[8]={0};
    memcpy(filename_temp,filename,hstrlen(filename));
    for(size_t i=0;i<sizeof(filename_temp);i++)
    {
        if(filename_temp[i]!='\0')
        {
            filename_temp[i]=toupper(filename_temp[i]);
        }
    }
    if(hstrcmp((const char *)filename_temp,"NUL")==0)
    {
        return hfiledescriptor_filev1_open(reuse_fd,&hfiledescriptor_reserved_file_nul);
    }

    if(hstrcmp((const char *)filename_temp,"CON")==0)
    {
        return hfiledescriptor_filev1_open(reuse_fd,&hfiledescriptor_reserved_file_con);
    }


    return ret;
}
static hfiledescriptor_fd_t  (*hfiledescriptor_open_namespace[])(hfiledescriptor_fd_t reuse_fd,const char * filename,int oflag,unsigned int mode)=
{
    hfiledescriptor_open_reserved
};


#if defined(HFILEDESCRIPTOR_OPEN)
extern hfiledescriptor_fd_t  HFILEDESCRIPTOR_OPEN(const char * filename,int oflag,unsigned int mode);
#endif
#if defined(HFILEDESCRIPTOR_OPENAT)
extern hfiledescriptor_fd_t  HFILEDESCRIPTOR_OPENAT(hfiledescriptor_fd_t reuse_fd,const char * filename,int oflag,unsigned int mode);
#endif

hfiledescriptor_fd_t  hfiledescriptor_open(const char * filename,int oflag,unsigned int mode)
{
    return hfiledescriptor_openat(-1,filename,oflag,mode);
}
hfiledescriptor_fd_t  hfiledescriptor_openat(hfiledescriptor_fd_t reuse_fd,const char * filename,int oflag,unsigned int mode)
{
    int ret=-1;
    /*
     * 命名空间。命名空间用于实现保留文件名（NUL、CON）或者使用文件路径访问内核对象(Windows下使用NT命名空间或者Win32设备命名空间，Linux下使用procfs、sysfs、设备文件)
     * 命名空间优先于普通文件系统,且一般不可以自由创建文件，通常是伪文件系统。     *
     */
     for(size_t i=0;i<sizeof(hfiledescriptor_open_namespace)/sizeof(hfiledescriptor_open_namespace[0]);i++)
     {
         if(hfiledescriptor_open_namespace[i]==NULL)
         {
             ret=hfiledescriptor_open_namespace[i](reuse_fd,filename,oflag,mode);
             if(ret >= 0)
             {
                 return ret;
             }
         }
     }


    /*
     * open函数中应当使用文件名打开文件，并且自行申请一个文件描述符返回，通常由文件系统实现。
     */

    if(ret < 0)
    {
#if defined(HFILEDESCRIPTOR_OPENAT)
        ret=HFILEDESCRIPTOR_OPENAT(reuse_fd,filename,oflag,mode);
#endif
    }


    if(ret < 0)
    {
#if defined(HFILEDESCRIPTOR_OPEN)
        ret=HFILEDESCRIPTOR_OPEN(filename,oflag,mode);
#endif
    }


    return ret;
}

int  hfiledescriptor_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len)
{
    int ret=-1;
    if(!hfiledescriptor_check_fd(fd))
    {
        return ret;
    }

    if(fd==HFILEDESCRIPTOR_STDIN)
    {
        hfiledescriptor_ssize_t (*std_read)(hfiledescriptor_fd_t fd, void * buff, hfiledescriptor_size_t buff_length)=hfiledescriptor_std_table_object.stdin_read;
        if(std_read!=NULL)
        {
            return std_read(fd,buff,buff_len);
        }
        else
        {
            return -1;
        }
    }

    switch(hfiledescriptor_common_table_type(fd))
    {
    case HFILEDESCRIPTOR_TYPE_FILEV1:
    {
        ret=hfiledescriptor_read(fd,buff,buff_len);
    }
    break;

    case 0:
    default:
    {
        /*
         * 不支持的文件描述符类型
         */
        ret=-1;
    }
    break;
    }

    return ret;
}
int  hfiledescriptor_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len)
{
    int ret=-1;
    if(!hfiledescriptor_check_fd(fd))
    {
        return ret;
    }

    if(fd==HFILEDESCRIPTOR_STDOUT)
    {
        hfiledescriptor_ssize_t (*std_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length)=hfiledescriptor_std_table_object.stdout_write;
        if(std_write!=NULL)
        {
            return std_write(fd,buff,buff_len);
        }
        else
        {
            return -1;
        }
    }

    if(fd==HFILEDESCRIPTOR_STDERR)
    {
        hfiledescriptor_ssize_t (*std_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length)=hfiledescriptor_std_table_object.stderr_write;
        if(std_write!=NULL)
        {
            return std_write(fd,buff,buff_len);
        }
        else
        {
            return -1;
        }
    }

    switch(hfiledescriptor_common_table_type(fd))
    {
    case HFILEDESCRIPTOR_TYPE_FILEV1:
    {
        ret=hfiledescriptor_write(fd,buff,buff_len);
    }
    break;

    case 0:
    default:
    {
        /*
         * 不支持的文件描述符类型
         */
        ret=-1;
    }
    break;
    }

    return ret;
}
int  hfiledescriptor_close(hfiledescriptor_fd_t fd)
{
    int ret=-1;
    if(!hfiledescriptor_check_fd(fd))
    {
        return ret;
    }

    switch(hfiledescriptor_common_table_type(fd))
    {
    case HFILEDESCRIPTOR_TYPE_FILEV1:
    {
        ret=hfiledescriptor_close(fd);
    }
    break;

    case 0:
    default:
    {
        /*
         * 不支持的文件描述符类型
         */
        ret=-1;
    }
    break;
    }


    return ret;
}
hfiledescriptor_off_t  hfiledescriptor_lseek(hfiledescriptor_fd_t fd, hfiledescriptor_off_t offset, int whence)
{
    int ret=-1;
    if(!hfiledescriptor_check_fd(fd))
    {
        return ret;
    }

    switch(hfiledescriptor_common_table_type(fd))
    {
    case HFILEDESCRIPTOR_TYPE_FILEV1:
    {
        ret=hfiledescriptor_lseek(fd,offset,whence);
    }
    break;

    case 0:
    default:
    {
        /*
         * 不支持的文件描述符类型
         */
        ret=-1;
    }
    break;
    }

    return ret;
}

int  hfiledescriptor_ioctl(hfiledescriptor_fd_t fd, unsigned long op, ...)
{
    int ret=-1;
    if(!hfiledescriptor_check_fd(fd))
    {
        return ret;
    }

    va_list va;
    va_start(va,op);
    switch(hfiledescriptor_common_table_type(fd))
    {
    case HFILEDESCRIPTOR_TYPE_FILEV1:
    {
        ret=hfiledescriptor_filev1_ioctl(fd,op,va);
    }
    break;

    case 0:
    default:
    {
        /*
         * 不支持的文件描述符类型
         */
        ret=-1;
    }
    break;
    }
    va_end(va);

    return ret;
}
