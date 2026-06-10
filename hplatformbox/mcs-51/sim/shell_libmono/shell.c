#include "stdio.h"
#include "stdint.h"
#include "8051.h"


#define LIBMONO_RUNTIME_LIBC_STDIO_SERIAL 1

#define LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME tasks

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
#define timer0_hz  1000
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
    while(1)
    {
        libmono_runtime_mainloop_process();
    }
}


static void maintask_entry(struct libmono_runtime_mainloop_task_context *ctx)
{
    uint16_t events=libmono_runtime_mainloop_task_event(ctx);
    if((events&LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_INIT)!=0)
    {
        libmono_runtime_mainloop_task_event_clear(ctx,LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_INIT);
        libmono_runtime_mainloop_task_event_set(ctx,LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_SERVICE_RUN);
    }
    if((events&LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_SERVICE_RUN)!=0)
    {
        int ch=getchar();
        libmono_shell_process(ch);
    }
}

libmono_runtime_mainloop_task_context_t maintask_context=
{
    maintask_entry,
    0
};

libmono_runtime_mainloop_task_context_t * const tasks[]=
{
    &maintask_context,
    NULL
};
