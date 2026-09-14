/***************************************************************
 * Name:      hsimplegui_font.h
 * Purpose:   声明hsimplegui_font接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIMPLEGUI_FONT_H__
#define __HSIMPLEGUI_FONT_H__
#include "hdefaults.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"


/*
 * 字体资源
 * 注意：使用此资源需要传入UTF8编码的字符串，简易的实现方法为将C源代码文件编码设置为UTF-8编码
 */

#ifdef __cplusplus
extern "C"
{
#endif

extern const SGUI_FONT_RES hsimplegui_font_ascii_8;
extern const SGUI_FONT_RES hsimplegui_font_ascii_12;
extern const SGUI_FONT_RES hsimplegui_font_ascii_16;

#ifdef __cplusplus
}
#endif

#endif
