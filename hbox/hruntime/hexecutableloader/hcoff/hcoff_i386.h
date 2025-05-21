/***************************************************************
 * Name:      hcoff_i386.h
 * Purpose:   声明hcoff_i386接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_I386_H__
#define __HCOFF_I386_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef hcoff_section_relocation_t hcoff_i386_section_relocation_t;

typedef struct hcoff_i386_section_relocation_info hcoff_i386_section_relocation_info_t;
struct hcoff_i386_section_relocation_info
{
    const char *name;
    uint32_t src_mask;
    uint32_t dst_mask;      /**< src_mask与dst_mask用于确定最终写入重定位地址的值:((x & ~dst_mask) | (((x & src_mask) + diff) & dst_mask)),其中x为原值，diff为差值或者绝对值 */
    uint16_t type;
    uint8_t  size_bytes;    /**< 待写入的字节数 */
};

/** \brief COFF I386获取重定位信息
 *
 * \param relocation hcoff_i386_section_relocation_t* 重定位指针
 * \return const hcoff_i386_section_relocation_info_t* 重定位信息指针
 *
 */
const hcoff_i386_section_relocation_info_t * hcoff_i386_section_relocation_info_get(const hcoff_i386_section_relocation_t *relocation);


/** \brief COFF I386重定位
 *
 * \param relocation_addr uint8_t* 实际存储待重定位值的地址，通常通过节加载至内存的地址+重定位中r_vaddr成员计算出。
 * \param diff uint32_t 差值或者绝对值,具体由重定向的类型确定。注意:对于位数小于32位的负数，应当先转化为int32_t再转化为uint32_t。
 * \param relocation const hcoff_i386_section_relocation_t* 重定位指针
 * \return bool 是否成功重定位
 *
 */
bool hcoff_i386_section_relocation_doit(uint8_t *relocation_addr,uint32_t diff,const hcoff_i386_section_relocation_t *relocation);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_I386_H__
