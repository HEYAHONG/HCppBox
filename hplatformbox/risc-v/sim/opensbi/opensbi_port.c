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
    size_t len=0;
    if(str!=NULL)
    {
        const char *str_ptr=str;
        while((*str_ptr++)!='\0')
        {
            len++;
        }
    }
    sbi_ecall(SBI_EXT_DBCN, SBI_EXT_DBCN_CONSOLE_WRITE,len, (unsigned long)str, 0, 0, 0, 0);
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

#if __riscv_xlen == 64
#define opensbi_entry_save_context() \
asm volatile (\
"addi sp, sp, -8*32\n"\
"sd x1,  1  * 8( sp )\n"\
"sd x2,  2  * 8( sp )\n"\
"sd x3,  3  * 8( sp )\n"\
"sd x4,  4  * 8( sp )\n"\
"sd x5,  5  * 8( sp )\n"\
"sd x6,  6  * 8( sp )\n"\
"sd x7,  7  * 8( sp )\n"\
"sd x8,  8  * 8( sp )\n"\
"sd x9,  9  * 8( sp )\n"\
"sd x10, 10  * 8( sp )\n"\
"sd x11, 11  * 8( sp )\n"\
"sd x12, 12 * 8( sp )\n"\
"sd x13, 13 * 8( sp )\n"\
"sd x14, 14 * 8( sp )\n"\
"sd x15, 15 * 8( sp )\n"\
"sd x16, 16 * 8( sp )\n"\
"sd x17, 17 * 8( sp )\n"\
"sd x18, 18 * 8( sp )\n"\
"sd x19, 19 * 8( sp )\n"\
"sd x20, 20 * 8( sp )\n"\
"sd x21, 21 * 8( sp )\n"\
"sd x22, 22 * 8( sp )\n"\
"sd x23, 23 * 8( sp )\n"\
"sd x24, 24 * 8( sp )\n"\
"sd x25, 25 * 8( sp )\n"\
"sd x26, 26 * 8( sp )\n"\
"sd x27, 27 * 8( sp )\n"\
"sd x28, 28 * 8( sp )\n"\
"sd x29, 29 * 8( sp )\n"\
"sd x30, 30 * 8( sp )\n"\
"sd x31, 31 * 8( sp )\n"\
:::)
#define opensbi_entry_restore_context() \
asm volatile (\
"ld x1,  1  * 8( sp )\n"\
"ld x2,  2  * 8( sp )\n"\
"ld x3,  3  * 8( sp )\n"\
"ld x4,  4  * 8( sp )\n"\
"ld x5,  5  * 8( sp )\n"\
"ld x6,  6  * 8( sp )\n"\
"ld x7,  7  * 8( sp )\n"\
"ld x8,  8  * 8( sp )\n"\
"ld x9,  9  * 8( sp )\n"\
"ld x10, 10  * 8( sp )\n"\
"ld x11, 11  * 8( sp )\n"\
"ld x12, 12 * 8( sp )\n"\
"ld x13, 13 * 8( sp )\n"\
"ld x14, 14 * 8( sp )\n"\
"ld x15, 15 * 8( sp )\n"\
"ld x16, 16 * 8( sp )\n"\
"ld x17, 17 * 8( sp )\n"\
"ld x18, 18 * 8( sp )\n"\
"ld x19, 19 * 8( sp )\n"\
"ld x20, 20 * 8( sp )\n"\
"ld x21, 21 * 8( sp )\n"\
"ld x22, 22 * 8( sp )\n"\
"ld x23, 23 * 8( sp )\n"\
"ld x24, 24 * 8( sp )\n"\
"ld x25, 25 * 8( sp )\n"\
"ld x26, 26 * 8( sp )\n"\
"ld x27, 27 * 8( sp )\n"\
"ld x28, 28 * 8( sp )\n"\
"ld x29, 29 * 8( sp )\n"\
"ld x30, 30 * 8( sp )\n"\
"ld x31, 31 * 8( sp )\n"\
"addi sp, sp, 8*32\n"\
:::)
#else
#define opensbi_entry_save_context() \
asm volatile (\
"addi sp, sp, -4*32\n"\
"sw x1,  1  * 4( sp )\n"\
"sw x2,  2  * 4( sp )\n"\
"sw x3,  3  * 4( sp )\n"\
"sw x4,  4  * 4( sp )\n"\
"sw x5,  5  * 4( sp )\n"\
"sw x6,  6  * 4( sp )\n"\
"sw x7,  7  * 4( sp )\n"\
"sw x8,  8  * 4( sp )\n"\
"sw x9,  9  * 4( sp )\n"\
"sw x10, 10  * 4( sp )\n"\
"sw x11, 11  * 4( sp )\n"\
"sw x12, 12 * 4( sp )\n"\
"sw x13, 13 * 4( sp )\n"\
"sw x14, 14 * 4( sp )\n"\
"sw x15, 15 * 4( sp )\n"\
"sw x16, 16 * 4( sp )\n"\
"sw x17, 17 * 4( sp )\n"\
"sw x18, 18 * 4( sp )\n"\
"sw x19, 19 * 4( sp )\n"\
"sw x20, 20 * 4( sp )\n"\
"sw x21, 21 * 4( sp )\n"\
"sw x22, 22 * 4( sp )\n"\
"sw x23, 23 * 4( sp )\n"\
"sw x24, 24 * 4( sp )\n"\
"sw x25, 25 * 4( sp )\n"\
"sw x26, 26 * 4( sp )\n"\
"sw x27, 27 * 4( sp )\n"\
"sw x28, 28 * 4( sp )\n"\
"sw x29, 29 * 4( sp )\n"\
"sw x30, 30 * 4( sp )\n"\
"sw x31, 31 * 4( sp )\n"\
:::)
#define opensbi_entry_restore_context() \
asm volatile (\
"lw x1,  1  * 4( sp )\n"\
"lw x2,  2  * 4( sp )\n"\
"lw x3,  3  * 4( sp )\n"\
"lw x4,  4  * 4( sp )\n"\
"lw x5,  5  * 4( sp )\n"\
"lw x6,  6  * 4( sp )\n"\
"lw x7,  7  * 4( sp )\n"\
"lw x8,  8  * 4( sp )\n"\
"lw x9,  9  * 4( sp )\n"\
"lw x10, 10  * 4( sp )\n"\
"lw x11, 11  * 4( sp )\n"\
"lw x12, 12 * 4( sp )\n"\
"lw x13, 13 * 4( sp )\n"\
"lw x14, 14 * 4( sp )\n"\
"lw x15, 15 * 4( sp )\n"\
"lw x16, 16 * 4( sp )\n"\
"lw x17, 17 * 4( sp )\n"\
"lw x18, 18 * 4( sp )\n"\
"lw x19, 19 * 4( sp )\n"\
"lw x20, 20 * 4( sp )\n"\
"lw x21, 21 * 4( sp )\n"\
"lw x22, 22 * 4( sp )\n"\
"lw x23, 23 * 4( sp )\n"\
"lw x24, 24 * 4( sp )\n"\
"lw x25, 25 * 4( sp )\n"\
"lw x26, 26 * 4( sp )\n"\
"lw x27, 27 * 4( sp )\n"\
"lw x28, 28 * 4( sp )\n"\
"lw x29, 29 * 4( sp )\n"\
"lw x30, 30 * 4( sp )\n"\
"lw x31, 31 * 4( sp )\n"\
"addi sp, sp, 4*32\n"\
:::)
#endif



/*
 * opensbi的入口，放在首地址
 */
__SECTION(".vector")  __attribute__((naked))  void opensbi_entry() ;
__SECTION(".vector")  __attribute__((naked))  void opensbi_tray_entry() ;
__NO_INIT sbi_entry_para_t sbi_entry_para_data;
void _start();
void opensbi_entry()
{
    /*
     * 保存入口参数
     */
    {
        register size_t a0 asm ("a0");
        register size_t a1 asm ("a1");
        sbi_entry_para_data.a0=a0;
        sbi_entry_para_data.a1=a1;
    }

    /*
     * 设置初始SP指针
     */
    asm volatile ("la sp,__stack");

    /*
     * 设置stvec
     */
    asm volatile ("csrw stvec,%0"::"r"(&opensbi_tray_entry));

    /*
     * 进入入口`
     */
    sbi_ecall_console_puts("firmware entry enter!\r\n");


    /*
     * 启动C语言入口
     */
    _start();

    /*
     * 固件退出（一般不会到这一步）
     */
    sbi_ecall_console_puts("firmware entry leave!\r\n");
}

void opensbi_tray_entry()
{
    /*
     * 保存上下文
     */
    opensbi_entry_save_context();
    {
        size_t scause=0;
        size_t stval=0;
        size_t sepc=0;
        size_t sstatus=0;
        asm volatile ("csrr %0,scause":"+r"(scause)::"memory");
        asm volatile ("csrr %0,stval":"+r"(stval)::"memory");
        asm volatile ("csrr %0,sepc":"+r"(sepc)::"memory");
        asm volatile ("csrr %0,sstatus":"+r"(sstatus)::"memory");
        if(scause!=0 || (scause==0 && stval!=0))
        {
            if(scause > (1ULL << (8*sizeof(size_t)-1)))
            {
                /*
                 * 处理中断
                 */
                switch(scause&0xFFFF)
                {
                case 1:
                {
                    /*
                     * 恢复上下文并跳转至freertos中断处理函数,采用软件中断切换任务(原版采用ecall指令)
                     */
                    opensbi_entry_restore_context();
                    asm volatile("j freertos_risc_v_software_interrupt_handler");
                }
                break;
                case 5:
                {
                    /*
                     * 恢复上下文并跳转至freertos中断处理函数
                     */
                    opensbi_entry_restore_context();
                    asm volatile("j freertos_risc_v_mtimer_interrupt_handler");
                }
                break;
                default:
                {
                    /*
                     * 恢复上下文并跳转至freertos中断处理函数
                     */
                    opensbi_entry_restore_context();
                    asm volatile("j freertos_risc_v_interrupt_handler");
                }
                break;
                }
            }
            else
            {
                //异常
                char val[256]= {0};
                itoa(scause,val,10);
                sbi_ecall_console_puts("exception=");
                sbi_ecall_console_puts(val);
                itoa(sepc,val,16);
                sbi_ecall_console_puts(",sepc=");
                sbi_ecall_console_puts(val);
                itoa(sstatus,val,16);
                sbi_ecall_console_puts(",status=");
                sbi_ecall_console_puts(val);
                sbi_ecall_console_puts("\r\n");
                while(true);
            }
            /*
             * 恢复上下文并采用sret返回
             */
            opensbi_entry_restore_context();
            asm volatile ("sret");
        }
    }

    /*
     * 恢复上下文
     */
    opensbi_entry_restore_context();
    asm volatile ("sret");
}

extern void vPortSetupTimerInterrupt( void );
extern const size_t uxTimerIncrementsForOneTick;
void vPortSetupTimerInterrupt( void )
{
    uint64_t ullNextTime=sbi_rdtime()+uxTimerIncrementsForOneTick;
    asm volatile ( "csrs sie, %0" ::"r" ( 0x22 ) :"memory"); //打开定时中断与软件中断
    asm volatile ( "csrs sstatus, %0" ::"r" ( 0x2 ) :"memory");//打开SIE
#if (__riscv_xlen) == 32
    {
        size_t reg_val=0;
        asm volatile ( "csrr %0,stimecmph":"+r"(reg_val)::"memory");
        reg_val=(ullNextTime>>32);
        asm volatile ( "csrw stimecmph,%0"::"r"(reg_val):"memory");
    }
#endif
    {
        size_t reg_val=0;
        asm volatile ( "csrr %0,stimecmp":"+r"(reg_val)::"memory");
        reg_val=ullNextTime;
        asm volatile ( "csrw stimecmp,%0"::"r"(reg_val):"memory");
    }
}
