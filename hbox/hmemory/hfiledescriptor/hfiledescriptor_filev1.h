/***************************************************************
 * Name:      hfiledescriptor_filev1.h
 * Purpose:   声明hfiledescriptor_filev1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFILEDESCRIPTOR_FILEV1_H__
#define __HFILEDESCRIPTOR_FILEV1_H__
#include "hfiledescriptor_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hfiledescriptor_filev1
{
    void *usr;
    int  (*filev1_read)(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len);
    int  (*filev1_write)(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len);
    int  (*filev1_close)(hfiledescriptor_fd_t fd);
    int  (*filev1_lseek)(hfiledescriptor_fd_t fd, hfiledescriptor_ssize_t offset, int whence);
    int  (*filev1_ioctl)(hfiledescriptor_fd_t fd, unsigned long op, va_list va);
};

typedef struct hfiledescriptor_filev1 hfiledescriptor_filev1_t;

hfiledescriptor_filev1_t *hfiledescriptor_filev1_get(hfiledescriptor_fd_t fd);


hfiledescriptor_fd_t hfiledescriptor_filev1_open(hfiledescriptor_fd_t reuse_fd,const hfiledescriptor_filev1_t *filev1);
int  hfiledescriptor_filev1_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len);
int  hfiledescriptor_filev1_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len);
int  hfiledescriptor_filev1_close(hfiledescriptor_fd_t fd);
int  hfiledescriptor_filev1_lseek(hfiledescriptor_fd_t fd, hfiledescriptor_ssize_t offset, int whence);
int  hfiledescriptor_filev1_ioctl(hfiledescriptor_fd_t fd, unsigned long op, va_list va);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFILEDESCRIPTOR_FILEV1_H__
