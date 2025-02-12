#include "stdio.h"
#include "stdint.h"
#include "8051.h"

void uart_init(void)
{
    SCON=0x90;
}

void uart_send_char(int ch)
{
    SBUF=(uint8_t)ch;
    /*
     * 注意：此代码要求实现串口功能，即发送完后TI需要被硬件（模拟器）置1。
     */
    while(TI==0);
    TI=0;
}

void uart_send_str(char *str)
{
    while(*str!='\0')
    {
        uart_send_char(*(str++));
    }
}

void main(void)
{
    uart_init();
    uart_send_str("Hello World!\n");
    while(1);
}
