/***************************************************************
 * Name:      hintelhex_reader.h
 * Purpose:   声明hintelhex_reader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HINTELHEX_READER_H__
#define __HINTELHEX_READER_H__
#include "hintelhex_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    HINTELHEX_READER_OPERATE_DATA,
    HINTELHEX_READER_OPERATE_END_OF_FILE,
    HINTELHEX_READER_OPERATE_EXTENDED_SEGMENT_ADDRESS,
    HINTELHEX_READER_OPERATE_START_SEGMENT_ADDRESS,
    HINTELHEX_READER_OPERATE_EXTENDED_LINEAR_ADDRESS,
    HINTELHEX_READER_OPERATE_START_LINEAR_ADDRESS,
} hintelhex_reader_operate_t;

typedef struct hintelhex_reader hintelhex_reader_t;
/** \brief 操作回调
 *
 * \param reader hintelhex_reader_t* 读取者指针
 * \param op hintelhex_reader_operate_t 操作
 * \param address uint32_t 地址
 * \param data const uint8_t*
 * \param data_len size_t
 *
 */
typedef void (*hintelhex_reader_operate_callback_t)(hintelhex_reader_t *reader,hintelhex_reader_operate_t op,uint32_t address,const uint8_t *data,size_t data_len);

struct hintelhex_reader
{
    hintelhex_common_buffer_t buffer;
    hintelhex_reader_operate_callback_t op;
    void *usr;
};

/** \brief 读取者初始化
 *
 * \param reader hintelhex_reader_t* 读取者指针
 * \param op hintelhex_reader_operate_callback_t 操作回调
 * \param usr void * 用户参数
 *
 */
void hintelhex_reader_init(hintelhex_reader_t *reader,hintelhex_reader_operate_callback_t op,void *usr);

/** \brief 读取者输入数据
 *
 * \param reader hintelhex_reader_t* 读取者指针
 * \param data const char*  数据指针
 * \param data_len size_t   数据长度
 *
 */
void hintelhex_reader_input(hintelhex_reader_t *reader,const char *data,size_t data_len);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HINTELHEX_READER_H__
