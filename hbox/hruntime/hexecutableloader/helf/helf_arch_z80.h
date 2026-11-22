/***************************************************************
 * Name:      helf_arch_z80.h
 * Purpose:   声明helf_arch_z80接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_ARCH_Z80_H__
#define __HELF_ARCH_Z80_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "helf_common.h"
#include "helf_arch.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef helf_arch_section_info_t helf_z80_section_info_t;
typedef helf_arch_segment_info_t helf_z80_segment_info_t;

/** \brief 是否为Z80 ELF文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Z80 ELF文件(ELF32)
 *
 */
bool helf_z80_is_z80(helf_file_input_t *input_file);

/** \brief 是否为Z80大端ELF文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Z80大端ELF文件
 *
 */
bool helf_z80_is_big_endian(helf_file_input_t *input_file);

/** \brief 是否为Z80可执行文件
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Z80可执行文件
 *
 */
bool helf_z80_is_executable(helf_file_input_t *input_file);

/** \brief 获取Z80 ELF入口地址
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param entry uint32_t* 入口地址
 * \return bool 是否成功
 *
 */
bool helf_z80_entry_get(helf_file_input_t *input_file,uint32_t *entry);

/** \brief 获取Z80 ELF节数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 节数量
 *
 */
size_t helf_z80_section_count_get(helf_file_input_t *input_file);

/** \brief 获取Z80 ELF节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 节索引
 * \param info helf_z80_section_info_t* 节信息
 * \return bool 是否成功
 *
 */
bool helf_z80_section_info_get(helf_file_input_t *input_file,size_t index,helf_z80_section_info_t *info);

/** \brief 是否为读取与执行节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行节
 *
 */
bool helf_z80_is_rx_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_z80_rx_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与执行节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_z80_rx_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为只读节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读节
 *
 */
bool helf_z80_is_ro_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_z80_ro_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取只读节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_z80_ro_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为读取与写入节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入节
 *
 */
bool helf_z80_is_rw_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节的文件偏移
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return intptr_t 文件偏移,-1=失败
 *
 */
intptr_t helf_z80_rw_section_offset_get(size_t index,helf_file_input_t *input_file);

/** \brief 获取读取与写入节总大小
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 总大小
 *
 */
size_t helf_z80_rw_section_size_get(helf_file_input_t *input_file);

/** \brief 是否为BSS节
 *
 * \param index size_t 节索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为BSS节
 *
 */
bool helf_z80_is_bss_section(size_t index,helf_file_input_t *input_file);

/** \brief 获取Z80 ELF程序头数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序头数量
 *
 */
size_t helf_z80_program_header_count_get(helf_file_input_t *input_file);

/** \brief 获取Z80 ELF程序头
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 程序头索引
 * \param programheader helf_elf32_program_header_t* 程序头
 * \return bool 是否成功
 *
 */
bool helf_z80_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader);

/** \brief 获取Z80 ELF程序段数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 程序段数量(程序头数量)
 *
 */
size_t helf_z80_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取Z80 ELF程序段信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 程序头索引
 * \param info helf_z80_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_z80_segment_info_get(helf_file_input_t *input_file,size_t index,helf_z80_segment_info_t *info);

/** \brief 是否为可加载程序段(PT_LOAD)
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为可加载程序段
 *
 */
bool helf_z80_is_load_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与执行程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与执行程序段
 *
 */
bool helf_z80_is_rx_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为只读程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为只读程序段
 *
 */
bool helf_z80_is_ro_segment(size_t index,helf_file_input_t *input_file);

/** \brief 是否为读取与写入程序段
 *
 * \param index size_t 程序头索引
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为读取与写入程序段
 *
 */
bool helf_z80_is_rw_segment(size_t index,helf_file_input_t *input_file);

/** \brief 获取可加载程序段(PT_LOAD)数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 可加载程序段数量
 *
 */
size_t helf_z80_load_segment_count_get(helf_file_input_t *input_file);

/** \brief 获取第index个可加载程序段(PT_LOAD)信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 可加载程序段索引(仅计数PT_LOAD段)
 * \param info helf_z80_segment_info_t* 程序段信息
 * \return bool 是否成功
 *
 */
bool helf_z80_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_z80_segment_info_t *info);

/** \brief 获取所有可加载程序段文件大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 文件大小之和
 *
 */
size_t helf_z80_load_segment_total_filesz_get(helf_file_input_t *input_file);

/** \brief 获取所有可加载程序段内存大小之和
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 内存大小之和
 *
 */
size_t helf_z80_load_segment_total_memsz_get(helf_file_input_t *input_file);

/** \brief 获取Z80重定位节数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 重定位节数量(SHT_REL/SHT_RELA)
 *
 */
size_t helf_z80_relocation_section_count_get(helf_file_input_t *input_file);

/** \brief 按重定位节序号获取Z80重定位节信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 重定位节序号(仅计数SHT_REL/SHT_RELA节)
 * \param info helf_arch_relocation_section_info_t* 重定位节信息
 * \return bool 是否成功
 *
 */
bool helf_z80_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info);

/** \brief 获取Z80重定位节表项数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param section_index size_t 重定位节索引
 * \return size_t 表项数量
 *
 */
size_t helf_z80_relocation_count_get(helf_file_input_t *input_file,size_t section_index);

/** \brief 获取Z80重定位表项
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param section_index size_t 重定位节索引
 * \param index size_t 表项索引
 * \param info helf_arch_relocation_info_t* 重定位表项信息
 * \return bool 是否成功
 *
 */
bool helf_z80_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info);

/** \brief 获取Z80重定位表项对应的符号信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param info helf_arch_symbol_info_t* 符号信息
 * \return bool 是否成功
 *
 */
bool helf_z80_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info);

/** \brief 获取Z80重定位类型名称
 *
 * \param type uint32_t 重定位类型
 * \return const char * 重定位类型名称,未知返回NULL
 *
 */
const char *helf_z80_relocation_type_name_get(uint32_t type);

/** \brief 获取Z80架构相关节类型名称
 *
 * \param type uint32_t 节类型
 * \return const char * 节类型名称,未知返回NULL
 *
 */
const char *helf_z80_section_type_name_get(uint32_t type);

/** \brief 获取Z80重定位类型对应的目标字节数
 *
 * \param type uint32_t 重定位类型
 * \return uint8_t 目标字节数,0表示未知
 *
 */
uint8_t helf_z80_relocation_size_get(uint32_t type);

/** \brief 解析Z80重定位地址信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param info helf_arch_relocation_address_info_t* 重定位地址信息
 * \return bool 是否成功
 *
 */
bool helf_z80_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info);

/** \brief 获取Z80待重定位地址的字节数据
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param relocation const helf_arch_relocation_info_t* 重定位表项信息
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \return size_t 实际读取的字节数,0表示失败(返回的字节保持目标文件字节序)
 *
 */
size_t helf_z80_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length);

/** \brief 是否为Z80动态链接库/共享对象
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Z80动态链接库/共享对象(ET_DYN)
 *
 */
bool helf_z80_is_shared_object(helf_file_input_t *input_file);

/** \brief 是否为Z80可重定位对象
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为Z80可重定位对象(ET_REL)
 *
 */
bool helf_z80_is_relocatable(helf_file_input_t *input_file);

/** \brief 获取Z80动态段项数量
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 动态段项数量
 *
 */
size_t helf_z80_dynamic_count_get(helf_file_input_t *input_file);

/** \brief 获取Z80动态段项信息
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param index size_t 动态段项索引
 * \param info helf_arch_dynamic_info_t* 动态段项信息
 * \return bool 是否成功
 *
 */
bool helf_z80_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info);

/** \brief 获取Z80动态字符串表中的字符串
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param offset uint64_t 字符串在动态字符串表中的偏移
 * \param buffer char* 用户提供的缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \return bool 是否成功
 *
 */
bool helf_z80_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length);

bool helf_z80_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HELF_ARCH_Z80_H__
