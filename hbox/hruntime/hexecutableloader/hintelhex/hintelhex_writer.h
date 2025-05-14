/***************************************************************
 * Name:      hintelhex_writer.h
 * Purpose:   声明hintelhex_writer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HINTELHEX_WRITER_H__
#define __HINTELHEX_WRITER_H__
#include "hintelhex_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hintelhex_writer hintelhex_writer_t;

/** \brief 写入者写入行(注意：需要用户实现回车换行符的写入)
 *
 * \param writer hintelhex_writer_t* 写入行
 * \param line const char* 行内容(不含回车换行符)
 * \param line_length size_t 行长度
 *
 */
typedef void (*hintelhex_writer_writeline_t)(hintelhex_writer_t *writer,const char *line,size_t line_length);

/*
 * 每次写入的数据的最大长度，默认16字节
 */
#ifndef HINTELHEX_WRITER_PAGE_SIZE
#define HINTELHEX_WRITER_PAGE_SIZE  16
#endif // HINTELHEX_WRITER_PAGE_SIZE

struct hintelhex_writer
{
    hintelhex_common_buffer_t buffer;
    hintelhex_writer_writeline_t writeline;
    void *usr;
};


/** \brief 写入者初始化
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param writeline hintelhex_writer_writeline_t 写行
 * \param usr void* 用户参数
 *
 */
void hintelhex_writer_init(hintelhex_writer_t *writer,hintelhex_writer_writeline_t writeline,void *usr);

/** \brief 写入者写入数据
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param address uint32_t  写入起始地址
 * \param data const uint8_t* 数据
 * \param data_len size_t   数据长度
 *
 */
void hintelhex_writer_write_data(hintelhex_writer_t *writer,uint32_t address,const uint8_t *data,size_t data_len);

/** \brief 写入者写入文件结束
 *
 * \param writer hintelhex_writer_t* 写入者
 *
 */
void hintelhex_writer_write_end_of_file(hintelhex_writer_t *writer);

/** \brief 写入者写入扩展段地址
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param segment_address uint16_t 段地址
 *
 */
void hintelhex_writer_write_extended_segment_address(hintelhex_writer_t *writer,uint16_t segment_address);

/** \brief 写入者写入起始（段）地址
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param cs_ip uint32_t CS:IP值，两个16位寄存器
 *
 */
void hintelhex_writer_write_start_segment_address(hintelhex_writer_t *writer,uint32_t cs_ip);

/** \brief 写入者写入扩展线性地址
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param address_high uint16_t 32位线性地址高16位
 *
 */
void hintelhex_writer_write_extended_linear_address(hintelhex_writer_t *writer,uint16_t address_high);

/** \brief 写入者写入起始（线性）地址
 *
 * \param writer hintelhex_writer_t* 写入者
 * \param eip uint32_t EIP值，32位寄存器
 *
 */
void hintelhex_writer_write_start_linear_address(hintelhex_writer_t *writer,uint32_t eip);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HINTELHEX_WRITER_H__
