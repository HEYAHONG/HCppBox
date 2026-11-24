/***************************************************************
 * Name:      _exit.c
 * Purpose:   实现moxiebox _exit系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <unistd.h>

#include "moxiebox_syscall.h"

void
_exit(int status)
{
    __moxiebox_exit(status);

    for (;;) {
    }
}
