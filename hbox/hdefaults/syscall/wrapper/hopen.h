/***************************************************************
 * Name:      hopen.h
 * Purpose:   声明hopen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HOPEN_H__
#define __HOPEN_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(_UCRT)|| defined(HAVE_FCNTL_H)
#include "fcntl.h"
#endif



#ifndef O_RDONLY
#define O_RDONLY         00
#endif // O_RDONLY
#ifndef O_WRONLY
#define O_WRONLY         01
#endif // O_WRONLY
#ifndef O_RDWR
#define O_RDWR           02
#endif // O_RDWR
#ifndef O_CREAT
#define O_CREAT        0100
#endif // O_CREAT
#ifndef O_EXCL
#define O_EXCL         0200
#endif // O_EXCL
#ifndef O_NOCTTY
#define O_NOCTTY       0400
#endif // O_NOCTTY
#ifndef O_TRUNC
#define O_TRUNC       01000
#endif // O_TRUNC
#ifndef O_APPEND
#define O_APPEND      02000
#endif // O_APPEND
#ifndef O_NONBLOCK
#define O_NONBLOCK    04000
#endif // O_NONBLOCK
#ifndef O_DSYNC
#define O_DSYNC      010000
#endif // O_DSYNC
#ifndef O_SYNC
#define O_SYNC     04010000
#endif // O_SYNC
#ifndef O_RSYNC
#define O_RSYNC    04010000
#endif // O_RSYNC
#ifndef O_BINARY
#define O_BINARY    0100000
#endif // O_BINARY
#ifndef O_DIRECTORY
#define O_DIRECTORY 0200000
#endif // O_DIRECTORY
#ifndef O_NOFOLLOW
#define O_NOFOLLOW  0400000
#endif // O_NOFOLLOW
#ifndef O_CLOEXEC
#define O_CLOEXEC  02000000
#endif // O_CLOEXEC
#ifndef O_ASYNC
#define O_ASYNC      020000
#endif // O_ASYNC
#ifndef O_DIRECT
#define O_DIRECT     040000
#endif // O_DIRECT
#ifndef O_LARGEFILE
#define O_LARGEFILE 0100000
#endif // O_LARGEFILE
#ifndef O_NOATIME
#define O_NOATIME  01000000
#endif // O_NOATIME
#ifndef O_PATH
#define O_PATH    010000000
#endif // O_PATH
#ifndef O_TMPFILE
#define O_TMPFILE 020200000
#endif // O_TMPFILE
#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif // O_NDELAY

#ifndef O_SEARCH
#define O_SEARCH  O_PATH
#endif // O_SEARCH
#ifndef O_EXEC
#define O_EXEC    O_PATH
#endif // O_EXEC

#ifndef O_ACCMODE
#define O_ACCMODE (03|O_SEARCH)
#endif

/** \brief 打开文件
 *
 * \param filename const char* 文件名
 * \param oflag int 打开文件标志
 * \param ... 可选参数，一般情况下只有mode参数(类型为unsigned int)。
 * \return int 文件描述符，-1表示出错
 *
 */
int hopen(const char * filename,int oflag,...);

/*
 * 64位版本同open
 */
#define hopen64 hopen

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HOPEN_H__
