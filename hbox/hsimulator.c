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


#include "simulator/rp_pio_sm/rp_pio.h"
#include "simulator/rp_pio_sm/rp_pio_sm.c"
#include "simulator/rp_pio_sm/rp_pio_rom.c"

#include "simulator/mcs_51/mcs_51.h"
#include "simulator/mcs_51/mcs_51_common.c"
#include "simulator/mcs_51/mcs_51_disassembly.c"
#include "simulator/mcs_51/mcs_51_rom.c"
#include "simulator/mcs_51/mcs_51_ram.c"
#include "simulator/mcs_51/mcs_51_core.c"
#include "simulator/mcs_51/mcs_51_serial.c"


#include "simulator/risc-v/risc-v.h"
#include "simulator/risc-v/risc-v_opcodes.c"
#include "simulator/risc-v/risc-v_common.c"
#include "simulator/risc-v/risc-v_core_rv32.c"

