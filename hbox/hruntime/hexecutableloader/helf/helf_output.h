/***************************************************************
 * Name:      helf_output.h
 * Purpose:   声明helf_output接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_OUTPUT_H__
#define __HELF_OUTPUT_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "helf_common.h"
#include "helf_section.h"
#include "helf_arch.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HELF_OUTPUT_SECTION_NAME_MAX
#define HELF_OUTPUT_SECTION_NAME_MAX 64
#endif

/*
 * 输出ELF文件头(架构无关部分)
 */
typedef struct helf_output_file_header
{
    uint8_t     osabi;              /**< 操作系统/ABI标识 */
    uint8_t     abiversion;         /**< ABI版本 */
    uint16_t    type;               /**< 文件类型(ET_*) */
    uint16_t    machine;            /**< 机器类型(EM_*) */
    uint32_t    version;            /**< ELF版本 */
    uint64_t    entry;              /**< 入口地址 */
    uint32_t    flags;              /**< 处理器相关标志 */
} helf_output_file_header_t;

/*
 * 输出ELF节描述
 */
typedef struct helf_output_section
{
    char        name[HELF_OUTPUT_SECTION_NAME_MAX]; /**< 节名称 */
    uint32_t    type;               /**< 节类型(SHT_*) */
    uint64_t    flags;              /**< 节标志(SHF_*) */
    uint64_t    address;            /**< 运行时地址 */
    uint64_t    size;               /**< 节大小 */
    uint32_t    link;               /**< 关联节索引 */
    uint32_t    info;               /**< 附加信息 */
    uint64_t    addralign;          /**< 对齐 */
    uint64_t    entsize;            /**< 表项大小 */
    void        *data;              /**< 用户提供的数据缓冲区 */
    uint64_t    data_size;          /**< 缓冲区中有效数据大小 */
    uint64_t    data_capacity;      /**< 缓冲区容量 */
    helf_file_input_t *source;      /**< 可选数据源(数据为空时从该输入读取) */
    uint64_t    source_offset;      /**< 数据源中的文件偏移 */
    uint64_t    file_offset;        /**< 布局后的文件偏移 */
    uint32_t    name_offset;        /**< 布局后的节名偏移 */
    uint64_t    original_offset;    /**< 加载时的原始文件偏移 */
    uint64_t    original_size;      /**< 加载时的原始大小 */
    uint32_t    original_name_offset;/**< 加载时的原始节名偏移 */
    bool        loaded;             /**< 是否由现有ELF加载 */
    bool        modified;           /**< 是否已修改(修改/新增数据追加到文件尾部) */
    bool        segment_copied;     /**< 数据是否已随程序段复制到文件尾部 */
    bool        present;            /**< 是否存在(删除后为false) */
} helf_output_section_t;

/*
 * 输出ELF程序段描述
 */
typedef struct helf_output_segment
{
    uint32_t    type;               /**< 段类型(PT_*) */
    uint32_t    flags;              /**< 段标志(PF_*) */
    uint64_t    offset;             /**< 文件偏移 */
    uint64_t    vaddr;              /**< 虚拟地址 */
    uint64_t    paddr;              /**< 物理地址 */
    uint64_t    filesz;             /**< 文件中的大小 */
    uint64_t    memsz;              /**< 内存中的大小 */
    uint64_t    addralign;          /**< 对齐 */
    uint32_t    section_index;      /**< 关联节索引(用于自动布局) */
    uint64_t    section_offset;     /**< 节内偏移(用于自动布局) */
    uint64_t    original_offset;    /**< 加载时的原始文件偏移 */
    uint64_t    original_filesz;    /**< 加载时的原始文件大小 */
    bool        auto_layout;        /**< 是否根据关联节自动更新布局字段 */
    bool        copy_appended;      /**< 是否已将程序段数据复制到文件尾部 */
    bool        loaded;             /**< 是否由现有ELF加载 */
    bool        modified;           /**< 是否已修改 */
    bool        present;            /**< 是否存在(删除后为false) */
} helf_output_segment_t;

typedef struct helf_output_editor helf_output_editor_t;
struct helf_output_editor
{
    helf_file_output_t          *output;                /**< 输出文件 */
    uint8_t                     elfclass;               /**< ELF类别(HELF_ELFCLASS_32/64) */
    uint8_t                     elfdata;                /**< ELF数据格式(HELF_ELFDATA_2_LSB/MSB) */
    helf_output_file_header_t   header;                 /**< 文件头 */
    helf_file_input_t           *source;                /**< 加载时的源输入文件 */
    helf_output_section_t       *sections;              /**< 节描述数组(用户提供) */
    size_t                      section_capacity;       /**< 节描述数组容量 */
    size_t                      section_count;          /**< 节描述数量 */
    helf_output_segment_t       *segments;              /**< 程序段描述数组(用户提供) */
    size_t                      segment_capacity;       /**< 程序段描述数组容量 */
    size_t                      segment_count;          /**< 程序段描述数量 */
    uint64_t                    program_header_offset;  /**< 布局后的程序头偏移 */
    uint64_t                    section_header_offset;  /**< 布局后的节头偏移 */
    uint16_t                    program_header_count;   /**< 布局后的程序头数量 */
    uint16_t                    section_header_count;   /**< 布局后的节头数量 */
    uint16_t                    section_name_table_index;/**< 布局后的节名表索引 */
    uint64_t                    file_size;              /**< 布局后的文件大小 */
    bool                        laid_out;               /**< 是否已布局 */
    bool                        preserve_layout;        /**< 是否保留已加载节的原始偏移 */
    uint64_t                    loaded_program_header_offset; /**< 加载时的程序头偏移 */
    uint16_t                    loaded_program_header_count;  /**< 加载时的程序头数量 */
    uint64_t                    loaded_section_header_offset; /**< 加载时的节头偏移 */
    uint16_t                    loaded_section_header_count;  /**< 加载时的节头数量 */
    uint16_t                    loaded_program_header_entry_size; /**< 加载时的程序头大小 */
    uint16_t                    loaded_section_header_entry_size;  /**< 加载时的节头大小 */
    uint16_t                    loaded_section_name_table_index;   /**< 加载时的节名表索引 */
    uint64_t                    source_file_size;       /**< 加载时的源文件大小上界 */
    uint64_t                    page_size;              /**< 程序段页对齐大小,默认0x1000 */
    bool                        strict_preserve;        /**< 无修改时按原布局逐字节还原 */
};

/** \brief 写入32位ELF文件头
 *
 * \param output helf_file_output_t* 输出文件
 * \param header const helf_elf32_file_header_t* 文件头
 * \return bool 是否成功
 *
 */
bool helf_file_output_32_bits_header_set(helf_file_output_t *output,const helf_elf32_file_header_t *header);

/** \brief 写入64位ELF文件头
 *
 * \param output helf_file_output_t* 输出文件
 * \param header const helf_elf64_file_header_t* 文件头
 * \return bool 是否成功
 *
 */
bool helf_file_output_64_bits_header_set(helf_file_output_t *output,const helf_elf64_file_header_t *header);

/** \brief 写入32位ELF节头
 *
 * \param output helf_file_output_t* 输出文件
 * \param index size_t 节索引
 * \param sectionheader const helf_elf32_section_header_t* 节头
 * \return bool 是否成功
 *
 */
bool helf_file_output_32_bits_section_header_set(helf_file_output_t *output,size_t index,const helf_elf32_section_header_t *sectionheader);

/** \brief 写入64位ELF节头
 *
 * \param output helf_file_output_t* 输出文件
 * \param index size_t 节索引
 * \param sectionheader const helf_elf64_section_header_t* 节头
 * \return bool 是否成功
 *
 */
bool helf_file_output_64_bits_section_header_set(helf_file_output_t *output,size_t index,const helf_elf64_section_header_t *sectionheader);

/** \brief 写入32位ELF程序头
 *
 * \param output helf_file_output_t* 输出文件
 * \param index size_t 程序头索引
 * \param programheader const helf_elf32_program_header_t* 程序头
 * \return bool 是否成功
 *
 */
bool helf_file_output_32_bits_program_header_set(helf_file_output_t *output,size_t index,const helf_elf32_program_header_t *programheader);

/** \brief 写入64位ELF程序头
 *
 * \param output helf_file_output_t* 输出文件
 * \param index size_t 程序头索引
 * \param programheader const helf_elf64_program_header_t* 程序头
 * \return bool 是否成功
 *
 */
bool helf_file_output_64_bits_program_header_set(helf_file_output_t *output,size_t index,const helf_elf64_program_header_t *programheader);

/** \brief 输出编辑器初始化
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param output helf_file_output_t* 输出文件
 * \param elfclass uint8_t ELF类别(HELF_ELFCLASS_32/HELF_ELFCLASS_64)
 * \param elfdata uint8_t ELF数据格式(HELF_ELFDATA_2_LSB/HELF_ELFDATA_2_MSB)
 * \param sections helf_output_section_t* 节描述数组(用户提供)
 * \param section_capacity size_t 节描述数组容量
 * \param segments helf_output_segment_t* 程序段描述数组(用户提供)
 * \param segment_capacity size_t 程序段描述数组容量
 *
 */
void helf_output_editor_init(helf_output_editor_t *editor,helf_file_output_t *output,uint8_t elfclass,uint8_t elfdata,helf_output_section_t *sections,size_t section_capacity,helf_output_segment_t *segments,size_t segment_capacity);

/** \brief 从输入ELF加载编辑器
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param input helf_file_input_t* 输入文件
 * \return bool 是否成功
 *
 * \note 加载后未修改的节数据将通过source/source_offset从输入文件读取。
 *
 */
bool helf_output_editor_load(helf_output_editor_t *editor,helf_file_input_t *input);

/** \brief 设置加载源文件的实际大小
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param size uint64_t 源文件大小
 * \return bool 是否成功
 *
 * \note 用于源文件末尾存在额外数据时进行逐字节还原。
 *
 */
bool helf_output_editor_source_file_size_set(helf_output_editor_t *editor,uint64_t size);

/** \brief 获取加载源文件的大小
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return uint64_t 源文件大小
 *
 */
uint64_t helf_output_editor_source_file_size_get(helf_output_editor_t *editor);

/** \brief 设置是否保留已加载节的原始布局
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param preserve bool 是否保留原始布局
 * \return bool 是否成功
 *
 */
bool helf_output_editor_preserve_layout_set(helf_output_editor_t *editor,bool preserve);

/** \brief 设置程序段页对齐大小
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param page_size uint64_t 页对齐大小,1表示不进行页对齐
 * \return bool 是否成功
 *
 */
bool helf_output_editor_page_size_set(helf_output_editor_t *editor,uint64_t page_size);

/** \brief 设置输出ELF文件头
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param header const helf_output_file_header_t* 文件头
 * \return bool 是否成功
 *
 */
bool helf_output_editor_file_header_set(helf_output_editor_t *editor,const helf_output_file_header_t *header);

/** \brief 获取输出ELF文件头
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param header helf_output_file_header_t* 文件头
 * \return bool 是否成功
 *
 */
bool helf_output_editor_file_header_get(helf_output_editor_t *editor,helf_output_file_header_t *header);

/** \brief 获取节数量
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return size_t 节数量(包含已删除的占位节)
 *
 */
size_t helf_output_editor_section_count_get(helf_output_editor_t *editor);

/** \brief 添加节
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param section const helf_output_section_t* 节描述
 * \return intptr_t 节索引,失败返回-1
 *
 */
intptr_t helf_output_editor_section_add(helf_output_editor_t *editor,const helf_output_section_t *section);

/** \brief 设置节
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 节索引
 * \param section const helf_output_section_t* 节描述
 * \return bool 是否成功
 *
 */
bool helf_output_editor_section_set(helf_output_editor_t *editor,size_t index,const helf_output_section_t *section);

/** \brief 获取节
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 节索引
 * \param section helf_output_section_t* 节描述
 * \return bool 是否成功
 *
 */
bool helf_output_editor_section_get(helf_output_editor_t *editor,size_t index,helf_output_section_t *section);

/** \brief 删除节(标记为SHT_NULL,保留索引)
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 节索引
 * \return bool 是否成功
 *
 */
bool helf_output_editor_section_delete(helf_output_editor_t *editor,size_t index);

/** \brief 按名称查找节
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param name const char* 节名称
 * \return intptr_t 节索引,失败返回-1
 *
 */
intptr_t helf_output_editor_section_find(helf_output_editor_t *editor,const char *name);

/** \brief 设置节数据
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 节索引
 * \param data void* 用户提供的数据缓冲区
 * \param size uint64_t 有效数据大小
 * \param capacity uint64_t 缓冲区容量
 * \return bool 是否成功
 *
 */
bool helf_output_editor_section_data_set(helf_output_editor_t *editor,size_t index,void *data,uint64_t size,uint64_t capacity);

/** \brief 获取节数据
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 节索引
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \return size_t 实际读取的字节数
 *
 */
size_t helf_output_editor_section_data_get(helf_output_editor_t *editor,size_t index,void *buffer,size_t buffer_length);

/** \brief 获取程序段数量
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return size_t 程序段数量(包含已删除的占位程序段)
 *
 */
size_t helf_output_editor_segment_count_get(helf_output_editor_t *editor);

/** \brief 添加程序段
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param segment const helf_output_segment_t* 程序段描述
 * \return intptr_t 程序段索引,失败返回-1
 *
 */
intptr_t helf_output_editor_segment_add(helf_output_editor_t *editor,const helf_output_segment_t *segment);

/** \brief 设置程序段
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 程序段索引
 * \param segment const helf_output_segment_t* 程序段描述
 * \return bool 是否成功
 *
 */
bool helf_output_editor_segment_set(helf_output_editor_t *editor,size_t index,const helf_output_segment_t *segment);

/** \brief 获取程序段
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 程序段索引
 * \param segment helf_output_segment_t* 程序段描述
 * \return bool 是否成功
 *
 */
bool helf_output_editor_segment_get(helf_output_editor_t *editor,size_t index,helf_output_segment_t *segment);

/** \brief 删除程序段
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 程序段索引
 * \return bool 是否成功
 *
 */
bool helf_output_editor_segment_delete(helf_output_editor_t *editor,size_t index);

/** \brief 设置程序段根据节自动布局
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param index size_t 程序段索引
 * \param section_index size_t 关联节索引
 * \param section_offset uint64_t 节内偏移
 * \return bool 是否成功
 *
 */
bool helf_output_editor_segment_from_section(helf_output_editor_t *editor,size_t index,size_t section_index,uint64_t section_offset);

/** \brief 向字符串表添加字符串
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param section_index size_t 字符串表节索引
 * \param string const char* 字符串
 * \param offset uint32_t* 字符串在表中的偏移
 * \return bool 是否成功
 *
 */
bool helf_output_editor_string_add(helf_output_editor_t *editor,size_t section_index,const char *string,uint32_t *offset);

/** \brief 向符号表添加符号
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param section_index size_t 符号表节索引
 * \param symbol const helf_arch_symbol_info_t* 符号信息
 * \param symbol_index uint32_t* 符号索引
 * \return bool 是否成功
 *
 */
bool helf_output_editor_symbol_add(helf_output_editor_t *editor,size_t section_index,const helf_arch_symbol_info_t *symbol,uint32_t *symbol_index);

/** \brief 向重定位节添加重定位表项
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param section_index size_t 重定位节索引
 * \param relocation const helf_arch_relocation_info_t* 重定位表项
 * \return bool 是否成功
 *
 */
bool helf_output_editor_relocation_add(helf_output_editor_t *editor,size_t section_index,const helf_arch_relocation_info_t *relocation);

/** \brief 向动态段节添加动态项
 *
 * \param editor helf_output_editor_t* 编辑器
 * \param section_index size_t 动态段节索引
 * \param tag int64_t 动态项类型
 * \param value uint64_t 动态项值
 * \return bool 是否成功
 *
 */
bool helf_output_editor_dynamic_add(helf_output_editor_t *editor,size_t section_index,int64_t tag,uint64_t value);

/** \brief 布局输出ELF
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return bool 是否成功
 *
 */
bool helf_output_editor_layout(helf_output_editor_t *editor);

/** \brief 写入输出ELF
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return bool 是否成功
 *
 */
bool helf_output_editor_write(helf_output_editor_t *editor);

/** \brief 获取写入的ELF文件大小
 *
 * \param editor helf_output_editor_t* 编辑器
 * \return uint64_t 写入的ELF文件大小,未布局时返回0
 *
 */
uint64_t helf_output_editor_file_size_get(helf_output_editor_t *editor);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HELF_OUTPUT_H__
