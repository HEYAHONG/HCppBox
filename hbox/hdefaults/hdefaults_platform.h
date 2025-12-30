/***************************************************************
 * Name:      hdefaults_platform.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_PLATFORM_H__
#define __HDEFAULTS_PLATFORM_H__

#if defined(ESP_PLATFORM)
/*
 * ESP平台,ESP_PLATFORM可由esp-idf自动定义或者手工定义,注意：这不是编译器的内置宏
 */
#include "hdefaults_platform_esp.h"
#ifndef  HDEFAULTS_PLATFORM_ESP
#define  HDEFAULTS_PLATFORM_ESP  1
#endif

#endif


#endif
