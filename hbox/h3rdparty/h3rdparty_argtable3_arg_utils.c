/***************************************************************
 * Name:      h3rdparty_argtable3.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc
#define realloc hrealloc

#if !defined(HDEFAULTS_LIBC_MSVC)

//定义打印函数
#define printf hprintf
#ifdef vsnprintf
#undef vsnprintf
#endif
#define vsnprintf hvsnprintf
#define puts hputs
#define fputs hfputs
#define vfprintf hvfprintf

//定义退出函数
#define abort habort
#define exit hexit

#endif


#ifdef _GETOPT_H_
#undef _GETOPT_H_
#endif
#ifdef _GETOPT_DECLARED
#undef _GETOPT_DECLARED
#endif
#ifdef _OPTRESET_DECLARED
#undef _OPTRESET_DECLARED
#endif

#ifdef  optarg
#undef  optarg
#endif
#define optarg argtable3_optarg
#ifdef  optind
#undef  optind
#endif
#define optind argtable3_optind
#ifdef  opterr
#undef  opterr
#endif
#define opterr argtable3_opterr
#ifdef  optopt
#undef  optopt
#endif
#define optopt argtable3_optopt
#ifdef  optreset
#undef  optreset
#endif
#define optreset argtable3_optreset
#ifdef  getopt_long
#undef  getopt_long
#endif
#define getopt_long argtable3_getopt_long
#ifdef  getopt
#undef  getopt
#endif
#define getopt argtable3_getopt

//定义环境变量相关函数
#define getenv hgetenv

//对默认函数进行重命名
#define dbg_printf argtable3_dbg_printf

#define ARG_REPLACE_GETOPT 1

/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifdef __GNUC__
#undef __GNUC__
#endif // __GNUC__
#ifndef __clang__
//关闭某些警告
#pragma diag_suppress 111
#endif
#endif // __ARMCC_VERSION


#ifndef  H3RDPARTY_USING_SYSTEM_ARGTABLE3

#include "3rdparty/argtable3/arg_utils.c"

#endif // H3RDPARTY_USING_SYSTEM_ARGTABLE3


