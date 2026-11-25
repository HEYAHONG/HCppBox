/***************************************************************
 * Name:      hbox_config.c
 * Purpose:   moxiebox基础固件HBox命令
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbox.h"
#include "stdlib.h"
#include "string.h"

static int sleep_entry(int argc,const char *argv[])
{
    hshell_context_t *ctx=hshell_context_get_from_main_argv(argc,argv);

    if(argc<2)
    {
        hshell_printf(ctx,"sleep [s]\r\n");
        return -1;
    }

    int n=atoi(argv[1]);
    if(n>0)
    {
        vTaskDelay((TickType_t)(n*1000));
    }

    return 0;
}
HSHELL_COMMAND_EXPORT(sleep,sleep_entry,sleep [s]);

static int ps_entry(int argc,const char *argv[])
{
    hshell_context_t *ctx=hshell_context_get_from_main_argv(argc,argv);
    char strbuff[2048];

    (void)argc;
    (void)argv;

    strbuff[0]='\0';
    vTaskSuspendAll();
    vTaskList(strbuff);
    xTaskResumeAll();
    hshell_printf(ctx,"%s\r\n",strbuff);

    return 0;
}
HSHELL_COMMAND_EXPORT(ps,ps_entry,show tasks);
