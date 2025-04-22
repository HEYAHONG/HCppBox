/***************************************************************
 * Name:      mcs_51_timer.c
 * Purpose:   实现mcs_51_timer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_timer.h"

void hs_mcs_51_timer_init(hs_mcs_51_timer_t *timer)
{
    if(timer!=NULL)
    {
        timer->clk_freq=HS_MCS_51_COMMON_CLK_FREQ/12;
        memset(&timer->flag,0,sizeof(timer->flag));
    }
}

static void hs_mcs_51_timer_timer_counter_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer,bool counter0,bool counter1,bool counter2)
{
    bool is_timer_clk=(!(counter0||counter1 || counter2));
    uint8_t tmod=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TMOD,&tmod);
    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TR0))
    {
        //定时器0已使能
        uint8_t modcfg=(tmod&0xF);
        bool    gate=((modcfg&0x8)!=0);
        bool    c_t=(((modcfg&0x4)!=0));
        uint8_t mode=(modcfg&3);
        if((!c_t && !counter0 && is_timer_clk) || (c_t && counter0))
        {
            if(!gate || hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_INT0))
            {
                uint8_t tl=0;
                uint8_t th=0;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TL0,&tl);
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH0,&th);
                switch(mode)
                {
                case 0:
                {
                    if((tl&0x1F)==31)
                    {
                        tl&=(~0x1F);
                        if(th==255)
                        {
                            //溢出
                            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF0,true);
                        }
                        th++;
                    }
                    else
                    {
                        tl++;
                    }
                }
                break;
                case 1:
                {
                    if(tl==255)
                    {
                        if(th==255)
                        {
                            //溢出
                            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF0,true);
                        }
                        th++;
                    }
                    tl++;
                }
                break;
                case 2:
                {
                    if(tl==255)
                    {
                        tl=th;
                        //溢出
                        hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF0,true);
                    }
                    else
                    {
                        tl++;
                    }
                }
                break;
                case 3:
                {
                    if(tl==255)
                    {
                        //溢出
                        hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF0,true);
                    }
                    tl++;
                }
                break;
                default:
                {

                }
                break;
                }
                hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TL0,tl);
                hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TH0,th);
            }
        }
    }
    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TR1))
    {
        if(((tmod&0x3)!=3))
        {
            //定时器1已使能
            uint8_t modcfg=((tmod>>4)&0xF);
            bool    gate=((modcfg&0x8)!=0);
            bool    c_t=(((modcfg&0x4)!=0));
            uint8_t mode=(modcfg&3);
            if((!c_t && !counter1 && is_timer_clk) || (c_t && counter1))
            {
                if(!gate || hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_INT1))
                {
                    uint8_t tl=0;
                    uint8_t th=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TL1,&tl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH1,&th);
                    switch(mode)
                    {
                    case 0:
                    {
                        if((tl&0x1F)==31)
                        {
                            tl&=(~0x1F);
                            if(th==255)
                            {
                                //溢出
                                hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF1,true);
                            }
                            th++;
                        }
                        else
                        {
                            tl++;
                        }
                    }
                    break;
                    case 1:
                    {
                        if(tl==255)
                        {
                            if(th==255)
                            {
                                //溢出
                                hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF1,true);
                            }
                            th++;
                        }
                        tl++;
                    }
                    break;
                    case 2:
                    {
                        if(tl==255)
                        {
                            tl=th;
                            //溢出
                            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF1,true);
                        }
                        else
                        {
                            tl++;
                        }
                    }
                    break;
                    default:
                    {

                    }
                    break;
                    }
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TL1,tl);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TH1,th);
                }
            }
        }
        else
        {
            if(is_timer_clk)
            {
                //使用TH0进行定时
                uint8_t th0=0;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH0,&th0);
                if(th0==255)
                {
                    //溢出
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF1,true);
                }
                th0++;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH0,&th0);
            }
        }
    }

    if(timer!=NULL && timer->flag.enable_t2 == 1)
    {
        //定时器2只支持部分功能（串口相关功能不予实现）
        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TR2))
        {
            bool c_t=hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_C_T2);
            if((!c_t && !counter2 && is_timer_clk) || (c_t && counter2))
            {
                uint8_t t2mod=0;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_T2MOD,&t2mod);
                bool up_or_down=(((t2mod&0x1)!=0 && timer->flag.t2ex == 1) || ((t2mod&0x1)==0));
                {
                    //T2作为串口时钟。此时只实现寄存器加
                    uint8_t tl=0;
                    uint8_t th=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TL2,&tl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH2,&th);
                    uint8_t rcapl=0;
                    uint8_t rcaph=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_RCAP2L,&rcapl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_RCAP2H,&rcaph);
                    if(up_or_down)
                    {
                        if(tl==255)
                        {
                            if(th==255)
                            {
                                tl=rcapl;
                                th=rcaph;
                                //溢出
                                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TCLK) || hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RCLK))
                                {
                                    //串口时钟
                                }
                                else
                                {
                                    //定时器/计数器
                                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF2,true);
                                }
                            }
                            else
                            {
                                th++;
                            }
                        }
                        else
                        {
                            tl++;
                        }
                    }
                    else
                    {
                        if(tl==0)
                        {
                            if(th==0)
                            {
                                th=0xFF;
                                tl=0xFF;
                                //溢出
                                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TCLK) || hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RCLK))
                                {
                                    //串口时钟
                                }
                                else
                                {
                                    //定时器/计数器
                                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF2,true);
                                }

                            }
                            else
                            {
                                th--;
                            }
                        }
                        else
                        {
                            tl--;
                        }
                    }
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TL2,tl);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TH2,th);
                }
            }
        }
    }
}

static void hs_mcs_51_timer_clk_tick(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer)
{
    hs_mcs_51_timer_timer_counter_inc(core,timer,false,false,false);
}

void hs_mcs_51_timer_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_timer_t *timer)
{
    if(core==NULL || timer==NULL || timer->clk_freq==0)
    {
        return;
    }

    switch(opt)
    {
    case HS_MCS_51_IO_TICK_ENTER:
    {
        if(timer->flag.external_clk==0)
        {
            hs_mcs_51_timer_clk_tick(core,timer);
        }
        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EA))
        {
            {
                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TF0))
                {
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_ET0))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PT0))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_TF0,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_TF0,false);
                            }
                        }

                    }
                }
            }

            {
                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TF1))
                {
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_ET1))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PT1))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_TF1,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_TF1,false);
                            }
                        }

                    }
                }
            }

            if(timer->flag.enable_t2 == 1)
            {
                uint8_t t2mod=0;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_T2MOD,&t2mod);
                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TF2) || (hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EXF2) && ((t2mod&0x01)==0)))
                {
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_ET2))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PT2))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8052_TF2,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8052_TF2,false);
                            }
                        }

                    }
                }
            }
        }
    }
    break;
    case HS_MCS_51_IO_INTERRUPT_ENTER:
    {
        /*
         * 设置内部状态用于清除中断标志
         */
        if(address==HS_MCS_51_INTERRUPT_8051_TF0)
        {
            timer->flag.tf0=1;
        }

        if(address==HS_MCS_51_INTERRUPT_8051_TF1)
        {
            timer->flag.tf1=1;
        }

        if(address==HS_MCS_51_INTERRUPT_8052_TF2)
        {
            timer->flag.tf2=1;
        }
    }
    break;
    case HS_MCS_51_IO_INTERRUPT_EXIT:
    {
        //清除部分中断标志

        if(timer->flag.tf0)
        {
            timer->flag.tf0=0;
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF0,false);
        }

        if(timer->flag.tf1)
        {
            timer->flag.tf1=0;
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TF1,false);
        }

        if(timer->flag.tf2)
        {
            timer->flag.tf2=0;
        }

    }
    break;
    default:
    {

    }
    break;
    }
}


void hs_mcs_51_timer_external_clk_tick(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer,bool enable_clk)
{
    if(timer==NULL)
    {
        return;
    }

    if(enable_clk)
    {
        timer->flag.external_clk=1;
        hs_mcs_51_timer_clk_tick(core,timer);
    }
    else
    {
        timer->flag.external_clk=0;
    }
}

size_t hs_mcs_51_timer_clk_freq_get(hs_mcs_51_timer_t *timer)
{
    if(timer==NULL)
    {
        return 0;
    }

    return timer->clk_freq;
}

void hs_mcs_51_timer_clk_freq_set(hs_mcs_51_timer_t *timer,size_t clk_freq)
{
    if(timer!=NULL)
    {
        timer->clk_freq=clk_freq;
    }
}

void hs_mcs_51_timer_counter0_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer)
{
    hs_mcs_51_timer_timer_counter_inc(core,timer,true,false,false);
}

void hs_mcs_51_timer_counter1_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer)
{
    hs_mcs_51_timer_timer_counter_inc(core,timer,false,true,false);
}

void hs_mcs_51_timer_counter2_inc(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer)
{
    hs_mcs_51_timer_timer_counter_inc(core,timer,false,false,true);
}

void hs_mcs_51_timer_timer2_enable(hs_mcs_51_timer_t *timer,bool enable)
{
    if(timer!=NULL)
    {
        timer->flag.enable_t2=(enable?1:0);
    }
}

void hs_mcs_51_timer_timer2_t2ex_set(hs_mcs_51_core_t *core,hs_mcs_51_timer_t *timer,bool value)
{
    if(core==NULL || timer == NULL)
    {
        return;
    }

    bool old_value=(timer->flag.t2ex!=0);
    timer->flag.t2ex=(value?1:0);
    if((old_value != value) && !value)
    {
        //T2EX产生负跳变

        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TR2))
        {
            if(!hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TCLK) && !hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RCLK) && hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EXEN2))
            {
                uint8_t t2mod=0;
                hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_T2MOD,&t2mod);
                bool up_or_down=(((t2mod&0x1)!=0 && timer->flag.t2ex == 1) || ((t2mod&0x1)==0));
                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_CP_RL2))
                {
                    //捕获
                    uint8_t tl=0;
                    uint8_t th=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TL2,&tl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH2,&th);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_RCAP2L,tl);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_RCAP2H,th);
                    tl=0;
                    th=0;
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TL2,tl);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TH2,th);
                }
                else
                {
                    //自动重装
                    uint8_t tl=0;
                    uint8_t th=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TL2,&tl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH2,&th);
                    uint8_t rcapl=0;
                    uint8_t rcaph=0;
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_RCAP2L,&rcapl);
                    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_RCAP2H,&rcaph);
                    if(up_or_down)
                    {
                        tl=rcapl;
                        th=rcaph;
                    }
                    else
                    {
                        tl=0xFF;
                        th=0xFF;
                    }
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TL2,tl);
                    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_TH2,th);
                }
                hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_EXF2,true);
            }
        }
    }
}
