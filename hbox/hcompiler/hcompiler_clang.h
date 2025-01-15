/***************************************************************
 * Name:      hcompiler_clang.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_CLANG_H__
#define __HCOMPILER_CLANG_H__

/*
 * 一般情况下，clang会保持对gcc的部分兼容
 */
#include "hcompiler_gcc.h"
//不定义HCOMPILER_GCC
#ifdef  HCOMPILER_GCC
#undef 	HCOMPILER_GCC
#endif

#endif
