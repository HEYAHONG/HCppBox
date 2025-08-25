/***************************************************************
 * Name:      hpkcs1.h
 * Purpose:   声明hpkcs1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPKCS1_H__
#define __HPKCS1_H__
#include "stdint.h"
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 *  rfc2313 填充结构：
 *  EB = 00 || BT || PS || 00 || D
 */

enum
{
    HPKCS1_BT_TYPE_0=0x00,  /**< PS采用0填充，只适合非0数据 */
    HPKCS1_BT_TYPE_1=0x01,  /**< PS采用0xFF填充，推荐私钥加密时使用 */
    HPKCS1_BT_TYPE_2=0x02,  /**< PS采用随机数填充 */
};

#define HPKCS1_PS_MIN_LENGTH    (8)

/** \brief PKCS1数据块最大长度数据长度
 *
 * \param data_block_length size_t 数据块长度
 * \return size_t  最大长度数据长度
 *
 */
size_t hpkcs1_data_block_max_data_length(size_t data_block_length);

/** \brief PKCS1填充
 *
 * \param data_block uint8_t* 数据块(通常是RSA数据块)
 * \param data_block_length size_t 数据块长度
 * \param data const uint8_t* 数据
 * \param data_length size_t 数据长度
 * \param bt_type uint8_t 类型,HPKCS1_BT_TYPE_0、HPKCS1_BT_TYPE_1、HPKCS1_BT_TYPE_2
 * \return bool 是否填充成功
 *
 */
bool hpkcs1_padding(uint8_t *data_block,size_t data_block_length,const uint8_t *data,size_t data_length,uint8_t bt_type);

/** \brief PKCS检查填充
 *
 * \param data_block const uint8_t* 数据块(通常是RSA数据块)
 * \param data_block_length size_t 数据块长度
 * \param data_ptr const uint8_t** 数据指针
 * \param length_ptr size_t* 数据长度指针
 * \return bool 填充是否成功
 *
 */
bool hpkcs1_check_padding(const uint8_t *data_block,size_t data_block_length,const uint8_t **data_ptr,size_t *length_ptr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPKCS1_H__
