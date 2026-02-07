/***************************************************************
 * Name:      hfiledescriptor_common.h
 * Purpose:   声明hfiledescriptor_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFILEDESCRIPTOR_COMMON_H__
#define __HFILEDESCRIPTOR_COMMON_H__

#include "stdbool.h"
#include "stdint.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef int         hfiledescriptor_fd_t;
typedef intptr_t    hfiledescriptor_ssize_t;
typedef uintptr_t   hfiledescriptor_size_t;

#define HFILEDESCRIPTOR_STDIN    (0)
#define HFILEDESCRIPTOR_STDOUT   (1)
#define HFILEDESCRIPTOR_STDERR   (2)

struct hfiledescriptor_std_table
{
    hfiledescriptor_ssize_t (*stdin_read)(hfiledescriptor_fd_t fd, void * buff, hfiledescriptor_size_t buff_length);
    hfiledescriptor_ssize_t (*stdout_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length);
    hfiledescriptor_ssize_t (*stderr_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length);
};
typedef struct hfiledescriptor_std_table hfiledescriptor_std_table_t;

/*
 * 标准文件描述符设置,fd可为HFILEDESCRIPTOR_STDIN、HFILEDESCRIPTOR_STDOUT、HFILEDESCRIPTOR_STDERR之一
 */
void hfiledescriptor_std_table_set_read(hfiledescriptor_fd_t fd,hfiledescriptor_ssize_t (*std_read)(hfiledescriptor_fd_t fd, void * buff, hfiledescriptor_size_t buff_length));
void hfiledescriptor_std_table_set_write(hfiledescriptor_fd_t fd,hfiledescriptor_ssize_t (*std_write)(hfiledescriptor_fd_t fd,const void * buff, hfiledescriptor_size_t buff_length));



struct hfiledescriptor_common_table
{
    /*
     * 描述符、用于存储信息
     */
    uintptr_t descriptors[8];
};
typedef struct hfiledescriptor_common_table hfiledescriptor_common_table_t;

/** \brief 获取文件描述符表
 *
 * \param fd hfiledescriptor_fd_t 文件描述符
 * \return hfiledescriptor_common_table_t* 文件描述符表指针,失败返回NULL
 *
 */
hfiledescriptor_common_table_t *hfiledescriptor_common_table_get(hfiledescriptor_fd_t fd);


/** \brief 分配文件描述符
 *
 * \param reuse_fd hfiledescriptor_fd_t 重用的文件描述符，若该文件描述符未使用，则优先使用该描述符，不使用可使用-1
 * \param type int 类型，不可为0
 * \return hfiledescriptor_fd_t 文件描述符，小于0为无效值
 *
 */
hfiledescriptor_fd_t  hfiledescriptor_common_table_alloc(hfiledescriptor_fd_t reuse_fd,int type);

/** \brief 获取文件描述符类型
 *
 * \param fd hfiledescriptor_fd_t 文件描述符
 * \return int 文件描述符类型,0表示未使用
 *
 */
int hfiledescriptor_common_table_type(hfiledescriptor_fd_t fd);


/** \brief 释放文件描述符
 *
 * \param fd hfiledescriptor_fd_t 文件描述符
 *
 */
void hfiledescriptor_common_table_free(hfiledescriptor_fd_t fd);

/** \brief 检查文件描述符是否有效
 *
 * \param fd hfiledescriptor_fd_t 文件描述符
 * \return bool 是否有效
 *
 */
bool hfiledescriptor_check_fd(hfiledescriptor_fd_t fd);

enum
{
    HFILEDESCRIPTOR_TYPE_FILEV1=1,
};

/*
 * 常用的文件描述符操作
 */
hfiledescriptor_fd_t  hfiledescriptor_open(const char * filename,int oflag,unsigned int mode);
int  hfiledescriptor_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len);
int  hfiledescriptor_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len);
int  hfiledescriptor_close(hfiledescriptor_fd_t fd);
int  hfiledescriptor_lseek(hfiledescriptor_fd_t fd, hfiledescriptor_ssize_t offset, int whence);
int  hfiledescriptor_ioctl(hfiledescriptor_fd_t fd, int op, ...);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFILEDESCRIPTOR_COMMON_H__
