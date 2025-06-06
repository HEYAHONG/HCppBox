/***************************************************************
 * Name:      huint320.h
 * Purpose:   声明huint320接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT320_H__
#define __HUINT320_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT320_BITS_COUNT 320

typedef struct huint320 huint320_t;
struct huint320
{
    uint32_t val[(HUINT320_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT320_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint320_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint320_load(huint320_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint320_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint320_load_be(huint320_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint320_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint320_load_uint32(huint320_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint320_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint320_load_uint64(huint320_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint320_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint320_store(huint320_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint320_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint320_store_be(huint320_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint320_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint320_store_uint32(const huint320_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint320_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint320_store_uint64(const huint320_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint320_t* 目标大数
 * \param src const huint320_t* 源大数
 *
 */
void huint320_copy(huint320_t *dst,const huint320_t *src);

/** \brief 按位非
 *
 * \param dst huint320_t* 目标大数
 * \param src const huint320_t* 源大数
 *
 */
void huint320_bitwise_not(huint320_t *dst,const huint320_t *src);

/** \brief 按位与
 *
 * \param dst huint320_t* 目标大数
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_bitwise_and(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);

/** \brief 按位或
 *
 * \param dst huint320_t* 目标大数
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_bitwise_or(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);

/** \brief 按位异或
 *
 * \param dst huint320_t* 目标大数
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_bitwise_xor(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);

/** \brief 比较
 *
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint320_compare(const huint320_t *src1,const huint320_t *src2);

/** \brief 求补码
 *
 * \param dst huint320_t* 目标大数
 * \param src const huint320_t* 源大数
 *
 */
void huint320_complement(huint320_t *dst,const huint320_t *src);

/** \brief 左移
 *
 * \param dst huint320_t* 目标大数
 * \param src const huint320_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint320_left_shift(huint320_t *dst,const huint320_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint320_t* 目标大数
 * \param src const huint320_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint320_right_shift(huint320_t *dst,const huint320_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint320_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint320_bit_set(huint320_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint320_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint320_bit_clear(huint320_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint320_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint320_bit(const huint320_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint320_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint320_clz(const huint320_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint320_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint320_ctz(const huint320_t *dst);

/** \brief 加
 *
 * \param dst huint320_t* 目标大数
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_add(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);


/** \brief 减
 *
 * \param dst huint320_t* 目标大数,dst=src1-src2
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_sub(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);

/** \brief 乘
 *
 * \param state huint320_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint320_t* 目标大数,dst=src1*src2
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_mul(huint320_t *state,huint320_t *dst,const huint320_t *src1,const huint320_t *src2);


/** \brief 除
 *
 * \param state huint320_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint320_t* 目标大数,dst=src1/src2
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_div(huint320_t *state,huint320_t *state1,huint320_t *state2,huint320_t *dst,const huint320_t *src1,const huint320_t *src2);

/** \brief 除(采用栈作为临时变量存储)
 *
 * \param mod huint320_t* 状态值，存储余数。mod=src1%src2
 * \param dst huint320_t* 目标大数,dst=src1/src2
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_div_with_stack(huint320_t *mod,huint320_t *dst,const huint320_t *src1,const huint320_t *src2);


/** \brief 幂函数
 *
 * \param state huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint320_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_power(huint320_t *state,huint320_t *state1,huint320_t *state2,huint320_t *dst,const huint320_t *src1,const huint320_t *src2);


/** \brief 幂函数(采用栈作为临时变量存储)
 *
 * \param dst huint320_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 *
 */
void huint320_power_with_stack(huint320_t *dst,const huint320_t *src1,const huint320_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint320_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint320_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 * \param src3 const huint320_t* 源大数3
 *
 */
void huint320_power_mod(huint320_t *state,huint320_t *state1,huint320_t *state2,huint320_t *state3,huint320_t *dst,const huint320_t *src1,const huint320_t *src2,const huint320_t *src3);

/** \brief 幂取模函数（常用于RSA等加密算法,采用栈作为临时变量存储）
 *
 * \param dst huint320_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint320_t* 源大数1
 * \param src2 const huint320_t* 源大数2
 * \param src3 const huint320_t* 源大数3
 *
 */
void huint320_power_mod_with_stack(huint320_t *dst,const huint320_t *src1,const huint320_t *src2,const huint320_t *src3);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT320_H__
