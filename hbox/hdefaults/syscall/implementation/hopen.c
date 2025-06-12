/***************************************************************
 * Name:      hopen.c
 * Purpose:   实现hopen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_open)
#define HDEFAULTS_SYSCALL_HOPEN  HDEFAULTS_OS_LINUX_SYSCALL32_open
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_open)
#define HDEFAULTS_SYSCALL_HOPEN  HDEFAULTS_OS_LINUX_SYSCALL64_open
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_open)
#define HDEFAULTS_SYSCALL_HOPEN  HDEFAULTS_OS_FREEBSD_SYSCALL_open
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HOPEN)
/*
 * 默认不支持 Emscripten
 */
#undef HDEFAULTS_SYSCALL_HOPEN
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HOPEN

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(_UCRT)|| defined(HAVE_FCNTL_H)
#include "fcntl.h"
#endif

#if defined(HOPEN)
extern int HOPEN(const char *filename,int oflag,...);
#endif

HDEFAULTS_USERCALL_DEFINE3(hopen,HDEFAULTS_SYSCALL_HOPEN,int,const char *,filename,int,oflag,unsigned int,mode)
{
    int ret=-1;
#if defined(HOPEN)
    ret=HOPEN(filename,oflag,mode);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
    ret=open(filename,oflag,mode);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_open(filename,oflag,mode);
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HOPEN

