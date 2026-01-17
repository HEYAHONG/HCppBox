/***************************************************************
 * Name:      hferror.h
 * Purpose:   声明hferror接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFERROR_H__
#define __HFERROR_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 检查文件错误
 *
 * \param stream FILE* 文件指针
 * \return int 0=未出错，非0=出错
 *
 */
int hferror( FILE *stream );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFERROR_H__
