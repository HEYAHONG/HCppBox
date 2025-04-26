/***************************************************************
 * Name:      hobject.h
 * Purpose:   声明hobject接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HOBJECT_H_INCLUDED__
#define __HOBJECT_H_INCLUDED__

/*
本文件主要用于实现一些基于C语言的简易的对象操作,可配合其它文件使用。
    主要包含以下对象:指针、double,无符号整数，有符号整数
    本文件不使用内存分配函数,若要创建一个对象，需要手动将结构体设置为初始化值
*/


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

typedef enum
{
    HOBJECT_BASE_TYPE_INVALID=0,/**< 无效类型 */
    HOBJECT_BASE_TYPE_SIMPLE_PTR=1,/**< 简单指针 */
    HOBJECT_BASE_TYPE_ARRAY_PTR,/**< 数组指针 */
    HOBJECT_BASE_TYPE_MANAGED_PTR,/**< 管理的指针(即带释放回调的指针) */
    HOBJECT_BASE_TYPE_MANAGED_ARRAY_PTR,/**< 管理的数组指针(即带释放回调的指针) */
    HOBJECT_BASE_TYPE_DOUBLE,/**< 浮点数 */
    HOBJECT_BASE_TYPE_INT8, /**< 整型(8位) */
    HOBJECT_BASE_TYPE_UINT8, /**< 无符号整型(8位) */
    HOBJECT_BASE_TYPE_INT16, /**< 整型(16位) */
    HOBJECT_BASE_TYPE_UINT16, /**< 无符号整型(16位) */
    HOBJECT_BASE_TYPE_INT32, /**< 整型(32位) */
    HOBJECT_BASE_TYPE_UINT32, /**< 无符号整型(32位) */
    HOBJECT_BASE_TYPE_INT64, /**< 整型(64位) */
    HOBJECT_BASE_TYPE_UINT64, /**< 无符号整型(64位) */
    HOBJECT_BASE_TYPE_STRUCT, /**< 结构体(实际的结构体附在结构体基类之后,通常使用动态内存进行分配) */
    HOBJECT_BASE_TYPE_MANAGED_STRUCT /**< 管理的结构体(实际的结构体附在结构体基类之后,通常使用动态内存进行分配,并带释放回调) */
} hobject_base_type_t; /**< 基本对象类型 */

typedef struct
{
    size_t ref_cnt;     /**< 引用计数,用于高层应用使用，本组件不使用且初始化为0（对于静态变量/栈上的变量而言，可为0）*/
    uint16_t type;      /**< 类型，类型必须为hobject_base_type_t规定的类型 */
    uint16_t usr_type;  /**< 用户类型,由用户自定义 */
} hobject_base_t;       /**< 对象基类 */

/** \brief 清理（注意:清理后将不能再使用该指针，除开free）
 *
 * \param obj_ptr hobject_base_t* hobject_base_t指针(仅限由派生类转化的指针)
 *
 */
void hobject_cleanup(hobject_base_t *obj_ptr);

/** \brief 从对象指针中获取基类指针(仅限由基类派生的对象)
 *
 * \param  对象指针
 * \return 对象基类指针
 *
 */
#define HOBJECT_BASE(obj_ptr) ((hobject_base_t *)(void *)obj_ptr)

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 指针
    *
    */
    void *ptr;

} hobject_simple_ptr_t; /**< SIMPLE_PTR结构体 */

#define HOBJECT_SIMPLE_PTR_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},NULL} /**< SIMPLE_PTR初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_simple_ptr_t初始化
 *
 * \param obj_ptr hobject_simple_ptr_t* hobject_simple_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param ptr void* 指针
 *
 */
void hobject_simple_ptr_init(hobject_simple_ptr_t *obj_ptr,uint16_t usr_type,void *ptr);

/** \brief  hobject_simple_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_simple_ptr_t* hobject_simple_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_simple_ptr_base(hobject_simple_ptr_t *obj_ptr);

/** \brief 获取hobject_simple_ptr_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_simple_ptr_t* hobject_simple_ptr_t对象指针,失败返回NULL
 *
 */
hobject_simple_ptr_t * hobject_simple_ptr(hobject_base_t *base);


typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数组首地址
    *
    */
    void *array_header;
    /** \brief 数组大小(即数组成员个数，非数组占用的字节数)
    *
    */
    size_t array_size;

} hobject_array_ptr_t; /**< ARRAY_PTR结构体 */

#define HOBJECT_ARRAY_PTR_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},NULL,0} /**< ARRAY_PTR初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_array_ptr_t初始化
 *
 * \param obj_ptr hobject_array_ptr_t* hobject_array_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param array_header void* 数组首地址
 * \param array_size size_t 数组大小(即数组成员个数，非数组占用的字节数)
 *
 */
void hobject_array_ptr_init(hobject_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size);

/** \brief hobject_array_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_array_ptr_t* hobject_array_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_array_ptr_base(hobject_array_ptr_t *obj_ptr);

/** \brief 获取hobject_array_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_array_ptr_t* hobject_array_ptr_t对象指针,失败返回NULL
 *
 */
hobject_array_ptr_t *hobject_array_ptr(hobject_base_t *base);

typedef struct __hobject_managed_ptr
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 指针
    *
    */
    void *ptr;
    /** \brief 释放回调，一般用于清理ptr,一般使用hobject_managed_ptr_cleanup自动调用
    *
    */
    void (*onfree)(struct __hobject_managed_ptr *obj_ptr);
} hobject_managed_ptr_t; /**< MANAGED_PTR结构体 */

#define HOBJECT_MANAGED_PTR_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},NULL,NULL} /**< MANAGED_PTR初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_managed_ptr_t初始化
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param ptr void* 指针
 * \param onfree 释放回调
 *
 */
void hobject_managed_ptr_init(hobject_managed_ptr_t *obj_ptr,uint16_t usr_type,void *ptr,void (*onfree)(hobject_managed_ptr_t *obj_ptr));


/** \brief 清理hobject_managed_ptr_t对象
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 *
 */
void hobject_managed_ptr_cleanup(hobject_managed_ptr_t *obj_ptr);

/** \brief hobject_managed_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_managed_ptr_t* hobject_managed_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_managed_ptr_base(hobject_managed_ptr_t *obj_ptr);

/** \brief 获取hobject_managed_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_managed_ptr_t* hobject_managed_ptr_t对象指针,失败返回NULL
 *
 */
hobject_managed_ptr_t *hobject_managed_ptr(hobject_base_t *base);

typedef struct __hobject_managed_array_ptr
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数组首地址
    *
    */
    void *array_header;
    /** \brief 数组大小(即数组成员个数，非数组占用的字节数)
    *
    */
    size_t array_size;
    /** \brief 释放回调，一般用于清理array_header,一般使用hobject_managed_array_ptr_cleanup自动调用
    *
    */
    void (*onfree)(struct __hobject_managed_array_ptr *obj_ptr);

} hobject_managed_array_ptr_t; /**< MANAGED_ARRAY_PTR结构体 */

#define HOBJECT_MANAGED_ARRAY_PTR_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},NULL,0,NULL} /**< MANAGED_ARRAY_PTR初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_managed_array_ptr_t初始化
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param array_header void* 数组首地址
 * \param array_size size_t 数组大小(即数组成员个数，非数组占用的字节数)
 * \param onfree 释放回调
 *
 */
void hobject_managed_array_ptr_init(hobject_managed_array_ptr_t *obj_ptr,uint16_t usr_type,void *array_header,size_t array_size,void (*onfree)(hobject_managed_array_ptr_t *obj_ptr));

/** \brief 清理hobject_managed_array_ptr_t对象
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象
 *
 */
void hobject_managed_array_ptr_cleanup(hobject_managed_array_ptr_t *obj_ptr);

/** \brief hobject_managed_array_ptr_t获取基类指针
 *
 * \param obj_ptr hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t *hobject_managed_array_ptr_base(hobject_managed_array_ptr_t *obj_ptr);

/** \brief 获取hobject_managed_array_ptr_t对象指针
 *
 * \param base hobject_base_t* hobject_base_t基类指针
 * \return hobject_managed_array_ptr_t* hobject_managed_array_ptr_t对象指针,失败返回NULL
 *
 */
hobject_managed_array_ptr_t *hobject_managed_array_ptr(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    double data;

} hobject_double_t; /**< DOUBLE结构体 */

#define HOBJECT_DOUBLE_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< DOUBLE初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_double_t初始化
 *
 * \param obj_ptr hobject_double_t* hobject_double_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data double 数据
 *
 */
void hobject_double_init(hobject_double_t *obj_ptr,uint16_t usr_type,double data);

/** \brief  hobject_double_t获取基类指针
 *
 * \param obj_ptr hobject_double_t* hobject_double_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_double_base(hobject_double_t *obj_ptr);

/** \brief 获取hobject_double_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_double_t* hobject_double_t对象指针,失败返回NULL
 *
 */
hobject_double_t * hobject_double(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    int8_t data;

} hobject_int8_t; /**< INT8结构体 */

#define HOBJECT_INT8_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< INT8初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_int8_t初始化
 *
 * \param obj_ptr hobject_int8_t* hobject_int8_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data int8_t 数据
 *
 */
void hobject_int8_init(hobject_int8_t *obj_ptr,uint16_t usr_type,int8_t data);

/** \brief  hobject_int8_t获取基类指针
 *
 * \param obj_ptr hobject_int8_t* hobject_int8_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_int8_base(hobject_int8_t *obj_ptr);

/** \brief 获取hobject_int8_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_int8_t* hobject_int8_t对象指针,失败返回NULL
 *
 */
hobject_int8_t * hobject_int8(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    uint8_t data;

} hobject_uint8_t; /**< UINT8结构体 */

#define HOBJECT_UINT8_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< UINT8初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_uint8_t初始化
 *
 * \param obj_ptr hobject_uint8_t* hobject_uint8_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data uint8_t 数据
 *
 */
void hobject_uint8_init(hobject_uint8_t *obj_ptr,uint16_t usr_type,uint8_t data);

/** \brief  hobject_uint8_t获取基类指针
 *
 * \param obj_ptr hobject_uint8_t* hobject_uint8_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_uint8_base(hobject_uint8_t *obj_ptr);

/** \brief 获取hobject_uint8_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_uint8_t* hobject_uint8_t对象指针,失败返回NULL
 *
 */
hobject_uint8_t * hobject_uint8(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    int16_t data;

} hobject_int16_t; /**< INT16结构体 */

#define HOBJECT_INT16_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< INT16初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_int16_t初始化
 *
 * \param obj_ptr hobject_int16_t* hobject_int16_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data int16_t 数据
 *
 */
void hobject_int16_init(hobject_int16_t *obj_ptr,uint16_t usr_type,int16_t data);

/** \brief  hobject_int16_t获取基类指针
 *
 * \param obj_ptr hobject_int16_t* hobject_int16_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_int16_base(hobject_int16_t *obj_ptr);

/** \brief 获取hobject_int16_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_int16_t* hobject_int16_t对象指针,失败返回NULL
 *
 */
hobject_int16_t * hobject_int16(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    uint16_t data;

} hobject_uint16_t; /**< UINT16结构体 */

#define HOBJECT_UINT16_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< UINT16初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_uint16_t初始化
 *
 * \param obj_ptr hobject_uint16_t* hobject_uint16_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data uint16_t 数据
 *
 */
void hobject_uint16_init(hobject_uint16_t *obj_ptr,uint16_t usr_type,uint16_t data);

/** \brief  hobject_uint16_t获取基类指针
 *
 * \param obj_ptr hobject_uint16_t* hobject_uint16_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_uint16_base(hobject_uint16_t *obj_ptr);

/** \brief 获取hobject_uint16_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_uint16_t* hobject_uint16_t对象指针,失败返回NULL
 *
 */
hobject_uint16_t * hobject_uint16(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    int32_t data;

} hobject_int32_t; /**< INT32结构体 */

#define HOBJECT_INT32_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< INT32初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_int32_t初始化
 *
 * \param obj_ptr hobject_int32_t* hobject_int32_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data int32_t 数据
 *
 */
void hobject_int32_init(hobject_int32_t *obj_ptr,uint16_t usr_type,int32_t data);

/** \brief  hobject_int32_t获取基类指针
 *
 * \param obj_ptr hobject_int32_t* hobject_int32_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_int32_base(hobject_int32_t *obj_ptr);

/** \brief 获取hobject_int32_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_int32_t* hobject_int32_t对象指针,失败返回NULL
 *
 */
hobject_int32_t * hobject_int32(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    uint32_t data;

} hobject_uint32_t; /**< UINT32结构体 */

#define HOBJECT_UINT32_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< UINT32初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_uint32_t初始化
 *
 * \param obj_ptr hobject_uint32_t* hobject_uint32_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data uint32_t 数据
 *
 */
void hobject_uint32_init(hobject_uint32_t *obj_ptr,uint16_t usr_type,uint32_t data);

/** \brief  hobject_uint32_t获取基类指针
 *
 * \param obj_ptr hobject_uint32_t* hobject_uint32_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_uint32_base(hobject_uint32_t *obj_ptr);

/** \brief 获取hobject_uint32_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_uint32_t* hobject_uint32_t对象指针,失败返回NULL
 *
 */
hobject_uint32_t * hobject_uint32(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    int64_t data;

} hobject_int64_t; /**< INT64结构体 */

#define HOBJECT_INT64_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< INT64初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_int64_t初始化
 *
 * \param obj_ptr hobject_int64_t* hobject_int64_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data int64_t 数据
 *
 */
void hobject_int64_init(hobject_int64_t *obj_ptr,uint16_t usr_type,int64_t data);

/** \brief  hobject_int64_t获取基类指针
 *
 * \param obj_ptr hobject_int64_t* hobject_int64_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_int64_base(hobject_int64_t *obj_ptr);

/** \brief 获取hobject_int64_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_int64_t* hobject_int64_t对象指针,失败返回NULL
 *
 */
hobject_int64_t * hobject_int64(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 数据
    *
    */
    uint64_t data;

} hobject_uint64_t; /**< UINT64结构体 */

#define HOBJECT_UINT64_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},0} /**< UINT64初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief hobject_uint64_t初始化
 *
 * \param obj_ptr hobject_uint64_t* hobject_uint64_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param data uint64_t 数据
 *
 */
void hobject_uint64_init(hobject_uint64_t *obj_ptr,uint16_t usr_type,uint64_t data);

/** \brief  hobject_uint64_t获取基类指针
 *
 * \param obj_ptr hobject_uint64_t* hobject_uint64_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_uint64_base(hobject_uint64_t *obj_ptr);

/** \brief 获取hobject_uint64_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_uint64_t* hobject_uint64_t对象指针,失败返回NULL
 *
 */
hobject_uint64_t * hobject_uint64(hobject_base_t *base);

typedef struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 结构体,表明真正的结构体的首地址。
    *
    */
    uint8_t o_struct[0];

} hobject_struct_t; /**< 结构体类型结构体 */

#define HOBJECT_STRUCT_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0}} /**< STRUCT初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief 获取实际需要申请的内存空间大小
 *
 * \param struct_size size_t 结构体大小
 * \return size_t 实际所需空间,如需申请空间使用此返回值
 *
 */
size_t hobject_struct_get_size(size_t struct_size);


/** \brief获取真正的结构体指针
 *
 * \param obj_ptr hobject_struct_t* hobject_struct_t对象指针
 * \return void* 真正的结构体指针
 *
 */
void * hobject_struct_get_struct_ptr(hobject_struct_t *obj_ptr);

/** \brief hobject_struct_t初始化
 *
 * \param obj_ptr hobject_struct_t* hobject_struct_t对象指针
 * \param usr_type uint16_t 用户类型
 *
 */
void hobject_struct_init(hobject_struct_t *obj_ptr,uint16_t usr_type);

/** \brief  hobject_struct_t获取基类指针
 *
 * \param obj_ptr hobject_struct_t* hobject_struct_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_struct_base(hobject_struct_t *obj_ptr);

/** \brief 获取hobject_struct_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_struct_t* hobject_struct_t对象指针,失败返回NULL
 *
 */
hobject_struct_t * hobject_struct(hobject_base_t *base);

typedef struct __hobject_managed_struct
{
    /** \brief 基类
    *
    */
    hobject_base_t base;

    /** \brief 释放回调，一般用于清理,一般使用hobject_managed_struct_cleanup自动调用
    *
    */
    void (*onfree)(struct  __hobject_managed_struct *obj_ptr);

    /** \brief 结构体,表明真正的结构体的首地址。
    *
    */
    uint8_t o_struct[0];

} hobject_managed_struct_t; /**< 管理的结构体类型结构体 */

#define HOBJECT_MANAGED_STRUCT_INITIALIZER {{0,HOBJECT_BASE_TYPE_INVALID,0},NULL} /**< MANAGED_STRUCT初始化值(用于初始化变量(包括指针所指变量)) */

/** \brief 获取实际需要申请的内存空间大小
 *
 * \param struct_size size_t 结构体大小
 * \return size_t 实际所需空间,如需申请空间使用此返回值
 *
 */
size_t hobject_managed_struct_get_size(size_t struct_size);


/** \brief获取真正的结构体指针
 *
 * \param obj_ptr hobject_managed_struct_t* hobject_managed_struct_t对象指针
 * \return void* 真正的结构体指针
 *
 */
void * hobject_managed_struct_get_struct_ptr(hobject_managed_struct_t *obj_ptr);

/** \brief 清理hobject_managed_struct_t对象
 *
 * \param obj_ptr hobject_managed_struct_t* hobject_managed_struct_t对象指针
 *
 */
void hobject_managed_struct_cleanup(hobject_managed_struct_t *obj_ptr);

/** \brief hobject_managed_struct_t初始化
 *
 * \param obj_ptr hobject_managed_struct_t* hobject_managed_struct_t对象指针
 * \param usr_type uint16_t 用户类型
 * \param onfree 释放回调
 *
 */
void hobject_managed_struct_init(hobject_managed_struct_t *obj_ptr,uint16_t usr_type,void (*onfree)(hobject_managed_struct_t *obj_ptr));

/** \brief  hobject_managed_struct_t获取基类指针
 *
 * \param obj_ptr hobject_managed_struct_t* hobject_managed_struct_t对象指针
 * \return hobject_base_t* hobject_base_t基类指针
 *
 */
hobject_base_t * hobject_managed_struct_base(hobject_managed_struct_t *obj_ptr);

/** \brief 获取hobject_managed_struct_t对象指针
 *
 * \param  base hobject_base_t* hobject_base_t基类指针
 * \return hobject_managed_struct_t* hobject_managed_struct_t对象指针,失败返回NULL
 *
 */
hobject_managed_struct_t * hobject_managed_struct(hobject_base_t *base);


/** \brief 获取结构体的某个成员的偏移
 *
 * \param  type 结构体类型
 * \param  member 成员名称
 *
 */
#ifndef GET_STRUCT_MEMBER_OFFSET
#define GET_STRUCT_MEMBER_OFFSET(type,member) ((uintptr_t) &(((type *)0)->member))
#endif

/** \brief 通过某个成员的偏移获取结构体地址,通常用于某些情况只能传结构体某个成员的地址（如回调函数中声明了某参数的类型）但又需要获取原结构体地址的情况。
 *
 * \param  member_ptr 某个成员的指针
 * \param  type 结构体类型
 * \param  member 成员名称
 *
 */
#ifndef GET_STRUCT_PTR_BY_MEMBER_PTR
#define GET_STRUCT_PTR_BY_MEMBER_PTR(member_ptr,type,member) ((const type *)(((uintptr_t)(member_ptr))-GET_STRUCT_MEMBER_OFFSET(type,member)))
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HOBJECT_H_INCLUDED
