/***************************************************************
 * Name:      hcompiler.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HCOMPILER_H_INCLUDED__
#define __HCOMPILER_H_INCLUDED__

/*
 * 本文件主要定义一些编译器相关宏定义。
 */

/*
 * 定义常用编译器名称
 */

//MSVC
#ifdef _MSC_VER
#include "hcompiler_msvc.h"
#ifndef HCOMPILER_MSVC
#define HCOMPILER_MSVC 1
#endif
#endif

//gcc、clang
#if !(defined(__CC_ARM) || defined(__ARMCC_VERSION))
#ifdef __GNUC__
#ifdef __clang__
#include "hcompiler_clang.h"
#ifndef HCOMPILER_CLANG
#define HCOMPILER_CLANG 1
#endif
#else
#include "hcompiler_gcc.h"
#ifndef HCOMPILER_GCC
#define HCOMPILER_GCC 1
#endif
#endif
#endif
#endif


//ARM Compiler(armcc、armclang)
#ifdef __ARMCC_VERSION
#ifdef __clang__
#include "hcompiler_armclang.h"
#ifndef HCOMPILER_ARMCLANG
#define HCOMPILER_ARMCLANG 1
#endif
#else
#include "hcompiler_armcc.h"
#ifndef HCOMPILER_ARMCC
#define HCOMPILER_ARMCC 1
#endif
#endif
#endif

//IAR C Compiler
#ifdef __IAR_SYSTEMS_ICC__
#include "hcompiler_icc.h"
#ifndef HCOMPILER_ICC
#define HCOMPILER_ICC 1
#endif
#endif

//默认设置
#include "hcompiler_default.h"


/*
 * 语言标准
 */
#include "hcompiler_lang_standard.h"



/*
 * 杂项
 */
#include "hcompiler_misc.h"

#endif // HCOMPILER_H_INCLUDED
