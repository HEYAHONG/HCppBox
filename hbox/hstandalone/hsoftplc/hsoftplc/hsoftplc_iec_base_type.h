/***************************************************************
 * Name:      hsoftplc_iec_base_type.h
 * Purpose:   声明hsoftplc_iec_base_type接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTPLC_IEC_BASE_TYPE_H_INCLUDE__
#define __HSOFTPLC_IEC_BASE_TYPE_H_INCLUDE__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
/*
 * 注意：本文件与matiec的C语言头文件不兼容，不可混用
 */


/*
 * 基础类型
 */

#ifndef IEC_BOOL
#define IEC_BOOL     uint8_t
#endif


#ifndef IEC_SINT
#define IEC_SINT     int8_t
#endif
#ifndef IEC_INT
#define IEC_INT      int16_t
#endif
#ifndef IEC_DINT
#define IEC_DINT     int32_t
#endif
#ifndef IEC_LINT
#define IEC_LINT     int64_t
#endif

#ifndef IEC_USINT
#define IEC_USINT    uint8_t
#endif
#ifndef IEC_UINT
#define IEC_UINT     uint16_t
#endif
#ifndef IEC_UDINT
#define IEC_UDINT    uint32_t
#endif
#ifndef IEC_ULINT
#define IEC_ULINT    uint64_t
#endif

#ifndef IEC_BYTE
#define IEC_BYTE     uint8_t
#endif
#ifndef IEC_WORD
#define IEC_WORD     uint16_t
#endif
#ifndef IEC_DWORD
#define IEC_DWORD    uint32_t
#endif
#ifndef IEC_LWORD
#define IEC_LWORD    uint64_t
#endif

#ifndef IEC_REAL
#define IEC_REAL     float
#endif

#ifndef IEC_LREAL
#define IEC_LREAL    double
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTPLC_IEC_BASE_TYPE_H_INCLUDE__
