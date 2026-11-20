/***************************************************************
 * Name:      hsrecord_common.c
 * Purpose:   实现hsrecord_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsrecord_common.h"

void hsrecord_common_buffer_init(hsrecord_common_buffer_t *buffer)
{
    if(buffer!=NULL)
    {
        memset(buffer,0,sizeof(hsrecord_common_buffer_t));
    }
}

void hsrecord_common_encode_big_endian(uint8_t *buffer,uint32_t value,size_t length)
{
    if(buffer==NULL || length==0)
    {
        return;
    }
    while(length > 0)
    {
        length--;
        buffer[length]=(uint8_t)(value & 0xFF);
        value>>=8;
    }
}

uint32_t hsrecord_common_decode_big_endian(const uint8_t *buffer,size_t length)
{
    uint32_t ret=0;
    if(buffer==NULL)
    {
        return ret;
    }
    for(size_t i=0; i<length; i++)
    {
        ret=(ret << 8) | buffer[i];
    }
    return ret;
}
