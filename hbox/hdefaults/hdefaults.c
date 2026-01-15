/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   实现一些一些默认函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hcompiler.h"
#include "hdefaults.h"
#include "time.h"

#include "hdefaults_baseapi.c"
#include "syscall/hdefaults_syscall.c"
#include "libc/hdefaults_libc_port.c"
#include "libdl/hdefaults_libdl_port.c"

extern void hdefaults_syscall_init(void);
extern void hdefaults_libc_port_init(void);
void hdefaults_init(void)
{
    hdefaults_syscall_init();
    hdefaults_libc_port_init();
}

extern void hdefaults_syscall_loop(void);
extern void hdefaults_libc_port_loop(void);
void  hdefaults_loop(void)
{
    hdefaults_syscall_loop();
    hdefaults_libc_port_loop();
}

