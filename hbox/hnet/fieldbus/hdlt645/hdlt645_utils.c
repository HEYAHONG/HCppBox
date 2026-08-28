/***************************************************************
 * Name:      hdlt645_utils.c
 * Purpose:   实现hdlt645_utils接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hdlt645_utils.h"

uint64_t hdlt645_uint64_to_bcd(uint64_t data)
{
    uint64_t bcd=0;
    /*
     *由于64位bcd码只能显示16位10进制数，因此最大支持16位十进制数
     */
    for(size_t i=1; i<=16; i++)
    {
        uint64_t d=((data)%((uint64_t)pow(10,(i))))/((uint64_t)pow(10,(i-1)));
        bcd+=((d)<<((i-1)*4));
    }
    return bcd;
}

uint64_t hdlt645_bcd_to_uint64(uint64_t bcd)
{
    uint64_t data=0;
    /*
     *由于64位bcd码只能显示16位10进制数，因此最大支持16位十进制数
     */
    for(size_t i=0; i<15; i++)
    {
        uint64_t d=(bcd%(0x01ULL<<((i+1)*4)))/(0x01ULL<<(i*4));
        data+=(d*pow(10,i));
    }
    {
        uint64_t d=bcd/(0x01ULL<<(15*4));
        data+= d*pow(10,15);
    }

    return data;
}


