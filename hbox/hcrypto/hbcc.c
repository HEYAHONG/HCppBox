/***************************************************************
 * Name:      hbcc.c
 * Purpose:   实现hbcc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbcc.h"

uint8_t hbcc_calculate(const uint8_t *data,size_t datalen)
{
    uint8_t ret=0;
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            ret ^= data[i];
        }
    }
    return ret;
}

bool hbcc_check(const uint8_t *data,size_t datalen,uint8_t check)
{
    return check==hbcc_calculate(data,datalen);
}
