/***************************************************************
 * Name:      hclock.h
 * Purpose:   声明hclock接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCLOCK_H__
#define __HCLOCK_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "time.h"

/** \brief 获取处理器时间（类似节拍），可通过除以CLOCKS_PER_SEC转化为秒数
 *
 * \return clock_t 当前处理器时间
 *
 */
clock_t hclock(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCLOCK_H__
