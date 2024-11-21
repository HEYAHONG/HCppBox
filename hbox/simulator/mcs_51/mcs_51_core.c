/***************************************************************
 * Name:      mcs_51_core.c
 * Purpose:   实现mcs_51_core接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

struct hs_mcs_51_core
{
    hs_mcs_51_io_t io;
    void *usr;
    struct
    {
        uint32_t delay_tick:2;//MCS-51具有多周期指令。为保证执行效果，对于多周期指令需要延时,最多延时3周期。
        uint32_t interrupt_nested:2;//中断嵌套层数，0=正常运行
        uint32_t pc:16;//PC
        uint32_t interrupt_low_priority_scan_table;//中断(低优先级)扫描表，位0表示中断0，最高支持32个中断
        uint32_t interrupt_high_priority_scan_table;//中断(高优先级)扫描表，位0表示中断0，最高支持32个中断
    };
};

size_t hs_mcs_51_core_size(void)
{
    return sizeof(hs_mcs_51_core_t);
}

hs_mcs_51_core_t *hs_mcs_51_core_init(void *mem,hs_mcs_51_io_t io,void *usr)
{
    if(mem!=NULL)
    {
        hs_mcs_51_core_t *core=(hs_mcs_51_core_t *)mem;
        core->io=io;
        core->usr=usr;
        hs_mcs_51_core_reset(core);
        return core;
    }
    return NULL;
}

static void hs_mcs_51_core_scan_interrupt(hs_mcs_51_core_t * core)
{
    if(core != NULL)
    {
        if(core->interrupt_nested < 2)
        {
            if(core->interrupt_high_priority_scan_table!=0)
            {
                for(uint8_t i=0; i<32; i++)
                {
                    if((core->interrupt_high_priority_scan_table&(1ULL<<i))!=0)
                    {
                        core->interrupt_high_priority_scan_table &= (~(1ULL<<i));
                        uint16_t address=3+8*i;
                        core->interrupt_nested++;//增加中断嵌套，RETI指令时自减1
                        {
                            //TODO:保存现场,执行中断
                        }
                        return;
                    }
                }
            }
        }
        if(core->interrupt_nested == 0)
        {
            if(core->interrupt_low_priority_scan_table!=0)
            {
                for(uint8_t i=0; i<32; i++)
                {
                    if((core->interrupt_low_priority_scan_table&(1ULL<<i))!=0)
                    {
                        core->interrupt_low_priority_scan_table &= (~(1ULL<<i));
                        uint16_t address=3+8*i;
                        core->interrupt_nested++;//增加中断嵌套，RETI指令时自减1
                        {
                            //TODO:保存现场,执行中断
                        }
                        return;
                    }
                }
            }
        }
    }
}

void hs_mcs_51_core_tick(hs_mcs_51_core_t * core,size_t cycles)
{
    if(core!=NULL)
    {
        while((cycles--)!=0)
        {
            if(core->delay_tick!=0)
            {
                core->delay_tick--;
                continue;
            }

            hs_mcs_51_core_scan_interrupt(core);

            //TODO:执行指令
        }
    }
}

void hs_mcs_51_core_reset(hs_mcs_51_core_t * core)
{
    if(core!=NULL)
    {
        hs_mcs_51_io_t io=core->io;
        void *usr=core->usr;
        memset(core,0,sizeof(hs_mcs_51_core_t));
        core->io=io;
        core->usr=usr;
        if(core->io!=NULL)
        {
            uint8_t dummy=0;
            core->io(core,HS_MCS_51_IO_RESET,0,&dummy,sizeof(dummy),core->usr);
        }
    }
}

void hs_mcs_51_core_interrupt_set(hs_mcs_51_core_t * core,hs_mcs_51_interrupt_number_t number,bool is_high_priority)
{
    if(core!=NULL)
    {
        if(is_high_priority)
        {
            core->interrupt_high_priority_scan_table|=(1ULL << ((size_t)number));
        }
        else
        {
            core->interrupt_low_priority_scan_table|=(1ULL << ((size_t)number));
        }
    }
}
