#include "hbox.h"
#include "stdint.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_ssi_handler();
__WEAK __attribute__((interrupt("machine"))) void vector_table_msi_handler();
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_sti_handler();
__WEAK __attribute__((interrupt("machine"))) void vector_table_mti_handler();
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_sei_handler();
__WEAK __attribute__((interrupt("machine"))) void vector_table_mei_handler();
__WEAK __attribute__((interrupt)) void vector_table_default_handler();
__WEAK __attribute__((interrupt)) void vector_table_trap();
__WEAK  __attribute__((naked))  void vector_table_reset_handler();
extern const uint32_t vector_table[256-64] __SECTION(".vector.usr");
#ifdef __cplusplus
}
#endif // __cplusplus


/*
 * RISC-V标准中断,默认情况下什么也不做，用户需要重写相应的实现
 */
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_ssi_handler()
{

}
__WEAK __attribute__((interrupt("machine"))) void vector_table_msi_handler()
{

}
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_sti_handler()
{

}
__WEAK __attribute__((interrupt("machine"))) void vector_table_mti_handler()
{

}
__WEAK __attribute__((interrupt("supervisor"))) void vector_table_sei_handler()
{

}
__WEAK __attribute__((interrupt("machine"))) void vector_table_mei_handler()
{

}


/*
 * 用户参数向量
 */
const uint32_t vector_table[256-64] __SECTION(".vector.usr") =
{
    0
};

/*
 * 默认处理函数
 */
__WEAK  __attribute__((interrupt))  void vector_table_default_handler()
{
    while(true);
}

/*
 * 陷入函数，当中断模式为向量时用于处理异常
 */
__WEAK  __attribute__((interrupt))  void vector_table_trap()
{
    while(true);
}

/*
 * 复位中断处理
 */
extern "C"  void _start();
__WEAK  __attribute__((naked)) void vector_table_reset_handler()
{
    _start();
    while(true);
}

