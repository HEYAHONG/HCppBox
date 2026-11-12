#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

/*
 * 控制台CSR地址可由用户统一修改
 */
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_OUT
#define HS_RISC_V_MACHINE_CSR_CONSOLE_OUT      0x800
#endif
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_IN
#define HS_RISC_V_MACHINE_CSR_CONSOLE_IN       0x801
#endif
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS
#define HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS   0x802
#endif

/*
 * 控制台状态位
 */
#define CONSOLE_STATUS_INPUT_AVAILABLE   0x01
#define CONSOLE_STATUS_OUTPUT_READY     0x02

/*
 * 输出一个字节到控制台
 */
int console_putchar(uint8_t ch);

/*
 * 从控制台读取一个字节，无数据时返回-1
 */
int console_getchar(void);

/*
 * 查询是否有输入数据
 */
int console_has_data(void);

#ifdef __cplusplus
}
#endif

#endif // __CONSOLE_H__
