/***************************************************************
 * Name:      hfsync.c
 * Purpose:   实现hfsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmemory.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_fsync)
#define HDEFAULTS_SYSCALL_HFSYNC  HDEFAULTS_OS_LINUX_SYSCALL32_fsync
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_fsync)
#define HDEFAULTS_SYSCALL_HFSYNC  HDEFAULTS_OS_LINUX_SYSCALL64_fsync
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_fsync)
#define HDEFAULTS_SYSCALL_HFSYNC  HDEFAULTS_OS_FREEBSD_SYSCALL_fsync
#endif



#ifdef HDEFAULTS_SYSCALL_HFSYNC

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif

#if defined(HFSYNC)
extern int HFSYNC(int fd);
#endif

HDEFAULTS_USERCALL_DEFINE1(hfsync,HDEFAULTS_SYSCALL_HFSYNC,int,int,fd)
{
    int ret=-1;
#if defined(HFSYNC)
    ret=HFSYNC(fd);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_commit(fd);
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)) && (!defined(HDEFAULTS_OS_EMSCRIPTEN))
    ret=fsync(fd);
#elif !defined(HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR)
    ret=0;
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HFSYNC

