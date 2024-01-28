/***************************************************************
 * Name:      hringbuf.h
 * Purpose:   声明hringbuf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HRINGBUF_H_INCLUDED__
#define __HRINGBUF_H_INCLUDED__

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
本文件用于实现简易环形缓冲区。
    简易环形缓冲区,不考虑效率及异常情况
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hringbuf; /**< hringbuf前置声明 */
typedef struct hringbuf hringbuf_t; /**< hringbuf_t类型定义 */

/** \brief 获取环形缓冲区指针
 *
 * \param buff uint8_t* 缓冲区指针,注意:指针要符合对齐,缓冲区应当初始化为0，否则可能出现异常
 * \param length size_t 缓冲区大小,注意:缓冲区前部分将用于环形缓冲区信息，因此缓冲区不可过小
 * \return hringbuf_t* hringbuf_t指针,失败返回NULL
 *
 */
hringbuf_t *hringbuf_get(uint8_t *buff,size_t length);

/** \brief 设置环形缓冲区锁
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 */
void hringbuf_set_lock(hringbuf_t * buff,void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *));


/** \brief 获取hringbuf_t的用户指针(usr)
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \return void* usr指针
 *
 */
void * hringbuf_get_usr_ptr(hringbuf_t * buff);

/** \brief 获取hringbuf_t的缓冲区中数据长度
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \return size_t 缓冲区中数据长度
 *
 */
size_t hringbuf_get_length(hringbuf_t * buff);

/** \brief 获取hringbuf_t的最大数据长度
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \return size_t 最大数据长度
 *
 */
size_t hringbuf_get_max_length(hringbuf_t * buff);

/** \brief 向环形缓冲区写入数据
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已写入的长度
 *
 */
size_t hringbuf_input(hringbuf_t * buff,const uint8_t *data,size_t data_length);

/** \brief 从环形缓冲区读取数据
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已读取的长度
 *
 */
size_t hringbuf_output(hringbuf_t * buff,uint8_t *data,size_t data_length);

/** \brief 从环形缓冲区读取数据(不清除缓冲区的数据)
 *
 * \param buff hringbuf_t* hringbuf_t指针
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已读取的长度
 *
 */
size_t hringbuf_output_no_clear(hringbuf_t * buff,uint8_t *data,size_t data_length);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRINGBUF_H_INCLUDED__
