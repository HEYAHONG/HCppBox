/***************************************************************
 * Name:      hcompiler_armclang.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_ARMCLANG_H__
#define __HCOMPILER_ARMCLANG_H__

/*
 * 默认使用Clang的宏定义
 */
#include "hcompiler_clang.h"
//不定义HCOMPILER_CLANG
#ifdef HCOMPILER_CLANG
#undef HCOMPILER_CLANG
#endif

#endif
