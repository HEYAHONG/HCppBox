/***************************************************************
 * Name:      hcoff_namespace.h
 * Purpose:   声明hcoff_namespace接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_NAMESPACE_H__
#define __HCOFF_NAMESPACE_H__
#include "hcoff_common.h"
#include "hcoff_relocatable.h"
#include "stdbool.h"
#include "hmemory.h"
#include "hruntime.h"
/*
 * 命名空间主要用于防止符号名称的冲突，不同命名空间的符号名称可以相同且不会出现覆盖的情况
 */
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief HCOFF 命名空间 获取本地机器类型
 *
 * \return uint16_t 机器类型,见 HCOFF_FILEHEADER_F_MAGIC_*
 *
 */
uint16_t hcoff_namespace_native_machine_type(void);

/** \brief HCOFF 命名空间 获取本地机器类型
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return uint16_t 机器类型,见 HCOFF_FILEHEADER_F_MAGIC_*
 *
 */
uint16_t hcoff_namespace_file_machine_type(hcoff_file_input_t *input_file);

/** \brief HCOFF 命名空间 是否是本地可重定位文件
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \return bool 是否是本地可重定位文件
 *
 */
bool hcoff_namespace_is_native_relocatable_file(hcoff_file_input_t *input_file);

typedef struct hcoff_namespace hcoff_namespace_t;
typedef  struct hcoff_namespace_relocatable hcoff_namespace_relocatable_t;
struct hcoff_namespace_relocatable
{
    hdoublylinkedlist_head_t relocatable_list_head;
    const void *      relocatable_handle;
    uint16_t    machine_type;
    bool        relocatable_buffer_owned;   /**< 映像缓冲区是否由内部申请(兼容旧接口) */
    hcoff_relocatable_t generic_relocatable;/**< 通用可重定位对象(ARM/RISC-V/LoongArch) */
};

/** \brief HCOFF 命名空间 初始化可重定位对象(使用用户提供的缓冲区)
 *
 * \param ns hcoff_namespace_t *命名空间
 * \param namespace_relocatable hcoff_namespace_relocatable_t* 可重定位对象(由调用者提供)
 * \param input_file hcoff_file_input_t* 输入文件
 * \param buffer void* 用户提供的映像缓冲区
 * \param buffer_size size_t 缓冲区大小
 * \return bool 是否成功
 *
 */
bool hcoff_namespace_relocatable_init(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size);

/** \brief HCOFF 命名空间 清理可重定位对象(不释放用户提供的缓冲区)
 *
 * \param ns hcoff_namespace_t *命名空间
 * \param namespace_relocatable hcoff_namespace_relocatable_t* 可重定位文件指针
 *
 */
void hcoff_namespace_relocatable_deinit(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable);

/** \brief HCOFF 命名空间 创建可重定位对象
 *
 * \param ns hcoff_namespace_t *命名空间
 * \param input_file hcoff_file_input_t* 输入文件
 * \return hcoff_namespace_relocatable_t* 可重定位文件指针
 *
 */
hcoff_namespace_relocatable_t *hcoff_namespace_relocatable_new(hcoff_namespace_t *ns,hcoff_file_input_t *input_file);

/** \brief HCOFF 命名空间 删除可重定位对象
 *
 * \param ns hcoff_namespace_t *命名空间
 * \param namespace_relocatable hcoff_namespace_relocatable_t* 可重定位文件指针
 *
 */
void hcoff_namespace_relocatable_delete(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable);

struct hcoff_namespace
{
    int flags;                                              /**< 标志位,通常由内部使用  */
    hdoublylinkedlist_head_t *relocatable_list_head;        /**< 可重定向对象链表 */
    void *usr;                                              /**< 用户参数 */
    uint16_t machine_type;                                  /**< 机器类型，同一命名空间只能加载同一类型的可重定向对象 */
};

/** \brief 创建命名空间
 *
 * \param usr void* 用户参数
 * \return hcoff_namespace_t* 命名空间
 *
 */
hcoff_namespace_t * hcoff_namespace_new(void *usr);

/** \brief 在命名空间中查找符号
 *
 * \param ns hcoff_namespace_t* 命名空间
 * \param name const char* 符号
 * \return const hruntime_symbol_t* 名称
 *
 */
const hruntime_symbol_t *hcoff_namespace_symbol_find(hcoff_namespace_t * ns,const char *name);

/** \brief 删除命名空间
 *
 * \param ns hcoff_namespace_t* 命名空间
 *
 */
void hcoff_namespace_delete(hcoff_namespace_t *ns);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_NAMESPACE_H__
