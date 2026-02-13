/***************************************************************
 * Name:      hfiledescriptor_filev1.c
 * Purpose:   实现hfiledescriptor_filev1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfiledescriptor_filev1.h"

hfiledescriptor_filev1_t *hfiledescriptor_filev1_get(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_common_table_t *obj=hfiledescriptor_common_table_get(fd);
    if(sizeof(hfiledescriptor_common_table_t) < sizeof(hfiledescriptor_filev1_t))
    {
        return NULL;
    }

    if(obj!=NULL && hfiledescriptor_common_table_type(fd)==HFILEDESCRIPTOR_TYPE_FILEV1)
    {
        return (hfiledescriptor_filev1_t *)obj;
    }

    return NULL;
}

hfiledescriptor_fd_t hfiledescriptor_filev1_open(hfiledescriptor_fd_t reuse_fd,const hfiledescriptor_filev1_t *filev1)
{
    if(sizeof(hfiledescriptor_common_table_t) < sizeof(hfiledescriptor_filev1_t) || filev1==NULL)
    {
        return -1;
    }
    hfiledescriptor_fd_t ret=hfiledescriptor_common_table_alloc(reuse_fd,HFILEDESCRIPTOR_TYPE_FILEV1);
    if(ret >= 0)
    {
        (*hfiledescriptor_filev1_get(ret))=(*filev1);
    }
    return ret;
}

int  hfiledescriptor_filev1_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len)
{
    hfiledescriptor_filev1_t *filev1=hfiledescriptor_filev1_get(fd);
    if(filev1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(filev1->filev1_read!=NULL)
    {
        ret=filev1->filev1_read(fd,buff,buff_len);
    }
    return ret;

}

int  hfiledescriptor_filev1_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len)
{
    hfiledescriptor_filev1_t *filev1=hfiledescriptor_filev1_get(fd);
    if(filev1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(filev1->filev1_write!=NULL)
    {
        ret=filev1->filev1_write(fd,buff,buff_len);
    }
    return ret;
}

int  hfiledescriptor_filev1_close(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_filev1_t *filev1=hfiledescriptor_filev1_get(fd);
    if(filev1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(filev1->filev1_close!=NULL)
    {
        ret=filev1->filev1_close(fd);
    }
    /*
     * 释放文件描述符,用户需要释放除文件描述符的其它资源
     */
    hfiledescriptor_common_table_free(fd);
    return ret;
}

hfiledescriptor_off_t  hfiledescriptor_filev1_lseek(hfiledescriptor_fd_t fd, hfiledescriptor_off_t offset, int whence)
{
    hfiledescriptor_filev1_t *filev1=hfiledescriptor_filev1_get(fd);
    if(filev1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(filev1->filev1_lseek!=NULL)
    {
        ret=filev1->filev1_lseek(fd,offset,whence);
    }
    return ret;
}

int  hfiledescriptor_filev1_ioctl(hfiledescriptor_fd_t fd, unsigned long op, va_list va)
{
    hfiledescriptor_filev1_t *filev1=hfiledescriptor_filev1_get(fd);
    if(filev1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(filev1->filev1_ioctl!=NULL)
    {
        ret=filev1->filev1_ioctl(fd,op,va);
    }
    return ret;
}

