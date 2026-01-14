/***************************************************************
 * Name:      hsyscall_random.c
 * Purpose:   实现hsyscall_random接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hsyscall_random.h"
#include "hcrypto/hrng.h"

hgetrandom_ssize_t hsyscall_getrandom (void *buffer, size_t length,unsigned int flags)
{
    if(buffer==NULL)
    {
        return  -1;
    }
    uint8_t *data=(uint8_t *)buffer;

    /*
     * 使用不安全的算法填充
     */
    for(size_t i=0; i<length; i++)
    {
        hdefaults_mutex_lock(NULL);
#if defined(HSYSCALL_GETRANDOM_USING_MT)
        /*
         * 使用梅森旋转法填充
         */
        data[i]=hrng_mt_rand(NULL)%0x100;
#else
        /*
         * 使用rand48填充
         */
        data[i]=hrng_linearcongruential_rand48_rand()%0x100;
#endif
        hdefaults_mutex_unlock(NULL);
    }
    /*
     * 更安全的随机数生成算法
     */
    if((HGETRANDOM_GRND_INSECURE&flags)==0)
    {
#ifndef  HSYSCALL_GETRANDOM_TINY
        hdefaults_mutex_lock(NULL);
        hrng_chacha20_rand(NULL,buffer,length);
        hdefaults_mutex_unlock(NULL);
#endif // HSYSCALL_GETRANDOM_TINY
    }
    return length;
}


