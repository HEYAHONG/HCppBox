#ifndef __HBOX_CONFIG_H__
#define __HBOX_CONFIG_H__

/*
 * 启用FreeRTOS内核
 */
#define FREERTOS_KERNEL 1
#define FREERTOS_KERNEL_MEMMANG_HEAP 4

/*
 * 不使用原子支持
 */
#define HCPPRT_NO_ATOMIC        1

/*
 * 启用C++初始化
 */
#define HCPPRT_USE_CTORS       1


/*
 * 启用初始化段
 */
#define HRUNTIME_USING_INIT_SECTION 1

/*
 * 启用循环段
 */
#define HRUNTIME_USING_LOOP_SECTION 1

/*
 * 启用符号段
 */
#define HRUNTIME_USING_SYMBOL_SECTION 1

/*
 * 重新定义putchar
 */
#define HPUTCHAR hbox_putchar

/*
 * 重新定义getchar
 */
#define HGETCHAR hbox_getchar

#endif // __HBOX_CONFIG_H__
