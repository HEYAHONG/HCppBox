/***************************************************************
 * Name:      huint8032.h
 * Purpose:   声明huint8032接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT8032_H__
#define __HUINT8032_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT8032_BITS_COUNT 8032

typedef struct huint8032 huint8032_t;
struct huint8032
{
    uint32_t val[(HUINT8032_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT8032_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint8032_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint8032_load(huint8032_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint8032_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint8032_load_be(huint8032_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint8032_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint8032_load_uint32(huint8032_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint8032_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint8032_load_uint64(huint8032_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint8032_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint8032_store(huint8032_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint8032_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint8032_store_be(huint8032_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint8032_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint8032_store_uint32(const huint8032_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint8032_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint8032_store_uint64(const huint8032_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint8032_t* 目标大数
 * \param src const huint8032_t* 源大数
 *
 */
void huint8032_copy(huint8032_t *dst,const huint8032_t *src);

/** \brief 按位非
 *
 * \param dst huint8032_t* 目标大数
 * \param src const huint8032_t* 源大数
 *
 */
void huint8032_bitwise_not(huint8032_t *dst,const huint8032_t *src);

/** \brief 按位与
 *
 * \param dst huint8032_t* 目标大数
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_bitwise_and(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);

/** \brief 按位或
 *
 * \param dst huint8032_t* 目标大数
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_bitwise_or(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);

/** \brief 按位异或
 *
 * \param dst huint8032_t* 目标大数
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_bitwise_xor(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);

/** \brief 比较
 *
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint8032_compare(const huint8032_t *src1,const huint8032_t *src2);

/** \brief 求补码
 *
 * \param dst huint8032_t* 目标大数
 * \param src const huint8032_t* 源大数
 *
 */
void huint8032_complement(huint8032_t *dst,const huint8032_t *src);

/** \brief 左移
 *
 * \param dst huint8032_t* 目标大数
 * \param src const huint8032_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint8032_left_shift(huint8032_t *dst,const huint8032_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint8032_t* 目标大数
 * \param src const huint8032_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint8032_right_shift(huint8032_t *dst,const huint8032_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint8032_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint8032_bit_set(huint8032_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint8032_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint8032_bit_clear(huint8032_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint8032_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint8032_bit(const huint8032_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint8032_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint8032_clz(const huint8032_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint8032_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint8032_ctz(const huint8032_t *dst);

/** \brief 加
 *
 * \param dst huint8032_t* 目标大数
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_add(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);


/** \brief 减
 *
 * \param dst huint8032_t* 目标大数,dst=src1-src2
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_sub(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);

/** \brief 乘
 *
 * \param state huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint8032_t* 目标大数,dst=src1*src2
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_mul(huint8032_t *state,huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);


/** \brief 除
 *
 * \param state huint8032_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint8032_t* 目标大数,dst=src1/src2
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_div(huint8032_t *state,huint8032_t *state1,huint8032_t *state2,huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);

/** \brief 除(采用栈作为临时变量存储)
 *
 * \param mod huint8032_t* 状态值，存储余数。mod=src1%src2
 * \param dst huint8032_t* 目标大数,dst=src1/src2
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_div_with_stack(huint8032_t *mod,huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);


/** \brief 幂函数
 *
 * \param state huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint8032_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_power(huint8032_t *state,huint8032_t *state1,huint8032_t *state2,huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);


/** \brief 幂函数(采用栈作为临时变量存储)
 *
 * \param dst huint8032_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 *
 */
void huint8032_power_with_stack(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint8032_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint8032_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 * \param src3 const huint8032_t* 源大数3
 *
 */
void huint8032_power_mod(huint8032_t *state,huint8032_t *state1,huint8032_t *state2,huint8032_t *state3,huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2,const huint8032_t *src3);

/** \brief 幂取模函数（常用于RSA等加密算法,采用栈作为临时变量存储）
 *
 * \param dst huint8032_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint8032_t* 源大数1
 * \param src2 const huint8032_t* 源大数2
 * \param src3 const huint8032_t* 源大数3
 *
 */
void huint8032_power_mod_with_stack(huint8032_t *dst,const huint8032_t *src1,const huint8032_t *src2,const huint8032_t *src3);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT8032_H__
