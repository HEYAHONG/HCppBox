/***************************************************************
 * Name:      hmkdir.c
 * Purpose:   实现hmkdir接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mkdir)
#define HDEFAULTS_SYSCALL_HMKDIR  HDEFAULTS_OS_LINUX_SYSCALL32_mkdir
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mkdir)
#define HDEFAULTS_SYSCALL_HMKDIR  HDEFAULTS_OS_LINUX_SYSCALL64_mkdir
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mkdir)
#define HDEFAULTS_SYSCALL_HMKDIR  HDEFAULTS_OS_FREEBSD_SYSCALL_mkdir
#endif


#ifdef HDEFAULTS_SYSCALL_HMKDIR

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_SYS_STAT_H)
#include "sys/stat.h"
#endif

#if defined(HMKDIR)
extern int HMKDIR(const char * pathname,unsigned long mode);
#endif

HDEFAULTS_USERCALL_DEFINE2(hmkdir,HDEFAULTS_SYSCALL_HMKDIR,int,const char *,pathname,unsigned int,mode)
{
    int ret=-1;
#if defined(HMKDIR)
    ret=HMKDIR(pathname,mode);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=mkdir(pathname);
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)) && (!defined(HDEFAULTS_OS_EMSCRIPTEN))
    ret=mkdir(pathname,mode);
#elif !defined(HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR)
    ret=-1;
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HMKDIR

