/***************************************************************
 * Name:      hopenat.c
 * Purpose:   实现hopenat接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_openat)
#define HDEFAULTS_SYSCALL_HOPENAT  HDEFAULTS_OS_LINUX_SYSCALL32_openat
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_openat)
#define HDEFAULTS_SYSCALL_HOPENAT  HDEFAULTS_OS_LINUX_SYSCALL64_openat
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_openat)
#define HDEFAULTS_SYSCALL_HOPENAT  HDEFAULTS_OS_FREEBSD_SYSCALL_openat
#endif


#ifdef HDEFAULTS_SYSCALL_HOPENAT

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(_UCRT)|| defined(HAVE_FCNTL_H)
#include "fcntl.h"
#endif

#if defined(HOPENAT)
extern int HOPENAT(int dirfd,const char *filename,int oflag,...);
#endif

HDEFAULTS_USERCALL_DEFINE4(hopenat,HDEFAULTS_SYSCALL_HOPENAT,int,int,dirfd,const char *,filename,int,oflag,unsigned int,mode)
{
    int ret=-1;
#if defined(HOPENAT)
    ret=HOPENAT(dirfd,filename,oflag,mode);
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)) && (!defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HDEFAULTS_OS_WINDOWS) && !( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) ))
    ret=openat(dirfd,filename,oflag,mode);
#elif ( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) )
    ret=open(filename,oflag,mode);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_open(filename,oflag,mode);
#elif !defined(HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR)
    /*
     * 注意:hfiledescriptor行为与unix的行为并不一致，hfiledescriptor用于重用文件描述符，unix用于在指定目录
     */
    ret=hfiledescriptor_openat(dirfd,filename,oflag,mode);
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HOPENAT

