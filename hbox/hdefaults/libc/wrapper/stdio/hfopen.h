/***************************************************************
 * Name:      hfopen.h
 * Purpose:   声明hfopen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFOPEN_H__
#define __HFOPEN_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 打开文件
 *
 * \param filename const char* 文件名称
 * \param mode const char* 文件模式
 * \return FILE* 文件指针，失败返回NULL
 *
 */
FILE * hfopen(const char* filename, const char* mode);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFOPEN_H__
