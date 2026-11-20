/***************************************************************
 * Name:      hcoff_arch.c
 * Purpose:   实现hcoff通用架构接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_arch.h"

const hcoff_arch_ops_t *hcoff_arch_ops_get(uint16_t machine)
{
    const hcoff_arch_ops_t *ret=NULL;
    switch(machine)
    {
    case HCOFF_FILEHEADER_F_MAGIC_ARM:
    case HCOFF_FILEHEADER_F_MAGIC_ARMNT:
    case HCOFF_FILEHEADER_F_MAGIC_THUMB:
    {
        ret=&hcoff_arm_arch_ops;
    }
    break;
    case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
    {
        ret=&hcoff_riscv_arch_ops;
    }
    break;
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
    {
        ret=&hcoff_loongarch_arch_ops;
    }
    break;
    default:
    {
    }
    break;
    }
    return ret;
}

const hcoff_arch_relocation_info_t *hcoff_arch_relocation_info_get(uint16_t machine,uint16_t type)
{
    const hcoff_arch_ops_t *ops=hcoff_arch_ops_get(machine);
    if(ops!=NULL && ops->relocation_info_get!=NULL)
    {
        return ops->relocation_info_get(type);
    }
    return NULL;
}
