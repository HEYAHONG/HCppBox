/***************************************************************
 * Name:      hdefaults_libc_port.h
 * Purpose:   声明hdefaults_libc_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_PORT_H__
#define __HDEFAULTS_LIBC_PORT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "wrapper/hputchar.h"
#include "wrapper/hgetchar.h"
#include "wrapper/hgetenv.h"
#include "wrapper/hmalloc.h"
#include "wrapper/hfree.h"
#include "wrapper/hcalloc.h"
#include "wrapper/hrealloc.h"
#include "wrapper/habort.h"
#include "wrapper/hexit.h"
#include "wrapper/hfputs.h"
#include "wrapper/hvfprintf.h"
#include "wrapper/hputs.h"
#include "wrapper/hferror.h"
#include "wrapper/hfread.h"
#include "wrapper/hfwrite.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDEFAULTS_LIBC_PORT_H__
