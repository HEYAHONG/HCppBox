/***************************************************************
 * Name:      hshell_defaults.h
 * Purpose:   声明hshell_defaults接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHELL_DEFAULTS_H__
#define __HSHELL_DEFAULTS_H__

#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "hcompiler.h"
#include "hdefaults.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef EOF
#define EOF (-1)
#endif // EOF

/*
 * hshell缓冲大小，此大小限制了用户一个命令的最大长度
 */
#ifndef HSHELL_CONTEXT_BUFFER_SIZE
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#define HSHELL_CONTEXT_BUFFER_SIZE 4096
#else
#define HSHELL_CONTEXT_BUFFER_SIZE 64
#endif
#endif // HSHELL_CONTEXT_BUFFER_SIZE

/*
 * 最大的argc大小
 */
#ifndef HSHELL_MAX_ARGC
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#define HSHELL_MAX_ARGC 256
#else
#define HSHELL_MAX_ARGC 8
#endif
#endif // HSHELL_MAX_ARGC


/*
 * 历史记录的大小
 */
#ifndef HSHELL_MAX_HISTORY_COUNT
#if defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_WINDOWS)
#define HSHELL_MAX_HISTORY_COUNT 64
#else
#define HSHELL_MAX_HISTORY_COUNT 2
#endif
#endif // HSHELL_MAX_HISTORY_COUNT

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSHELL_DEFAULTS_H__
