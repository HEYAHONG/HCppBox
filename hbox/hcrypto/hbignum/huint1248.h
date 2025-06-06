/***************************************************************
 * Name:      huint1248.h
 * Purpose:   声明huint1248接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT1248_H__
#define __HUINT1248_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT1248_BITS_COUNT 1248

typedef struct huint1248 huint1248_t;
struct huint1248
{
    uint32_t val[(HUINT1248_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT1248_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint1248_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1248_load(huint1248_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint1248_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1248_load_be(huint1248_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint1248_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint1248_load_uint32(huint1248_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint1248_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint1248_load_uint64(huint1248_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint1248_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1248_store(huint1248_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint1248_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint1248_store_be(huint1248_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint1248_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint1248_store_uint32(const huint1248_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint1248_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint1248_store_uint64(const huint1248_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint1248_t* 目标大数
 * \param src const huint1248_t* 源大数
 *
 */
void huint1248_copy(huint1248_t *dst,const huint1248_t *src);

/** \brief 按位非
 *
 * \param dst huint1248_t* 目标大数
 * \param src const huint1248_t* 源大数
 *
 */
void huint1248_bitwise_not(huint1248_t *dst,const huint1248_t *src);

/** \brief 按位与
 *
 * \param dst huint1248_t* 目标大数
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_bitwise_and(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);

/** \brief 按位或
 *
 * \param dst huint1248_t* 目标大数
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_bitwise_or(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);

/** \brief 按位异或
 *
 * \param dst huint1248_t* 目标大数
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_bitwise_xor(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);

/** \brief 比较
 *
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint1248_compare(const huint1248_t *src1,const huint1248_t *src2);

/** \brief 求补码
 *
 * \param dst huint1248_t* 目标大数
 * \param src const huint1248_t* 源大数
 *
 */
void huint1248_complement(huint1248_t *dst,const huint1248_t *src);

/** \brief 左移
 *
 * \param dst huint1248_t* 目标大数
 * \param src const huint1248_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint1248_left_shift(huint1248_t *dst,const huint1248_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint1248_t* 目标大数
 * \param src const huint1248_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint1248_right_shift(huint1248_t *dst,const huint1248_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint1248_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint1248_bit_set(huint1248_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint1248_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint1248_bit_clear(huint1248_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint1248_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint1248_bit(const huint1248_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint1248_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint1248_clz(const huint1248_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint1248_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint1248_ctz(const huint1248_t *dst);

/** \brief 加
 *
 * \param dst huint1248_t* 目标大数
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_add(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);


/** \brief 减
 *
 * \param dst huint1248_t* 目标大数,dst=src1-src2
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_sub(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);

/** \brief 乘
 *
 * \param state huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1248_t* 目标大数,dst=src1*src2
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_mul(huint1248_t *state,huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);


/** \brief 除
 *
 * \param state huint1248_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1248_t* 目标大数,dst=src1/src2
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_div(huint1248_t *state,huint1248_t *state1,huint1248_t *state2,huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);

/** \brief 除(采用栈作为临时变量存储)
 *
 * \param mod huint1248_t* 状态值，存储余数。mod=src1%src2
 * \param dst huint1248_t* 目标大数,dst=src1/src2
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_div_with_stack(huint1248_t *mod,huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);


/** \brief 幂函数
 *
 * \param state huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1248_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_power(huint1248_t *state,huint1248_t *state1,huint1248_t *state2,huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);


/** \brief 幂函数(采用栈作为临时变量存储)
 *
 * \param dst huint1248_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 *
 */
void huint1248_power_with_stack(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint1248_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint1248_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 * \param src3 const huint1248_t* 源大数3
 *
 */
void huint1248_power_mod(huint1248_t *state,huint1248_t *state1,huint1248_t *state2,huint1248_t *state3,huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2,const huint1248_t *src3);

/** \brief 幂取模函数（常用于RSA等加密算法,采用栈作为临时变量存储）
 *
 * \param dst huint1248_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint1248_t* 源大数1
 * \param src2 const huint1248_t* 源大数2
 * \param src3 const huint1248_t* 源大数3
 *
 */
void huint1248_power_mod_with_stack(huint1248_t *dst,const huint1248_t *src1,const huint1248_t *src2,const huint1248_t *src3);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT1248_H__
