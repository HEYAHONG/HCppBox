/***************************************************************
 * Name:      iob.c
 * Purpose:   实现moxiebox标准输入输出流
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <stdio.h>

#include "moxiebox_syscall.h"

static int
moxiebox_putc(char c, int fd)
{
    long ret = __moxiebox_write(fd, &c, 1);

    if (ret != 1)
        return _FDEV_ERR;
    return (unsigned char)c;
}

static int
moxiebox_stdout_putc(char c, FILE *file)
{
    (void)file;
    return moxiebox_putc(c, 1);
}

static int
moxiebox_stderr_putc(char c, FILE *file)
{
    (void)file;
    return moxiebox_putc(c, 2);
}

static int
moxiebox_stdin_getc(FILE *file)
{
    unsigned char c;
    long          ret;

    (void)file;
    ret = __moxiebox_read(0, &c, 1);
    if (ret == 0)
        return _FDEV_EOF;
    if (ret < 0)
        return _FDEV_ERR;
    return (int)c;
}

FILE __stdin = FDEV_SETUP_STREAM(NULL, moxiebox_stdin_getc, NULL,
                                 _FDEV_SETUP_READ);
FILE __stdout = FDEV_SETUP_STREAM(moxiebox_stdout_putc, NULL, NULL,
                                  _FDEV_SETUP_WRITE);
FILE __stderr = FDEV_SETUP_STREAM(moxiebox_stderr_putc, NULL, NULL,
                                  _FDEV_SETUP_WRITE);

FILE *const stdin = &__stdin;
FILE *const stdout = &__stdout;
FILE *const stderr = &__stderr;
