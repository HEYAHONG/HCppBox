/***************************************************************
 * Name:      newlib_compat.c
 * Purpose:   为moxie工具链的libstdc++提供newlib兼容符号
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include <errno.h>
#include <signal.h>
#include <stdio.h>

extern FILE __stdin;
extern FILE __stdout;
extern FILE __stderr;
extern const char _ctype_b[];

int *__errno(void)
{
    return &errno;
}

int raise(int sig)
{
    (void)sig;
    return 0;
}

/*
 * newlib的struct _reent前缀，libstdc++仅使用_errno与标准流指针。
 */
struct newlib_reent_compat
{
    int _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    char _pad[64];
};

static struct newlib_reent_compat newlib_reent=
{
    0,&__stdin,&__stdout,&__stderr,{0}
};

struct newlib_reent_compat *_impure_ptr=&newlib_reent;

/*
 * 保持_ctype_b被链接进来，_ctype_由链接脚本定义为其+127。
 */
const char *const __moxiebox_keep_ctype_b=_ctype_b;
