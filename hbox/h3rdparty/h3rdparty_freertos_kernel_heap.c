/***************************************************************
 * Name:      h3rdparty_freertos_kernel.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"



#ifdef  FREERTOS_KERNEL

#ifndef FREERTOS_KERNEL_MEMMANG_HEAP
#define FREERTOS_KERNEL_MEMMANG_HEAP 3
#endif

#if (FREERTOS_KERNEL_MEMMANG_HEAP) == 1
#include "3rdparty/FreeRTOS/Kernel/heap_1.c"
#elif (FREERTOS_KERNEL_MEMMANG_HEAP) == 2
#include "3rdparty/FreeRTOS/Kernel/heap_2.c"
#elif (FREERTOS_KERNEL_MEMMANG_HEAP) == 4
#include "3rdparty/FreeRTOS/Kernel/heap_4.c"
#elif (FREERTOS_KERNEL_MEMMANG_HEAP) == 5
#include "3rdparty/FreeRTOS/Kernel/heap_5.c"
#else
/*
 * 默认使用heap_3
 */
#include "3rdparty/FreeRTOS/Kernel/heap_3.c"
#endif

#endif // FREERTOS_KERNEL 


