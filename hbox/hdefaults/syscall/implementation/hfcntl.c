/***************************************************************
 * Name:      hfcntl.c
 * Purpose:   实现hfcntl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-03-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_fcntl)
#define HDEFAULTS_SYSCALL_HFCNTL  HDEFAULTS_OS_LINUX_SYSCALL32_fcntl
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_fcntl)
#define HDEFAULTS_SYSCALL_HFCNTL  HDEFAULTS_OS_LINUX_SYSCALL64_fcntl
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_fcntl)
#define HDEFAULTS_SYSCALL_HFCNTL  HDEFAULTS_OS_FREEBSD_SYSCALL_fcntl
#endif


#ifdef HDEFAULTS_SYSCALL_HFCNTL

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif
#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_FCNTL_H)
#include "fcntl.h"
#endif

#if defined(HFCNTL)
extern int HFCNTL(int fd, int op, va_list va);
#endif

HDEFAULTS_USERCALL_DEFINE3(hfcntl,HDEFAULTS_SYSCALL_HFCNTL,int,int,fd,int,op,va_list *,va)
{
    int ret=-1;
    if(va==NULL)
    {
        return ret;
    }
#if defined(HFCNTL)
    ret=HFCNTL(fd,op,*va);
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)) && (!defined(HDEFAULTS_OS_EMSCRIPTEN)) && !defined(HDEFAULTS_OS_WINDOWS)  && !( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) )
    {
        /*
         * 根据实际需要支持部分fcntl
         */
        switch(op)
        {
        case F_GETFD:
        case F_GETFL:
        {
            ret=fcntl(fd,op);
        }
        break;
        case F_DUPFD:
        case F_SETFD:
        case F_SETFL:
        {
            int parameter=va_arg(*va,int);
            ret=fcntl(fd,op,parameter);
        }
        break;
        default:
        {
            void *parameter=va_arg(*va,void *);
            ret=fcntl(fd,op,parameter);
        }
        break;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        /*
         * 根据实际需要支持部分fcntl
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
    ret=hfiledescriptor_fcntl(fd,op,*va);
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HFCNTL

