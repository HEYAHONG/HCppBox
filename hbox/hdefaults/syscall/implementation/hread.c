/***************************************************************
 * Name:      hread.c
 * Purpose:   实现hread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_read)
#define HDEFAULTS_SYSCALL_HREAD  HDEFAULTS_OS_LINUX_SYSCALL32_read
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_read)
#define HDEFAULTS_SYSCALL_HREAD  HDEFAULTS_OS_LINUX_SYSCALL64_read
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_read)
#define HDEFAULTS_SYSCALL_HREAD  HDEFAULTS_OS_FREEBSD_SYSCALL_read
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HREAD)
/*
 * 默认不支持 Emscripten
 */
#undef HDEFAULTS_SYSCALL_HREAD
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HREAD

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif

#if defined(HREAD)
extern hread_ssize_t HREAD(int fd,void *buff,size_t buff_count);
#endif

HDEFAULTS_USERCALL_DEFINE3(hread,HDEFAULTS_SYSCALL_HREAD,hread_ssize_t,int,fd,void*,buff,size_t,buff_count)
{
    hread_ssize_t ret=-1;
#if defined(HREAD)
    ret=HREAD(fd,buff,buff_count);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
    ret=read(fd,buff,buff_count);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_read(fd,buff,buff_count);
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HREAD

