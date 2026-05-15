#ifndef __OPENSBI_PORT_H__
#define __OPENSBI_PORT_H__
#include "hbox.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include H3RDPARTY_LIBFDT_HEADER

typedef struct sbi_entry_para sbi_entry_para_t;
struct sbi_entry_para
{
    uintptr_t a0;
    uintptr_t a1;   /**< 通常是设备树地址 */
};

extern sbi_entry_para_t sbi_entry_para_data;

void sbi_ecall_putc(char ch);
int  sbi_ecall_getc(void);
void sbi_ecall_console_puts(const char *str);
void sbi_ecall_shutdown(void);
void sbi_ecall_reboot(void);

size_t sbi_rdtime(void);

#ifdef __cplusplus
}
#endif
#endif // __OPENSBI_PORT_H__
