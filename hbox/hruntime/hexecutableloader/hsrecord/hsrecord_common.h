/***************************************************************
 * Name:      hsrecord_common.h
 * Purpose:   声明hsrecord_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSRECORD_COMMON_H__
#define __HSRECORD_COMMON_H__
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 携带的数据最大长度
 *
 * 注意: S记录中的数据长度字段最大为255,地址最大为4字节,校验为1字节,
 *       因此数据最大长度为255-4-1=250。
 */
#define HSRECORD_MAX_DATA_LEN 250

/** \brief 每行的最大长度(二进制,包含长度、地址、数据与校验)
 *
 */
#define HSRECORD_MAX_LINE_BIN_LEN (HSRECORD_MAX_DATA_LEN+6)

/** \brief 每行的最大长度(文本,'S'+类型+十六进制数据)
 *
 */
#define HSRECORD_MAX_LINE_LEN (2+(HSRECORD_MAX_LINE_BIN_LEN*2))

/** \brief S记录类型
 *
 */
typedef enum
{
    HSRECORD_TYPE_HEADER                =   0,      /**< S0 文件头 */
    HSRECORD_TYPE_DATA_16BIT            =   1,      /**< S1 16位地址数据 */
    HSRECORD_TYPE_DATA_24BIT            =   2,      /**< S2 24位地址数据 */
    HSRECORD_TYPE_DATA_32BIT            =   3,      /**< S3 32位地址数据 */
    HSRECORD_TYPE_RESERVED              =   4,      /**< S4 保留 */
    HSRECORD_TYPE_DATA_COUNT_16BIT      =   5,      /**< S5 16位数据记录计数 */
    HSRECORD_TYPE_DATA_COUNT_24BIT      =   6,      /**< S6 24位数据记录计数 */
    HSRECORD_TYPE_START_ADDRESS_32BIT   =   7,      /**< S7 32位起始地址 */
    HSRECORD_TYPE_START_ADDRESS_24BIT   =   8,      /**< S8 24位起始地址 */
    HSRECORD_TYPE_START_ADDRESS_16BIT   =   9       /**< S9 16位起始地址 */
} hsrecord_type_t;

typedef struct hsrecord_common_buffer hsrecord_common_buffer_t;
struct hsrecord_common_buffer
{
    uint8_t line[HSRECORD_MAX_LINE_LEN+1];                      /**< 用于存储文本行 */
    uint8_t line_binary[HSRECORD_MAX_LINE_BIN_LEN];             /**< 用于存储转化为二进制数据的行 */
    size_t line_length;                                         /**< 当前文本行长度 */
};

/** \brief 初始化hsrecord缓冲
 *
 * \param buffer hsrecord_common_buffer_t* hsrecord缓冲指针
 *
 */
void hsrecord_common_buffer_init(hsrecord_common_buffer_t *buffer);

/** \brief 将数值按大端序编码为字节
 *
 * \param buffer uint8_t* 目标缓冲
 * \param value uint32_t 数值
 * \param length size_t 字节数
 *
 */
void hsrecord_common_encode_big_endian(uint8_t *buffer,uint32_t value,size_t length);

/** \brief 将大端序字节解码为数值
 *
 * \param buffer const uint8_t* 源缓冲
 * \param length size_t 字节数
 * \return uint32_t 数值
 *
 */
uint32_t hsrecord_common_decode_big_endian(const uint8_t *buffer,size_t length);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSRECORD_COMMON_H__
