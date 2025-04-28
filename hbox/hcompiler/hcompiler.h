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
本文件主要定义一些编译器相关宏定义。
*/

//定义常用编译器名称

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

/*
 * __INLINE
 */
#ifndef __INLINE
#define __INLINE  inline
#endif // __INLINE

/*
 * __STATIC_INLINE
 */
#ifndef __STATIC_INLINE
#define __STATIC_INLINE  static inline
#endif // __STATIC_INLINE


/*
 * __STATIC_FORCEINLINE
 */
#ifndef __STATIC_FORCEINLINE
#define __STATIC_FORCEINLINE __STATIC_INLINE
#endif //__STATIC_FORCEINLINE

/*
 * __NO_RETURN
 */
#ifndef __NO_RETURN
#define __NO_RETURN
#endif //__NO_RETURN

/*
 * __NO_RETURN
 */
#ifndef __NO_RETURN
#define __NO_RETURN
#endif //__NO_RETURN


/*
 * __USED
 */
#ifndef __USED
#define __USED
#endif //__USED


/*
 * __WEAK
 */
#ifndef __WEAK
#define __WEAK
#endif //__WEAK


/*
 * __PACKED
 */
#ifndef __PACKED
#define __PACKED
#endif //__PACKED

/*
 * __ALIGNED
 */
#ifndef __ALIGNED
#define __ALIGNED(x)
#endif //__ALIGNED

/*
 * __RESTRICT
 */
#ifndef __RESTRICT
#define __RESTRICT restrict
#endif //__RESTRICT

/*
 * __ALIAS
 */
#ifndef __ALIAS
#define __ALIAS(x)
#endif //__ALIAS

/*
 * __SECTION
 */
#ifndef __SECTION
#define __SECTION(x)
#endif //__SECTION

/*
未使用，通常用于消除编译器警告(未使用的参数)
*/
#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif // UNUSED

/*
标识IO输入,一般定义为volatile,通常用于嵌入式
*/
#ifndef __IO_I
#define __IO_I volatile
#endif // __IO_I

/*
标识IO输出,一般定义为volatile,通常用于嵌入式
*/
#ifndef __IO_O
#define __IO_O volatile
#endif // __IO_O

/*
标识IO输入输出,,一般定义为volatile,通常用于嵌入式
*/
#ifndef __IO
#define __IO volatile
#endif // __IO

/*
C语言标准
*/
#ifdef __STDC_VERSION__

/*
默认最低采用C99标准,故不判断是否为C99
*/

//C11
#if __STDC_VERSION__ >= 201112L
#define __HAS_C11 1
#endif // __STDC_VERSION__

//C17
#if __STDC_VERSION__ >= 201710L
#define __HAS_C17 1
#endif // __STDC_VERSION__

//C23
#if __STDC_VERSION__ >= 202311L
#define __HAS_C23 1
#endif // __STDC_VERSION__

#ifndef __C_VERSION
#define __C_VERSION  __STDC_VERSION__
#endif // __C_VERSION

#endif // __STDC_VERSION__

/*
C11特性
*/
#ifdef __HAS_C11

#ifndef __STDC_NO_ATOMICS__
#ifndef HAVE_STDATOMIC_H
#define HAVE_STDATOMIC_H 1
#endif // HAVE_STDATOMIC_H
#endif // __STDC_NO_ATOMICS__

#endif // __HAS_C11

/*
C++语言标准
*/
#ifdef __cplusplus

#ifdef _MSVC_LANG
#define __cplusplus_date _MSVC_LANG
#else
#define __cplusplus_date __cplusplus
#endif // _MSVC_LANG

//C++11
#if __cplusplus_date >= 201103L
#define __HAS_CPP11 1
#endif // __cplusplus_date

//C++14
#if __cplusplus_date >= 201402L
#define __HAS_CPP14 1
#endif // __cplusplus_date

//C++17
#if __cplusplus_date >= 201703L
#define __HAS_CPP17 1
#endif // __cplusplus_date

//C++20
#if __cplusplus_date >= 202002L
#define __HAS_CPP20 1
#endif // __cplusplus_date

//C++23
#if __cplusplus_date >= 202302L
#define __HAS_CPP23 1
#endif // __cplusplus_date

#ifndef __CPP_VERSION
#define __CPP_VERSION  __cplusplus_date
#endif // __CPP_VERSION

#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 在C++代码中，可能不能获取到C编译器C语言版本(编译库时)，定义一个函数获取__STDC_VERSION__
 */

/** \brief  获取C语言版本
 *
 * \return long C语言版本
 *
 */
long hcompiler_get_stdc_version(void);

/** \brief 获取编译日期
 *
 * \return const char* 日期
 *
 */
const char * hcompiler_get_date(void);

/** \brief 获取日期年
 *
 * \return int 年
 *
 */
int hcompiler_get_date_year(void);

/** \brief 获取日期月
 *
 * \return int 月 1-12
 *
 */
int hcompiler_get_date_month(void);

/** \brief  获取日期日
 *
 * \return int 日
 *
 */
int hcompiler_get_date_day(void);

/** \brief 获取编译时间
 *
 * \return const char* 时间
 *
 */
const char * hcompiler_get_time(void);

/** \brief 获取编译时间时
 *
 * \return int 时
 *
 */
int hcompiler_get_time_hour(void);

/** \brief 获取编译时间分
 *
 * \return int 分
 *
 */
int hcompiler_get_time_minute(void);

/** \brief 获取编译时间秒
 *
 * \return int 秒
 *
 */
int hcompiler_get_time_second(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HCOMPILER_H_INCLUDED
