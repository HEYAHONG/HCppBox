/***************************************************************
 * Name:      hwrite.c
 * Purpose:   实现hwrite接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_write)
#define HDEFAULTS_SYSCALL_HWRITE  HDEFAULTS_OS_LINUX_SYSCALL32_write
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_write)
#define HDEFAULTS_SYSCALL_HWRITE  HDEFAULTS_OS_LINUX_SYSCALL64_write
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_write)
#define HDEFAULTS_SYSCALL_HWRITE  HDEFAULTS_OS_FREEBSD_SYSCALL_write
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HWRITE)
/*
 * 默认不支持 Emscripten
 */
#undef HDEFAULTS_SYSCALL_HWRITE
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HWRITE

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif

#if defined(HWRITE)
extern hwrite_ssize_t HWRITE(int fd,const void *buff,size_t buff_count);
#endif

HDEFAULTS_USERCALL_DEFINE3(hwrite,HDEFAULTS_SYSCALL_HWRITE,hwrite_ssize_t,int,fd,const void*,buff,size_t,buff_count)
{
    hwrite_ssize_t ret=-1;
#if defined(HWRITE)
    ret=HWRITE(fd,buff,buff_count);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
    ret=write(fd,buff,buff_count);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_write(fd,buff,buff_count);
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HWRITE

