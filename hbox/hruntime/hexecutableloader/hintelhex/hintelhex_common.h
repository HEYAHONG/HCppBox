/***************************************************************
 * Name:      hintelhex_common.h
 * Purpose:   声明hintelhex_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HINTELHEX_COMMON_H__
#define __HINTELHEX_COMMON_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 携带的数据最大长度
 *
 */
#define HINTELHEX_MAX_DATA_LEN 255

/** \brief 每行的最大长度(二进制)
 *
 */
#define HINTELHEX_MAX_LINE_BIN_LEN (HINTELHEX_MAX_DATA_LEN+5)

/** \brief 每行的最大长度
 *
 */
#define HINTELHEX_MAX_LINE_LEN (((HINTELHEX_MAX_LINE_BIN_LEN)*2)+1)

typedef enum
{
    HINTELHEX_RECORD_TYPE_DATA_RECORD                           =   0,
    HINTELHEX_RECORD_TYPE_END_OF_FILE_RECORD                    =   1,
    HINTELHEX_RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS_RECORD       =   2,
    HINTELHEX_RECORD_TYPE_START_SEGMENT_ADDRESS_RECORD          =   3,
    HINTELHEX_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS_RECORD        =   4,
    HINTELHEX_RECORD_TYPE_START_LINEAR_ADDRESS_RECORD           =   5
} hintelhex_record_type_t;

typedef struct hintelhex_common_buffer hintelhex_common_buffer_t;
struct hintelhex_common_buffer
{
    uint32_t address;                                           /**< 线性地址,此处使用高16位存储线性地址高16位 */
    uint32_t segment;                                           /**< 段地址,本代码中数据的地址偏移=线性地址 + (段地址 << 4) */
    uint8_t line[HINTELHEX_MAX_LINE_LEN+1];                     /**< 用于存储文本行 */
    uint8_t line_binary[HINTELHEX_MAX_LINE_BIN_LEN];            /**< 用于存储转化为二进制数据的行 */
};

/** \brief 初始化hintelhex缓冲
 *
 * \param buffer hintelhex_common_buffer_t* hintelhex缓冲指针
 *
 */
void hintelhex_common_buffer_init(hintelhex_common_buffer_t *buffer);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HINTELHEX_COMMON_H__
