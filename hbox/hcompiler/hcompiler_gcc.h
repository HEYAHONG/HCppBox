/***************************************************************
 * Name:      hcompiler_gcc.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_GCC_H__
#define __HCOMPILER_GCC_H__

#ifndef   __ASM
#define __ASM                                  __asm
#endif

#ifndef   __INLINE
#define __INLINE                               inline
#endif

#ifndef   __STATIC_INLINE
#define __STATIC_INLINE                        static inline
#endif

#ifndef   __STATIC_FORCEINLINE
#define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif

#ifndef   __NO_RETURN
#define __NO_RETURN                            __attribute__((__noreturn__))
#endif

#ifndef   __USED
#define __USED                                 __attribute__((used))
#endif

#ifndef   __WEAK
#define __WEAK                                 __attribute__((weak))
#endif

#ifndef   __PACKED
#define __PACKED                               __attribute__((packed, aligned(1)))
#endif

#ifndef   __ALIGNED
#define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

#ifndef   __RESTRICT
#define __RESTRICT                             __restrict
#endif

#ifndef __NO_INIT
#define __NO_INIT                              __attribute__ ((section (".bss.noinit")))
#endif

#ifndef __ALIAS
#define __ALIAS(x)                             __attribute__ ((alias(x)))
#endif

#ifndef __SECTION
#define __SECTION(x)                          __attribute__((section(x)))
#endif

#endif
