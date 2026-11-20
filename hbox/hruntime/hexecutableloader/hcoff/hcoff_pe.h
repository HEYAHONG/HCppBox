/***************************************************************
 * Name:      hcoff_pe.h
 * Purpose:   声明hcoff_pe接口(Portable Executable,PE/PE32+)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_PE_H__
#define __HCOFF_PE_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * PE文件既可以表示可执行文件(EXE),也可以表示动态链接库(DLL)。
 *
 * 与一般的COFF对象文件不同,PE文件以DOS头(MZ)开始,真正的COFF文件头位于
 * DOS头e_lfanew字段指向的NT头中。其结构如下:
 *
 *   DOS头(64字节) + DOS存根 + "PE\0\0" + COFF文件头 + 可选头 + 节表 + 节数据
 *
 * 本组件仅实现PE文件的读取与解析(包括导出表、导入表、基址重定位、TLS等),
 * 不依赖操作系统的加载器。
 */

#define HCOFF_PE_DOS_SIGNATURE                      0x5A4D          /**< "MZ" */
#define HCOFF_PE_NT_SIGNATURE                       0x00004550      /**< "PE\0\0" */
#define HCOFF_PE_OPTIONAL_HEADER_MAGIC_PE32         0x010B          /**< PE32 */
#define HCOFF_PE_OPTIONAL_HEADER_MAGIC_PE32PLUS     0x020B          /**< PE32+ */

#define HCOFF_PE_NUMBEROF_DIRECTORY_ENTRIES         16

/*
 * 数据目录索引
 */
#define HCOFF_PE_DIRECTORY_ENTRY_EXPORT             0
#define HCOFF_PE_DIRECTORY_ENTRY_IMPORT             1
#define HCOFF_PE_DIRECTORY_ENTRY_RESOURCE           2
#define HCOFF_PE_DIRECTORY_ENTRY_EXCEPTION          3
#define HCOFF_PE_DIRECTORY_ENTRY_SECURITY           4
#define HCOFF_PE_DIRECTORY_ENTRY_BASERELOC          5
#define HCOFF_PE_DIRECTORY_ENTRY_DEBUG              6
#define HCOFF_PE_DIRECTORY_ENTRY_ARCHITECTURE       7
#define HCOFF_PE_DIRECTORY_ENTRY_GLOBALPTR          8
#define HCOFF_PE_DIRECTORY_ENTRY_TLS                9
#define HCOFF_PE_DIRECTORY_ENTRY_LOAD_CONFIG        10
#define HCOFF_PE_DIRECTORY_ENTRY_BOUND_IMPORT       11
#define HCOFF_PE_DIRECTORY_ENTRY_IAT                12
#define HCOFF_PE_DIRECTORY_ENTRY_DELAY_IMPORT       13
#define HCOFF_PE_DIRECTORY_ENTRY_COM_DESCRIPTOR     14
#define HCOFF_PE_DIRECTORY_ENTRY_RESERVED           15

/*
 * 子系统
 */
#define HCOFF_PE_SUBSYSTEM_UNKNOWN                  0
#define HCOFF_PE_SUBSYSTEM_NATIVE                   1
#define HCOFF_PE_SUBSYSTEM_WINDOWS_GUI              2
#define HCOFF_PE_SUBSYSTEM_WINDOWS_CUI              3
#define HCOFF_PE_SUBSYSTEM_POSIX_CUI                7
#define HCOFF_PE_SUBSYSTEM_WINDOWS_CE_GUI           9
#define HCOFF_PE_SUBSYSTEM_EFI_APPLICATION          10
#define HCOFF_PE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11
#define HCOFF_PE_SUBSYSTEM_EFI_RUNTIME_DRIVER       12
#define HCOFF_PE_SUBSYSTEM_SAL_RUNTIME_DRIVER       13
#define HCOFF_PE_SUBSYSTEM_XBOX                     14

/*
 * DLL特性
 */
#define HCOFF_PE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA     0x0020
#define HCOFF_PE_DLLCHARACTERISTICS_DYNAMIC_BASE        0x0040
#define HCOFF_PE_DLLCHARACTERISTICS_FORCE_INTEGRITY     0x0080
#define HCOFF_PE_DLLCHARACTERISTICS_NX_COMPAT           0x0100
#define HCOFF_PE_DLLCHARACTERISTICS_NO_ISOLATION        0x0200
#define HCOFF_PE_DLLCHARACTERISTICS_NO_SEH              0x0400
#define HCOFF_PE_DLLCHARACTERISTICS_NO_BIND             0x0800
#define HCOFF_PE_DLLCHARACTERISTICS_APPCONTAINER        0x1000
#define HCOFF_PE_DLLCHARACTERISTICS_WDM_DRIVER          0x2000
#define HCOFF_PE_DLLCHARACTERISTICS_GUARD_CF            0x4000
#define HCOFF_PE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE 0x8000

/*
 * 基址重定位类型
 */
#define HCOFF_PE_REL_BASED_ABSOLUTE                 0
#define HCOFF_PE_REL_BASED_HIGH                     1
#define HCOFF_PE_REL_BASED_LOW                      2
#define HCOFF_PE_REL_BASED_HIGHLOW                  3
#define HCOFF_PE_REL_BASED_HIGHADJ                  4
#define HCOFF_PE_REL_BASED_MIPS_JMPADDR             5
#define HCOFF_PE_REL_BASED_ARM_MOV32                5
#define HCOFF_PE_REL_BASED_RISCV_HIGH20             5
#define HCOFF_PE_REL_BASED_THUMB_MOV32              7
#define HCOFF_PE_REL_BASED_RISCV_LOW12I             7
#define HCOFF_PE_REL_BASED_RISCV_LOW12S             8
#define HCOFF_PE_REL_BASED_LOONGARCH32_MARK_LA      8
#define HCOFF_PE_REL_BASED_LOONGARCH64_MARK_LA      8
#define HCOFF_PE_REL_BASED_MIPS_JMPADDR16           9
#define HCOFF_PE_REL_BASED_DIR64                    10

typedef struct hcoff_pe_data_directory hcoff_pe_data_directory_t;
struct hcoff_pe_data_directory
{
    uint32_t virtual_address;       /**< RVA */
    uint32_t size;                  /**< 大小 */
};

typedef struct hcoff_pe_header hcoff_pe_header_t;
struct hcoff_pe_header
{
    uint32_t dos_header_offset;                             /**< DOS头偏移,通常为0 */
    uint32_t nt_headers_offset;                             /**< NT头偏移(e_lfanew) */
    uint32_t optional_header_offset;                        /**< 可选头偏移 */
    uint32_t optional_header_size;                          /**< 可选头大小 */
    uint32_t section_table_offset;                          /**< 节表偏移 */
    hcoff_fileheader_t coff_header;                         /**< COFF文件头 */
    uint16_t optional_magic;                                /**< 可选头幻数(PE32/PE32+) */
    bool     is_64bit;                                      /**< 是否是PE32+ */
    uint8_t  major_linker_version;
    uint8_t  minor_linker_version;
    uint32_t size_of_code;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_of_entry_point;                        /**< 入口点RVA */
    uint32_t base_of_code;
    uint32_t base_of_data;                                  /**< PE32有效 */
    uint64_t image_base;                                    /**< 首选加载基址 */
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_operating_system_version;
    uint16_t minor_operating_system_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;
    uint32_t size_of_image;
    uint32_t size_of_headers;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint64_t size_of_stack_reserve;
    uint64_t size_of_stack_commit;
    uint64_t size_of_heap_reserve;
    uint64_t size_of_heap_commit;
    uint32_t loader_flags;
    uint32_t number_of_rva_and_sizes;
    hcoff_pe_data_directory_t data_directory[HCOFF_PE_NUMBEROF_DIRECTORY_ENTRIES];
};

/** \brief 判断文件是否为PE文件
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否为PE文件
 *
 */
bool hcoff_pe_is_pe(hcoff_file_input_t *input_file);

/** \brief 读取PE文件头
 *
 * \param header hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否成功
 *
 */
bool hcoff_pe_header_read(hcoff_pe_header_t *header,hcoff_file_input_t *input_file);

/** \brief 判断PE文件是否为DLL
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \return bool 是否为DLL
 *
 */
bool hcoff_pe_header_is_dll(const hcoff_pe_header_t *header);

/** \brief 判断PE文件是否为可执行文件
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \return bool 是否为可执行文件
 *
 */
bool hcoff_pe_header_is_executable(const hcoff_pe_header_t *header);

/** \brief 读取PE节头
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param sectionheader hcoff_sectionheader_t* 节头
 * \param index size_t 引索,从0开始
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否成功
 *
 */
bool hcoff_pe_sectionheader_read(const hcoff_pe_header_t *header,hcoff_sectionheader_t *sectionheader,size_t index,hcoff_file_input_t *input_file);

/** \brief PE RVA转换为文件偏移
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param rva uint32_t RVA
 * \param file_offset uint32_t* 文件偏移
 * \param available_size uint32_t* 从文件偏移开始至节末尾的可用大小(可为NULL)
 * \return bool 是否成功
 *
 */
bool hcoff_pe_rva_to_file_offset(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,uint32_t *file_offset,uint32_t *available_size);

/** \brief 在PE RVA处读取数据
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param rva uint32_t RVA
 * \param buffer void* 缓冲区
 * \param buffer_length size_t 缓冲区大小
 * \return size_t 已读取大小
 *
 */
size_t hcoff_pe_read_at_rva(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,void *buffer,size_t buffer_length);

/** \brief 在PE RVA处读取字符串(以'\0'结尾)
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param rva uint32_t RVA
 * \param buffer char* 缓冲区
 * \param buffer_length size_t 缓冲区大小
 * \return const char* 字符串指针,失败返回NULL
 *
 */
const char *hcoff_pe_read_string_at_rva(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint32_t rva,char *buffer,size_t buffer_length);

typedef struct hcoff_pe_export hcoff_pe_export_t;
struct hcoff_pe_export
{
    const char *name;               /**< 导出名称(指向调用者缓冲区),可能为NULL */
    const char *forwarder;          /**< 转发名称(指向调用者缓冲区),非转发为NULL */
    uint32_t ordinal;               /**< 导出序号(真实序号,包含Ordinal Base) */
    uint32_t function_index;        /**< 函数在导出地址表中的索引 */
    uint32_t function_rva;          /**< 函数RVA;转发时为转发字符串的RVA */
    uint32_t name_rva;              /**< 名称字符串RVA */
    uint32_t forwarder_rva;         /**< 转发字符串RVA,非转发时为0 */
    bool     is_forwarder;          /**< 是否为转发导出 */
};

/** \brief 获取PE导出函数数量(地址表数量)
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 导出函数数量
 *
 */
size_t hcoff_pe_export_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file);

/** \brief 获取PE导出名称数量
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 导出名称数量
 *
 */
size_t hcoff_pe_export_name_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file);

/** \brief 按名称表索引读取PE导出
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param index size_t 名称表索引,从0开始
 * \param export_info hcoff_pe_export_t* 导出信息
 * \param name_buffer char* 名称缓冲区
 * \param name_buffer_length size_t 名称缓冲区大小
 * \param forwarder_buffer char* 转发名称缓冲区
 * \param forwarder_buffer_length size_t 转发名称缓冲区大小
 * \return bool 是否成功
 *
 */
bool hcoff_pe_export_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t index,hcoff_pe_export_t *export_info,char *name_buffer,size_t name_buffer_length,char *forwarder_buffer,size_t forwarder_buffer_length);

/** \brief 按名称查找PE导出
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param name const char* 名称
 * \param export_info hcoff_pe_export_t* 导出信息
 * \param name_buffer char* 名称缓冲区(用于存储export_info->name)
 * \param name_buffer_length size_t 名称缓冲区大小
 * \param forwarder_buffer char* 转发名称缓冲区(用于存储export_info->forwarder)
 * \param forwarder_buffer_length size_t 转发名称缓冲区大小
 * \return bool 是否找到
 *
 */
bool hcoff_pe_export_find(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const char *name,hcoff_pe_export_t *export_info,char *name_buffer,size_t name_buffer_length,char *forwarder_buffer,size_t forwarder_buffer_length);

/** \brief 获取PE导出名称(转发器)
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param export_info const hcoff_pe_export_t* 导出信息(通常由hcoff_pe_export_name_get获取)
 * \param buffer char* 名称缓冲区
 * \param buffer_length size_t 缓冲区大小
 * \return const char* 名称,失败返回NULL
 *
 */
const char *hcoff_pe_export_forwarder_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const hcoff_pe_export_t *export_info,char *buffer,size_t buffer_length);

/** \brief 获取PE导出名称
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param export_info const hcoff_pe_export_t* 导出信息
 * \param buffer char* 名称缓冲区
 * \param buffer_length size_t 缓冲区大小
 * \return const char* 名称,失败返回NULL
 *
 */
const char *hcoff_pe_export_name_ptr_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const hcoff_pe_export_t *export_info,char *buffer,size_t buffer_length);

typedef struct hcoff_pe_import hcoff_pe_import_t;
struct hcoff_pe_import
{
    const char *dll_name;           /**< DLL名称(指向调用者缓冲区) */
    const char *function_name;      /**< 函数名称(指向调用者缓冲区),按序号导入时为NULL */
    uint16_t ordinal;               /**< 按序号导入时的序号 */
    uint16_t hint;                  /**< 提示值 */
    bool     by_ordinal;            /**< 是否按序号导入 */
    uint32_t dll_index;             /**< 导入描述符索引 */
    uint32_t thunk_index;           /**< 描述符内thunk索引 */
    uint32_t iat_rva;               /**< IAT中的RVA */
    uint32_t lookup_rva;            /**< 原始thunk中的RVA */
};

/** \brief 获取PE导入DLL数量
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 导入DLL数量
 *
 */
size_t hcoff_pe_import_dll_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file);

/** \brief 判断PE可执行文件是否为静态链接
 *
 * 判断依据:不存在实际导入的DLL且不存在延迟导入目录。
 * 仅检查动态依赖,不判断是否静态链接了C运行库。
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否为静态链接
 *
 */
bool hcoff_pe_is_static_linked(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file);

/** \brief 获取PE导入DLL名称
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param dll_index size_t DLL索引
 * \param buffer char* 名称缓冲区
 * \param buffer_length size_t 缓冲区大小
 * \return const char* DLL名称,失败返回NULL
 *
 */
const char *hcoff_pe_import_dll_name_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index,char *buffer,size_t buffer_length);

/** \brief 获取PE导入函数数量
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param dll_index size_t DLL索引
 * \return size_t 导入函数数量
 *
 */
size_t hcoff_pe_import_count(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index);

/** \brief 获取PE导入函数
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param dll_index size_t DLL索引
 * \param thunk_index size_t thunk索引
 * \param import_info hcoff_pe_import_t* 导入信息
 * \param dll_name_buffer char* DLL名称缓冲区
 * \param dll_name_buffer_length size_t DLL名称缓冲区大小
 * \param function_name_buffer char* 函数名称缓冲区
 * \param function_name_buffer_length size_t 函数名称缓冲区大小
 * \return bool 是否成功
 *
 */
bool hcoff_pe_import_get(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,size_t dll_index,size_t thunk_index,hcoff_pe_import_t *import_info,char *dll_name_buffer,size_t dll_name_buffer_length,char *function_name_buffer,size_t function_name_buffer_length);

/** \brief 按名称查找PE导入(在全部DLL中查找)
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param name const char* 函数名称
 * \param import_info hcoff_pe_import_t* 导入信息
 * \param dll_name_buffer char* DLL名称缓冲区(用于存储import_info->dll_name)
 * \param dll_name_buffer_length size_t DLL名称缓冲区大小
 * \param function_name_buffer char* 函数名称缓冲区(用于存储import_info->function_name)
 * \param function_name_buffer_length size_t 函数名称缓冲区大小
 * \return bool 是否找到
 *
 */
bool hcoff_pe_import_find(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,const char *name,hcoff_pe_import_t *import_info,char *dll_name_buffer,size_t dll_name_buffer_length,char *function_name_buffer,size_t function_name_buffer_length);

typedef bool (*hcoff_pe_base_relocation_callback_t)(uint32_t rva,uint16_t type,uint16_t data,void *usr);

/** \brief 遍历PE基址重定位
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param callback hcoff_pe_base_relocation_callback_t 回调
 * \param usr void* 用户参数
 * \return size_t 处理的条目数量
 *
 */
size_t hcoff_pe_base_relocation_foreach(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,hcoff_pe_base_relocation_callback_t callback,void *usr);

/** \brief 应用PE基址重定位
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param image uint8_t* 已加载的映像
 * \param image_size size_t 映像大小
 * \param delta int64_t 加载基址与首选基址的差值
 * \return bool 是否成功
 *
 */
bool hcoff_pe_base_relocation_apply(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,uint8_t *image,size_t image_size,int64_t delta);

typedef struct hcoff_pe_tls_directory hcoff_pe_tls_directory_t;
struct hcoff_pe_tls_directory
{
    uint64_t start_address_of_raw_data;
    uint64_t end_address_of_raw_data;
    uint64_t address_of_index;
    uint64_t address_of_callbacks;
    uint32_t size_of_zero_fill;
    uint32_t characteristics;
};

/** \brief 读取PE TLS目录
 *
 * \param header const hcoff_pe_header_t* PE文件头
 * \param input_file hcoff_file_input_t* 输入文件
 * \param tls hcoff_pe_tls_directory_t* TLS目录
 * \return bool 是否成功
 *
 */
bool hcoff_pe_tls_directory_read(const hcoff_pe_header_t *header,hcoff_file_input_t *input_file,hcoff_pe_tls_directory_t *tls);

typedef struct hcoff_pe_image hcoff_pe_image_t;
struct hcoff_pe_image
{
    hcoff_pe_header_t header;
    uint8_t *image;                 /**< 用户提供的映像缓冲区 */
    size_t image_size;              /**< 映像实际大小 */
    size_t buffer_size;             /**< 用户提供的缓冲区大小 */
    uint64_t preferred_base;
    uint64_t loaded_base;
    bool buffer_owned;              /**< 缓冲区是否由内部通过hmmap申请 */
    bool relocated;
};

/** \brief 获取PE映像所需缓冲区大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return size_t 所需缓冲区大小,失败返回0
 *
 */
size_t hcoff_pe_image_buffer_size_get(hcoff_file_input_t *input_file);

/** \brief 初始化PE映像(按照PE头部信息将各节映射到用户提供的缓冲区)
 *
 * \param image hcoff_pe_image_t* 映像(由调用者提供)
 * \param input_file hcoff_file_input_t* 输入文件
 * \param buffer void* 用户提供的映像缓冲区
 * \param buffer_size size_t 缓冲区大小,应当不小于hcoff_pe_image_buffer_size_get的返回值
 * \return bool 是否成功
 *
 * 注意:若需要执行映像中的代码,调用者应当保证缓冲区具有可执行权限(如使用hmmap)。
 */
bool hcoff_pe_image_init(hcoff_pe_image_t *image,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size);

/** \brief 清理PE映像(不释放用户提供的缓冲区)
 *
 * \param image hcoff_pe_image_t* 映像
 *
 */
void hcoff_pe_image_deinit(hcoff_pe_image_t *image);

/** \brief 创建PE映像(映像使用hmmap申请,结构体使用hmalloc申请)
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return hcoff_pe_image_t* 映像,失败返回NULL
 *
 */
hcoff_pe_image_t *hcoff_pe_image_new(hcoff_file_input_t *input_file);

/** \brief 删除PE映像(释放内部申请的内存)
 *
 * \param image hcoff_pe_image_t* 映像
 *
 */
void hcoff_pe_image_delete(hcoff_pe_image_t *image);

/** \brief 获取PE映像中的RVA地址
 *
 * \param image const hcoff_pe_image_t* 映像
 * \param rva uint32_t RVA
 * \return void* 地址,失败返回NULL
 *
 */
void *hcoff_pe_image_rva_to_ptr(const hcoff_pe_image_t *image,uint32_t rva);

/** \brief 获取PE映像入口点
 *
 * \param image const hcoff_pe_image_t* 映像
 * \return void* 入口点地址
 *
 */
void *hcoff_pe_image_entry_point(const hcoff_pe_image_t *image);

/** \brief 查找PE映像导出
 *
 * \param image const hcoff_pe_image_t* 映像
 * \param name const char* 导出名称
 * \return void* 导出地址,失败返回NULL
 *
 */
void *hcoff_pe_image_export_find(const hcoff_pe_image_t *image,const char *name);

typedef uintptr_t (*hcoff_pe_import_resolve_t)(const char *dll_name,const char *function_name,uint16_t ordinal,void *usr);

/** \brief 解析PE映像导入
 *
 * \param image hcoff_pe_image_t* 映像
 * \param resolve hcoff_pe_import_resolve_t 导入解析回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hcoff_pe_image_resolve_imports(hcoff_pe_image_t *image,hcoff_pe_import_resolve_t resolve,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_PE_H__
