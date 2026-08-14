/***************************************************************
 * Name:      hfflush.h
 * Purpose:   声明hfflush接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HFFLUSH_H__
#define __HFFLUSH_H__
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 清空文件缓存
 *
 * \param stream FILE* 待清空缓存的文件
 * \return int 成功返回0,失败返回EOF(-1)
 *
 */
int hfflush(FILE * stream);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HFFLUSH_H__
