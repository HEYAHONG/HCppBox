/***************************************************************
 * Name:      mcs_51_common.h
 * Purpose:   声明mcs_51_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_COMMON_H__
#define __HS_MCS_51_COMMON_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  MCS-51主时钟,对于模拟器而言，MCS-51显然一般不会用真实时钟，为方便某些外设（如定时器）的计算，需要定义一个时钟频率。
 *
 *
 */
extern size_t hs_mcs_51_common_clk_freq;
#ifndef HS_MCS_51_COMMON_CLK_FREQ
#define HS_MCS_51_COMMON_CLK_FREQ                   (hs_mcs_51_common_clk_freq)
#endif // HS_MCS_51_COMMON_CLK_FREQ

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_COMMON_H__
