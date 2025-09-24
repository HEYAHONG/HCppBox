/***************************************************************
 * Name:      h3rdparty_port_FreeRTOS.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_FREERTOS_H_INCLUDE__
#define __H3RDPARTY_PORT_FREERTOS_H_INCLUDE__

/*
 * FreeRTOS头文件
 */
#ifndef FREERTOS_KERNEL_FREERTOS_HEADER
#define FREERTOS_KERNEL_FREERTOS_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/FreeRTOS.h"
#endif
#ifndef FREERTOS_KERNEL_TASK_HEADER
#define FREERTOS_KERNEL_TASK_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/task.h"
#endif
#ifndef FREERTOS_KERNEL_TIMERS_HEADER
#define FREERTOS_KERNEL_TIMERS_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/timers.h"
#endif
#ifndef FREERTOS_KERNEL_QUEUE_HEADER
#define FREERTOS_KERNEL_QUEUE_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/queue.h"
#endif
#ifndef FREERTOS_KERNEL_SEMPHR_HEADER
#define FREERTOS_KERNEL_SEMPHR_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/semphr.h"
#endif
#ifndef FREERTOS_KERNEL_CROUTINE_HEADER
#define FREERTOS_KERNEL_CROUTINE_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/croutine.h"
#endif
#ifndef FREERTOS_KERNEL_LIST_HEADER
#define FREERTOS_KERNEL_LIST_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/list.h"
#endif
#ifndef FREERTOS_KERNEL_EVENT_GROUPS_HEADER
#define FREERTOS_KERNEL_EVENT_GROUPS_HEADER "h3rdparty/3rdparty/FreeRTOS/Kernel/event_groups.h"
#endif

#ifdef FREERTOS_KERNEL

/*
 * 默认使用heap_3.c
 */
#ifndef FREERTOS_KERNEL_MEMMANG_HEAP
#define FREERTOS_KERNEL_MEMMANG_HEAP 3
#endif

/*
 * 头文件包含(自动包含FreeRTOS内核头文件)
 */
#ifndef FREERTOS_KERNEL_NO_AUTO_INCLUDE_HEADER
#include FREERTOS_KERNEL_FREERTOS_HEADER
#include FREERTOS_KERNEL_TASK_HEADER
#include FREERTOS_KERNEL_TIMERS_HEADER
#include FREERTOS_KERNEL_QUEUE_HEADER
#include FREERTOS_KERNEL_SEMPHR_HEADER
#include FREERTOS_KERNEL_CROUTINE_HEADER
#include FREERTOS_KERNEL_LIST_HEADER
#include FREERTOS_KERNEL_EVENT_GROUPS_HEADER
#endif

#endif

#endif // __H3RDPARTY_PORT_FREERTOS_H_INCLUDE__
