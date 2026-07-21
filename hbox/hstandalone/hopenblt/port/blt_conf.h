#ifndef __BLT_CONFIG_H__
#define __BLT_CONFIG_H__

#include "hdefaults.h"

/*
 * dummy配置
 */
#ifndef BOOT_CPU_XTAL_SPEED_KHZ
/*
 * 此项用于定时器及某些外设初始化
 */
#define BOOT_CPU_XTAL_SPEED_KHZ     80000
#endif
#ifndef BOOT_CPU_SYSTEM_SPEED_KHZ
/*
 * 此项用于定时器及某些外设初始化
 */
#define BOOT_CPU_SYSTEM_SPEED_KHZ   80000
#endif
#ifndef BOOT_CPU_BYTE_ORDER_MOTOROLA
#if((HDEFAULTS_ENDIAN_ORDER)==(HDEFAULTS_ENDIAN_ORDER_BIG))
#define BOOT_CPU_BYTE_ORDER_MOTOROLA  (1)
#else
#define BOOT_CPU_BYTE_ORDER_MOTOROLA  (0)
#endif
#endif
#ifndef BOOT_NVM_SIZE_KB
/*
 * 此项必须由用户配置为实际存储的大小
 */
#define BOOT_NVM_SIZE_KB  1
#endif

#endif
