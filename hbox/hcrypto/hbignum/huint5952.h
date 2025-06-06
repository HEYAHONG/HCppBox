/***************************************************************
 * Name:      huint5952.h
 * Purpose:   声明huint5952接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT5952_H__
#define __HUINT5952_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT5952_BITS_COUNT 5952

typedef struct huint5952 huint5952_t;
struct huint5952
{
    uint32_t val[(HUINT5952_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT5952_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint5952_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint5952_load(huint5952_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint5952_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint5952_load_be(huint5952_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint5952_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint5952_load_uint32(huint5952_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint5952_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint5952_load_uint64(huint5952_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint5952_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint5952_store(huint5952_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint5952_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint5952_store_be(huint5952_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint5952_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint5952_store_uint32(const huint5952_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint5952_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint5952_store_uint64(const huint5952_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint5952_t* 目标大数
 * \param src const huint5952_t* 源大数
 *
 */
void huint5952_copy(huint5952_t *dst,const huint5952_t *src);

/** \brief 按位非
 *
 * \param dst huint5952_t* 目标大数
 * \param src const huint5952_t* 源大数
 *
 */
void huint5952_bitwise_not(huint5952_t *dst,const huint5952_t *src);

/** \brief 按位与
 *
 * \param dst huint5952_t* 目标大数
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_bitwise_and(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);

/** \brief 按位或
 *
 * \param dst huint5952_t* 目标大数
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_bitwise_or(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);

/** \brief 按位异或
 *
 * \param dst huint5952_t* 目标大数
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_bitwise_xor(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);

/** \brief 比较
 *
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint5952_compare(const huint5952_t *src1,const huint5952_t *src2);

/** \brief 求补码
 *
 * \param dst huint5952_t* 目标大数
 * \param src const huint5952_t* 源大数
 *
 */
void huint5952_complement(huint5952_t *dst,const huint5952_t *src);

/** \brief 左移
 *
 * \param dst huint5952_t* 目标大数
 * \param src const huint5952_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint5952_left_shift(huint5952_t *dst,const huint5952_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint5952_t* 目标大数
 * \param src const huint5952_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint5952_right_shift(huint5952_t *dst,const huint5952_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint5952_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint5952_bit_set(huint5952_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint5952_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint5952_bit_clear(huint5952_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint5952_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint5952_bit(const huint5952_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint5952_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint5952_clz(const huint5952_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint5952_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint5952_ctz(const huint5952_t *dst);

/** \brief 加
 *
 * \param dst huint5952_t* 目标大数
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_add(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);


/** \brief 减
 *
 * \param dst huint5952_t* 目标大数,dst=src1-src2
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_sub(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);

/** \brief 乘
 *
 * \param state huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint5952_t* 目标大数,dst=src1*src2
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_mul(huint5952_t *state,huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);


/** \brief 除
 *
 * \param state huint5952_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint5952_t* 目标大数,dst=src1/src2
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_div(huint5952_t *state,huint5952_t *state1,huint5952_t *state2,huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);

/** \brief 除(采用栈作为临时变量存储)
 *
 * \param mod huint5952_t* 状态值，存储余数。mod=src1%src2
 * \param dst huint5952_t* 目标大数,dst=src1/src2
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_div_with_stack(huint5952_t *mod,huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);


/** \brief 幂函数
 *
 * \param state huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint5952_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_power(huint5952_t *state,huint5952_t *state1,huint5952_t *state2,huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);


/** \brief 幂函数(采用栈作为临时变量存储)
 *
 * \param dst huint5952_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 *
 */
void huint5952_power_with_stack(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint5952_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint5952_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 * \param src3 const huint5952_t* 源大数3
 *
 */
void huint5952_power_mod(huint5952_t *state,huint5952_t *state1,huint5952_t *state2,huint5952_t *state3,huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2,const huint5952_t *src3);

/** \brief 幂取模函数（常用于RSA等加密算法,采用栈作为临时变量存储）
 *
 * \param dst huint5952_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint5952_t* 源大数1
 * \param src2 const huint5952_t* 源大数2
 * \param src3 const huint5952_t* 源大数3
 *
 */
void huint5952_power_mod_with_stack(huint5952_t *dst,const huint5952_t *src1,const huint5952_t *src2,const huint5952_t *src3);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT5952_H__
