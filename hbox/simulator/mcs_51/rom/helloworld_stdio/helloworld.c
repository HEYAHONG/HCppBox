#include "stdio.h"
#include "stdint.h"
#include "8051.h"


/*
 *	定义PSBANK的SFR，用于兼容model-huge,此变量用于SDCC的C运行库crtbank.asm。
 */
__sfr __at (0xB1) PSBANK;


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
    uint8_t data=SBUF;
    RI=0;
    return data;
}

void uart_send_str(char *str)
{
    while(*str!='\0')
    {
        uart_send_char(*(str++));
    }
}

/*
 * 使用printf需要移植putchar
 */
int putchar(int ch)
{
    uart_send_char(ch);
    return ch;
}

void main(void)
{
    uart_init();
    printf("Hello World!\n");
    {
        //打印9x9乘法表
        printf("9x9 multiplication table:\n");
        for(uint8_t i=1; i<=9; i++)
        {
            for(uint8_t j=1; j<=i; j++)
            {
                printf("%d*%d=%02d ",i,j,i*j);
            }
            printf("\n");
        }
    }
    while(1)
    {
        int data=uart_receive_char();
        uart_send_char(data);
    }
}

