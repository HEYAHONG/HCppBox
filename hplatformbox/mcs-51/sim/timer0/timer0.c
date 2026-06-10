#include "stdio.h"
#include "stdint.h"
#include "8051.h"


/*
 *  定义PSBANK的SFR，用于兼容model-huge,此变量用于SDCC的C运行库crtbank.asm。
 */
__sfr __at (0xB1) PSBANK;


//外部中断0
void ie0_isr(void) __interrupt(IE0_VECTOR)
{

}

//定时器0
static unsigned long int count=0;
void tf0_isr(void) __interrupt(TF0_VECTOR)
{
    count++;
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
 * Timer0 模式2 中断,需要模拟器实现定时器功能
 */
#define timer0_cnt 10
#define timer0_hz  1000
void timer0_init(void)
{
    TMOD |= 0x02;
    TH0   = (unsigned char)((unsigned)0x100-timer0_cnt);
    TL0   = TH0;
    ET0   = 1;
    EA    = 1;
    TR0 = 1;
}

void timer0_event_process(void)
{
    static unsigned  long int last_count=0;
    ET0=0;
    unsigned long int temp=count-last_count;
    ET0=1;
    if(temp > (timer0_hz/timer0_cnt))
    {
        ET0 = 0;
        last_count=count;
        ET0 = 1;
        uart_send_str("second reached!\n");
    }
}

void main(void)
{
    uart_init();
    uart_send_str("timer0 init!\n");
    timer0_init();
    while(1)
    {
        timer0_event_process();
    }
}

