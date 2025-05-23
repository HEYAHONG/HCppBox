/***************************************************************
 * Name:      hmmap.c
 * Purpose:   实现hmmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmman.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mmap)
#define HDEFAULTS_SYSCALL_HMMAP  HDEFAULTS_OS_LINUX_SYSCALL32_mmap
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mmap)
#define HDEFAULTS_SYSCALL_HMMAP  HDEFAULTS_OS_LINUX_SYSCALL64_mmap
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mmap)
#define HDEFAULTS_SYSCALL_HMMAP  HDEFAULTS_OS_FREEBSD_SYSCALL_mmap
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HMMAP)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HMMAP
#endif // HDEFAULTS_OS_EMSCRIPTEN


#ifdef HDEFAULTS_SYSCALL_HMMAP

#if defined(HAVE_SYS_MMAN_H)
#include <sys/mman.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <io.h>
#include <errno.h>

#ifndef FILE_MAP_EXECUTE
# define FILE_MAP_EXECUTE   0x0020
#endif

static int hmman_mmap_error(DWORD err, int deferr)
{
    if (0 == err)
        return deferr;
    return err;
}

static DWORD hmman_mmap_prot_page(int prot)
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

static DWORD hmman_mmap_prot_file(int prot)
{
    DWORD desiredAccess = 0;

    if (HMMAN_PROT_NONE == prot)
        return desiredAccess;

    if (prot & HMMAN_PROT_READ)
        desiredAccess |= FILE_MAP_READ;

    if (prot & HMMAN_PROT_WRITE)
        desiredAccess |= FILE_MAP_WRITE;

    if (prot & HMMAN_PROT_EXEC)
        desiredAccess |= FILE_MAP_EXECUTE;

    return desiredAccess;
}

#endif

#if defined(HMMAP)
extern void *HMMAP(void *addr, size_t len, int prot, int flags, int fildes, uint64_t off);
#endif //

HDEFAULTS_USERCALL_DEFINE6(hmmap,HDEFAULTS_SYSCALL_HMMAP,void *,void *,addr, size_t,len, int,prot, int,flags, int,fildes, uint64_t,off)
{
    void* ret=HMMAN_MAP_FAILED;
#if defined(HMMAP)
    ret=HMMAP(addr,len,prot,flags,fildes,off);
#elif defined(HAVE_SYS_MMAN_H)
    ret=mmap(addr,len,prot,flags,fildes,off);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        HANDLE fm, h;

        DWORD protect = hmman_mmap_prot_page(prot);
        DWORD desiredAccess = hmman_mmap_prot_file(prot);

        DWORD dwFileOffsetHigh = 0;
        DWORD dwFileOffsetLow = (DWORD)off;

        DWORD dwMaxSizeHigh = 0;
        DWORD dwMaxSizeLow = (DWORD)(off + len);

        errno = 0;

        if (!len
                /* Unsupported flag combinations */
                || (flags & HMMAN_MAP_FIXED)
                /* Usupported protection combinations */
                || (HMMAN_PROT_EXEC == prot))
        {
            errno = EINVAL;
            return HMMAN_MAP_FAILED;
        }

        h = !(flags & HMMAN_MAP_ANONYMOUS) ? (HANDLE)_get_osfhandle(fildes) : INVALID_HANDLE_VALUE;

        if ((INVALID_HANDLE_VALUE == h) && !(flags & HMMAN_MAP_ANONYMOUS))
        {
            errno = EBADF;
            return HMMAN_MAP_FAILED;
        }

        fm = CreateFileMapping(h, NULL, protect, dwMaxSizeHigh, dwMaxSizeLow, NULL);

        if (!fm)
        {
            errno = hmman_mmap_error(GetLastError(), EPERM);
            return HMMAN_MAP_FAILED;
        }

        ret = MapViewOfFile(fm, desiredAccess, dwFileOffsetHigh, dwFileOffsetLow, len);

        CloseHandle(fm);

        if (!ret)
        {
            errno = hmman_mmap_error(GetLastError(), EPERM);
            return HMMAN_MAP_FAILED;
        }
    }
#else
    if(len!=0)
    {
        if((HMMAN_MAP_ANONYMOUS&flags)!=0)
        {
            //匿名映射采用malloc模拟
            void *mem=hmalloc(HMMAN_ALIGNED_SIZE+HMMAN_ALIGNED_PARAMETER_SIZE+len);
            if(mem!=NULL)
            {
                hmman_parameter_t *para=HMMAN_ALIGNED_MEM_PTR_SIZE(mem);
                memset(mem,0,HMMAN_ALIGNED_SIZE+HMMAN_ALIGNED_PARAMETER_SIZE+len);
                para->mem=mem;
                para->len=len;
                para->prot=prot;
                para->flags=flags;
                para->fildes=fildes;
                para->offset=off;
                ret=(void *)(((uintptr_t)para)+HMMAN_ALIGNED_PARAMETER_SIZE);
            }
        }
    }
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HMMAP

