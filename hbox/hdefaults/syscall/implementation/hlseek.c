/***************************************************************
 * Name:      hlseek.c
 * Purpose:   实现hlseek接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_lseek)
#define HDEFAULTS_SYSCALL_HLSEEK  HDEFAULTS_OS_LINUX_SYSCALL32_lseek
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_lseek)
#define HDEFAULTS_SYSCALL_HLSEEK  HDEFAULTS_OS_LINUX_SYSCALL64_lseek
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_lseek)
#define HDEFAULTS_SYSCALL_HLSEEK  HDEFAULTS_OS_FREEBSD_SYSCALL_lseek
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HLSEEK)
/*
 * 默认不支持 Emscripten
 */
#undef HDEFAULTS_SYSCALL_HLSEEK
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HLSEEK

#if defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
#include "unistd.h"
#elif defined(HDEFAULTS_OS_WINDOWS)
#include "windows.h"
#endif

#if defined(HLSEEK)
extern hlseek_off_t HLSEEK(int fd,hlseek_off_t offset,int whence);
#endif

HDEFAULTS_USERCALL_DEFINE3(hlseek,HDEFAULTS_SYSCALL_HLSEEK,hlseek_off_t,int,fd,hlseek_off_t,offset,int,whence)
{
    hlseek_off_t ret=-1;
#if defined(HLSEEK)
    ret=HLSEEK(fd,offset,whence);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HAVE_UNISTD_H)
    ret=lseek(fd,offset,whence);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=_lseek(fd,offset,whence);
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HLSEEK

