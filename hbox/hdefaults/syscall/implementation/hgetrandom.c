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

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HGETRANDOM)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HGETRANDOM
#endif // HDEFAULTS_OS_EMSCRIPTEN

#ifdef HDEFAULTS_SYSCALL_HGETRANDOM

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/random.h>
#endif

#if defined(HGETRANDOM)
extern hgetrandom_ssize_t HGETRANDOM(void *buffer, size_t length,unsigned int flags);
#endif // defined

HDEFAULTS_USERCALL_DEFINE3(hgetrandom,HDEFAULTS_SYSCALL_HGETRANDOM,hgetrandom_ssize_t,void *,buffer, size_t,length,unsigned int,flags)
{
    hgetrandom_ssize_t ret=-1;
#if defined(HGETRANDOM)
    ret=HGETRANDOM(buffer,length,flags);
#elif defined(HDEFAULTS_OS_UNIX) && !(defined(HDEFAULTS_OS_ANDROID)) && (!defined(HDEFAULTS_LIBC_UCLIBC))
    ret=getrandom(buffer,length,flags);
#else
    {
        static bool is_random_init=false;
        if(!is_random_init)
        {
            //使用当前时间作为随机数种子
            hgettimeofday_timeval_t tv= {0};
            hgettimeofday(&tv,NULL);
            if((tv.tv_usec+tv.tv_sec)!=0)
            {
                srand(tv.tv_sec+tv.tv_usec);
                is_random_init=true;
            }
        }
        {
            //使用C库随机数函数
            uint8_t *buffer_array=(uint8_t *)buffer;
            if(buffer_array!=NULL)
            {
                for(size_t i=0; i<length; i++)
                {
                    buffer_array[i]=rand()%0x100;
                }
                ret=length;
            }
        }
    }
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HGETRANDOM
