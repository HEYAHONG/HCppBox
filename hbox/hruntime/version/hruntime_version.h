/***************************************************************
 * Name:      hruntime_version.h
 * Purpose:   声明hruntime_version接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRUNTIME_VERSION_H_INCLUDE__
#define __HRUNTIME_VERSION_H_INCLUDE__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * hruntime版本，也可指示hbox的版本
 */
extern const int hruntime_version_major;
extern const int hruntime_version_minor;
extern const int hruntime_version_patch;
extern const int hruntime_version_build;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRUNTIME_VERSION_H_INCLUDE__
