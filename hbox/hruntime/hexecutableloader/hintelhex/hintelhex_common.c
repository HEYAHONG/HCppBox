/***************************************************************
 * Name:      hintelhex_common.c
 * Purpose:   实现hintelhex_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hintelhex_common.h"

void hintelhex_common_buffer_init(hintelhex_common_buffer_t *buffer)
{
    if(buffer!=NULL)
    {
        memset(buffer,0,sizeof(hintelhex_common_buffer_t));
    }
}
