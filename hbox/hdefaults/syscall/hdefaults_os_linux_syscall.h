#ifndef __HDEFAULTS_OS_LINUX_SYSCALL_H__
#define __HDEFAULTS_OS_LINUX_SYSCALL_H__

#include "hdefaults.h"

#ifdef HDEFAULTS_BITS_32
/*
 * 32位系统
 */
#include "hdefaults_os_linux_syscall32.h"
#endif // HDEFAULTS_BITS_32


#ifdef HDEFAULTS_BITS_64
/*
 * 64位系统
 */
#include "hdefaults_os_linux_syscall64.h"
#endif // HDEFAULTS_BITS_64


#endif /* __HDEFAULTS_OS_LINUX_SYSCALL_H__ */
