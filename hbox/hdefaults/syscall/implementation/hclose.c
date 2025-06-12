/***************************************************************
 * Name:      hclose.c
 * Purpose:   实现hclose接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_close)
#define HDEFAULTS_SYSCALL_HCLOSE  HDEFAULTS_OS_LINUX_SYSCALL32_close
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_close)
#define HDEFAULTS_SYSCALL_HCLOSE  HDEFAULTS_OS_LINUX_SYSCALL64_close
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_close)
#define HDEFAULTS_SYSCALL_HCLOSE  HDEFAULTS_OS_FREEBSD_SYSCALL_close
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HCLOSE)
/*
 * 默认不支持 Emscripten
 */
#undef HDEFAULTS_SYSCALL_HCLOSE
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HCLOSE

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif

#if defined(HCLOSE)
extern int HCLOSE(int fd);
#endif

HDEFAULTS_USERCALL_DEFINE1(hclose,HDEFAULTS_SYSCALL_HCLOSE,int,int,fd)
{
    int ret=-1;
#if defined(HCLOSE)
    ret=HCLOSE(fd);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
    ret=close(fd);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_close(fd);
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HCLOSE

