/***************************************************************
 * Name:      hfwrite.h
 * Purpose:   声明hfwrite接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFWRITE_H__
#define __HFWRITE_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 向文件写入内容
 *
 * \param const void*buffer 缓冲区
 * \param obj_size size_t 读取的对象大小
 * \param obj_count size_t 读取的对象数量
 * \param stream FILE* 文件指针
 * \return size_t 已读取的对象数量(可能小于obj_count)
 *
 */
size_t hfwrite(const void*buffer,size_t obj_size,size_t obj_count,FILE * stream );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFWRITE_H__
