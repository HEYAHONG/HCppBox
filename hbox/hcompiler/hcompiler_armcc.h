/***************************************************************
 * Name:      hcompiler_armcc.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_ARMCC_H__
#define __HCOMPILER_ARMCC_H__

/*
Arm Compiler 4/5 (keil mdk)
*/
#ifndef   __ASM
#define __ASM                                  __asm
#endif

#ifndef   __INLINE
#define __INLINE                               __inline
#endif

#ifndef   __STATIC_INLINE
#define __STATIC_INLINE                        static __inline
#endif

#ifndef   __STATIC_FORCEINLINE
#define __STATIC_FORCEINLINE                   static __forceinline
#endif

#ifndef   __NO_RETURN
#define __NO_RETURN                            __declspec(noreturn)
#endif

#ifndef   __USED
#define __USED                                 __attribute__((used))
#endif

#ifndef   __WEAK
#define __WEAK                                 __attribute__((weak))
#endif

#ifndef   __PACKED
#define __PACKED                               __attribute__((packed))
#endif

#ifndef   __ALIGNED
#define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

#ifndef   __RESTRICT
#define __RESTRICT                             __restrict
#endif

#ifndef __NO_INIT
#define __NO_INIT                              __attribute__ ((section (".bss.noinit"), zero_init))
#endif

#ifndef __ALIAS
#define __ALIAS(x)                             __attribute__ ((alias(x)))
#endif

#ifndef __SECTION
#define __SECTION(x)                          __attribute__  ((section(x)))
#endif


#endif
