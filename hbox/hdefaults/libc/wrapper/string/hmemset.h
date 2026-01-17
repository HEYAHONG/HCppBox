/***************************************************************
 * Name:      hmemset.h
 * Purpose:   声明hmemset接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMEMSET_H__
#define __HMEMSET_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "string.h"



/** \brief 填充内存
 *
 * \param dest void* 目标
 * \param ch int 待设置的字节
 * \param dest_count size_t 填充字节数
 * \return void* 目标
 *
 */
void* hmemset(void* dest, int ch, size_t dest_count);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMEMSET_H__
