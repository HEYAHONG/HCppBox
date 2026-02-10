/***************************************************************
 * Name:      hioctl.c
 * Purpose:   实现hioctl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_ioctl)
#define HDEFAULTS_SYSCALL_HIOCTL  HDEFAULTS_OS_LINUX_SYSCALL32_ioctl
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_ioctl)
#define HDEFAULTS_SYSCALL_HIOCTL  HDEFAULTS_OS_LINUX_SYSCALL64_ioctl
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_ioctl)
#define HDEFAULTS_SYSCALL_HIOCTL  HDEFAULTS_OS_FREEBSD_SYSCALL_ioctl
#endif


#ifdef HDEFAULTS_SYSCALL_HIOCTL

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_SYS_IOCTL_H)
#include "sys/ioctl.h"
#endif

#if defined(HIOCTL)
extern int HIOCTL(int fd, unsigned long op, va_list va);
#endif

HDEFAULTS_USERCALL_DEFINE3(hioctl,HDEFAULTS_SYSCALL_HIOCTL,int,int,fd,unsigned long,op,va_list *,va)
{
    int ret=-1;
    if(va==NULL)
    {
        return ret;
    }
#if defined(HIOCTL)
    ret=HIOCTL(fd,op,*va);
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)) && (!defined(HDEFAULTS_OS_EMSCRIPTEN))  && !( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) )
    {
        /*
         * 根据实际需要支持部分ioctl
         */
        switch(op)
        {

        default:
        {
            void *parameter=va_arg(*va,void *);
            ret=ioctl(fd,op,parameter);
        }
        break;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        /*
         * 根据实际需要支持部分ioctl
         */
        switch(op)
        {

        default:
        {
            ret=-1;
        }
        break;
        }
    }
#elif !defined(HDEFAULTS_SYSCALL_NO_HFILEDESCRIPTOR)
    ret=hfiledescriptor_ioctl(fd,op,*va);
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HIOCTL

