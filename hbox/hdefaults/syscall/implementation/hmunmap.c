/***************************************************************
 * Name:      hmunmap.c
 * Purpose:   实现hmunmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_munmap)
#define HDEFAULTS_SYSCALL_HMUNMAP  HDEFAULTS_OS_LINUX_SYSCALL32_munmap
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_munmap)
#define HDEFAULTS_SYSCALL_HMUNMAP  HDEFAULTS_OS_LINUX_SYSCALL64_munmap
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_munmap)
#define HDEFAULTS_SYSCALL_HMUNMAP  HDEFAULTS_OS_FREEBSD_SYSCALL_munmap
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HMUNMAP)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HMUNMAP
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HMUNMAP

#if defined(HAVE_SYS_MMAN_H)
#include <sys/mman.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
static int hmman_munmap_error(DWORD err, int deferr)
{
    if (0 == err)
        return deferr;
    return err;
}
#endif

#if defined(HMUNMAP)
extern void *HMUNMAP(void *addr, size_t len);
#endif //

HDEFAULTS_USERCALL_DEFINE2(hmunmap,HDEFAULTS_SYSCALL_HMUNMAP,int,void *,addr, size_t,len)
{
    int ret=-1;
#if defined(HMUNMAP)
    ret=HMUNMAP(addr,len);
#elif defined(HAVE_SYS_MMAN_H)
    ret=munmap(addr,len);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        if (!UnmapViewOfFile(addr))
        {
            errno =  hmman_munmap_error(GetLastError(), EPERM);
            ret=-1;
        }
        else
        {
            ret=0;
        }
    }
#else
    //不支持munmap
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HMUNMAP

