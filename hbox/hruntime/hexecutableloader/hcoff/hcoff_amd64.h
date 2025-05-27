/***************************************************************
 * Name:      hcoff_amd64.h
 * Purpose:   声明hcoff_amd64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_AMD64_H__
#define __HCOFF_AMD64_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#include "hruntime.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef hcoff_section_relocation_t hcoff_amd64_section_relocation_t;

typedef struct hcoff_amd64_section_relocation_info hcoff_amd64_section_relocation_info_t;
struct hcoff_amd64_section_relocation_info
{
    const char *name;
    uint64_t src_mask;
    uint64_t dst_mask;      /**< src_mask与dst_mask用于确定最终写入重定位地址的值:((x & ~dst_mask) | (((x & src_mask) + diff) & dst_mask)),其中x为原值，diff为差值或者绝对值 */
    uint16_t type;
    uint8_t  size_bytes;    /**< 待写入的字节数 */
};

/** \brief COFF AMD64获取重定位信息
 *
 * \param relocation hcoff_amd64_section_relocation_t* 重定位指针
 * \return const hcoff_amd64_section_relocation_info_t* 重定位信息指针
 *
 */
const hcoff_amd64_section_relocation_info_t * hcoff_amd64_section_relocation_info_get(const hcoff_amd64_section_relocation_t *relocation);


/** \brief COFF AMD64重定位
 *
 * \param relocation_addr uint8_t* 实际存储待重定位值的地址，通常通过节加载至内存的地址+重定位中r_vaddr成员计算出。
 * \param diff uint64_t 差值或者绝对值,具体由重定向的类型确定。注意:对于位数小于32位的负数，应当先转化为int32_t再转化为uint32_t。
 * \param relocation const hcoff_amd64_section_relocation_t* 重定位指针
 * \return bool 是否成功重定位
 *
 */
bool hcoff_amd64_section_relocation_doit(uint8_t *relocation_addr,uint64_t diff,const hcoff_amd64_section_relocation_t *relocation);


/** \brief 获取是否是读取与执行的节
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否是读取与执行的节
 *
 */
bool hcoff_amd64_is_rx_section(size_t index,hcoff_file_input_t *input_file);


/** \brief 获取具有读取与执行的节的偏移
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 偏移，-1=失败
 *
 */
intptr_t hcoff_amd64_rx_section_offset_get(size_t index,hcoff_file_input_t *input_file);

/** \brief 获取具有读取与执行的节的大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 大小
 *
 */
size_t hcoff_amd64_rx_section_size_get(hcoff_file_input_t *input_file);

/** \brief 获取是否是只读的节
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否是只读的节
 *
 */
bool hcoff_amd64_is_ro_section(size_t index,hcoff_file_input_t *input_file);


/** \brief 获取具有只读的节的偏移
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 偏移，-1=失败
 *
 */
intptr_t hcoff_amd64_ro_section_offset_get(size_t index,hcoff_file_input_t *input_file);

/** \brief 获取具有只读的节的大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 大小
 *
 */
size_t hcoff_amd64_ro_section_size_get(hcoff_file_input_t *input_file);

/** \brief 获取是否是读取与写入的节
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否是读取与写入的节
 *
 */
bool hcoff_amd64_is_rw_section(size_t index,hcoff_file_input_t *input_file);


/** \brief 获取具有读取与写入的节的偏移
 *
 * \param index size_t 引索
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 偏移，-1=失败
 *
 */
intptr_t hcoff_amd64_rw_section_offset_get(size_t index,hcoff_file_input_t *input_file);

/** \brief 获取具有读取与写入的节的大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 大小
 *
 */
size_t hcoff_amd64_rw_section_size_get(hcoff_file_input_t *input_file);


typedef struct hcoff_amd64_relocatable  hcoff_amd64_relocatable_t;
struct hcoff_amd64_relocatable
{
    void * rx_sections_ptr;
    size_t rx_sections_size;
    void * ro_sections_ptr;
    size_t ro_sections_size;
    void * rw_sections_ptr;
    size_t rw_sections_size;
    hruntime_symbol_t * symbol_table;
    size_t             symbol_table_size;
};

typedef const hruntime_symbol_t *(*hcoff_amd64_external_symbol_find_t)(const char *name);

/** \brief 创建可重定位对象
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param extern_symbol hcoff_amd64_external_symbol_find_t 外部符号寻找回调
 * \return const hcoff_amd64_relocatable_t* 可重定位对象指针,失败返回NULL
 *
 */
const hcoff_amd64_relocatable_t *hcoff_amd64_relocatable_new(hcoff_file_input_t *input_file,hcoff_amd64_external_symbol_find_t extern_symbol);


typedef const hruntime_symbol_t *(*hcoff_amd64_external_symbol_find_with_usr_t)(const char *name,void *usr);

/** \brief 创建可重定位对象
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param extern_symbol hcoff_amd64_external_symbol_find_with_usr_t 外部符号寻找回调
 * \param usr void * 用户参数
 * \return const hcoff_amd64_relocatable_t* 可重定位对象指针,失败返回NULL
 *
 */
const hcoff_amd64_relocatable_t *hcoff_amd64_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_amd64_external_symbol_find_with_usr_t extern_symbol,void *usr);

/** \brief 删除可重定位对象
 *
 * \param amd64_relocatable const hcoff_amd64_relocatable_t* 可重定位对象指针
 *
 */
void hcoff_amd64_relocatable_delete(const hcoff_amd64_relocatable_t * amd64_relocatable);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_AMD64_H__
