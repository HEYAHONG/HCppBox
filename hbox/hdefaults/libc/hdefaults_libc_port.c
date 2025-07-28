/***************************************************************
 * Name:      hdefaults_libc_port.c
 * Purpose:   实现hdefaults_libc_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults_libc_port.h"

/*
 * 默认优化级别为0
 */
#ifndef HDEFAULTS_LIBC_OPTIMIZE_LEVEL
#define HDEFAULTS_LIBC_OPTIMIZE_LEVEL     0
#endif // HDEFAULTS_LIBC_OPTIMIZE_LEVEL


#include "wrapper/hputchar.c"
#include "wrapper/hgetchar.c"
#include "wrapper/hgetenv.c"
#include "wrapper/hmalloc.c"
#include "wrapper/hfree.c"
#include "wrapper/hcalloc.c"
#include "wrapper/hrealloc.c"
#include "wrapper/habort.c"
#include "wrapper/hexit.c"
#include "wrapper/hfputs.c"
#include "wrapper/hvfprintf.c"
#include "wrapper/hfprintf.c"
#include "wrapper/hputs.c"
#include "wrapper/hferror.c"
#include "wrapper/hfread.c"
#include "wrapper/hfwrite.c"
#include "wrapper/htime.c"
#include "wrapper/hclock.c"

