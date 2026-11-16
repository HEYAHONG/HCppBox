/***************************************************************
 * Name:      helf_moxie.h
 * Purpose:   声明helf_moxie接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_MOXIE_H__
#define __HELF_MOXIE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "helf_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HELF_MOXIE_SECTION_NAME_MAX
#define HELF_MOXIE_SECTION_NAME_MAX 64
#endif

/*
 * Moxie ELF节信息
 */
typedef struct helf_moxie_section_info
{
    char        name[HELF_MOXIE_SECTION_NAME_MAX];   /**< 节名称 */
    uint32_t    type;                               /**< 节类型(SHT_*) */
    uint32_t    flags;                              /**< 节标志(SHF_*) */
    uint32_t    address;                            /**< 运行时地址 */
    uint32_t    offset;                             /**< 文件偏移 */
    uint32_t    size;                               /**< 节大小 */
    uint32_t    link;                               /**< 关联节索引 */
    uint32_t    info;                               /**< 附加信息 */
    uint32_t    addralign;                          /**< 对齐 */
    uint32_t    entsize;                            /**< 表项大小 */
} helf_moxie_section_info_t;

/** \brief 是否为Moxie ELF文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Moxie ELF文件(ELF32,机器类型为Moxie或旧版Moxie)
 *
 */
bool helf_moxie_is_moxie(helf_file_input_t *input_file);

/** \brief 是否为Moxie大端ELF文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Moxie大端ELF文件
 *
 */
bool helf_moxie_is_big_endian(helf_file_input_t *input_file);

/** \brief 是否为Moxie可执行文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Moxie可执行文件
 *
 */
bool helf_moxie_is_executable(helf_file_input_t *input_file);

/** \brief 获取Moxie ELF入口地址
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param entry uint32_t* 入口地址
 * \return bool 是否成功
 *
 */
bool helf_moxie_entry_get(helf_file_input_t *input_file,uint32_t *entry);

/** \brief 获取Moxie ELF节数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 节数量
 *
 */
size_t helf_moxie_section_count_get(helf_file_input_t *input_file);

/** \brief 获取Moxie ELF节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 节索引
 * \param info helf_moxie_section_info_t* 节信息
 * \return bool 是否成功
 *
 */
bool helf_moxie_section_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_section_info_t *info);

/** \brief 是否为读取与执行节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行节
 *
 */
bool helf_moxie_is_rx_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_moxie_rx_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_moxie_rx_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为只读节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读节
 *
 */
bool helf_moxie_is_ro_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_moxie_ro_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_moxie_ro_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为读取与写入节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入节
 *
 */
bool helf_moxie_is_rw_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_moxie_rw_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_moxie_rw_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为BSS节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为BSS节
 *
 */
bool helf_moxie_is_bss_section(size_t index,helf_file_input_t *input_file);

/*
 * Moxie ELF程序段信息(程序头视角)
 */
typedef struct helf_moxie_segment_info
{
    uint32_t    type;                   /**< 段类型(PT_*) */
    uint32_t    flags;                  /**< 段标志(PF_*) */
    uint32_t    offset;                 /**< 文件偏移 */
    uint32_t    vaddr;                  /**< 虚拟地址 */
    uint32_t    paddr;                  /**< 物理地址 */
    uint32_t    filesz;                 /**< 文件中的大小 */
    uint32_t    memsz;                  /**< 内存中的大小 */
    uint32_t    addralign;              /**< 对齐 */
} helf_moxie_segment_info_t;

/** \brief 获取Moxie ELF程序段数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序段数量(程序头数量)
 *
 */
size_t helf_moxie_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取Moxie ELF程序段信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 程序头索引
 * \param info helf_moxie_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_moxie_segment_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_segment_info_t *info);

/** \brief 是否为可加载程序段(PT_LOAD)
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为可加载程序段
 *
 */
bool helf_moxie_is_load_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与执行程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行程序段
 *
 */
bool helf_moxie_is_rx_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为只读程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读程序段
 *
 */
bool helf_moxie_is_ro_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与写入程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入程序段
 *
 */
bool helf_moxie_is_rw_segment(size_t index,helf_file_input_t *input_file);

/** \brief 获取可加载程序段(PT_LOAD)数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 可加载程序段数量
 *
 */
size_t helf_moxie_load_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取第index个可加载程序段(PT_LOAD)信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 可加载程序段索引(仅计数PT_LOAD段)
 * \param info helf_moxie_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_moxie_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_segment_info_t *info);

/** \brief 获取所有可加载程序段文件大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 文件大小之和
 *
 */
size_t helf_moxie_load_segment_total_filesz_get(helf_file_input_t *input_file);

/** \brief 获取所有可加载程序段内存大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 内存大小之和
 *
 */
size_t helf_moxie_load_segment_total_memsz_get(helf_file_input_t *input_file);

/** \brief 获取Moxie ELF程序头数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序头数量
 *
 */
size_t helf_moxie_program_header_count_get(helf_file_input_t *input_file);

/** \brief 获取Moxie ELF程序头
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 程序头索引
 * \param programheader helf_elf32_program_header_t* 程序头
 * \return bool 是否成功
 *
 */
bool helf_moxie_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HELF_MOXIE_H__
