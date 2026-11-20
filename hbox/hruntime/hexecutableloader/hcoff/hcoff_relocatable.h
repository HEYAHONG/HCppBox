/***************************************************************
 * Name:      hcoff_relocatable.h
 * Purpose:   声明hcoff通用可重定位对象接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_RELOCATABLE_H__
#define __HCOFF_RELOCATABLE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "hcoff_common.h"
#include "hcoff_arch.h"

struct hruntime_symbol;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hcoff_relocatable hcoff_relocatable_t;

typedef const struct hruntime_symbol *(*hcoff_relocatable_external_symbol_find_t)(const char *name,void *usr);

struct hcoff_relocatable
{
    uint16_t machine_type;
    const hcoff_arch_ops_t *arch;
    hcoff_file_input_t *input_file;                 /**< 输入文件,在对象生命周期内应当保持有效 */
    void *image;                                    /**< 用户提供的映像缓冲区 */
    size_t image_size;                              /**< 映像实际大小 */
    size_t buffer_size;                             /**< 用户提供的缓冲区大小 */
    void *usr;                                      /**< 用户参数 */
    hcoff_relocatable_external_symbol_find_t extern_symbol;
    bool buffer_owned;                              /**< 缓冲区是否由内部通过hmmap申请 */
    bool initialized;
};

/** \brief 获取可重定位对象所需映像缓冲区大小
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param machine uint16_t 机器类型,见HCOFF_FILEHEADER_F_MAGIC_*
 * \return size_t 所需缓冲区大小,失败返回0
 *
 */
size_t hcoff_relocatable_image_size_get(hcoff_file_input_t *input_file,uint16_t machine);

/** \brief 初始化可重定位对象
 *
 * \param relocatable hcoff_relocatable_t* 可重定位对象(由调用者提供)
 * \param input_file hcoff_file_input_t* 输入文件
 * \param machine uint16_t 机器类型,见HCOFF_FILEHEADER_F_MAGIC_*
 * \param buffer void* 用户提供的映像缓冲区
 * \param buffer_size size_t 缓冲区大小,应当不小于hcoff_relocatable_image_size_get的返回值
 * \param extern_symbol hcoff_relocatable_external_symbol_find_t 外部符号寻找回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 * 注意:若需要执行映像中的代码,调用者应当保证缓冲区具有可执行权限(如使用hmmap)。
 */
bool hcoff_relocatable_init(hcoff_relocatable_t *relocatable,hcoff_file_input_t *input_file,uint16_t machine,void *buffer,size_t buffer_size,hcoff_relocatable_external_symbol_find_t extern_symbol,void *usr);

/** \brief 清理可重定位对象(不释放用户提供的缓冲区)
 *
 * \param relocatable hcoff_relocatable_t* 可重定位对象
 *
 */
void hcoff_relocatable_deinit(hcoff_relocatable_t *relocatable);

/** \brief 创建可重定位对象(映像使用hmmap申请,结构体使用hmalloc申请)
 *
 * \param input_file hcoff_file_input_t* 输入文件
 * \param machine uint16_t 机器类型,见HCOFF_FILEHEADER_F_MAGIC_*
 * \param extern_symbol hcoff_relocatable_external_symbol_find_t 外部符号寻找回调
 * \param usr void* 用户参数
 * \return const hcoff_relocatable_t* 可重定位对象,失败返回NULL
 *
 */
const hcoff_relocatable_t *hcoff_relocatable_new_with_usr(hcoff_file_input_t *input_file,uint16_t machine,hcoff_relocatable_external_symbol_find_t extern_symbol,void *usr);

/** \brief 删除可重定位对象(释放内部申请的内存)
 *
 * \param relocatable const hcoff_relocatable_t* 可重定位对象
 *
 */
void hcoff_relocatable_delete(const hcoff_relocatable_t * relocatable);

/** \brief 查找可重定位对象中的符号
 *
 * \param relocatable hcoff_relocatable_t* 可重定位对象
 * \param name const char* 符号名称
 * \return const struct hruntime_symbol* 符号,失败返回NULL
 *
 * 注意:返回的符号指针指向内部静态缓存,下一次查找会覆盖该内容。
 */
const struct hruntime_symbol *hcoff_relocatable_symbol_find(hcoff_relocatable_t *relocatable,const char *name);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_RELOCATABLE_H__
