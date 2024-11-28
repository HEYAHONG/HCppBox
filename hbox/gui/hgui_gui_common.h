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

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_COMMON_H__
