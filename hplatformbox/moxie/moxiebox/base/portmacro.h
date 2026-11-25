/***************************************************************
 * Name:      portmacro.h
 * Purpose:   moxie FreeRTOS移植宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define portSTACK_TYPE          uint32_t
#define portBASE_TYPE           int32_t
#define portUBASE_TYPE          uint32_t
#define portPOINTER_SIZE_TYPE   uint32_t

typedef portSTACK_TYPE   StackType_t;
typedef portBASE_TYPE    BaseType_t;
typedef portUBASE_TYPE   UBaseType_t;
typedef portUBASE_TYPE   TickType_t;

#define portMAX_DELAY           ((TickType_t)0xffffffffUL)
#define portSTACK_GROWTH        (-1)
#define portTICK_PERIOD_MS      ((TickType_t)(1000 / configTICK_RATE_HZ))
#define portBYTE_ALIGNMENT      4
#define portTICK_TYPE_IS_ATOMIC 1
#define portCRITICAL_NESTING_IN_TCB 0

#define portNOP()               __asm__ volatile("nop")
#define portMEMORY_BARRIER()    __asm__ volatile("" ::: "memory")

extern void vPortYield(void);
#define portYIELD()             vPortYield()

extern void vTaskSwitchContext(void);
#define portEND_SWITCHING_ISR(x) \
    do {                         \
        if ((x) != 0) {          \
            vTaskSwitchContext();\
        }                        \
    } while (0)
#define portYIELD_FROM_ISR(x)   portEND_SWITCHING_ISR(x)

extern void vPortDisableInterrupts(void);
extern void vPortEnableInterrupts(void);
extern UBaseType_t vPortSetInterruptMask(void);
extern void vPortClearInterruptMask(UBaseType_t mask);

#define portDISABLE_INTERRUPTS() vPortDisableInterrupts()
#define portENABLE_INTERRUPTS()  vPortEnableInterrupts()
#define portSET_INTERRUPT_MASK_FROM_ISR() vPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortClearInterruptMask(x)

extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);
#define portENTER_CRITICAL()    vPortEnterCritical()
#define portEXIT_CRITICAL()     vPortExitCritical()

#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters)       void vFunction(void *pvParameters)

#ifdef __cplusplus
}
#endif

#endif
