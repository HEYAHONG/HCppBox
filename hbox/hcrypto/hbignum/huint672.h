/***************************************************************
 * Name:      huint672.h
 * Purpose:   声明huint672接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT672_H__
#define __HUINT672_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT672_BITS_COUNT 672

typedef struct huint672 huint672_t;
struct huint672
{
    uint32_t val[(HUINT672_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT672_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint672_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint672_load(huint672_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint672_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint672_load_be(huint672_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint672_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint672_load_uint32(huint672_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint672_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint672_load_uint64(huint672_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint672_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint672_store(huint672_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint672_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint672_store_be(huint672_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint672_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint672_store_uint32(const huint672_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint672_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint672_store_uint64(const huint672_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint672_t* 目标大数
 * \param src const huint672_t* 源大数
 *
 */
void huint672_copy(huint672_t *dst,const huint672_t *src);

/** \brief 按位非
 *
 * \param dst huint672_t* 目标大数
 * \param src const huint672_t* 源大数
 *
 */
void huint672_bitwise_not(huint672_t *dst,const huint672_t *src);

/** \brief 按位与
 *
 * \param dst huint672_t* 目标大数
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_bitwise_and(huint672_t *dst,const huint672_t *src1,const huint672_t *src2);

/** \brief 按位或
 *
 * \param dst huint672_t* 目标大数
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_bitwise_or(huint672_t *dst,const huint672_t *src1,const huint672_t *src2);

/** \brief 按位异或
 *
 * \param dst huint672_t* 目标大数
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_bitwise_xor(huint672_t *dst,const huint672_t *src1,const huint672_t *src2);

/** \brief 比较
 *
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint672_compare(const huint672_t *src1,const huint672_t *src2);

/** \brief 求补码
 *
 * \param dst huint672_t* 目标大数
 * \param src const huint672_t* 源大数
 *
 */
void huint672_complement(huint672_t *dst,const huint672_t *src);

/** \brief 左移
 *
 * \param dst huint672_t* 目标大数
 * \param src const huint672_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint672_left_shift(huint672_t *dst,const huint672_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint672_t* 目标大数
 * \param src const huint672_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint672_right_shift(huint672_t *dst,const huint672_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint672_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint672_bit_set(huint672_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint672_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint672_bit_clear(huint672_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint672_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint672_bit(const huint672_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint672_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint672_clz(const huint672_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint672_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint672_ctz(const huint672_t *dst);

/** \brief 加
 *
 * \param dst huint672_t* 目标大数
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_add(huint672_t *dst,const huint672_t *src1,const huint672_t *src2);


/** \brief 减
 *
 * \param dst huint672_t* 目标大数,dst=src1-src2
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_sub(huint672_t *dst,const huint672_t *src1,const huint672_t *src2);

/** \brief 乘
 *
 * \param state huint672_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint672_t* 目标大数,dst=src1*src2
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_mul(huint672_t *state,huint672_t *dst,const huint672_t *src1,const huint672_t *src2);


/** \brief 除
 *
 * \param state huint672_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint672_t* 目标大数,dst=src1/src2
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_div(huint672_t *state,huint672_t *state1,huint672_t *state2,huint672_t *dst,const huint672_t *src1,const huint672_t *src2);


/** \brief 幂函数
 *
 * \param state huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint672_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 *
 */
void huint672_power(huint672_t *state,huint672_t *state1,huint672_t *state2,huint672_t *dst,const huint672_t *src1,const huint672_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint672_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint672_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint672_t* 源大数1
 * \param src2 const huint672_t* 源大数2
 * \param src3 const huint672_t* 源大数3
 *
 */
void huint672_power_mod(huint672_t *state,huint672_t *state1,huint672_t *state2,huint672_t *state3,huint672_t *dst,const huint672_t *src1,const huint672_t *src2,const huint672_t *src3);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT672_H__
