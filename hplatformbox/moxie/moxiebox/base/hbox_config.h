/***************************************************************
 * Name:      hbox_config.h
 * Purpose:   moxiebox基础固件HBox配置
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HBOX_CONFIG_H__
#define __HBOX_CONFIG_H__

/*
 * 不使用C++原子支持
 */
#define HCPPRT_NO_ATOMIC 1

/*
 * 启用HRuntime初始化、循环与符号段
 */
#define HRUNTIME_USING_INIT_SECTION 1
#define HRUNTIME_USING_LOOP_SECTION 1
#define HRUNTIME_USING_SYMBOL_SECTION 1

/*
 * 关闭不兼容的syscall调用
 */
#define HDEFAULTS_SYSCALL_NO_HIOCTL     1
#define HDEFAULTS_SYSCALL_NO_HMSYNC     1
#define HDEFAULTS_SYSCALL_NO_HMPROTECT  1
#define HDEFAULTS_SYSCALL_NO_HFSYNC     1
#define HDEFAULTS_SYSCALL_NO_HFCNTL     1
#define HDEFAULTS_SYSCALL_NO_HOPENAT    1
#define HDEFAULTS_SYSCALL_NO_HMKDIR     1

#endif // __HBOX_CONFIG_H__
