#include "stdio.h"
#include "stdint.h"
#include "8051.h"


#define LIBMONO_RUNTIME_LIBC_STDIO_SERIAL 1

#include "../../lib/libmono.c"


/*
 *  定义PSBANK的SFR，用于兼容model-huge,此变量用于SDCC的C运行库crtbank.asm。
 */
__sfr __at (0xB1) PSBANK;


//外部中断0
void ie0_isr(void) __interrupt(IE0_VECTOR)
{

}

//定时器0
/*
 * 定时器0频率
 */
#define timer0_hz  1000000
/*
 * 定时器0计数值,MCS-51（模拟器）速率一般不高，逻辑复杂后若定时器中断太频繁会导致丢中断，采用10ms中断比较合适
 */
#define timer0_cnt (timer0_hz/1000*10)
static uint16_t timer0_count=0;
void tf0_isr(void) __interrupt(TF0_VECTOR)
{
    TR0=0;
    TH0=(0x10000-timer0_cnt)/256;
    TL0=(0x10000-timer0_cnt)%256;
    TR0=1;
    libmono_runtime_tick_inc(1000/(timer0_hz/timer0_cnt));
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
    /*
     * 注意：此代码要求实现串口功能，即发送完后TI需要被硬件（模拟器）置1。
     */
    putchar(ch);
}

uint8_t uart_receive_char(void)
{
    /*
     * 注意：此代码要求实现串口功能，即接收数据时RI需要被硬件（模拟器）置1。
     */
    int data=-1;
    while((data=getchar())<0)
    {

    }
    return (uint8_t)data;
}

void uart_send_str(char *str)
{
    while(*str!='\0')
    {
        uart_send_char(*(str++));
    }
}



/*
 * Timer0 模式1 中断,需要模拟器实现定时器功能
 */
void timer0_init(void)
{
    TMOD |= 0x01;
    TH0 = (0x10000-timer0_cnt)/256;
    TL0 = (0x10000-timer0_cnt)%256;
    ET0 = 1;
    TR0 = 1;
}


void main(void)
{
    timer0_init();
    uart_init();
    libmono_base_core_enable_interrupt();
    uart_send_str("Hello World!\n");
    {
        //打印9x9乘法表
        uart_send_str("9x9 multiplication table:\n");
        for(uint8_t i=1; i<=9; i++)
        {
            for(uint8_t j=1; j<=i; j++)
            {
                uart_send_char(i+'0');
                uart_send_char('*');
                uart_send_char(j+'0');
                uart_send_char('=');
                if(i*j >= 10)
                {
                    uart_send_char('0'+i*j/10);
                }
                uart_send_char('0'+i*j%10);
                if(i*j < 10)
                {
                    uart_send_char(' ');
                }
                uart_send_char(' ');
            }
            uart_send_char('\n');
        }
    }
    while(1)
    {
        int data=uart_receive_char();
        uart_send_char(data);
    }
}

