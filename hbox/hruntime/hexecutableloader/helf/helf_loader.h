/***************************************************************
 * Name:      helf_loader.h
 * Purpose:   声明helf_loader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_LOADER_H__
#define __HELF_LOADER_H__
#include "helf_common.h"
#include "helf_arch.h"
#include "hmemory.h"
#include "hruntime.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct helf_namespace helf_namespace_t;
typedef struct helf_loaded helf_loaded_t;

/*
 * 已加载ELF类型
 */
typedef enum
{
    HELF_LOADED_TYPE_NONE=0,            /**< 未知 */
    HELF_LOADED_TYPE_RELOCATABLE=1,     /**< 可重定位对象(ET_REL) */
    HELF_LOADED_TYPE_SHARED_OBJECT=2,   /**< 动态链接库/共享对象(ET_DYN) */
    HELF_LOADED_TYPE_EXECUTABLE=3       /**< 可执行文件(ET_EXEC) */
} helf_loaded_type_t;

/*
 * 外部符号查找回调(用于加载时需要用户提供地址的符号)
 */
typedef const hruntime_symbol_t *(*helf_external_symbol_find_t)(const char *name,void *usr);

/*
 * 加载额外参数
 */
typedef struct helf_load_param
{
    struct
    {
        uint32_t load_address:1;        /**< 是否设置加载地址 */
        uint32_t fixed_address:1;       /**< 是否必须使用指定的加载地址 */
    } flags;                            /**< 标志位 */
    uintptr_t load_address;             /**< 加载地址(对应文件最小虚拟地址) */
    helf_external_symbol_find_t external_symbol_find; /**< 外部符号查找回调 */
    void *usr;                          /**< 外部符号查找用户参数 */
} helf_load_param_t;

/*
 * 加载额外参数需求
 */
typedef struct helf_load_param_requirement
{
    struct
    {
        uint32_t load_address:1;        /**< 是否需要用户设置加载地址 */
        uint32_t fixed_address:1;       /**< 加载地址是否必须精确匹配 */
        uint32_t external_symbol:1;     /**< 是否需要外部符号查找回调 */
    } required;                         /**< 需求标志 */
    uintptr_t suggested_address;        /**< 建议加载地址(通常为ET_EXEC的最小虚拟地址) */
} helf_load_param_requirement_t;

/*
 * 已加载ELF对象
 */
struct helf_loaded
{
    hdoublylinkedlist_head_t    list_head;          /**< 命名空间链表节点 */
    helf_loaded_type_t          type;               /**< ELF类型 */
    uint16_t                    machine;            /**< 机器类型(HELF_EM_*) */
    uint8_t                     elfclass;           /**< ELF类别 */
    uint8_t                     elfdata;            /**< ELF数据格式 */
    helf_file_input_t           *input_file;        /**< 加载时输入文件(仅加载期间有效) */
    void                        *image;             /**< 映像基地址 */
    size_t                      image_size;         /**< 映像大小 */
    void                        *rx_image;          /**< 可读可执行区域 */
    size_t                      rx_size;            /**< 可读可执行区域大小 */
    void                        *ro_image;          /**< 只读区域 */
    size_t                      ro_size;            /**< 只读区域大小 */
    void                        *rw_image;          /**< 可读可写区域 */
    size_t                      rw_size;            /**< 可读可写区域大小 */
    uintptr_t                   min_vaddr;          /**< 程序段最小虚拟地址 */
    uintptr_t                   load_bias;          /**< 加载偏移 */
    hruntime_symbol_t           *symbol_table;      /**< 导出符号表 */
    size_t                      symbol_table_size;  /**< 导出符号数量 */
    char                        *symbol_names;      /**< 符号名称缓冲区 */
    size_t                      symbol_names_size;  /**< 符号名称缓冲区大小 */
    helf_external_symbol_find_t external_symbol_find; /**< 加载时外部符号查找回调 */
    void                        *external_usr;      /**< 加载时外部符号查找用户参数 */
    void                        *toc;               /**< PPC64 TOC */
    size_t                      toc_size;           /**< TOC大小 */
    size_t                      toc_used;           /**< TOC已使用字节数 */
    uintptr_t                   toc_base;           /**< TOC基地址(.TOC.) */
    struct
    {
        uint32_t image_owned:1;                     /**< 映像是否由内部申请 */
        uint32_t symbols_owned:1;                   /**< 符号表是否由内部申请 */
        uint32_t loaded_owned:1;                    /**< helf_loaded_t本身是否由内部申请 */
        uint32_t toc_owned:1;                       /**< TOC是否由内部申请 */
    } flags;                                        /**< 标志位 */
    void                        *usr;               /**< 用户参数 */
};

/*
 * 命名空间
 */
struct helf_namespace
{
    hdoublylinkedlist_head_t   *loaded_list_head;   /**< 已加载对象链表 */
    void                       *usr;                /**< 用户参数 */
    uint16_t                    machine;            /**< 命名空间机器类型 */
    uint8_t                     elfclass;           /**< 命名空间ELF类别 */
    uint8_t                     elfdata;            /**< 命名空间ELF数据格式 */
    helf_load_param_t           load_param;         /**< 默认加载额外参数 */
    uintptr_t                   reloc_load_address; /**< 可重定位对象默认加载地址(内部使用) */
    struct
    {
        uint32_t load_param_set:1;                  /**< 是否已设置默认加载额外参数 */
    } flags;                                        /**< 标志位 */
};

/** \brief 获取本地机器类型
 *
 * \return uint16_t 机器类型(HELF_EM_*),未知返回HELF_EM_NONE
 *
 */
uint16_t helf_namespace_native_machine_type(void);

/** \brief 获取输入文件机器类型
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return uint16_t 机器类型(HELF_EM_*),未知返回HELF_EM_NONE
 *
 */
uint16_t helf_namespace_file_machine_type(helf_file_input_t *input_file);

/** \brief 输入文件是否为本地架构ELF
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return bool 是否为本地架构ELF
 *
 */
bool helf_namespace_is_native_file(helf_file_input_t *input_file);

/** \brief 初始化加载额外参数
 *
 * \param param helf_load_param_t* 参数
 *
 */
void helf_load_param_init(helf_load_param_t *param);

/** \brief 查询加载额外参数需求
 *
 * 用于判断加载指定ELF是否需要用户提供加载地址或外部符号查找回调。
 *
 * \param input_file helf_file_input_t* 输入文件
 * \param requirement helf_load_param_requirement_t* 需求
 * \return bool 是否成功
 *
 */
bool helf_namespace_loaded_param_requirement_get(helf_file_input_t *input_file,helf_load_param_requirement_t *requirement);

/** \brief 设置命名空间默认加载额外参数
 *
 * \param ns helf_namespace_t* 命名空间
 * \param param const helf_load_param_t* 参数,NULL表示清除
 * \return bool 是否成功
 *
 */
bool helf_namespace_load_param_set(helf_namespace_t *ns,const helf_load_param_t *param);

/** \brief 获取命名空间默认加载额外参数
 *
 * \param ns helf_namespace_t* 命名空间
 * \return const helf_load_param_t* 参数,未设置返回NULL
 *
 */
const helf_load_param_t *helf_namespace_load_param_get(helf_namespace_t *ns);

/** \brief 创建命名空间
 *
 * \param usr void* 用户参数
 * \return helf_namespace_t* 命名空间,失败返回NULL
 *
 */
helf_namespace_t *helf_namespace_new(void *usr);

/** \brief 删除命名空间
 *
 * 内部会清理所有已加载对象。由 loaded_new 创建的对象连同对象本身一起释放；
 * 由 loaded_init 使用调用者对象/缓冲区的, 只做 deinit, 不释放对象和缓冲区。
 *
 * \param ns helf_namespace_t* 命名空间
 *
 */
void helf_namespace_delete(helf_namespace_t *ns);

/** \brief 在命名空间中查找符号
 *
 * \param ns helf_namespace_t* 命名空间
 * \param name const char* 符号名称
 * \return const hruntime_symbol_t* 符号,未找到返回NULL
 *
 */
const hruntime_symbol_t *helf_namespace_symbol_find(helf_namespace_t *ns,const char *name);

/** \brief 获取加载对象所需缓冲区大小(用户提供缓冲区接口)
 *
 * 返回的大小已包含缓冲区起始地址对齐所需的余量,调用者可直接使用任意地址的缓冲区。
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 所需缓冲区大小,失败返回0
 *
 */
size_t helf_namespace_loaded_buffer_size_get(helf_file_input_t *input_file);

/** \brief 获取加载对象所需缓冲区总大小(包含helf_loaded_t)
 *
 * 用于 helf_namespace_loaded_buffer_new。返回大小已包含元数据与对齐余量。
 *
 * \param input_file helf_file_input_t* 输入文件
 * \return size_t 所需缓冲区总大小,失败返回0
 *
 */
size_t helf_namespace_loaded_buffer_total_size_get(helf_file_input_t *input_file);

/** \brief 初始化已加载ELF对象(使用用户提供的缓冲区)
 *
 * 缓冲区在 loaded 生命周期内必须保持有效,deinit 不会释放它。
 * 若需要执行 ET_REL/ET_DYN 中的代码,缓冲区必须具有可执行权限。
 *
 * \param ns helf_namespace_t* 命名空间
 * \param loaded helf_loaded_t* 已加载对象(由调用者提供)
 * \param input_file helf_file_input_t* 输入文件
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_size size_t 缓冲区大小
 * \return bool 是否成功
 *
 */
bool helf_namespace_loaded_init(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,void *buffer,size_t buffer_size);

/** \brief 初始化已加载ELF对象(使用用户提供的缓冲区,可设置额外参数)
 *
 * 若 param 设置了 load_address,缓冲区中的映像起始地址必须与之相同。
 * 若需要执行代码,缓冲区必须可执行。
 *
 * \param ns helf_namespace_t* 命名空间
 * \param loaded helf_loaded_t* 已加载对象(由调用者提供)
 * \param input_file helf_file_input_t* 输入文件
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_size size_t 缓冲区大小
 * \param param const helf_load_param_t* 加载额外参数,NULL使用命名空间默认值
 * \return bool 是否成功
 *
 */
bool helf_namespace_loaded_init_ex(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,void *buffer,size_t buffer_size,const helf_load_param_t *param);

/** \brief 使用用户缓冲区创建并注册已加载ELF对象
 *
 * 元数据 helf_loaded_t 与映像/符号表均位于 buffer 内,不额外申请内存。
 * 缓冲区总大小由 helf_namespace_loaded_buffer_total_size_get 获取。
 * 失败返回NULL。清理使用 helf_namespace_loaded_deinit 或
 * helf_namespace_loaded_delete(不会释放 buffer)。
 *
 * \param ns helf_namespace_t* 命名空间
 * \param input_file helf_file_input_t* 输入文件
 * \param buffer void* 用户提供的缓冲区
 * \param buffer_size size_t 缓冲区大小
 * \param param const helf_load_param_t* 加载额外参数,NULL使用命名空间默认值
 * \return helf_loaded_t* 已加载对象,失败返回NULL
 *
 */
helf_loaded_t *helf_namespace_loaded_buffer_new(helf_namespace_t *ns,helf_file_input_t *input_file,void *buffer,size_t buffer_size,const helf_load_param_t *param);

/** \brief 清理已加载ELF对象(不释放用户提供的缓冲区)
 *
 * \param ns helf_namespace_t* 命名空间
 * \param loaded helf_loaded_t* 已加载对象
 *
 */
void helf_namespace_loaded_deinit(helf_namespace_t *ns,helf_loaded_t *loaded);

/** \brief 创建已加载ELF对象(内部申请内存)
 *
 * \param ns helf_namespace_t* 命名空间
 * \param input_file helf_file_input_t* 输入文件
 * \return helf_loaded_t* 已加载对象,失败返回NULL
 *
 */
helf_loaded_t *helf_namespace_loaded_new(helf_namespace_t *ns,helf_file_input_t *input_file);

/** \brief 创建已加载ELF对象(内部申请内存,可设置额外参数)
 *
 * 若设置 load_address 且 fixed_address=1, 使用固定地址映射(会覆盖该地址已有映射)。
 * 若只设置 load_address, 则作为地址要求, 系统未满足时加载失败。
 *
 * \param ns helf_namespace_t* 命名空间
 * \param input_file helf_file_input_t* 输入文件
 * \param param const helf_load_param_t* 加载额外参数,NULL使用命名空间默认值
 * \return helf_loaded_t* 已加载对象,失败返回NULL
 *
 */
helf_loaded_t *helf_namespace_loaded_new_ex(helf_namespace_t *ns,helf_file_input_t *input_file,const helf_load_param_t *param);

/** \brief 删除已加载ELF对象(内部申请内存)
 *
 * 若对象由 loaded_new 创建, 同时释放对象本身；若由 loaded_init 初始化,
 * 仅执行 deinit, 不释放调用者提供的对象和缓冲区。
 *
 * \param ns helf_namespace_t* 命名空间
 * \param loaded helf_loaded_t* 已加载对象
 *
 */
void helf_namespace_loaded_delete(helf_namespace_t *ns,helf_loaded_t *loaded);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HELF_LOADER_H__
