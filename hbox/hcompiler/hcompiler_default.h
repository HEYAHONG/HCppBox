/***************************************************************
 * Name:      hcompiler_default.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_DEFAULT_H__
#define __HCOMPILER_DEFAULT_H__

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


#endif
