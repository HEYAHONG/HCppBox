/***************************************************************
 * Name:      hfread.h
 * Purpose:   声明hfread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFREAD_H__
#define __HFREAD_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 从文件读取内容
 *
 * \param void*buffer 缓冲区
 * \param obj_size size_t 读取的对象大小
 * \param obj_count size_t 读取的对象数量
 * \param stream FILE* 文件指针
 * \return size_t 已读取的对象数量(可能小于obj_count)
 *
 */
size_t hfread(void*buffer,size_t obj_size,size_t obj_count,FILE * stream );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFREAD_H__
