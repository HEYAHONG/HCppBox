/***************************************************************
 * Name:      hcoff.c
 * Purpose:   实现hcoff接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff.h"

#include "hcoff/hcoff_common.c"
#include "hcoff/hcoff_arch.c"
#include "hcoff/hcoff_relocatable.c"
#include "hcoff/hcoff_arch_i386.c"
#include "hcoff/hcoff_arch_amd64.c"
#include "hcoff/hcoff_arch_aarch64.c"
#include "hcoff/hcoff_arch_arm.c"
#include "hcoff/hcoff_arch_riscv.c"
#include "hcoff/hcoff_arch_loongarch.c"
#include "hcoff/hcoff_pe.c"
#include "hcoff/hcoff_namespace.c"
