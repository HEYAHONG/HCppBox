
#if !defined(LIBMONO_BASE_GLOBALCONFIG_H)
#define LIBMONO_BASE_GLOBALCONFIG_H

/** \brief LIBMONO库数据属性定义，通常用于定义数据位置（如 __idata、__pdata、__xdata）
 *
 *
 */
#if !defined(LIBMONO_DATA_ATTRIBUTE)
#define LIBMONO_DATA_ATTRIBUTE
#endif

/** \brief LIBMONO库常量数据属性定义,通常可定义数据位置(如__code)
 *
 *
 */
#if !defined(LIBMONO_CONST_DATA_ATTRIBUTE)
#define LIBMONO_CONST_DATA_ATTRIBUTE
#endif

/** \brief LIBMONO库函数属性定义，可定义一些函数属性(如__reentrant)
 *
 *
 */
#if !defined(LIBMONO_FUNCTION_ATTRIBUTE)
#define LIBMONO_FUNCTION_ATTRIBUTE
#endif

/** \brief LIBMONO库可重入函数属性定义,与LIBMONO_FUNCTION_ATTRIBUTE不会同时使用
 *
 *
 *
 */
#if !defined(LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE)
#define LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE __reentrant
#endif

#endif

