/***************************************************************
 * Name:      htime.h
 * Purpose:   声明htime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HTIME_H__
#define __HTIME_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "time.h"

/** \brief 获取当前时间
 *
 * \param arg time_t* 用于存储当前时间的指针
 * \return time_t 当前时间
 *
 */
time_t htime( time_t* arg);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HTIME_H__
