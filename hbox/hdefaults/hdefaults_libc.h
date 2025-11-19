/***************************************************************
 * Name:      hdefaults_libc.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_H__
#define __HDEFAULTS_LIBC_H__

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdarg.h"

//判断Libc

//newlib
#ifdef __NEWLIB__
#include "hdefaults_libc_newlib.h"
#ifndef HDEFAULTS_LIBC_NEWLIB
#define HDEFAULTS_LIBC_NEWLIB 1
#endif // HDEFAULTS_LIBC_NEWLIB
#endif // __NEWLIB__

//picolibc
#ifdef __PICOLIBC__
#include "hdefaults_libc_picolibc.h"
#ifndef HDEFAULTS_LIBC_PICOLIBC
#define HDEFAULTS_LIBC_PICOLIBC 1
#endif // HDEFAULTS_LIBC_PICOLIBC
#endif

//mingw
#ifdef __MINGW32__
#include "hdefaults_libc_mingw.h"
#ifndef HDEFAULTS_LIBC_MINGW
#define HDEFAULTS_LIBC_MINGW 1
#endif // HDEFAULTS_LIBC_MINGW
#endif

//msvc
#ifdef _MSC_VER
#include "hdefaults_libc_msvc.h"
#ifndef HDEFAULTS_LIBC_MSVC
#define HDEFAULTS_LIBC_MSVC 1
#endif // HDEFAULTS_LIBC_MSVC
#endif

//glibc
#ifdef __GLIBC__
#include "hdefaults_libc_glibc.h"
#ifndef HDEFAULTS_LIBC_GLIBC
#define HDEFAULTS_LIBC_GLIBC 1
#endif // HDEFAULTS_LIBC_GLIBC
#endif

//musl
#if !defined(__GLIBC__) && !defined(__UCLIBC__) && (defined(HDEFAULTS_OS_EMSCRIPTEN) || defined(__linux__)) && !defined(HDEFAULTS_OS_ANDROID)
#include "hdefaults_libc_musl.h"
#ifndef HDEFAULTS_LIBC_MUSL
#define HDEFAULTS_LIBC_MUSL 1
#endif // HDEFAULTS_LIBC_MUSL
#endif

//uclibc
#ifdef __UCLIBC__
#include "hdefaults_libc_uclibc.h"
#ifndef HDEFAULTS_LIBC_UCLIBC
#define HDEFAULTS_LIBC_UCLIBC 1
#endif // HDEFAULTS_LIBC_UCLIBC
#endif

//armclib
#ifdef __ARMCLIB_VERSION
#include "hdefaults_libc_armclib.h"
#ifndef HDEFAULTS_LIBC_ARMCLIB
#define HDEFAULTS_LIBC_ARMCLIB 1
#endif // HDEFAULTS_LIBC_ARMCLIB
#endif

//iar icc
#ifdef __IAR_SYSTEMS_ICC__
#include "hdefaults_libc_icc.h"
#ifndef HDEFAULTS_LIBC_ICC
#define HDEFAULTS_LIBC_ICC 1
#endif // HDEFAULTS_LIBC_ICC
#endif


#endif
