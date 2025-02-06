/***************************************************************
 * Name:      hsimulator.h
 * Purpose:   实现hsimulator接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsimulator.h"

#include "simulator/common/hs_common.h"
#include "simulator/common/hs_common.c"


#include "simulator/rp_pio_sm/rp_pio_sm.h"
#include "simulator/rp_pio_sm/rp_pio_sm.c"

#include "simulator/mcs_51/mcs_51.h"
#include "simulator/mcs_51/mcs_51_core.c"

#ifdef __HS_RISC_V_H__
#undef __HS_RISC_V_H__
#endif // __HS_RISC_V_H__
#ifndef HS_RISC_V_BUILD_C_SOURCE
#define HS_RISC_V_BUILD_C_SOURCE    1
#endif // HS_RISC_V_BUILD_C_SOURCE
#include "simulator/risc-v/risc-v.h"
#include "simulator/risc-v/risc-v_common.c"

