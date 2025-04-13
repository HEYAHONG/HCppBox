/***************************************************************
 * Name:      hlrc.c
 * Purpose:   实现hlrc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hlrc.h"

uint8_t hlrc_calculate(const uint8_t *data,size_t datalen)
{
    uint8_t ret=0;
    if(data!=NULL && datalen > 0)
    {
        for(size_t i=0; i<datalen; i++)
        {
            ret += data[i];
        }
    }
    return (~ret)+1;
}

bool hlrc_check(const uint8_t *data,size_t datalen,uint8_t check)
{
    return check==hlrc_calculate(data,datalen);
}
