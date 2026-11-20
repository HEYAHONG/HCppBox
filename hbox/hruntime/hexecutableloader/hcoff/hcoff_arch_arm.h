/***************************************************************
 * Name:      hcoff_arch_arm.h
 * Purpose:   声明hcoff_arm接口(ARM/Thumb COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_ARM_H__
#define __HCOFF_ARM_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#include "hcoff_arch.h"
#include "hcoff_relocatable.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * PE/COFF ARM重定位类型
 */
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ABSOLUTE        0x0000
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32          0x0001
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32NB        0x0002
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH24        0x0003
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH11        0x0004
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_TOKEN           0x0005
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX24           0x0008
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX11           0x0009
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_REL32           0x000A
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECTION         0x000E
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECREL          0x000F
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_MOV32           0x0010
#define HCOFF_SECTION_RELOCATION_R_TYPE_ARM_THUMB_MOV32     0x0011

typedef hcoff_section_relocation_t hcoff_arm_section_relocation_t;
typedef hcoff_arch_relocation_info_t hcoff_arm_section_relocation_info_t;

/** \brief COFF ARM获取重定位信息
 *
 * \param relocation hcoff_arm_section_relocation_t* 重定位指针
 * \return const hcoff_arm_section_relocation_info_t* 重定位信息指针
 *
 */
const hcoff_arm_section_relocation_info_t * hcoff_arm_section_relocation_info_get(const hcoff_arm_section_relocation_t *relocation);

/** \brief COFF ARM重定位
 *
 * \param relocation_addr uint8_t* 实际存储待重定位值的地址
 * \param symbol_addr uint64_t 符号地址
 * \param pc_address uintptr_t 重定位处的地址
 * \param image_base uintptr_t 映像基址
 * \param relocation const hcoff_arm_section_relocation_t* 重定位指针
 * \return bool 是否成功重定位
 *
 */
bool hcoff_arm_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_arm_section_relocation_t *relocation);

typedef hcoff_relocatable_t hcoff_arm_relocatable_t;
typedef const struct hruntime_symbol *(*hcoff_arm_external_symbol_find_t)(const char *name);
typedef const struct hruntime_symbol *(*hcoff_arm_external_symbol_find_with_usr_t)(const char *name,void *usr);

/** \brief 获取ARM可重定位对象所需映像缓冲区大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 所需缓冲区大小,失败返回0
 *
 */
size_t hcoff_arm_relocatable_image_size_get(hcoff_file_input_t *input_file);

/** \brief 初始化ARM可重定位对象
 *
 * \param arm_relocatable hcoff_arm_relocatable_t* 可重定位对象(由调用者提供)
 * \param input_file hcoff_file_input_t* 输入文件
 * \param buffer void* 用户提供的映像缓冲区
 * \param buffer_size size_t 缓冲区大小
 * \param extern_symbol hcoff_arm_external_symbol_find_with_usr_t 外部符号寻找回调
 * \param usr void * 用户参数
 * \return bool 是否成功
 *
 */
bool hcoff_arm_relocatable_init(hcoff_arm_relocatable_t *arm_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_arm_external_symbol_find_with_usr_t extern_symbol,void *usr);

/** \brief 清理ARM可重定位对象(不释放用户提供的缓冲区)
 *
 * \param arm_relocatable hcoff_arm_relocatable_t* 可重定位对象指针
 *
 */
void hcoff_arm_relocatable_deinit(hcoff_arm_relocatable_t * arm_relocatable);

/** \brief 创建ARM可重定位对象(内部申请内存)
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param extern_symbol hcoff_arm_external_symbol_find_t 外部符号寻找回调
 * \return const hcoff_arm_relocatable_t* 可重定位对象指针,失败返回NULL
 *
 */
const hcoff_arm_relocatable_t *hcoff_arm_relocatable_new(hcoff_file_input_t *input_file,hcoff_arm_external_symbol_find_t extern_symbol);

/** \brief 创建ARM可重定位对象(带用户参数,内部申请内存)
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param extern_symbol hcoff_arm_external_symbol_find_with_usr_t 外部符号寻找回调
 * \param usr void * 用户参数
 * \return const hcoff_arm_relocatable_t* 可重定位对象指针,失败返回NULL
 *
 */
const hcoff_arm_relocatable_t *hcoff_arm_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_arm_external_symbol_find_with_usr_t extern_symbol,void *usr);

/** \brief 删除ARM可重定位对象(释放内部申请的内存)
 *
 * \param arm_relocatable const hcoff_arm_relocatable_t* 可重定位对象指针
 *
 */
void hcoff_arm_relocatable_delete(const hcoff_arm_relocatable_t * arm_relocatable);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_ARM_H__
