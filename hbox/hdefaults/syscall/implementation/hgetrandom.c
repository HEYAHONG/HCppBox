/***************************************************************
 * Name:      hgetrandom.c
 * Purpose:   实现hgetrandom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "stdlib.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_getrandom)
#define HDEFAULTS_SYSCALL_HGETRANDOM  HDEFAULTS_OS_LINUX_SYSCALL32_getrandom
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_getrandom)
#define HDEFAULTS_SYSCALL_HGETRANDOM  HDEFAULTS_OS_LINUX_SYSCALL64_getrandom
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_getrandom)
#define HDEFAULTS_SYSCALL_HGETRANDOM  HDEFAULTS_OS_FREEBSD_SYSCALL_getrandom
#endif


#ifdef HDEFAULTS_SYSCALL_HGETRANDOM

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/random.h>
#endif

#if defined(HDEFAULTS_OS_WINDOWS)
#include "wincrypt.h"
#endif

#if defined(HGETRANDOM)
extern hgetrandom_ssize_t HGETRANDOM(void *buffer, size_t length,unsigned int flags);
#endif // defined

HDEFAULTS_USERCALL_DEFINE3(hgetrandom,HDEFAULTS_SYSCALL_HGETRANDOM,hgetrandom_ssize_t,void *,buffer, size_t,length,unsigned int,flags)
{
    hgetrandom_ssize_t ret=-1;
#if defined(HGETRANDOM)
    ret=HGETRANDOM(buffer,length,flags);
#elif defined(HDEFAULTS_OS_UNIX) && !(defined(HDEFAULTS_OS_ANDROID)) && (!defined(HDEFAULTS_LIBC_UCLIBC) && !defined(HDEFAULTS_OS_EMSCRIPTEN))
    ret=getrandom(buffer,length,flags);
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        HCRYPTPROV hCryptProv;
        if(CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT))
        {
            if(CryptGenRandom(hCryptProv,length,(BYTE *)buffer))
            {
                ret=length;
            }
            CryptReleaseContext(hCryptProv,0);
        }
    }
#elif  !defined(HSYSCALL_NO_IMPLEMENTATION) && !defined(HSYSCALL_NO_RANDOM)
    {
        ret=hsyscall_getrandom(buffer,length,flags);
    }
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HGETRANDOM
