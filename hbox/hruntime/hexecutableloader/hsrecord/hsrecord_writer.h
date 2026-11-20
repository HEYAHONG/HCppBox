/***************************************************************
 * Name:      hsrecord_writer.h
 * Purpose:   声明hsrecord_writer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSRECORD_WRITER_H__
#define __HSRECORD_WRITER_H__
#include "hsrecord_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hsrecord_writer hsrecord_writer_t;

/** \brief 写入者写入行(注意：需要用户实现回车换行符的写入)
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param line const char* 行内容(不含回车换行符)
 * \param line_length size_t 行长度
 *
 */
typedef void (*hsrecord_writer_writeline_t)(hsrecord_writer_t *writer,const char *line,size_t line_length);

/** \brief 写入错误
 *
 */
typedef enum
{
    HSRECORD_WRITER_ERROR_NONE                  =   0,      /**< 无错误 */
    HSRECORD_WRITER_ERROR_DATA_TOO_LONG         =   1,      /**< 数据过长 */
    HSRECORD_WRITER_ERROR_INVALID_PARAM         =   2       /**< 参数无效 */
} hsrecord_writer_error_t;

/** \brief 地址类型(地址宽度)
 *
 */
typedef enum
{
    HSRECORD_WRITER_ADDRESS_TYPE_16BIT          =   2,      /**< 16位地址(S1/S9) */
    HSRECORD_WRITER_ADDRESS_TYPE_24BIT          =   3,      /**< 24位地址(S2/S8) */
    HSRECORD_WRITER_ADDRESS_TYPE_32BIT          =   4       /**< 32位地址(S3/S7) */
} hsrecord_writer_address_type_t;

/*
 * 每次写入的数据的最大长度，默认32字节(与srecord默认值一致)
 */
#ifndef HSRECORD_WRITER_PAGE_SIZE
#define HSRECORD_WRITER_PAGE_SIZE  32
#endif // HSRECORD_WRITER_PAGE_SIZE

struct hsrecord_writer
{
    hsrecord_common_buffer_t buffer;
    hsrecord_writer_writeline_t writeline;
    void *usr;
    size_t page_size;                                   /**< 每个数据记录的最大数据长度 */
    size_t address_length;                              /**< 地址类型(2:16位,3:24位,4:32位),作为最小地址宽度 */
    uint32_t data_count;                                /**< 已写入的数据记录数 */
    bool data_count_written;                            /**< 是否已写入数据记录计数 */
    bool enable_data_count;                             /**< 是否在写入起始地址时自动写入数据记录计数 */
    hsrecord_writer_error_t error;                      /**< 首个错误 */
};

/** \brief 写入者初始化
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param writeline hsrecord_writer_writeline_t 写行
 * \param usr void* 用户参数
 *
 */
void hsrecord_writer_init(hsrecord_writer_t *writer,hsrecord_writer_writeline_t writeline,void *usr);

/** \brief 写入者设置每个数据记录的最大数据长度
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param page_size size_t 最大数据长度(1~HSRECORD_MAX_DATA_LEN)
 *
 */
void hsrecord_writer_set_page_size(hsrecord_writer_t *writer,size_t page_size);

/** \brief 写入者设置地址类型(地址宽度)
 *
 * 与srecord的-address-length选项一致,该值为最小地址宽度:
 * 2使用S1/S9(地址不足时自动升级),3使用S2/S8,4使用S3/S7。
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param address_length size_t 地址宽度(2~4,其它值将被限制到该范围)
 *
 */
void hsrecord_writer_set_address_length(hsrecord_writer_t *writer,size_t address_length);

/** \brief 写入者设置地址类型
 *
 * 与hsrecord_writer_set_address_length等价。
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param address_type hsrecord_writer_address_type_t 地址类型
 *
 */
void hsrecord_writer_set_address_type(hsrecord_writer_t *writer,hsrecord_writer_address_type_t address_type);

/** \brief 写入者写入文件头(S0)
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param data const uint8_t* 头部数据
 * \param data_len size_t 头部数据长度
 *
 */
void hsrecord_writer_write_header(hsrecord_writer_t *writer,const uint8_t *data,size_t data_len);

/** \brief 写入者写入数据
 *
 * 根据地址自动选择S1/S2/S3记录。
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param address uint32_t  写入起始地址
 * \param data const uint8_t* 数据
 * \param data_len size_t   数据长度
 *
 */
void hsrecord_writer_write_data(hsrecord_writer_t *writer,uint32_t address,const uint8_t *data,size_t data_len);

/** \brief 写入者写入数据记录计数(S5/S6)
 *
 * \param writer hsrecord_writer_t* 写入者
 *
 */
void hsrecord_writer_write_data_count(hsrecord_writer_t *writer);

/** \brief 写入者写入起始地址(S7/S8/S9)
 *
 * \param writer hsrecord_writer_t* 写入者
 * \param address uint32_t 起始地址
 *
 */
void hsrecord_writer_write_start_address(hsrecord_writer_t *writer,uint32_t address);

/** \brief 写入者写入文件结束(起始地址为0的S9记录)
 *
 * \param writer hsrecord_writer_t* 写入者
 *
 */
void hsrecord_writer_write_end_of_file(hsrecord_writer_t *writer);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSRECORD_WRITER_H__
