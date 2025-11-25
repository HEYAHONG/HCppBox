/***************************************************************
 * Name:      hcompiler_msvc.h
 * Purpose:   定义编译器相关宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_MSVC_H__
#define __HCOMPILER_MSVC_H__

/*
 * 动态连接库导出
 */
#ifndef __DLLEXPORT
#define __DLLEXPORT  __declspec( dllexport )
#endif // __DLLEXPORT


/*
 * 动态连接库导入
 */
#ifndef __DLLIMPORT
#define __DLLIMPORT  __declspec( dllimport )
#endif // __DLLIMPORT

#endif
