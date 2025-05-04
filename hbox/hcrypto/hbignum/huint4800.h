/***************************************************************
 * Name:      huint4800.h
 * Purpose:   声明huint4800接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT4800_H__
#define __HUINT4800_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT4800_BITS_COUNT 4800

typedef struct huint4800 huint4800_t;
struct huint4800
{
    uint32_t val[(HUINT4800_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT4800_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint4800_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4800_load(huint4800_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint4800_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4800_load_be(huint4800_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint4800_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint4800_load_uint32(huint4800_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint4800_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint4800_load_uint64(huint4800_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint4800_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4800_store(huint4800_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint4800_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4800_store_be(huint4800_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint4800_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint4800_store_uint32(const huint4800_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint4800_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint4800_store_uint64(const huint4800_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint4800_t* 目标大数
 * \param src const huint4800_t* 源大数
 *
 */
void huint4800_copy(huint4800_t *dst,const huint4800_t *src);

/** \brief 按位非
 *
 * \param dst huint4800_t* 目标大数
 * \param src const huint4800_t* 源大数
 *
 */
void huint4800_bitwise_not(huint4800_t *dst,const huint4800_t *src);

/** \brief 按位与
 *
 * \param dst huint4800_t* 目标大数
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_bitwise_and(huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);

/** \brief 按位或
 *
 * \param dst huint4800_t* 目标大数
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_bitwise_or(huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);

/** \brief 按位异或
 *
 * \param dst huint4800_t* 目标大数
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_bitwise_xor(huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);

/** \brief 比较
 *
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint4800_compare(const huint4800_t *src1,const huint4800_t *src2);

/** \brief 求补码
 *
 * \param dst huint4800_t* 目标大数
 * \param src const huint4800_t* 源大数
 *
 */
void huint4800_complement(huint4800_t *dst,const huint4800_t *src);

/** \brief 左移
 *
 * \param dst huint4800_t* 目标大数
 * \param src const huint4800_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint4800_left_shift(huint4800_t *dst,const huint4800_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint4800_t* 目标大数
 * \param src const huint4800_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint4800_right_shift(huint4800_t *dst,const huint4800_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint4800_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint4800_bit_set(huint4800_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint4800_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint4800_bit_clear(huint4800_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint4800_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint4800_bit(const huint4800_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint4800_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint4800_clz(const huint4800_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint4800_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint4800_ctz(const huint4800_t *dst);

/** \brief 加
 *
 * \param dst huint4800_t* 目标大数
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_add(huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);


/** \brief 减
 *
 * \param dst huint4800_t* 目标大数,dst=src1-src2
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_sub(huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);

/** \brief 乘
 *
 * \param state huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4800_t* 目标大数,dst=src1*src2
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_mul(huint4800_t *state,huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);


/** \brief 除
 *
 * \param state huint4800_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4800_t* 目标大数,dst=src1/src2
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_div(huint4800_t *state,huint4800_t *state1,huint4800_t *state2,huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);


/** \brief 幂函数
 *
 * \param state huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4800_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 *
 */
void huint4800_power(huint4800_t *state,huint4800_t *state1,huint4800_t *state2,huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint4800_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4800_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint4800_t* 源大数1
 * \param src2 const huint4800_t* 源大数2
 * \param src3 const huint4800_t* 源大数3
 *
 */
void huint4800_power_mod(huint4800_t *state,huint4800_t *state1,huint4800_t *state2,huint4800_t *state3,huint4800_t *dst,const huint4800_t *src1,const huint4800_t *src2,const huint4800_t *src3);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT4800_H__
