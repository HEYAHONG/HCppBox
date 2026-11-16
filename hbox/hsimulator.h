/***************************************************************
 * Name:      hsimulator.h
 * Purpose:   声明hsimulator接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIMULATOR_H__
#define __HSIMULATOR_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hsimulator/common/hs_common.h"
#include "hsimulator/rp_pio_sm/rp_pio.h"
#include "hsimulator/mcs_51/mcs_51.h"
#include "hsimulator/risc-v/risc-v.h"
#include "hsimulator/moxie/moxie_core.h"
#include "hsimulator/moxie/moxie_core_el.h"
#include "hsimulator/moxie/moxie_moxiebox_common.h"
#include "hsimulator/moxie/moxie_moxiebox.h"
#include "hsimulator/moxie/moxie_moxiebox_el.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSIMULATOR_H__

