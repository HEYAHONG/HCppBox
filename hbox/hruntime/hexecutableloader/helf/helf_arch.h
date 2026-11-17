/***************************************************************
 * Name:      helf_arch.h
 * Purpose:   声明helf_arch接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_ARCH_H__
#define __HELF_ARCH_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "helf_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HELF_ARCH_SECTION_NAME_MAX
#define HELF_ARCH_SECTION_NAME_MAX 64
#endif

#ifndef HELF_ARCH_SYMBOL_NAME_MAX
#define HELF_ARCH_SYMBOL_NAME_MAX 128
#endif

/*
 * ELF通用节信息(支持32位与64位ELF)
 */
typedef struct helf_arch_section_info
{
    char        name[HELF_ARCH_SECTION_NAME_MAX];   /**< 节名称 */
    uint32_t    name_offset;                        /**< 节名称偏移(sh_name) */
    uint32_t    type;                               /**< 节类型(SHT_*) */
    uint64_t    flags;                              /**< 节标志(SHF_*) */
    uint64_t    address;                            /**< 运行时地址 */
    uint64_t    offset;                             /**< 文件偏移 */
    uint64_t    size;                               /**< 节大小 */
    uint32_t    link;                               /**< 关联节索引 */
    uint32_t    info;                               /**< 附加信息 */
    uint64_t    addralign;                          /**< 对齐 */
    uint64_t    entsize;                            /**< 表项大小 */
} helf_arch_section_info_t;

/*
 * ELF通用程序段信息(支持32位与64位ELF)
 */
typedef struct helf_arch_segment_info
{
    uint32_t    type;                   /**< 段类型(PT_*) */
    uint32_t    flags;                  /**< 段标志(PF_*) */
    uint64_t    offset;                 /**< 文件偏移 */
    uint64_t    vaddr;                  /**< 虚拟地址 */
    uint64_t    paddr;                  /**< 物理地址 */
    uint64_t    filesz;                 /**< 文件中的大小 */
    uint64_t    memsz;                  /**< 内存中的大小 */
    uint64_t    addralign;              /**< 对齐 */
} helf_arch_segment_info_t;

/*
 * ELF通用符号信息(支持32位与64位ELF)
 */
typedef struct helf_arch_symbol_info
{
    char        name[HELF_ARCH_SYMBOL_NAME_MAX];    /**< 符号名称 */
    uint64_t    value;                              /**< 符号值 */
    uint64_t    size;                               /**< 符号大小 */
    uint8_t     info;                               /**< 符号类型与绑定(STT_xxx/STB_xxx) */
    uint8_t     other;                              /**< 符号可见性 */
    uint16_t    shndx;                              /**< 所属节索引 */
} helf_arch_symbol_info_t;

/*
 * ELF重定位节信息
 */
typedef struct helf_arch_relocation_section_info
{
    size_t      index;              /**< 重定位节索引 */
    uint32_t    type;               /**< 节类型(SHT_REL/SHT_RELA) */
    uint64_t    offset;             /**< 文件偏移 */
    uint64_t    size;               /**< 节大小 */
    uint32_t    link;               /**< 符号表节索引 */
    uint32_t    info;               /**< 目标节索引 */
    uint64_t    addralign;          /**< 对齐 */
    uint64_t    entsize;            /**< 表项大小 */
    size_t      count;              /**< 重定位表项数量 */
} helf_arch_relocation_section_info_t;

/*
 * ELF重定位表项信息
 */
typedef struct helf_arch_relocation_info
{
    uint32_t    section_index;      /**< 重定位节索引 */
    uint32_t    symbol_table_index; /**< 符号表节索引 */
    uint32_t    target_section_index;/**< 目标节索引 */
    size_t      index;              /**< 表项在重定位节中的索引 */
    uint64_t    offset;             /**< 重定位地址(r_offset) */
    uint64_t    info;               /**< 原始r_info */
    int64_t     addend;             /**< 加数(r_addend),SHT_REL时为0 */
    uint32_t    symbol;             /**< 符号索引 */
    uint32_t    type;               /**< 重定位类型 */
    uint32_t    type2;              /**< 第二重定位类型(MIPS64) */
    uint32_t    type3;              /**< 第三重定位类型(MIPS64) */
    uint32_t    special_symbol;     /**< 特殊符号(MIPS64 r_ssym) */
    bool        has_addend;         /**< 是否包含加数(SHT_RELA) */
} helf_arch_relocation_info_t;

/*
 * 重定位类型名称表项
 */
typedef struct helf_arch_relocation_type_name
{
    uint32_t    type;               /**< 重定位类型 */
    uint8_t     size;               /**< 重定位目标字节数,0表示未知 */
    const char *name;               /**< 重定位类型名称 */
} helf_arch_relocation_type_name_t;

/*
 * 架构相关节类型名称表项
 */
typedef struct helf_arch_section_type_name
{
    uint32_t    type;               /**< 节类型(SHT_*) */
    const char *name;               /**< 节类型名称 */
} helf_arch_section_type_name_t;

/*
 * ELF动态段信息
 */
typedef struct helf_arch_dynamic_info
{
    int64_t     tag;                /**< 动态项类型(DT_*) */
    uint64_t    value;              /**< 动态项值(d_val/d_ptr) */
} helf_arch_dynamic_info_t;

/*
 * 重定位地址信息
 */
typedef struct helf_arch_relocation_address_info
{
    uint64_t    address;            /**< 待重定位地址(r_offset) */
    uint64_t    file_offset;        /**< 待重定位数据在文件中的偏移 */
    size_t      size;               /**< 待重定位数据长度 */
    bool        in_file;            /**< 数据是否存在于文件中 */
    bool        in_section;         /**< 是否通过节头解析地址 */
    bool        in_segment;         /**< 是否通过程序头解析地址 */
    bool        is_bss;             /**< 是否位于BSS等无文件数据区域 */
} helf_arch_relocation_address_info_t;

/** \brief 判断ELF文件是否属于指定架构
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param machines const uint16_t* 机器类型列表(HELF_EM_*)
 * \param machine_count size_t 机器类型数量
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64),0表示不限制
 * \return bool 是否属于指定架构
 *
 */
bool helf_arch_is(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass);

/** \brief 判断ELF文件是否属于指定架构且为可执行文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param machines const uint16_t* 机器类型列表(HELF_EM_*)
 * \param machine_count size_t 机器类型数量
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64),0表示不限制
 * \return bool 是否属于指定架构且为可执行文件
 *
 */
bool helf_arch_is_executable(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass);

/** \brief 获取ELF入口地址
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param entry uint64_t* 入口地址
 * \return bool 是否成功
 *
 */
bool helf_arch_entry_get(helf_file_input_t *input_file,uint64_t *entry);

/** \brief 获取ELF节数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 节数量
 *
 */
size_t helf_arch_section_count_get(helf_file_input_t *input_file);

/** \brief 获取ELF节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 节索引
 * \param info helf_arch_section_info_t* 节信息
 * \return bool 是否成功
 *
 */
bool helf_arch_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_section_info_t *info);

/** \brief 判断节是否为读取与执行节
 *
 * \param info const helf_arch_section_info_t* 节信息
 * \return bool 是否为读取与执行节
 *
 */
bool helf_arch_section_is_rx(const helf_arch_section_info_t *info);

/** \brief 判断节是否为只读节
 *
 * \param info const helf_arch_section_info_t* 节信息
 * \return bool 是否为只读节
 *
 */
bool helf_arch_section_is_ro(const helf_arch_section_info_t *info);

/** \brief 判断节是否为读取与写入节
 *
 * \param info const helf_arch_section_info_t* 节信息
 * \return bool 是否为读取与写入节
 *
 */
bool helf_arch_section_is_rw(const helf_arch_section_info_t *info);

/** \brief 判断节是否为BSS节
 *
 * \param info const helf_arch_section_info_t* 节信息
 * \return bool 是否为BSS节
 *
 */
bool helf_arch_section_is_bss(const helf_arch_section_info_t *info);

/** \brief 是否为读取与执行节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行节
 *
 */
bool helf_arch_is_rx_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_arch_rx_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_arch_rx_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为只读节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读节
 *
 */
bool helf_arch_is_ro_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_arch_ro_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_arch_ro_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为读取与写入节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入节
 *
 */
bool helf_arch_is_rw_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_arch_rw_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_arch_rw_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为BSS节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为BSS节
 *
 */
bool helf_arch_is_bss_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取ELF程序头数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序头数量
 *
 */
size_t helf_arch_program_header_count_get(helf_file_input_t *input_file);

/** \brief 获取ELF程序段数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序段数量(程序头数量)
 *
 */
size_t helf_arch_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取ELF程序段信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 程序头索引
 * \param info helf_arch_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_arch_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arch_segment_info_t *info);

/** \brief 是否为可加载程序段(PT_LOAD)
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为可加载程序段
 *
 */
bool helf_arch_is_load_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与执行程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行程序段
 *
 */
bool helf_arch_is_rx_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为只读程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读程序段
 *
 */
bool helf_arch_is_ro_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与写入程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入程序段
 *
 */
bool helf_arch_is_rw_segment(size_t index,helf_file_input_t *input_file);

/** \brief 获取可加载程序段(PT_LOAD)数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 可加载程序段数量
 *
 */
size_t helf_arch_load_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取第index个可加载程序段(PT_LOAD)信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 可加载程序段索引(仅计数PT_LOAD段)
 * \param info helf_arch_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_arch_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arch_segment_info_t *info);

/** \brief 获取所有可加载程序段文件大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 文件大小之和
 *
 */
size_t helf_arch_load_segment_total_filesz_get(helf_file_input_t *input_file);

/** \brief 获取所有可加载程序段内存大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 内存大小之和
 *
 */
size_t helf_arch_load_segment_total_memsz_get(helf_file_input_t *input_file);

/** \brief 获取重定位类型名称
 *
 * \param table const helf_arch_relocation_type_name_t* 名称表
 * \param count size_t 名称表项数量
 * \param type uint32_t 重定位类型
 * \return const char * 重定位类型名称,未知返回NULL
 *
 */
const char *helf_arch_relocation_type_name_get(const helf_arch_relocation_type_name_t *table,size_t count,uint32_t type);

/** \brief 获取架构相关节类型名称
 *
 * \param table const helf_arch_section_type_name_t* 名称表
 * \param count size_t 名称表项数量
 * \param type uint32_t 节类型
 * \return const char * 节类型名称,未知返回NULL
 *
 */
const char *helf_arch_section_type_name_get(const helf_arch_section_type_name_t *table,size_t count,uint32_t type);

/** \brief 获取重定位节数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 重定位节数量(SHT_REL/SHT_RELA)
 *
 */
size_t helf_arch_relocation_section_count_get(helf_file_input_t *input_file);

/** \brief 按重定位节序号获取重定位节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 重定位节序号(仅计数SHT_REL/SHT_RELA节)
 * \param info helf_arch_relocation_section_info_t* 重定位节信息
 * \return bool 是否成功
 *
 */
bool helf_arch_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info);

/** \brief 按节索引获取重定位节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param section_index size_t 节索引
 * \param info helf_arch_relocation_section_info_t* 重定位节信息
 * \return bool 是否成功
 *
 */
bool helf_arch_relocation_section_info_get_by_section(helf_file_input_t *input_file,size_t section_index,helf_arch_relocation_section_info_t *info);

/** \brief 获取重定位节表项数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param section_index size_t 重定位节索引
 * \return size_t 表项数量
 *
 */
size_t helf_arch_relocation_count_get(helf_file_input_t *input_file,size_t section_index);

/** \brief 获取重定位表项
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param section_index size_t 重定位节索引
 * \param index size_t 表项索引
 * \param info helf_arch_relocation_info_t* 重定位表项信息
 * \return bool 是否成功
 *
 */
bool helf_arch_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info);

/** \brief 获取重定位表项对应的符号信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param info helf_arch_symbol_info_t* 符号信息
 * \return bool 是否成功
 *
 */
bool helf_arch_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info);

/** \brief 获取符号表中符号数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 符号数量
 *
 */
size_t helf_arch_symbol_count_get(helf_file_input_t *input_file);

/** \brief 获取符号表中符号信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 符号索引
 * \param info helf_arch_symbol_info_t* 符号信息
 * \return bool 是否成功
 *
 */
bool helf_arch_symbol_info_get(helf_file_input_t *input_file,size_t index,helf_arch_symbol_info_t *info);

/** \brief 获取指定符号表节中符号数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param symbol_table_index size_t 符号表节索引
 * \return size_t 符号数量
 *
 */
size_t helf_arch_symbol_section_count_get(helf_file_input_t *input_file,size_t symbol_table_index);

/** \brief 获取指定符号表节中符号信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param symbol_table_index size_t 符号表节索引
 * \param index size_t 符号索引
 * \param info helf_arch_symbol_info_t* 符号信息
 * \return bool 是否成功
 *
 */
bool helf_arch_symbol_section_info_get(helf_file_input_t *input_file,size_t symbol_table_index,size_t index,helf_arch_symbol_info_t *info);

/** \brief 获取ELF文件类型
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param type uint16_t* 文件类型(HELF_ET_*)
 * \return bool 是否成功
 *
 */
bool helf_arch_type_get(helf_file_input_t *input_file,uint16_t *type);

/** \brief 判断ELF文件是否属于指定架构且为指定类型
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param machines const uint16_t* 机器类型列表(HELF_EM_*)
 * \param machine_count size_t 机器类型数量
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64),0表示不限制
 * \param type uint16_t 文件类型(HELF_ET_*)
 * \return bool 是否属于指定架构且为指定类型
 *
 */
bool helf_arch_is_type(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass,uint16_t type);

/** \brief 判断ELF文件是否属于指定架构且为动态链接库/共享对象
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param machines const uint16_t* 机器类型列表(HELF_EM_*)
 * \param machine_count size_t 机器类型数量
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64),0表示不限制
 * \return bool 是否属于指定架构且为动态链接库/共享对象(ET_DYN)
 *
 */
bool helf_arch_is_shared_object(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass);

/** \brief 判断ELF文件是否属于指定架构且为可重定位对象
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param machines const uint16_t* 机器类型列表(HELF_EM_*)
 * \param machine_count size_t 机器类型数量
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64),0表示不限制
 * \return bool 是否属于指定架构且为可重定位对象(ET_REL)
 *
 */
bool helf_arch_is_relocatable(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass);

/** \brief 获取重定位类型对应的目标字节数
 *
 * \param table const helf_arch_relocation_type_name_t* 名称表
 * \param count size_t 名称表项数量
 * \param type uint32_t 重定位类型
 * \return uint8_t 目标字节数,0表示未知
 *
 */
uint8_t helf_arch_relocation_type_size_get(const helf_arch_relocation_type_name_t *table,size_t count,uint32_t type);

/** \brief 解析重定位地址信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param size size_t 待重定位数据长度
 * \param info helf_arch_relocation_address_info_t* 重定位地址信息
 * \return bool 是否成功
 *
 */
bool helf_arch_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,size_t size,helf_arch_relocation_address_info_t *info);

/** \brief 获取待重定位地址的字节数据
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param size size_t 待重定位数据长度
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \return size_t 实际读取的字节数,0表示失败(返回的字节保持目标文件字节序)
 *
 */
size_t helf_arch_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,size_t size,void *buffer,size_t buffer_length);

/** \brief 获取动态段项数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 动态段项数量
 *
 */
size_t helf_arch_dynamic_count_get(helf_file_input_t *input_file);

/** \brief 获取动态段项信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 动态段项索引
 * \param info helf_arch_dynamic_info_t* 动态段项信息
 * \return bool 是否成功
 *
 */
bool helf_arch_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info);

/** \brief 获取动态字符串表中的字符串
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param offset uint64_t 字符串在动态字符串表中的偏移(通常为DT_NEEDED/DT_SONAME等的d_val)
 * \param buffer char* 用户提供的缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \return bool 是否成功
 *
 */
bool helf_arch_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HELF_ARCH_H__
