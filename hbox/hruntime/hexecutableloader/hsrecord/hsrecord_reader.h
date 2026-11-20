/***************************************************************
 * Name:      hsrecord_reader.h
 * Purpose:   声明hsrecord_reader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSRECORD_READER_H__
#define __HSRECORD_READER_H__
#include "hsrecord_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    HSRECORD_READER_OPERATE_HEADER,                 /**< S0 文件头 */
    HSRECORD_READER_OPERATE_DATA,                   /**< S1/S2/S3 数据 */
    HSRECORD_READER_OPERATE_DATA_COUNT,             /**< S5/S6 数据记录计数 */
    HSRECORD_READER_OPERATE_START_ADDRESS           /**< S7/S8/S9 起始地址 */
} hsrecord_reader_operate_t;

/** \brief 读取错误
 *
 */
typedef enum
{
    HSRECORD_READER_ERROR_NONE                  =   0,      /**< 无错误 */
    HSRECORD_READER_ERROR_LINE_TOO_LONG         =   1,      /**< 行过长 */
    HSRECORD_READER_ERROR_INVALID_HEX           =   2,      /**< 十六进制数据无效 */
    HSRECORD_READER_ERROR_INVALID_LENGTH        =   3,      /**< 记录长度无效 */
    HSRECORD_READER_ERROR_INVALID_CHECKSUM      =   4,      /**< 校验错误 */
    HSRECORD_READER_ERROR_INVALID_RECORD_TYPE   =   5,      /**< 记录类型无效 */
    HSRECORD_READER_ERROR_DATA_COUNT_MISMATCH   =   6       /**< 数据记录计数不匹配 */
} hsrecord_reader_error_t;

typedef struct hsrecord_reader hsrecord_reader_t;
/** \brief 操作回调
 *
 * \param reader hsrecord_reader_t* 读取者指针
 * \param op hsrecord_reader_operate_t 操作
 * \param address uint32_t 地址(对于数据记录为数据起始地址,对于计数记录为记录数,对于起始地址记录为起始地址)
 * \param data const uint8_t* 数据(文件头或数据记录)
 * \param data_len size_t 数据长度
 *
 */
typedef void (*hsrecord_reader_operate_callback_t)(hsrecord_reader_t *reader,hsrecord_reader_operate_t op,uint32_t address,const uint8_t *data,size_t data_len);

struct hsrecord_reader
{
    hsrecord_common_buffer_t buffer;
    hsrecord_reader_operate_callback_t op;
    void *usr;
    uint32_t data_count;                                /**< 已读取的数据记录数 */
    uint32_t line_number;                               /**< 当前行号 */
    hsrecord_reader_error_t error;                      /**< 首个错误 */
};

/** \brief 读取者初始化
 *
 * \param reader hsrecord_reader_t* 读取者指针
 * \param op hsrecord_reader_operate_callback_t 操作回调
 * \param usr void * 用户参数
 *
 */
void hsrecord_reader_init(hsrecord_reader_t *reader,hsrecord_reader_operate_callback_t op,void *usr);

/** \brief 读取者输入数据
 *
 * \param reader hsrecord_reader_t* 读取者指针
 * \param data const char*  数据指针
 * \param data_len size_t   数据长度
 *
 */
void hsrecord_reader_input(hsrecord_reader_t *reader,const char *data,size_t data_len);

/** \brief 读取者结束输入
 *
 * 处理未以换行符结束的最后一行。
 *
 * \param reader hsrecord_reader_t* 读取者指针
 *
 */
void hsrecord_reader_finish(hsrecord_reader_t *reader);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSRECORD_READER_H__
