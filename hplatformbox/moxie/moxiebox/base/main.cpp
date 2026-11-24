/***************************************************************
 * Name:      main.cpp
 * Purpose:   moxiebox基础固件入口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime.h"
#include "hshell.h"
#include "hrc.h"

int main(void)
{
    hruntime_init_lowlevel();
    hruntime_init();

    {
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            hshell_printf(NULL,"%s",banner);
        }
    }

    hshell_printf(NULL,"HBox hshell base\r\n");

    for(;;)
    {
        hruntime_loop();
    }

    return 0;
}
