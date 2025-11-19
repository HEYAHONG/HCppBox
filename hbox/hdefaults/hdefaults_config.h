/***************************************************************
 * Name:      hdefaults_config.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_CONFIG_H__
#define __HDEFAULTS_CONFIG_H__

//导入配置文件

//导入config.h
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef hdefaults_xstr
#define hdefaults_xstr(s) hdefaults_str(s)
#endif // hdefaults_xstr


#ifndef hdefaults_str
#define hdefaults_str(s) #s
#endif // hdefaults_str

//导入外部配置，使用命令行定义HBOX_CONFIG_HEADER指定外部配置文件名。
#ifdef HBOX_CONFIG_HEADER
#include hdefaults_xstr(HBOX_CONFIG_HEADER)
#endif

#endif
