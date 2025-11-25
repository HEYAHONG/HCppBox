/***************************************************************
 * Name:      hcompiler_lang_standard.h
 * Purpose:   定义语言标准相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_LANG_STANDARD_H__
#define __HCOMPILER_LANG_STANDARD_H__

/*
 *C语言标准
 */
#ifdef __STDC_VERSION__

/*
 *默认最低采用C99标准,故不判断是否为C99
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
 *C11特性
 */
#ifdef __HAS_C11

#ifndef __STDC_NO_ATOMICS__
#ifndef HAVE_STDATOMIC_H
#define HAVE_STDATOMIC_H 1
#endif // HAVE_STDATOMIC_H
#endif // __STDC_NO_ATOMICS__

#endif // __HAS_C11

/*
 *C++语言标准
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


#endif
