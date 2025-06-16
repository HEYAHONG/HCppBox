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
#if (SIZE_MAX) <= (UINT16_MAX)
#define HDEFAULTS_BITS_16_OR_8      1
#elif (SIZE_MAX) <= (UINT32_MAX)
#define HDEFAULTS_BITS_32           1
#elif (SIZE_MAX) <= (UINT64_MAX)
#define HDEFAULTS_BITS_64           1
#else
#define HDEFAULTS_BITS_ABOVE_64     1
#endif

//定义硬件架构(支持部分常用架构)
#if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(__i486__) || defined(i486) || defined(__i586__) || defined(i586) || defined(__i686__) || defined(i686) || defined(__i786__) || defined(i786)
#include "hdefaults_arch_x86.h"
#ifndef HDEFAULTS_ARCH_X86
#define HDEFAULTS_ARCH_X86 1
#endif
#elif defined(_M_X64) || defined(__x86_64) || defined(__x86_64__)
#include "hdefaults_arch_x86_64.h"
#ifndef HDEFAULTS_ARCH_X86_64
#define HDEFAULTS_ARCH_X86_64 1
#endif
#elif defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
#include "hdefaults_arch_aarch64.h"
#ifndef HDEFAULTS_ARCH_AARCH64
#define HDEFAULTS_ARCH_AARCH64 1
#endif
#elif defined(_M_ARM) || defined(__arm__) || defined(__ARMEL__)
#include "hdefaults_arch_arm.h"
#ifndef HDEFAULTS_ARCH_ARM
#define HDEFAULTS_ARCH_ARM 1
#endif
/*
 * Thumb指令集,通常用于MCU
 */
#if defined(_M_THUMB) || defined(__thumb__) || defined(__thumb2__)|| defined(__THUMBEL__)
#include "hdefaults_arch_arm_thumb.h"
#ifndef HDEFAULTS_ARCH_ARM_THUMB
#define HDEFAULTS_ARCH_ARM_THUMB 1
#endif
#endif
#elif defined(__riscv)
#include "hdefaults_arch_riscv.h"
#ifndef HDEFAULTS_ARCH_RISCV
#define HDEFAULTS_ARCH_RISCV 1
#endif
#if (__riscv_xlen)==64
#ifndef HDEFAULTS_ARCH_RISCV64
#define HDEFAULTS_ARCH_RISCV64 1
#endif
#endif
#if (__riscv_xlen)==32
#ifndef HDEFAULTS_ARCH_RISCV32
#define HDEFAULTS_ARCH_RISCV32 1
#endif
#endif
#elif defined(__wasm) || defined(__wasm__)
#include "hdefaults_arch_wasm.h"
#ifndef HDEFAULTS_ARCH_WASM
#define HDEFAULTS_ARCH_WASM 1
#endif
/*
 * wasm默认情况下使用wasm32
 */
#if defined(__wasm32) || defined(__wasm32__)
#ifndef HDEFAULTS_ARCH_WASM32
#define HDEFAULTS_ARCH_WASM32 1
#endif
#endif
/*
 * wasm当指定MEMORY64=1时使用wasm64
 */
#if defined(__wasm64) || defined(__wasm64__)
#ifndef HDEFAULTS_ARCH_WASM64
#define HDEFAULTS_ARCH_WASM64 1
#endif
#endif
#endif


//定义操作系统

//RT-Thread
#ifdef __RTTHREAD__
#include "hdefaults_os_rtthread.h"
#ifndef HDEFAULTS_OS_RTTHREAD
#define HDEFAULTS_OS_RTTHREAD 1
#endif // HDEFAULTS_OS_RTTHREAD
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __RTTHREAD__

//NuttX
#ifdef __NuttX__
#include "hdefaults_os_nuttx.h"
#ifndef HDEFAULTS_OS_NUTTX
#define HDEFAULTS_OS_NUTTX 1
#endif // HDEFAULTS_OS_NUTTX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __NuttX__

//Windows
//提高windows下的兼容性
#ifdef _WIN32
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // _WIN32
#ifdef __WIN32
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // __WIN32
#ifdef __WIN32__
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // __WIN32__
#ifdef WIN32
#include "hdefaults_os_windows.h"
#ifndef HDEFAULTS_OS_WINDOWS
#define HDEFAULTS_OS_WINDOWS 1
#endif // HDEFAULTS_OS_WINDOWS
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // WIN32

//(类)unix,注意：Linux也使用此配置。
#ifdef __unix__
#include "hdefaults_os_unix.h"
#ifndef HDEFAULTS_OS_UNIX
#define HDEFAULTS_OS_UNIX 1
#endif // HDEFAULTS_OS_UNIX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __unix__

//linux
#ifdef __linux__
#include "hdefaults_os_linux.h"
#ifndef HDEFAULTS_OS_LINUX
#define HDEFAULTS_OS_LINUX 1
#endif // HDEFAULTS_OS_LINUX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//FreeBSD
#ifdef __FreeBSD__
#include "hdefaults_os_freebsd.h"
#ifndef HDEFAULTS_OS_FREEBSD
#define HDEFAULTS_OS_FREEBSD 1
#endif // HDEFAULTS_OS_FREEBSD
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//Android
#ifdef __ANDROID__
#include "hdefaults_os_android.h"
#ifndef HDEFAULTS_OS_ANDROID
#define HDEFAULTS_OS_ANDROID 1
#endif // HDEFAULTS_OS_ANDROID
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//Cygwin
#ifdef __CYGWIN__
#include "hdefaults_os_cygwin.h"
#ifndef HDEFAULTS_OS_CYGWIN
#define HDEFAULTS_OS_CYGWIN 1
#endif // HDEFAULTS_OS_CYGWIN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __CYGWIN__

//emscripten
#ifdef __EMSCRIPTEN__
#include "hdefaults_os_emscripten.h"
#ifndef HDEFAULTS_OS_EMSCRIPTEN
#define HDEFAULTS_OS_EMSCRIPTEN 1
#endif // HDEFAULTS_OS_EMSCRIPTEN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __EMSCRIPTEN__


//无操作系统(裸机)
#ifndef HDEFAULTS_HAVE_OS
#include "hdefaults_os_none.h"
#ifndef HDEFAULTS_OS_NONE
#define HDEFAULTS_OS_NONE 1
#endif // HDEFAULTS_OS_NONE
#endif

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


//导入配置文件

//导入config.h
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef hdefaults_xstr
#define hdefaults_xstr(s) hdefaults_str(s)
#endif // hdefaults_xstr


#ifndef hdefaults_str
#define hdefaults_str(s) #s
#endif // hdefaults_str

//导入外部配置，使用命令行定义HBOX_CONFIG_HEADER指定外部配置文件名。
#ifdef HBOX_CONFIG_HEADER
#include hdefaults_xstr(HBOX_CONFIG_HEADER)
#endif


//定义hdefaults_tick_t
#ifdef HDEFAULTS_OS_RTTHREAD
#define hdefaults_tick_t rt_tick_t
#endif // HDEFAULTS_OS_RTTHREAD
#ifdef HDEFAULTS_OS_WINDOWS
#define hdefaults_tick_t DWORD
#endif // HDEFAULTS_OS_WINDOWS
#ifdef HDEFAULTS_OS_UNIX
#include <time.h>
#include <sys/time.h>
#endif // HDEFAULTS_OS_UNIX
#ifndef hdefaults_tick_t
#define hdefaults_tick_t uint32_t
#endif // hdefaults_tick_t

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 获取当前节拍
 *
 * \return hdefaults_tick_t 当前节拍(毫秒),注意，并非启动时间
 *
 */
hdefaults_tick_t hdefaults_tick_get(void);

/** \brief 默认内存分配
 *
 * \param nBytes size_t 待分配的字节数
 * \param usr void* 用户指针(一般不用)
 * \return void* 分配的指针
 *
 */
void * hdefaults_malloc(size_t nBytes,void *usr);

/** \brief 默认内存释放
 *
 * \param ptr void* 待释放的指针
 * \param usr void* 用户指针(一般不用)
 *
 */
void hdefaults_free(void *ptr,void *usr);

/** \brief 互斥锁加锁(默认是通过进出临界区实现,而非直接创建互斥锁)
 *
 * \param usr void*用户指针(一般不用)
 *
 */
void  hdefaults_mutex_lock(void *usr);

/** \brief 互斥锁解锁(默认是通过进出临界区实现,而非直接创建互斥锁)
 *
 * \param usr void*用户指针(一般不用)
 *
 */
void  hdefaults_mutex_unlock(void *usr);

enum
{
    HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_BEGIN=0,                                 /**< 为syscall保留的调用号，一般调用号均从此值开始 */
    HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_END=1024,                                /**< 为syscall保留的调用号, 常见系统的调用号均不超过此值（一般在500左右）*/
    HDEFAULTS_USERCALL_NUMBER_BEGIN=HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_END,     /**< 一般调用号均从此值开始 */
    HDEFAULTS_USERCALL_NUMBER_TICK=HDEFAULTS_USERCALL_NUMBER_BEGIN,                     /**< 同hdefaults_tick_get*/
    HDEFAULTS_USERCALL_NUMBER_MALLOC,                                                   /**< 同hdefaults_malloc*/
    HDEFAULTS_USERCALL_NUMBER_FREE,                                                     /**< 同hdefaults_free,注意：返回值类型为int*/
    HDEFAULTS_USERCALL_NUMBER_GLOCK,                                                    /**< 同 hdefaults_mutex_lock,注意：返回值类型为int */
    HDEFAULTS_USERCALL_NUMBER_GUNLOCK,                                                  /**< 同 hdefaults_mutex_unlock,注意：返回值类型为int */
    HDEFAULTS_USERCALL_NUMBER_END=4096,                                                 /**< 调用号均不超过此值 */
};


/** \brief 默认用户调用返回值检查
 *
 * \param usercall_number uintptr_t 用户调用号
 * \param return_value intptr_t 返回值（注意:这不是用户调用实现的返回值,通常用于找不到相应调用时使用）
 * \return intptr_t 同返回值
 *
 */
intptr_t __hdefaults_usercall_return_check(uintptr_t usercall_number,intptr_t return_value);


/** \brief 用户调用返回值检查(如需使用，需要#undef重新#define)
 *
 *
 */
#define hdefaults_usercall_return_check(usercall_number,return_value) __hdefaults_usercall_return_check(usercall_number,return_value)

/** \brief 用户调用
 *
 * \param usercall_number 用户调用号,见HDEFAULTS_USERCALL_NUMBER_*
 * \param ...             参数（通常第一个参数才是返回值地址（若需要返回值））
 *
 */
#define hdefaults_usercall(usercall_number,...) \
        {\
            const hdefaults_api_table_t *api_table=hdefaults_get_api_table();\
            intptr_t hdefaults_usercall_ret_value=0;\
            if(api_table!=NULL && api_table->usercall!=NULL)\
            {\
                    hdefaults_usercall_ret_value=api_table->usercall((uintptr_t)(usercall_number),##__VA_ARGS__);\
            }\
            hdefaults_usercall_return_check(usercall_number,hdefaults_usercall_ret_value);\
        }

#ifdef __cplusplus
#define HDEFAULTS_USERCALL_DECLARE(NAME) \
        extern "C"\
        {\
            extern intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args);\
            extern intptr_t hdefaults_usercall_##NAME(uintptr_t number,...);\
        }

#else
#define HDEFAULTS_USERCALL_DECLARE(NAME) \
        extern intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args);\
        extern intptr_t hdefaults_usercall_##NAME(uintptr_t number,...);
#endif

/*
 * 用于定义参数列表，P表示处理除最后一对参数的参数，E表示处理最后一对参数，K表示键名，V表示键值
 */
#define __HDEFAULTS_MAP1(P,E,K,V)       E(K,V)
#define __HDEFAULTS_MAP2(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP1(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP3(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP2(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP4(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP3(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP5(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP4(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP6(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP5(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP7(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP6(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP8(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP7(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP9(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP8(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP10(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP9(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP11(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP10(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP12(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP11(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP13(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP12(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP14(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP13(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP15(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP14(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP16(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP15(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP17(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP16(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP18(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP17(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP19(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP18(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP20(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP19(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP21(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP20(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP22(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP21(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP23(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP22(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP24(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP23(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP25(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP24(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP26(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP24(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP27(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP26(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP28(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP27(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP29(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP28(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP30(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP29(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP31(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP30(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP32(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP31(P,E,##__VA_ARGS__)

#define HDEFAULTS_USERCALL_PARAMETER_PROCESS(K,V)       K V,
#define HDEFAULTS_USERCALL_PARAMETER_END(K,V)           K V
#define HDEFAULTS_USERCALL_PARAMETER_CALL_PROCESS(K,V)  V,
#define HDEFAULTS_USERCALL_PARAMETER_CALL_END(K,V)      V
#define HDEFAULTS_USERCALL_VA_LOCAL_PROCESS(K,V)        K V=va_arg(va_list_args,K);
#define HDEFAULTS_USERCALL_VA_LOCAL_END(K,V)            K V=va_arg(va_list_args,K);


#define HDEFAULTS_USERCALL_DEFINEx(X,NAME,NUMBER,RETURN_TYPE,...) \
        HDEFAULTS_USERCALL_DECLARE(NAME)\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_PROCESS,HDEFAULTS_USERCALL_PARAMETER_END,__VA_ARGS__));\
        intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args)\
        {\
            if(number!=(NUMBER))\
            {\
                return -1;\
            }\
            RETURN_TYPE *__hdefaults_usercall_##NAME##_return_ptr=(RETURN_TYPE *)va_arg(va_list_args,RETURN_TYPE *);\
            __HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_VA_LOCAL_PROCESS,HDEFAULTS_USERCALL_VA_LOCAL_END,__VA_ARGS__);\
            RETURN_TYPE __hdefaults_usercall_##NAME##_return_value;\
            __hdefaults_usercall_##NAME##_return_value=do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_CALL_PROCESS,HDEFAULTS_USERCALL_PARAMETER_CALL_END,##__VA_ARGS__));\
            if(__hdefaults_usercall_##NAME##_return_ptr!=NULL)\
            {\
                (*__hdefaults_usercall_##NAME##_return_ptr)=__hdefaults_usercall_##NAME##_return_value;\
            }\
            return 0;\
        }\
        intptr_t hdefaults_usercall_##NAME(uintptr_t number,...)\
        {\
            va_list va;\
            va_start(va,number);\
            intptr_t ret=__hdefaults_usercall_##NAME(number,va);\
            va_end(va);\
            return ret;\
        }\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_PROCESS,HDEFAULTS_USERCALL_PARAMETER_END,##__VA_ARGS__))

#define HDEFAULTS_USERCALL_DEFINE0(NAME,NUMBER,RETURN_TYPE) \
        HDEFAULTS_USERCALL_DECLARE(NAME)\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(void);\
        intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args)\
        {\
            if(number!=(NUMBER))\
            {\
                return -1;\
            }\
            RETURN_TYPE *__hdefaults_usercall_##NAME##_return_ptr=(RETURN_TYPE *)va_arg(va_list_args,RETURN_TYPE *);\
            RETURN_TYPE __hdefaults_usercall_##NAME##_return_value;\
            __hdefaults_usercall_##NAME##_return_value=do_hdefaults_usercall_##NAME();\
            if(__hdefaults_usercall_##NAME##_return_ptr!=NULL)\
            {\
                (*__hdefaults_usercall_##NAME##_return_ptr)=__hdefaults_usercall_##NAME##_return_value;\
            }\
            return 0;\
        }\
        intptr_t hdefaults_usercall_##NAME(uintptr_t number,...)\
        {\
            va_list va;\
            va_start(va,number);\
            intptr_t ret=__hdefaults_usercall_##NAME(number,va);\
            va_end(va);\
            return ret;\
        }\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(void)


#define HDEFAULTS_USERCALL_DEFINE1(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(1,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE2(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(2,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE3(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(3,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE4(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(4,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE5(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(5,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE6(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(6,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE7(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(7,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE8(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(8,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE9(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(9,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE10(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(10,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE11(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(11,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE12(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(12,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE13(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(13,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE14(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(14,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE15(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(15,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE16(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(16,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE17(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(17,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE18(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(18,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE19(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(19,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE20(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(20,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE21(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(21,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE22(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(22,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE23(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(23,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE24(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(24,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE25(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(25,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE26(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(26,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE27(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(27,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE28(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(28,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE29(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(29,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE30(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(30,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE31(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(31,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE32(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(32,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)


/*
 *  hdefaults默认API表
 */
typedef struct hdefaults_api_table
{
    hdefaults_tick_t    (*tick_get)(void);
    void *              (*mem_alloc)(size_t,void *);
    void                (*mem_free)(void *,void *);
    void                (*mutex_lock)(void *);
    void                (*mutex_unlock)(void *);
    intptr_t            (*usercall)(uintptr_t,...);
} hdefaults_api_table_t;

/** \brief 获取API表
 *
 * \return const hdefaults_api_table_t* 当前API表地址
 *
 */
const hdefaults_api_table_t * hdefaults_get_api_table(void);

/** \brief 设置API表
 *  注意：在运行过程中改变API表是危险操作
 *
 * \param new_api_table const hdefaults_api_table_t* 新API表地址
 * \return const hdefaults_api_table_t* 原API表地址
 *
 */
const hdefaults_api_table_t * hdefaults_set_api_table(const hdefaults_api_table_t* new_api_table);

#include "syscall/hdefaults_syscall.h"
#include "libc/hdefaults_libc_port.h"
#include "libdl/hdefaults_libdl_port.h"


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDEFAULTS_H_INCLUDED
