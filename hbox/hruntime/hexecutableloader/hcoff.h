/***************************************************************
 * Name:      hcoff.h
 * Purpose:   声明hcoff接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_H__
#define __HCOFF_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hcoff/hcoff_common.h"
#include "hcoff/hcoff_arch.h"
#include "hcoff/hcoff_relocatable.h"
#include "hcoff/hcoff_arch_i386.h"
#include "hcoff/hcoff_arch_amd64.h"
#include "hcoff/hcoff_arch_aarch64.h"
#include "hcoff/hcoff_arch_arm.h"
#include "hcoff/hcoff_arch_riscv.h"
#include "hcoff/hcoff_arch_loongarch.h"
#include "hcoff/hcoff_pe.h"
#include "hcoff/hcoff_namespace.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_H__
