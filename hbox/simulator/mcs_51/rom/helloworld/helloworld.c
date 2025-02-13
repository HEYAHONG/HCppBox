#include "stdio.h"
#include "stdint.h"
#include "8051.h"

//外部中断0
void ie0_isr(void) __interrupt(IE0_VECTOR)
{

}

//定时器0
void tf0_isr(void) __interrupt(TF0_VECTOR)
{

}

//外部中断1
void ie1_isr(void) __interrupt(IE1_VECTOR)
{

}

//定时器1
void tf1_isr(void) __interrupt(TF1_VECTOR)
{

}

//串口0
void si0_isr(void) __interrupt(SI0_VECTOR)
{

}




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

uint8_t uart_receive_char(void)
{
    /*
     * 注意：此代码要求实现串口功能，即接收数据时RI需要被硬件（模拟器）置1。
     */
    while(RI==0);
    RI=0;
    return SBUF;
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
    while(1)
    {
        int data=uart_receive_char();
        uart_send_char(data);
    }
}

