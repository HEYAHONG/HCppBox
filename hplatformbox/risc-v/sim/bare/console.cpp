#include "console.h"

int console_has_data(void)
{
    uint32_t status=0;
    __asm__ volatile("csrr %0, %1":"=r"(status):"i"(HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS));
    return (status&CONSOLE_STATUS_INPUT_AVAILABLE)!=0;
}

int console_getchar(void)
{
    uint32_t data=0;
    if(!console_has_data())
    {
        return -1;
    }
    __asm__ volatile("csrr %0, %1":"=r"(data):"i"(HS_RISC_V_MACHINE_CSR_CONSOLE_IN));
    return (int)(data&0xFF);
}

int console_putchar(uint8_t ch)
{
    __asm__ volatile("csrw %0, %1"::"i"(HS_RISC_V_MACHINE_CSR_CONSOLE_OUT),"r"((uint32_t)ch));
    return ch;
}
