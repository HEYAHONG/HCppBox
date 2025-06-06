/***************************************************************
 * Name:      huint352.h
 * Purpose:   声明huint352接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT352_H__
#define __HUINT352_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT352_BITS_COUNT 352

typedef struct huint352 huint352_t;
struct huint352
{
    uint32_t val[(HUINT352_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT352_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint352_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint352_load(huint352_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint352_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint352_load_be(huint352_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint352_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint352_load_uint32(huint352_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint352_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint352_load_uint64(huint352_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint352_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint352_store(huint352_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint352_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint352_store_be(huint352_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint352_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint352_store_uint32(const huint352_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint352_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint352_store_uint64(const huint352_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint352_t* 目标大数
 * \param src const huint352_t* 源大数
 *
 */
void huint352_copy(huint352_t *dst,const huint352_t *src);

/** \brief 按位非
 *
 * \param dst huint352_t* 目标大数
 * \param src const huint352_t* 源大数
 *
 */
void huint352_bitwise_not(huint352_t *dst,const huint352_t *src);

/** \brief 按位与
 *
 * \param dst huint352_t* 目标大数
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_bitwise_and(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);

/** \brief 按位或
 *
 * \param dst huint352_t* 目标大数
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_bitwise_or(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);

/** \brief 按位异或
 *
 * \param dst huint352_t* 目标大数
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_bitwise_xor(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);

/** \brief 比较
 *
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint352_compare(const huint352_t *src1,const huint352_t *src2);

/** \brief 求补码
 *
 * \param dst huint352_t* 目标大数
 * \param src const huint352_t* 源大数
 *
 */
void huint352_complement(huint352_t *dst,const huint352_t *src);

/** \brief 左移
 *
 * \param dst huint352_t* 目标大数
 * \param src const huint352_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint352_left_shift(huint352_t *dst,const huint352_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint352_t* 目标大数
 * \param src const huint352_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint352_right_shift(huint352_t *dst,const huint352_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint352_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint352_bit_set(huint352_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint352_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint352_bit_clear(huint352_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint352_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint352_bit(const huint352_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint352_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint352_clz(const huint352_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint352_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint352_ctz(const huint352_t *dst);

/** \brief 加
 *
 * \param dst huint352_t* 目标大数
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_add(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);


/** \brief 减
 *
 * \param dst huint352_t* 目标大数,dst=src1-src2
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_sub(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);

/** \brief 乘
 *
 * \param state huint352_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint352_t* 目标大数,dst=src1*src2
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_mul(huint352_t *state,huint352_t *dst,const huint352_t *src1,const huint352_t *src2);


/** \brief 除
 *
 * \param state huint352_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint352_t* 目标大数,dst=src1/src2
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_div(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *dst,const huint352_t *src1,const huint352_t *src2);

/** \brief 除(采用栈作为临时变量存储)
 *
 * \param mod huint352_t* 状态值，存储余数。mod=src1%src2
 * \param dst huint352_t* 目标大数,dst=src1/src2
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_div_with_stack(huint352_t *mod,huint352_t *dst,const huint352_t *src1,const huint352_t *src2);


/** \brief 幂函数
 *
 * \param state huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint352_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_power(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *dst,const huint352_t *src1,const huint352_t *src2);


/** \brief 幂函数(采用栈作为临时变量存储)
 *
 * \param dst huint352_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 *
 */
void huint352_power_with_stack(huint352_t *dst,const huint352_t *src1,const huint352_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint352_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint352_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 * \param src3 const huint352_t* 源大数3
 *
 */
void huint352_power_mod(huint352_t *state,huint352_t *state1,huint352_t *state2,huint352_t *state3,huint352_t *dst,const huint352_t *src1,const huint352_t *src2,const huint352_t *src3);

/** \brief 幂取模函数（常用于RSA等加密算法,采用栈作为临时变量存储）
 *
 * \param dst huint352_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint352_t* 源大数1
 * \param src2 const huint352_t* 源大数2
 * \param src3 const huint352_t* 源大数3
 *
 */
void huint352_power_mod_with_stack(huint352_t *dst,const huint352_t *src1,const huint352_t *src2,const huint352_t *src3);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT352_H__
