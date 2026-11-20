/***************************************************************
 * Name:      hcoff_arch.h
 * Purpose:   声明hcoff通用架构接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_ARCH_H__
#define __HCOFF_ARCH_H__
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "hcoff_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE      (1U << 0)  /**< PC相对重定位 */
#define HCOFF_ARCH_RELOCATION_FLAG_IGNORED          (1U << 1)  /**< 可忽略的重定位 */

typedef struct hcoff_arch_relocation_info hcoff_arch_relocation_info_t;
struct hcoff_arch_relocation_info
{
    const char *name;           /**< 重定位名称 */
    uint16_t type;              /**< 重定位类型 */
    uint8_t  size_bytes;        /**< 重定位作用的数据大小 */
    uint8_t  flags;             /**< 标志位,见HCOFF_ARCH_RELOCATION_FLAG_* */
};

typedef bool (*hcoff_arch_relocation_doit_t)(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,uint16_t type);

typedef const hcoff_arch_relocation_info_t *(*hcoff_arch_relocation_info_get_t)(uint16_t type);

typedef struct hcoff_arch_ops hcoff_arch_ops_t;
struct hcoff_arch_ops
{
    uint16_t machine;
    const char *name;
    bool is_64bit;
    size_t relocation_entry_size;
    hcoff_arch_relocation_doit_t relocation_doit;
    hcoff_arch_relocation_info_get_t relocation_info_get;
};

/** \brief 获取COFF架构操作
 *
 * \param machine uint16_t 机器类型,见HCOFF_FILEHEADER_F_MAGIC_*
 * \return const hcoff_arch_ops_t* 架构操作,暂不支持时返回NULL
 *
 */
const hcoff_arch_ops_t *hcoff_arch_ops_get(uint16_t machine);

/** \brief 获取COFF重定位信息
 *
 * \param machine uint16_t 机器类型,见HCOFF_FILEHEADER_F_MAGIC_*
 * \param type uint16_t 重定位类型
 * \return const hcoff_arch_relocation_info_t* 重定位信息
 *
 */
const hcoff_arch_relocation_info_t *hcoff_arch_relocation_info_get(uint16_t machine,uint16_t type);

extern const hcoff_arch_ops_t hcoff_arm_arch_ops;
extern const hcoff_arch_ops_t hcoff_riscv_arch_ops;
extern const hcoff_arch_ops_t hcoff_loongarch_arch_ops;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_ARCH_H__
