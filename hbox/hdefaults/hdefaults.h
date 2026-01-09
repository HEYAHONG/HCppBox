/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   定义一些默认宏定义及函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HDEFAULTS_H_INCLUDED__
#define __HDEFAULTS_H_INCLUDED__

/*
本文件主要定义一些默认相关宏定义及函数。
*/


#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdarg.h"

//定义位宽(仅用于区分32位或者64位环境，其它位数不予保证)
#include "hdefaults_bits.h"

//定义硬件架构(支持部分常用架构)
#include "hdefaults_arch.h"

//定义平台(支持部分常用平台)
#include "hdefaults_platform.h"

//定义操作系统
#include "hdefaults_os.h"

//判断Libc
#include "hdefaults_libc.h"


//导入配置文件
#include "hdefaults_config.h"


//基础API
#include "hdefaults_baseapi.h"

//用户调用
#include "hdefaults_usercall.h"

//syscall移植
#include "syscall/hdefaults_syscall.h"

//libc移植
#include "libc/hdefaults_libc_port.h"

//libdl移植
#include "libdl/hdefaults_libdl_port.h"

/** \brief hdefaults组件初始化
 *  注意：默认由hruntime调用
 *
 */
void hdefaults_init(void);

/** \brief hdefaults组件循环
 *  注意：默认由hruntime调用
 *
 */
void  hdefaults_loop(void);

#endif // HDEFAULTS_H_INCLUDED
