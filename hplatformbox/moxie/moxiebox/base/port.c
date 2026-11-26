/***************************************************************
 * Name:      port.c
 * Purpose:   moxie FreeRTOS移植
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#define PORT_SREG_STATUS        0
#define PORT_SREG_HANDLER       1
#define PORT_SREG_CAUSE         2
#define PORT_SREG_VALUE         3
#define PORT_SREG_MIE           11
#define PORT_SREG_TIME_LO       12
#define PORT_SREG_TIME_HI       13
#define PORT_SREG_TIMECMP_LO    14
#define PORT_SREG_TIMECMP_HI    15
#define PORT_SREG_MSTATUS       16
#define PORT_SREG_MIP           17
#define PORT_SREG_CC            18

#define PORT_EXCEPTION_IRQ      2
#define PORT_INTERRUPT_TIMER    7
#define PORT_INTERRUPT_SOFTWARE 3

#define PORT_MIE_MSIE           (1UL << 3)
#define PORT_MIE_MTIE           (1UL << 7)
#define PORT_MSTATUS_MIE        (1UL << 3)
#define PORT_MIP_MSIP           (1UL << 3)

#define PORT_TIMER_INTERVAL_US  (1000000UL / configTICK_RATE_HZ)

volatile UBaseType_t uxCriticalNesting = 0;
volatile uint32_t ulMainStackPointer = 0;

extern void *volatile pxCurrentTCB;
extern void prvTaskExitError(void);
extern void vPortStartFirstTask(void);
extern void vPortExceptionHandler(void);

static inline uint32_t
port_get_sp(void)
{
    uint32_t sp;

    __asm__ volatile("mov %0, $sp" : "=r"(sp));
    return sp;
}

static inline uint32_t
port_sreg_read(uint32_t sreg)
{
    uint32_t value;

    __asm__ volatile("gsr %0, %1" : "=r"(value) : "i"(sreg));
    return value;
}

static inline void
port_sreg_write(uint32_t sreg, uint32_t value)
{
    __asm__ volatile("ssr %0, %1" : : "r"(value), "i"(sreg) : "memory");
}

static inline uint64_t
port_time_read(void)
{
    uint32_t lo = port_sreg_read(PORT_SREG_TIME_LO);
    uint32_t hi = port_sreg_read(PORT_SREG_TIME_HI);

    return ((uint64_t)hi << 32) | lo;
}

static inline void
port_timecmp_write(uint64_t value)
{
    port_sreg_write(PORT_SREG_TIMECMP_LO, (uint32_t)value);
    port_sreg_write(PORT_SREG_TIMECMP_HI, (uint32_t)(value >> 32));
}

static void
prvAdvanceTimer(void)
{
    port_timecmp_write(port_time_read() + PORT_TIMER_INTERVAL_US);
}

void
vPortDisableInterrupts(void)
{
    uint32_t status = port_sreg_read(PORT_SREG_STATUS);

    port_sreg_write(PORT_SREG_STATUS, status & ~1UL);
}

void
vPortEnableInterrupts(void)
{
    uint32_t status = port_sreg_read(PORT_SREG_STATUS);

    port_sreg_write(PORT_SREG_STATUS, status | 1UL);
}

UBaseType_t
vPortSetInterruptMask(void)
{
    UBaseType_t mask = (UBaseType_t)port_sreg_read(PORT_SREG_STATUS);

    port_sreg_write(PORT_SREG_STATUS, (uint32_t)mask & ~1UL);
    return mask;
}

void
vPortClearInterruptMask(UBaseType_t mask)
{
    port_sreg_write(PORT_SREG_STATUS, (uint32_t)mask);
}

void
vPortEnterCritical(void)
{
    vPortDisableInterrupts();
    uxCriticalNesting++;
}

void
vPortExitCritical(void)
{
    if (uxCriticalNesting != 0) {
        uxCriticalNesting--;
        if (uxCriticalNesting == 0) {
            vPortEnableInterrupts();
        }
    }
}

void
vPortYield(void)
{
    port_sreg_write(PORT_SREG_MIP, PORT_MIP_MSIP);
}

uint32_t
vPortExceptionC(uint32_t context)
{
    (void)context;

    if (port_sreg_read(PORT_SREG_CAUSE) == PORT_EXCEPTION_IRQ) {
        uint32_t value = port_sreg_read(PORT_SREG_VALUE);

        if (value == PORT_INTERRUPT_TIMER) {
            prvAdvanceTimer();
            if (xTaskIncrementTick() != pdFALSE) {
                vTaskSwitchContext();
            }
        } else if (value == PORT_INTERRUPT_SOFTWARE) {
            vTaskSwitchContext();
        }
    }

    return (uint32_t)(uintptr_t)*(StackType_t **)pxCurrentTCB;
}

static void
prvSetupTimerInterrupt(void)
{
    port_sreg_write(PORT_SREG_HANDLER, (uint32_t)(uintptr_t)vPortExceptionHandler);
    port_sreg_write(PORT_SREG_MIE, PORT_MIE_MTIE | PORT_MIE_MSIE);
    port_sreg_write(PORT_SREG_MSTATUS, PORT_MSTATUS_MIE);
    port_timecmp_write(port_time_read() + PORT_TIMER_INTERVAL_US);
}

BaseType_t
xPortStartScheduler(void)
{
    /*
     * 保存调度器启动时的主栈指针，异常处理程序使用该栈执行C代码。
     * 任务上下文仍保存在各自任务栈上。
     */
    ulMainStackPointer = port_get_sp();
    uxCriticalNesting = 0;
    prvSetupTimerInterrupt();
    vPortStartFirstTask();

    for (;;) {
    }
}

void
vPortEndScheduler(void)
{
    for (;;) {
    }
}

StackType_t *
pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    StackType_t *context = pxTopOfStack - 32;
    StackType_t *trampoline = context + 16;
    StackType_t *frame = trampoline + 3;
    size_t       i;

    context[0] = (StackType_t)(uintptr_t)pvParameters;
    for (i = 1; i < 16; i++) {
        context[i] = 0;
    }

    trampoline[0] = (StackType_t)(uintptr_t)frame;
    trampoline[1] = (StackType_t)(uintptr_t)pxCode;
    trampoline[2] = 0;

    frame[0] = 0;
    frame[1] = (StackType_t)(uintptr_t)prvTaskExitError;
    frame[2] = 0;

    return context;
}

/*
 * 异常处理程序：
 * - 上下文偏移60保存条件码cc（sreg18）
 * - 最终ret前将恢复任务的PC写入sreg5，
 *   内核据此在ret返回到EXCEPTION_PC时从MPIE恢复MIE
 */
__asm__(".section .text\n"
        ".global vPortExceptionHandler\n"
        ".type vPortExceptionHandler,@function\n"
        "vPortExceptionHandler:\n"
        "    dec $sp, 12\n"
        "    sto.l 0($sp), $fp\n"
        "    dec $sp, 64\n"
        "    sto.l 0($sp), $r0\n"
        "    sto.l 4($sp), $r1\n"
        "    sto.l 8($sp), $r2\n"
        "    sto.l 12($sp), $r3\n"
        "    sto.l 16($sp), $r4\n"
        "    sto.l 20($sp), $r5\n"
        "    sto.l 24($sp), $r6\n"
        "    sto.l 28($sp), $r7\n"
        "    sto.l 32($sp), $r8\n"
        "    sto.l 36($sp), $r9\n"
        "    sto.l 40($sp), $r10\n"
        "    sto.l 44($sp), $r11\n"
        "    sto.l 48($sp), $r12\n"
        "    sto.l 52($sp), $r13\n"
        "    sto.l 56($sp), $fp\n"
        "    gsr $r0, 5\n"
        "    sto.l 68($sp), $r0\n"
        "    gsr $r0, 18\n"
        "    sto.l 60($sp), $r0\n"
        "    ldi.l $r0, pxCurrentTCB\n"
        "    ld.l $r0, ($r0)\n"
        "    st.l ($r0), $sp\n"
        "    mov $r0, $sp\n"
        "    ldi.l $r1, ulMainStackPointer\n"
        "    ld.l $sp, ($r1)\n"
        "    jsra vPortExceptionC\n"
        "    mov $sp, $r0\n"
        "    ldo.l $fp, 60($sp)\n"
        "    ssr $fp, 18\n"
        "    ldo.l $r0, 0($sp)\n"
        "    ldo.l $r1, 4($sp)\n"
        "    ldo.l $r2, 8($sp)\n"
        "    ldo.l $r3, 12($sp)\n"
        "    ldo.l $r4, 16($sp)\n"
        "    ldo.l $r5, 20($sp)\n"
        "    ldo.l $r6, 24($sp)\n"
        "    ldo.l $r7, 28($sp)\n"
        "    ldo.l $r8, 32($sp)\n"
        "    ldo.l $r9, 36($sp)\n"
        "    ldo.l $r10, 40($sp)\n"
        "    ldo.l $r11, 44($sp)\n"
        "    ldo.l $r12, 48($sp)\n"
        "    ldo.l $r13, 52($sp)\n"
        "    gsr $fp, 0\n"
        "    inc $fp, 1\n"
        "    ssr $fp, 0\n"
        "    mov $fp, $sp\n"
        "    inc $fp, 64\n"
        "    ldo.l $r0, 4($fp)\n"
        "    ssr $r0, 5\n"
        "    ldo.l $r0, 0($sp)\n"
        "    ret\n"
        ".size vPortExceptionHandler, .-vPortExceptionHandler\n"
        "\n"
        ".global vPortStartFirstTask\n"
        ".type vPortStartFirstTask,@function\n"
        "vPortStartFirstTask:\n"
        "    ldi.l $r0, pxCurrentTCB\n"
        "    ld.l $r0, ($r0)\n"
        "    ld.l $sp, ($r0)\n"
        "    ldo.l $fp, 60($sp)\n"
        "    ssr $fp, 18\n"
        "    ldo.l $r0, 0($sp)\n"
        "    ldo.l $r1, 4($sp)\n"
        "    ldo.l $r2, 8($sp)\n"
        "    ldo.l $r3, 12($sp)\n"
        "    ldo.l $r4, 16($sp)\n"
        "    ldo.l $r5, 20($sp)\n"
        "    ldo.l $r6, 24($sp)\n"
        "    ldo.l $r7, 28($sp)\n"
        "    ldo.l $r8, 32($sp)\n"
        "    ldo.l $r9, 36($sp)\n"
        "    ldo.l $r10, 40($sp)\n"
        "    ldo.l $r11, 44($sp)\n"
        "    ldo.l $r12, 48($sp)\n"
        "    ldo.l $r13, 52($sp)\n"
        "    gsr $fp, 0\n"
        "    inc $fp, 1\n"
        "    ssr $fp, 0\n"
        "    mov $fp, $sp\n"
        "    inc $fp, 64\n"
        "    ret\n"
        ".size vPortStartFirstTask, .-vPortStartFirstTask\n"
        "\n"
        ".global prvTaskExitError\n"
        ".type prvTaskExitError,@function\n"
        "prvTaskExitError:\n"
        "    gsr $r0, 0\n"
        "    ldi.l $r1, 0xFFFFFFFE\n"
        "    and $r0, $r1\n"
        "    ssr $r0, 0\n"
        ".LprvTaskExitError:\n"
        "    jmpa .LprvTaskExitError\n"
        ".size prvTaskExitError, .-prvTaskExitError\n"
        ".text\n");
