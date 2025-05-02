/***************************************************************
 * Name:      hruntime.h
 * Purpose:   声明hruntime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRUNTIME_H__
#define __HRUNTIME_H__
#include "hdefaults.h"
#include "hcompiler.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief hruntime初始化(底层)。
 * 一般由hcpprt组件使用，当不能使用C++时，需要在hruntime_init前调用，此函数可不在任务中调用。
 *
 *
 */
void hruntime_init_lowlevel(void);


/** \brief hruntime初始化
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中调用。
 *
 */
void hruntime_init(void);

/** \brief hruntime循环
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中周期性调用。
 *
 */
void hruntime_loop(void);

/*
 * 运行时函数(用于初始化或者循环中使用)
 */
typedef struct hruntime_function hruntime_function_t;
typedef void  (*hruntime_function_entry_t)(const hruntime_function_t *func);
struct hruntime_function
{
    size_t                      priority;           /**< 优先级,越小表示优先级越高 */
    hruntime_function_entry_t   entry;              /**< 入口 */
    void *                      usr;                /**< 用户参数 */
};


/** \brief 运行时函数数组
 * 一般用于使用段进行初始化与循环时，供内部使用
 *
 * \param array_base const hruntime_function_t* 运行时函数数组
 * \param array_size size_t 运行时函数数组大小
 *
 */
void hruntime_function_array_invoke(const hruntime_function_t *array_base,size_t array_size);

#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
/*
 * armcc/armclang,使用名称为HRuntimeInit的section
 */

/*
 * 导出初始化
 */
#define HRUNTIME_INIT_EXPORT(name,priority,entry,usr) \
    __USED\
    __SECTION("HRuntimeInit")\
    static const hruntime_function_t hruntime_init_##name = \
    {\
        (priority),\
        (entry) ,\
        (usr) \
    }

/*
 * 调用导出的初始化函数
 */
extern const  int HRuntimeInit$$Base;
extern const  int HRuntimeInit$$Limit;
#define HRUNTIME_INIT_INVOKE() \
hruntime_function_array_invoke((hruntime_function_t *)&HRuntimeInit$$Base,(((uintptr_t)(hruntime_function_t *)&HRuntimeInit$$Limit)-((uintptr_t)(hruntime_function_t *)&HRuntimeInit$$Base))/sizeof(hruntime_function_t))

#elif defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
/*
 * gcc/clang,使用名称为.HRuntimeInit的section
 * gcc/clang必须在链接脚本中提供__hruntime_init_start与__hruntime_init_end,脚本示例如下:
 *          PROVIDE ( __hruntime_init_start = . );
 *          KEEP (*(.HRuntimeInit))
 *          PROVIDE ( __hruntime_init_end = . );
 *
 */

/*
* 导出初始化
*/
#define HRUNTIME_INIT_EXPORT(name,priority,entry,usr) \
    __USED\
    __SECTION(".HRuntimeInit")\
    static const hruntime_function_t hruntime_init_##name = \
    {\
        (priority),\
        (entry) ,\
        (usr) \
    }

/*
 * 调用导出的初始化函数
 */
extern const  hruntime_function_t __hruntime_init_start[];
extern const  hruntime_function_t __hruntime_init_end[];
#define HRUNTIME_INIT_INVOKE() \
hruntime_function_array_invoke(__hruntime_init_start,(((uintptr_t)__hruntime_init_end)-((uintptr_t)__hruntime_init_start))/sizeof(hruntime_function_t))




#else
/*
 * 不支持的编译器（使用相关宏定义将无任何效果，也不会报错）
 */


/*
 * 导出初始化
 */
#define HRUNTIME_INIT_EXPORT(name,priority,entry,usr)

/*
 * 调用导出的初始化函数
 */
#define HRUNTIME_INIT_INVOKE()


#endif


#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
/*
 * armcc/armclang,使用名称为HRuntimeLoop的section
 */

/*
 * 导出循环
 */
#define HRUNTIME_LOOP_EXPORT(name,priority,entry,usr) \
    __USED\
    __SECTION("HRuntimeLoop")\
    static const hruntime_function_t hruntime_loop_##name = \
    {\
        (priority),\
        (entry) ,\
        (usr) \
    }

/*
 * 调用导出的循环函数
 */
extern const  int HRuntimeLoop$$Base;
extern const  int HRuntimeLoop$$Limit;
#define HRUNTIME_LOOP_INVOKE() \
hruntime_function_array_invoke((hruntime_function_t *)&HRuntimeLoop$$Base,(((uintptr_t)(hruntime_function_t *)&HRuntimeLoop$$Limit)-((uintptr_t)(hruntime_function_t *)&HRuntimeLoop$$Base))/sizeof(hruntime_function_t))

#elif defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
/*
 * gcc/clang,使用名称为.HRuntimeLoop的section
 * gcc/clang必须在链接脚本中提供__hruntime_loop_start与__hruntime_loop_end,脚本示例如下:
 *          PROVIDE ( __hruntime_loop_start = . );
 *          KEEP (*(.HRuntimeLoop))
 *          PROVIDE ( __hruntime_loop_end = . );
 *
 */

/*
* 导出循环
*/
#define HRUNTIME_LOOP_EXPORT(name,priority,entry,usr) \
    __USED\
    __SECTION(".HRuntimeLoop")\
    static const hruntime_function_t hruntime_loop_##name = \
    {\
        (priority),\
        (entry) ,\
        (usr) \
    }

/*
 * 调用导出的循环函数
 */
extern const  hruntime_function_t __hruntime_loop_start[];
extern const  hruntime_function_t __hruntime_loop_end[];
#define HRUNTIME_LOOP_INVOKE() \
hruntime_function_array_invoke(__hruntime_loop_start,(((uintptr_t)__hruntime_loop_end)-((uintptr_t)__hruntime_loop_start))/sizeof(hruntime_function_t))




#else
/*
 * 不支持的编译器（使用相关宏定义将无任何效果，也不会报错）
 */


/*
 * 导出循环
 */
#define HRUNTIME_LOOP_EXPORT(name,priority,entry,usr)

/*
 * 调用导出的循环函数
 */
#define HRUNTIME_LOOP_INVOKE()


#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRUNTIME_H__
