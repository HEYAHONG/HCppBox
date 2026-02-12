/***************************************************************
 * Name:      hlibc_stdio.c
 * Purpose:   实现hlibc_stdio接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibc_stdio.h"

int hlibc_getchar(void)
{
    int ret=-1;
    uint8_t ch_data=0;
    /*
     * 从标准输入(0)读取数据
     */
    if(hread(0,&ch_data,sizeof(ch_data))==sizeof(ch_data))
    {
        ret=ch_data;
    }
    return ret;
}
int hlibc_putchar(int ch)
{
    int ret=-1;
    /*
     * 从标准输出(1)读取数据
     */
    uint8_t ch_data=(uint8_t)ch;
    if(hwrite(1,&ch_data,sizeof(ch_data))==sizeof(ch_data))
    {
        ret=ch_data;
    }
    return ret;
}

