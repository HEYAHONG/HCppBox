/***************************************************************
 * Name:      hsimulator.h
 * Purpose:   实现hsimulator接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsimulator.h"

#include "hsimulator/common/hs_common.h"
#include "hsimulator/common/hs_common.c"


#include "hsimulator/rp_pio_sm/rp_pio.h"
#include "hsimulator/rp_pio_sm/rp_pio_sm.c"
#include "hsimulator/rp_pio_sm/rp_pio_rom.c"

#include "hsimulator/mcs_51/mcs_51.h"
#include "hsimulator/mcs_51/mcs_51_common.c"
#include "hsimulator/mcs_51/mcs_51_disassembly.c"
#include "hsimulator/mcs_51/mcs_51_rom.c"
#include "hsimulator/mcs_51/mcs_51_ram.c"
#include "hsimulator/mcs_51/mcs_51_core.c"
#include "hsimulator/mcs_51/mcs_51_serial.c"
#include "hsimulator/mcs_51/mcs_51_pin.c"
#include "hsimulator/mcs_51/mcs_51_timer.c"


#include "hsimulator/risc-v/risc-v.h"
#include "hsimulator/risc-v/risc-v_opcodes.c"
#include "hsimulator/risc-v/risc-v_common.c"
#include "hsimulator/risc-v/risc-v_core_rv32.c"

