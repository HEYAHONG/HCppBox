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
#include "hmemory.h"
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

/** \brief hruntime初始化(底层)是否完成。
 *
 * \return bool 是否完成
 *
 */
bool hruntime_init_lowlevel_done(void);

/** \brief hruntime初始化
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中调用。
 *
 */
void hruntime_init(void);

/** \brief hruntime初始化是否完成。
 *
 * \return bool 是否完成
 *
 */
bool hruntime_init_done(void);

/** \brief hruntime循环
 * 一般由hcpprt组件使用，当不能使用C++时，需要在空闲任务或者优先级较低的任务中周期性调用。
 *
 */
void hruntime_loop(void);

/** \brief hruntime循环是否开始
 *
 * \return bool 是否开始,若结束标志也为假则表示从未进入 hruntime循环
 *
 */
bool hruntime_loop_begin(void);

/** \brief hruntime循环是否结束
 *
 * \return bool 是否结束
 *
 */
bool hruntime_loop_end(void);


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


typedef struct hruntime_symbol hruntime_symbol_t;
struct hruntime_symbol
{
    const char *symbol_name;
    uintptr_t   symbol_addr;
};

/** \brief 寻找符号（一般用于模块化时加载模块,与hdefaults的API表不同，此函数一般在模块加载过程中使用）
 *
 * \param name const char* 符号名称
 * \return const hruntime_symbol_t* 符号地址
 *
 */
const hruntime_symbol_t *hruntime_symbol_find(const char *name);


/** \brief 注册符号表
 *
 * \param table_start const hruntime_symbol_t* 符号表起始地址
 * \param table_size size_t              符号表大小
 * \return bool 是否成功
 *
 */
bool hruntime_symbol_dynamic_table_register(const hruntime_symbol_t *table_start,size_t table_size);

/** \brief 反注册符号表
 *
 * \param table_start const hruntime_symbol_t* 符号表起始地址
 * \param table_size size_t              符号表大小
 * \return bool 是否成功
 *
 */
bool hruntime_symbol_dynamic_table_unregister(const hruntime_symbol_t *table_start,size_t table_size);


/** \brief 寻找动态表中的符号（一般用于模块化时加载模块,与hdefaults的API表不同，此函数一般在模块加载过程中使用）
 *
 * \param name const char* 符号名称
 * \return const hruntime_symbol_t* 符号地址
 *
 */
const hruntime_symbol_t *hruntime_symbol_dynamic_find(const char *name);

#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
/*
 * armcc/armclang,使用名称为HRuntimeSymbol的section
 */

/*
 * 导出符号
 */
#define HRUNTIME_SYMBOL_EXPORT(name) \
    __USED\
    __SECTION("HRuntimeSymbol")\
    static const hruntime_symbol_t hruntime_symbol_##name = \
    {\
        #name ,\
        (uintptr_t)&name\
    }

/*
 * 调用导出的符号函数
 */
extern const  int HRuntimeSymbol$$Base;
extern const  int HRuntimeSymbol$$Limit;

#elif defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
/*
 * gcc/clang,使用名称为.HRuntimeSymbol的section
 * gcc/clang必须在链接脚本中提供__hruntime_symbol_start与__hruntime_symbol_end,脚本示例如下:
 *          PROVIDE ( __hruntime_symbol_start = . );
 *          KEEP (*(.HRuntimeSymbol))
 *          PROVIDE ( __hruntime_symbol_end = . );
 *
 */

/*
* 导出符号
*/
#define HRUNTIME_SYMBOL_EXPORT(name) \
    __USED\
    __SECTION(".HRuntimeSymbol")\
    static const hruntime_symbol_t hruntime_symbol_##name = \
    {\
        #name ,\
        (uintptr_t)&name\
    }

/*
 * 调用导出的符号函数
 */
extern const  hruntime_symbol_t __hruntime_symbol_start[];
extern const  hruntime_symbol_t __hruntime_symbol_end[];


#else
/*
 * 不支持的编译器（使用相关宏定义将无任何效果，也不会报错）
 */


/*
 * 导出符号
 */
#define HRUNTIME_SYMBOL_EXPORT(name)


#endif

#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_UNIX) || defined(HDEFAULTS_OS_CYGWIN) || defined(HDEFAULTS_OS_EMSCRIPTEN)
/*
 *  某些操作系统下，默认启用符号表
 */
#ifndef HRUNTIME_USING_SYMBOL_TABLE
#define HRUNTIME_USING_SYMBOL_TABLE     1
#endif

/*
 *  某些操作系统下，默认启用动态符号表
 */
#ifndef HRUNTIME_USING_SYMBOL_DYNAMIC_TABLE
#define HRUNTIME_USING_SYMBOL_DYNAMIC_TABLE     1
#endif
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRUNTIME_H__
