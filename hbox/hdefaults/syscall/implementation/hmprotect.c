/***************************************************************
 * Name:      hmprotect.c
 * Purpose:   实现hmprotect接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmman.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mprotect)
#define HDEFAULTS_SYSCALL_HMPROTECT  HDEFAULTS_OS_LINUX_SYSCALL32_mprotect
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mprotect)
#define HDEFAULTS_SYSCALL_HMPROTECT  HDEFAULTS_OS_LINUX_SYSCALL64_mprotect
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mprotect)
#define HDEFAULTS_SYSCALL_HMPROTECT  HDEFAULTS_OS_FREEBSD_SYSCALL_mprotect
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HMPROTECT)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HMPROTECT
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HMPROTECT

#if defined(HAVE_SYS_MMAN_H)
#include <sys/mman.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <errno.h>

static int hmman_mprotect_error(DWORD err, int deferr)
{
    if (0 == err)
        return deferr;
    return err;
}
static DWORD hmman_mprotect_prot_page(int prot)
{
    DWORD protect = 0;

    if (HMMAN_PROT_NONE == prot)
        return protect;

    if (prot & HMMAN_PROT_EXEC)
        protect = (prot & HMMAN_PROT_WRITE) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
    else
        protect = (prot & HMMAN_PROT_WRITE) ? PAGE_READWRITE : PAGE_READONLY;

    return protect;
}

#endif

#if defined(HMPROTECT)
extern void *HMPROTECT(void *addr, size_t len, int prot);
#endif //

HDEFAULTS_USERCALL_DEFINE3(hmprotect,HDEFAULTS_SYSCALL_HMPROTECT,int,void *,addr, size_t,len, int,prot)
{
    int ret=-1;
#if defined(HMPROTECT)
    ret=HMPROTECT(addr,len,prot);
#elif defined(HAVE_SYS_MMAN_H)
    ret=mprotect(addr,len,prot);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        DWORD newProtect =hmman_mprotect_prot_page(prot);
        DWORD oldProtect = 0;

        if (!VirtualProtect(addr, len, newProtect, &oldProtect))
        {
            errno = hmman_mprotect_error(GetLastError(), EPERM);
            ret= -1;
        }
        else
        {
            ret=0;
        }

        if((prot&HMMAN_PROT_EXEC)!=0)
        {

            if(!FlushInstructionCache(GetCurrentProcess(),addr,len))
            {
                ret=-1;
            }
        }
    }
#else
    //不支持mprotect
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HMPROTECT

