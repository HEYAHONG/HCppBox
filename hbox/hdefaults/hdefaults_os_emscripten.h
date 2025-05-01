/***************************************************************
 * Name:      hdefaults_os_emscripten.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_EMSCRIPTEN_H__
#define __HDEFAULTS_OS_EMSCRIPTEN_H__
#include "hdefaults_os_unix.h"
#ifdef __cplusplus
#include "emscripten.h"
#endif // __cplusplus

/*
 * 系统调用号
 */
#include "syscall/hdefaults_os_emscripten_syscall.h"

#endif
