#include "hbox.h"
#include "stdint.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
__WEAK __SECTION(".vector.default_handler") __attribute__((interrupt)) void vector_table_default_handler();
__WEAK __SECTION(".vector.trap") __attribute__((interrupt)) void vector_table_trap();
__WEAK __SECTION(".vector.reset_handler") __attribute__((naked))  void vector_table_reset_handler();
extern const uint32_t vector_table[256] __SECTION(".vector");
#ifdef __cplusplus
}
#endif // __cplusplus

/*
 * 向量表.定义中断或者库函数的地址向量
 */
const uint32_t vector_table[256] __SECTION(".vector") =
{
    /*
     * 前64个向量为中断向量,前16个为RISV-V标准保留
     */
    (uint32_t)(uintptr_t)vector_table_trap,                          //陷入函数
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_reset_handler,                     //复位中断
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    (uint32_t)(uintptr_t)vector_table_default_handler,
    /*
     * 向量表后为库函数向量,默认为空
     */

};

/*
 * 默认处理函数
 */
__WEAK __SECTION(".vector.default_handler") __attribute__((interrupt))  void vector_table_default_handler()
{
    while(true);
}

/*
 * 陷入函数，当中断模式为向量时用于处理异常
 */
__WEAK __SECTION(".vector.trap") __attribute__((interrupt))  void vector_table_trap()
{
    while(true);
}

/*
 * 复位中断处理
 */
extern "C"  void _start();
__WEAK __SECTION(".vector.reset_handler")  __attribute__((naked)) void vector_table_reset_handler()
{
    _start();
    while(true);
}

