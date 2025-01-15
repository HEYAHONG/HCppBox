/***************************************************************
 * Name:      hcompiler_icc.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_ICC_H__
#define __HCOMPILER_ICC_H__

#ifdef __ICCARM__
/*
IAR ARM编译器
*/

#pragma system_include

#define __IAR_FT _Pragma("inline=forced") __intrinsic

#if (__VER__ >= 8000000)
#define __ICCARM_V8 1
#else
#define __ICCARM_V8 0
#endif

#ifndef __ALIGNED
#if __ICCARM_V8
#define __ALIGNED(x) __attribute__((aligned(x)))
#elif (__VER__ >= 7080000)
/* Needs IAR language extensions */
#define __ALIGNED(x) __attribute__((aligned(x)))
#else
#warning No compiler specific solution for __ALIGNED.__ALIGNED is ignored.
#define __ALIGNED(x)
#endif
#endif


#ifndef __NO_INIT
#define __NO_INIT __attribute__ ((section (".noinit")))
#endif

#ifndef __ALIAS
#define __ALIAS(x) __attribute__ ((alias(x)))
#endif

#ifndef __ASM
#define __ASM __asm
#endif


#ifndef __INLINE
#define __INLINE inline
#endif

#ifndef   __NO_RETURN
#if __ICCARM_V8
#define __NO_RETURN __attribute__((__noreturn__))
#else
#define __NO_RETURN _Pragma("object_attribute=__noreturn")
#endif
#endif

#ifndef   __PACKED
#if __ICCARM_V8
#define __PACKED __attribute__((packed, aligned(1)))
#else
/* Needs IAR language extensions */
#define __PACKED __packed
#endif
#endif


#ifndef   __RESTRICT
#if __ICCARM_V8
#define __RESTRICT            __restrict
#else
/* Needs IAR language extensions */
#define __RESTRICT            restrict
#endif
#endif

#ifndef   __STATIC_INLINE
#define __STATIC_INLINE       static inline
#endif

#ifndef   __FORCEINLINE
#define __FORCEINLINE         _Pragma("inline=forced")
#endif

#ifndef   __STATIC_FORCEINLINE
#define __STATIC_FORCEINLINE  __FORCEINLINE __STATIC_INLINE
#endif

#ifndef   __USED
#if __ICCARM_V8
#define __USED __attribute__((used))
#else
#define __USED _Pragma("__root")
#endif
#endif

#undef __WEAK
#ifndef   __WEAK
#if __ICCARM_V8
#define __WEAK __attribute__((weak))
#else
#define __WEAK _Pragma("__weak")
#endif
#endif

#ifndef __SECTION
#define __SECTION(x)                          @ x
#endif

#endif // __ICCARM__


#endif
