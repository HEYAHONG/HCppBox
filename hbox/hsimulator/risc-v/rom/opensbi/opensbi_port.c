#include "opensbi_port.h"
#include "hbox.h"
#include <sbi/sbi_ecall_interface.h>


typedef struct sbiret
{
    unsigned long error;
    unsigned long value;
} sbiret_t;

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,unsigned long arg1, unsigned long arg2,unsigned long arg3, unsigned long arg4,unsigned long arg5)
{
    struct sbiret ret= {0};;

    register size_t a0 asm ("a0") = (unsigned long)(arg0);
    register size_t a1 asm ("a1") = (unsigned long)(arg1);
    register size_t a2 asm ("a2") = (unsigned long)(arg2);
    register size_t a3 asm ("a3") = (unsigned long)(arg3);
    register size_t a4 asm ("a4") = (unsigned long)(arg4);
    register size_t a5 asm ("a5") = (unsigned long)(arg5);
    register size_t a6 asm ("a6") = (unsigned long)(fid);
    register size_t a7 asm ("a7") = (unsigned long)(ext);
    asm volatile ("ecall"
                  : "+r" (a0), "+r" (a1)
                  : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
                  : "memory");
    ret.error = a0;
    ret.value = a1;

    return ret;
}

void sbi_ecall_putc(char ch)
{
    sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0,(uint8_t)ch, 0, 0, 0, 0, 0);
}


int  sbi_ecall_getc(void)
{
    return sbi_ecall(SBI_EXT_0_1_CONSOLE_GETCHAR, 0,0, 0, 0, 0, 0, 0).error;
}

void sbi_ecall_console_puts(const char *str)
{
    sbi_ecall(SBI_EXT_DBCN, SBI_EXT_DBCN_CONSOLE_WRITE,strlen(str), (unsigned long)str, 0, 0, 0, 0);
}

void sbi_ecall_shutdown(void)
{
    sbi_ecall(SBI_EXT_SRST, SBI_EXT_SRST_RESET,SBI_SRST_RESET_TYPE_SHUTDOWN, SBI_SRST_RESET_REASON_NONE, 0, 0, 0, 0);
}
void sbi_ecall_reboot(void)
{
    sbi_ecall(SBI_EXT_SRST, SBI_EXT_SRST_RESET,SBI_SRST_RESET_TYPE_COLD_REBOOT, SBI_SRST_RESET_REASON_NONE, 0, 0, 0, 0);
}


size_t sbi_rdtime(void)
{
    size_t ret;
    asm volatile ("rdtime %0":"+r"(ret)::"memory");
    return ret;
}

/*
 * opensbi的入口，放在首地址
 */
__SECTION(".vector")  __attribute__((naked))  void opensbi_entry(uintptr_t a0, uintptr_t a1) ;
__NO_INIT sbi_entry_para_t sbi_entry_para_data;
void _start();
void opensbi_entry(uintptr_t a0, uintptr_t a1)
{

    {
        /*
         * 此入口同时也是S-mode下的陷入(trap)地址（opensbi默认会设置为直接模式）
         */
        size_t scause=0;
        asm volatile ("csrr %0,scause":"+r"(scause)::"memory");
        if(scause!=0)
        {
            if(scause > (1ULL << (8*sizeof(size_t)-1)))
            {
                //中断
            }
            else
            {
                //异常
                char code[256]={0};
                itoa(scause,code,10);
                sbi_ecall_console_puts("exception=");
                sbi_ecall_console_puts(code);
                sbi_ecall_console_puts("\r\n");
                while(true);
            }
            /*
             * 采用sret返回
             */
            asm volatile ("sret");
        }

    }

    /*
     * 设置初始SP指针
     */
    asm volatile ("la sp,__stack");

    /*
     *
     */
    sbi_ecall_console_puts("firmware entry enter!\r\n");

    /*
     * 保存入口参数
     */
    sbi_entry_para_data.a0=a0;
    sbi_entry_para_data.a1=a1;

    /*
     * 启动C语言入口
     */
    _start();

    /*
     * 固件退出（一般不会到这一步）
     */
    sbi_ecall_console_puts("firmware entry leave!\r\n");
}


