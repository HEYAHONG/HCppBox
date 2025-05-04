/***************************************************************
 * Name:      huint1408.h
 * Purpose:   声明huint1408接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT1408_H__
#define __HUINT1408_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT1408_BITS_COUNT 1408

typedef struct huint1408 huint1408_t;
struct huint1408
{
    uint32_t val[(HUINT1408_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT1408_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint1408_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1408_load(huint1408_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint1408_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1408_load_be(huint1408_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint1408_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint1408_load_uint32(huint1408_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint1408_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint1408_load_uint64(huint1408_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint1408_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1408_store(huint1408_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint1408_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1408_store_be(huint1408_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint1408_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint1408_store_uint32(const huint1408_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint1408_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint1408_store_uint64(const huint1408_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint1408_t* 目标大数
 * \param src const huint1408_t* 源大数
 *
 */
void huint1408_copy(huint1408_t *dst,const huint1408_t *src);

/** \brief 按位非
 *
 * \param dst huint1408_t* 目标大数
 * \param src const huint1408_t* 源大数
 *
 */
void huint1408_bitwise_not(huint1408_t *dst,const huint1408_t *src);

/** \brief 按位与
 *
 * \param dst huint1408_t* 目标大数
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_bitwise_and(huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);

/** \brief 按位或
 *
 * \param dst huint1408_t* 目标大数
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_bitwise_or(huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);

/** \brief 按位异或
 *
 * \param dst huint1408_t* 目标大数
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_bitwise_xor(huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);

/** \brief 比较
 *
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint1408_compare(const huint1408_t *src1,const huint1408_t *src2);

/** \brief 求补码
 *
 * \param dst huint1408_t* 目标大数
 * \param src const huint1408_t* 源大数
 *
 */
void huint1408_complement(huint1408_t *dst,const huint1408_t *src);

/** \brief 左移
 *
 * \param dst huint1408_t* 目标大数
 * \param src const huint1408_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint1408_left_shift(huint1408_t *dst,const huint1408_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint1408_t* 目标大数
 * \param src const huint1408_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint1408_right_shift(huint1408_t *dst,const huint1408_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint1408_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint1408_bit_set(huint1408_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint1408_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint1408_bit_clear(huint1408_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint1408_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint1408_bit(const huint1408_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint1408_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint1408_clz(const huint1408_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint1408_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint1408_ctz(const huint1408_t *dst);

/** \brief 加
 *
 * \param dst huint1408_t* 目标大数
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_add(huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);


/** \brief 减
 *
 * \param dst huint1408_t* 目标大数,dst=src1-src2
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_sub(huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);

/** \brief 乘
 *
 * \param state huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1408_t* 目标大数,dst=src1*src2
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_mul(huint1408_t *state,huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);


/** \brief 除
 *
 * \param state huint1408_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1408_t* 目标大数,dst=src1/src2
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_div(huint1408_t *state,huint1408_t *state1,huint1408_t *state2,huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);


/** \brief 幂函数
 *
 * \param state huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1408_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 *
 */
void huint1408_power(huint1408_t *state,huint1408_t *state1,huint1408_t *state2,huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint1408_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1408_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint1408_t* 源大数1
 * \param src2 const huint1408_t* 源大数2
 * \param src3 const huint1408_t* 源大数3
 *
 */
void huint1408_power_mod(huint1408_t *state,huint1408_t *state1,huint1408_t *state2,huint1408_t *state3,huint1408_t *dst,const huint1408_t *src1,const huint1408_t *src2,const huint1408_t *src3);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT1408_H__
