/***************************************************************
 * Name:      HCPPGui.h
 * Purpose:   声明HCPPGui对外接口。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPGUI_H__
#define __HCPPGUI_H__
#include "hbox.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief HCPPGui初始化
 *
 * \return void* 不使用此指针
 *
 */
void *HCPPGuiInit(void);



#ifdef main
//windows下,SDL库会重新定义main为SDL_main,在此处取消定义
#undef main
#endif // main



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HCPPGUI_H__

