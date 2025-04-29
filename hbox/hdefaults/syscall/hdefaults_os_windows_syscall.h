#ifndef __HDEFAULTS_OS_WINDOWS_SYSCALL_H__
#define __HDEFAULTS_OS_WINDOWS_SYSCALL_H__

/*
 * 虽然Windows下也有其自身的系统调用及系统调用号（或称系统服务号），但由于Windows不是开源操作系统且其系统库相对稳定，故而此处一般不直接使用Windows的系统调用。
 * 在Windows下模拟unix环境可使用Cygwin/MSYS2.
 * 使用Linux的系统调用号，一般模拟极个别Linux系统调用号.
 */
#include "hdefaults_os_linux_syscall.h"


#endif /* __HDEFAULTS_OS_WINDOWS_SYSCALL_H__ */
