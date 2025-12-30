/***************************************************************
 * Name:      hdefaults_platform_esp.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_PLATFORM_ESP_H__
#define __HDEFAULTS_PLATFORM_ESP_H__

#if defined(IDF_VER)
/*
 * ESP_IDF框架
 */


/*
 * 包含Kconfig配置
 */
#include "sdkconfig.h"

/*
 * ESP-IDF自带cJSON，故而默认使用系统cJSON
 */
#define H3RDPARTY_USING_SYSTEM_CJSON 1

/*
 * ESP-IDF(非ESP8266版本)自带tlsf,故而默认使用系统tlsf。注意：需要手动添加包含路径,或者不编译tlsf第三方移植代码且不使用任何tlsf相关功能
 */
#ifndef CONFIG_IDF_TARGET_ESP8266
#define H3RDPARTY_USING_SYSTEM_TLSF 1
#endif

#endif

#endif
