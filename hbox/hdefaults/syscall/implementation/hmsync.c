/***************************************************************
 * Name:      hmsync.c
 * Purpose:   实现hmsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmman.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_msync)
#define HDEFAULTS_SYSCALL_HMSYNC  HDEFAULTS_OS_LINUX_SYSCALL32_msync
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_msync)
#define HDEFAULTS_SYSCALL_HMSYNC  HDEFAULTS_OS_LINUX_SYSCALL64_msync
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_msync)
#define HDEFAULTS_SYSCALL_HMSYNC  HDEFAULTS_OS_FREEBSD_SYSCALL_msync
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HMSYNC)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HMSYNC
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HMSYNC

#if defined(HAVE_SYS_MMAN_H)
#include <sys/mman.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <errno.h>

static int hmman_msync_error(DWORD err, int deferr)
{
    if (0 == err)
        return deferr;
    return err;
}
#endif

#if defined(HMSYNC)
extern void *HMSYNC(void *addr, size_t len, int flags);
#endif //

HDEFAULTS_USERCALL_DEFINE3(hmsync,HDEFAULTS_SYSCALL_HMSYNC,int,void *,addr, size_t,len, int,flags)
{
    int ret=-1;
#if defined(HMSYNC)
    ret=HMSYNC(addr,len,flags);
#elif defined(HAVE_SYS_MMAN_H)
    ret=msync(addr,len,flags);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        if (!FlushViewOfFile(addr, len))
        {
            errno = hmman_msync_error(GetLastError(), EPERM);
            ret= -1;
        }
        else
        {
            ret= 0;
        }

    }
#else
    {
        if(addr!=NULL &&(((uintptr_t)addr)%HMMAN_ALIGNED_SIZE)==0)
        {
            hmman_parameter_t *para=(hmman_parameter_t *)(((uintptr_t)addr)-HMMAN_ALIGNED_SIZE);
            if(para!=NULL && para->mem!=NULL)
            {
                if((para->flags&HMMAN_MAP_ANONYMOUS)==0)
                {
                    //非匿名映射需要保存文件
                }
                else
                {
                    ret=0;
                }
            }
        }
    }
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HMSYNC

