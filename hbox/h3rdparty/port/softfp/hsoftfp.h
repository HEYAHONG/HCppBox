/***************************************************************
 * Name:      hsoftfp.h
 * Purpose:   声明hsoftfp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTFP_H_INCLUDE__
#define __HSOFTFP_H_INCLUDE__
#include "h3rdparty_port_softfp.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * softfp库函数包装。
 * 注意：本头文件不直接包含softfp第三方头文件，用户只需包含本头文件即可。
 */

/** \brief 软件浮点舍入模式
 *
 */
typedef enum
{
    HSOFTFP_RM_RNE=0,       /**< 就近舍入，平局取偶(Round to Nearest, ties to Even) */
    HSOFTFP_RM_RTZ,         /**< 向零舍入(Round towards Zero) */
    HSOFTFP_RM_RDN,         /**< 向下舍入(Round Down) */
    HSOFTFP_RM_RUP,         /**< 向上舍入(Round Up) */
    HSOFTFP_RM_RMM,         /**< 就近舍入，平局取最大绝对值(Round to Nearest, ties to Max Magnitude) */
} hsoftfp_rounding_mode_t;

#define HSOFTFP_FFLAG_INVALID_OP  (1 << 4)      /**< 非法操作 */
#define HSOFTFP_FFLAG_DIVIDE_ZERO (1 << 3)      /**< 除零 */
#define HSOFTFP_FFLAG_OVERFLOW    (1 << 2)      /**< 上溢 */
#define HSOFTFP_FFLAG_UNDERFLOW   (1 << 1)      /**< 下溢 */
#define HSOFTFP_FFLAG_INEXACT     (1 << 0)      /**< 不精确 */

#define HSOFTFP_FCLASS_NINF       (1 << 0)      /**< 负无穷 */
#define HSOFTFP_FCLASS_NNORMAL    (1 << 1)      /**< 负规格化数 */
#define HSOFTFP_FCLASS_NSUBNORMAL (1 << 2)      /**< 负次规格化数 */
#define HSOFTFP_FCLASS_NZERO      (1 << 3)      /**< 负零 */
#define HSOFTFP_FCLASS_PZERO      (1 << 4)      /**< 正零 */
#define HSOFTFP_FCLASS_PSUBNORMAL (1 << 5)      /**< 正次规格化数 */
#define HSOFTFP_FCLASS_PNORMAL    (1 << 6)      /**< 正规格化数 */
#define HSOFTFP_FCLASS_PINF       (1 << 7)      /**< 正无穷 */
#define HSOFTFP_FCLASS_SNAN       (1 << 8)      /**< 信号NaN */
#define HSOFTFP_FCLASS_QNAN       (1 << 9)      /**< 静默NaN */

#define HSOFTFP_FSIGN_MASK32      (1UL << 31)                   /**< 单精度符号位掩码 */
#define HSOFTFP_FSIGN_MASK64      (((uint64_t)1) << 63)         /**< 双精度符号位掩码 */

typedef uint32_t hsoftfp_float32_t;     /**< 单精度软件浮点类型 */
typedef uint64_t hsoftfp_float64_t;     /**< 双精度软件浮点类型 */

/*
 * 单精度浮点运算
 */

/** \brief 单精度浮点加法
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_add_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点减法
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_sub_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点乘法
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_mul_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点除法
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_div_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点平方根
 *
 * \param a hsoftfp_float32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_sqrt_sf32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点乘加
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param c hsoftfp_float32_t 操作数c
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果(a*b+c)
 *
 */
hsoftfp_float32_t hsoftfp_fma_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,hsoftfp_float32_t c,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点最小值
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_min_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags);

/** \brief 单精度浮点最大值
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_max_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags);

/** \brief 单精度浮点静默比较(相等)
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示相等
 *
 */
int hsoftfp_eq_quiet_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags);

/** \brief 单精度浮点比较(小于等于)
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示a<=b
 *
 */
int hsoftfp_le_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags);

/** \brief 单精度浮点比较(小于)
 *
 * \param a hsoftfp_float32_t 操作数a
 * \param b hsoftfp_float32_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示a<b
 *
 */
int hsoftfp_lt_sf32(hsoftfp_float32_t a,hsoftfp_float32_t b,uint32_t *pfflags);

/** \brief 单精度浮点分类
 *
 * \param a hsoftfp_float32_t 操作数
 * \return uint32_t 分类值(HSOFTFP_FCLASS_*)
 *
 */
uint32_t hsoftfp_fclass_sf32(hsoftfp_float32_t a);

/** \brief 单精度浮点转换为双精度浮点
 *
 * \param a hsoftfp_float32_t 操作数
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_cvt_sf32_sf64(hsoftfp_float32_t a,uint32_t *pfflags);

/** \brief 双精度浮点转换为单精度浮点
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_cvt_sf64_sf32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点转换为有符号32位整数
 *
 * \param a hsoftfp_float32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int32_t 结果
 *
 */
int32_t hsoftfp_cvt_sf32_i32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点转换为无符号32位整数
 *
 * \param a hsoftfp_float32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return uint32_t 结果
 *
 */
uint32_t hsoftfp_cvt_sf32_u32(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点转换为有符号64位整数
 *
 * \param a hsoftfp_float32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int64_t 结果
 *
 */
int64_t hsoftfp_cvt_sf32_i64(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 单精度浮点转换为无符号64位整数
 *
 * \param a hsoftfp_float32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return uint64_t 结果
 *
 */
uint64_t hsoftfp_cvt_sf32_u64(hsoftfp_float32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 有符号32位整数转换为单精度浮点
 *
 * \param a int32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_cvt_i32_sf32(int32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 无符号32位整数转换为单精度浮点
 *
 * \param a uint32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_cvt_u32_sf32(uint32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 有符号64位整数转换为单精度浮点
 *
 * \param a int64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_cvt_i64_sf32(int64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 无符号64位整数转换为单精度浮点
 *
 * \param a uint64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float32_t 结果
 *
 */
hsoftfp_float32_t hsoftfp_cvt_u64_sf32(uint64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/*
 * 双精度浮点运算
 */

/** \brief 双精度浮点加法
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_add_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点减法
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_sub_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点乘法
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_mul_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点除法
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_div_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点平方根
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_sqrt_sf64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点乘加
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param c hsoftfp_float64_t 操作数c
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果(a*b+c)
 *
 */
hsoftfp_float64_t hsoftfp_fma_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,hsoftfp_float64_t c,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点最小值
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_min_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags);

/** \brief 双精度浮点最大值
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_max_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags);

/** \brief 双精度浮点静默比较(相等)
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示相等
 *
 */
int hsoftfp_eq_quiet_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags);

/** \brief 双精度浮点比较(小于等于)
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示a<=b
 *
 */
int hsoftfp_le_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags);

/** \brief 双精度浮点比较(小于)
 *
 * \param a hsoftfp_float64_t 操作数a
 * \param b hsoftfp_float64_t 操作数b
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int 非0表示a<b
 *
 */
int hsoftfp_lt_sf64(hsoftfp_float64_t a,hsoftfp_float64_t b,uint32_t *pfflags);

/** \brief 双精度浮点分类
 *
 * \param a hsoftfp_float64_t 操作数
 * \return uint32_t 分类值(HSOFTFP_FCLASS_*)
 *
 */
uint32_t hsoftfp_fclass_sf64(hsoftfp_float64_t a);

/** \brief 双精度浮点转换为有符号32位整数
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int32_t 结果
 *
 */
int32_t hsoftfp_cvt_sf64_i32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点转换为无符号32位整数
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return uint32_t 结果
 *
 */
uint32_t hsoftfp_cvt_sf64_u32(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点转换为有符号64位整数
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return int64_t 结果
 *
 */
int64_t hsoftfp_cvt_sf64_i64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 双精度浮点转换为无符号64位整数
 *
 * \param a hsoftfp_float64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return uint64_t 结果
 *
 */
uint64_t hsoftfp_cvt_sf64_u64(hsoftfp_float64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 有符号32位整数转换为双精度浮点
 *
 * \param a int32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_cvt_i32_sf64(int32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 无符号32位整数转换为双精度浮点
 *
 * \param a uint32_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_cvt_u32_sf64(uint32_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 有符号64位整数转换为双精度浮点
 *
 * \param a int64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_cvt_i64_sf64(int64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

/** \brief 无符号64位整数转换为双精度浮点
 *
 * \param a uint64_t 操作数
 * \param rm hsoftfp_rounding_mode_t 舍入模式
 * \param pfflags uint32_t* 浮点标志指针，可为NULL
 * \return hsoftfp_float64_t 结果
 *
 */
hsoftfp_float64_t hsoftfp_cvt_u64_sf64(uint64_t a,hsoftfp_rounding_mode_t rm,uint32_t *pfflags);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSOFTFP_H_INCLUDE__
