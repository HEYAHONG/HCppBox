/***************************************************************
 * Name:      hgui_gui_common.h
 * Purpose:   声明common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_COMMON_H__
#define __HGUI_GUI_COMMON_H__
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "wchar.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(_MSC_VER)

#include <BaseTsd.h>
#ifndef ssize_t
typedef SSIZE_T ssize_t;
#endif

#else

#ifndef ssize_t
typedef intptr_t ssize_t;
#endif // ssize_t

/** \brief GUI内存分配
 *
 * \param nBytes size_t 大小
 * \return void* 指针，失败为NULL
 *
 */
void * hgui_gui_malloc(size_t nBytes);


typedef void * (*hgui_gui_malloc_t)(size_t nBytes);

/** \brief 设置GUI内存分配钩子
 *
 * \param hook hgui_gui_malloc_t malloc 钩子
 *
 */
void hgui_gui_malloc_set(hgui_gui_malloc_t hook);


/** \brief GUI内存默认分配(不受钩子影响)
 *
 * \param nBytes size_t 大小
 * \return void* 指针，失败为NULL
 *
 */
void * hgui_gui_default_malloc(size_t nBytes);


/** \brief GUI内存释放
 *
 * \param ptr void* 指针
 *
 */
void hgui_gui_free(void *ptr);


typedef void (*hgui_gui_free_t)(void *ptr);

/** \brief  设置GUI内存释放钩子
 *
 * \param hook hgui_gui_free_t free钩子
 *
 */
void hgui_gui_free_set(hgui_gui_free_t hook);

/** \brief GUI内存默认释放(不受钩子影响)
 *
 * \param ptr void* 指针
 *
 */
void hgui_gui_default_free(void *ptr);



#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_COMMON_H__
