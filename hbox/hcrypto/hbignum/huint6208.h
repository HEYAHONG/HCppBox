/***************************************************************
 * Name:      huint6208.h
 * Purpose:   声明huint6208接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT6208_H__
#define __HUINT6208_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT6208_BITS_COUNT 6208

typedef struct huint6208 huint6208_t;
struct huint6208
{
    uint32_t val[(HUINT6208_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT6208_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint6208_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint6208_load(huint6208_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint6208_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint6208_load_be(huint6208_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint6208_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint6208_load_uint32(huint6208_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint6208_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint6208_load_uint64(huint6208_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint6208_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint6208_store(huint6208_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint6208_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint6208_store_be(huint6208_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint6208_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint6208_store_uint32(const huint6208_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint6208_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint6208_store_uint64(const huint6208_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint6208_t* 目标大数
 * \param src const huint6208_t* 源大数
 *
 */
void huint6208_copy(huint6208_t *dst,const huint6208_t *src);

/** \brief 按位非
 *
 * \param dst huint6208_t* 目标大数
 * \param src const huint6208_t* 源大数
 *
 */
void huint6208_bitwise_not(huint6208_t *dst,const huint6208_t *src);

/** \brief 按位与
 *
 * \param dst huint6208_t* 目标大数
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_bitwise_and(huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);

/** \brief 按位或
 *
 * \param dst huint6208_t* 目标大数
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_bitwise_or(huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);

/** \brief 按位异或
 *
 * \param dst huint6208_t* 目标大数
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_bitwise_xor(huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);

/** \brief 比较
 *
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint6208_compare(const huint6208_t *src1,const huint6208_t *src2);

/** \brief 求补码
 *
 * \param dst huint6208_t* 目标大数
 * \param src const huint6208_t* 源大数
 *
 */
void huint6208_complement(huint6208_t *dst,const huint6208_t *src);

/** \brief 左移
 *
 * \param dst huint6208_t* 目标大数
 * \param src const huint6208_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint6208_left_shift(huint6208_t *dst,const huint6208_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint6208_t* 目标大数
 * \param src const huint6208_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint6208_right_shift(huint6208_t *dst,const huint6208_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint6208_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint6208_bit_set(huint6208_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint6208_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint6208_bit_clear(huint6208_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint6208_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint6208_bit(const huint6208_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint6208_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint6208_clz(const huint6208_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint6208_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint6208_ctz(const huint6208_t *dst);

/** \brief 加
 *
 * \param dst huint6208_t* 目标大数
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_add(huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);


/** \brief 减
 *
 * \param dst huint6208_t* 目标大数,dst=src1-src2
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_sub(huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);

/** \brief 乘
 *
 * \param state huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint6208_t* 目标大数,dst=src1*src2
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_mul(huint6208_t *state,huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);


/** \brief 除
 *
 * \param state huint6208_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint6208_t* 目标大数,dst=src1/src2
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_div(huint6208_t *state,huint6208_t *state1,huint6208_t *state2,huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);


/** \brief 幂函数
 *
 * \param state huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint6208_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 *
 */
void huint6208_power(huint6208_t *state,huint6208_t *state1,huint6208_t *state2,huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint6208_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint6208_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint6208_t* 源大数1
 * \param src2 const huint6208_t* 源大数2
 * \param src3 const huint6208_t* 源大数3
 *
 */
void huint6208_power_mod(huint6208_t *state,huint6208_t *state1,huint6208_t *state2,huint6208_t *state3,huint6208_t *dst,const huint6208_t *src1,const huint6208_t *src2,const huint6208_t *src3);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT6208_H__
