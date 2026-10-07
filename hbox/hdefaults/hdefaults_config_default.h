/***************************************************************
 * Name:      hdefaults_config_default.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_CONFIG_DEFAULT_H__
#define __HDEFAULTS_CONFIG_DEFAULT_H__

/*
 * 定义变量属性（初始化为0）
 */
#if !defined(HDEFAULTS_ZI_ATTRIBUTE)
#define HDEFAULTS_ZI_ATTRIBUTE
#endif

/*
 * 定义变量属性（初始化不为0）
 */
#if !defined(HDEFAULTS_RW_ATTRIBUTE)
#define HDEFAULTS_RW_ATTRIBUTE
#endif

/*
 * 定义常量属性
 */
#if !defined(HDEFAULTS_RO_ATTRIBUTE)
#define HDEFAULTS_RO_ATTRIBUTE
#endif

/*
 * 定义pack属性，通常用于网络数据包解析
 */
#if !defined(HDEFAULTS_PACKED_ATTRIBUTE)
#include "hcompiler.h"
#define HDEFAULTS_PACKED_ATTRIBUTE __PACKED
#endif
#endif
