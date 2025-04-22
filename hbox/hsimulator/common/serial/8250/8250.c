/***************************************************************
 * Name:      hs_common.c
 * Purpose:   实现hs_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "8250.h"

void  hs_common_serial_8250_init(hs_common_serial_8250_t *dev,hs_common_serial_8250_io_callback_t io,void *usr,size_t clk_freq)
{
    hs_common_serial_8250_t temp=HS_COMMON_SERIAL_8250_INITIALIZER;
    temp.io=io;
    temp.usr=usr;
    if(clk_freq!=0)
    {
        temp.clk_freq=clk_freq;
    }
    if(dev!=NULL)
    {
        (*dev)=temp;
    }
}

void hs_common_serial_8250_bus_write(hs_common_serial_8250_t *dev,uint8_t address,uint8_t reg_data)
{
    if(dev==NULL)
    {
        return;
    }
    //地址只允许0～7
    address%=8;
    bool DLAB=((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR] &0x80)!=0);
    switch(address)
    {
    case 0:
    {
        if(DLAB)
        {
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLL]=reg_data;
        }
        else
        {
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_THR]=reg_data;
            if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MCR]&0x10)!=0)
            {
                //Loop Back
                if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&0x01)==0)
                {
                    //当前未接收到数据
                    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_RBR]=reg_data;
                    //当前数据就绪
                    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]|=0x01;
                }
            }
            if(dev->io!=NULL)
            {
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_TRANSMIT_BYTE,&dev->registers[HS_COMMON_SERIAL_8250_REGISTER_THR]);
            }
        }
    }
    break;
    case 1:
    {
        if(DLAB)
        {
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLM]=reg_data;
        }
        else
        {
            //高4位为保留
            reg_data&=0x0F;
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER]=reg_data;
        }
    }
    break;
    case 2:
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_FCR]=reg_data;
    }
    break;
    case 3:
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR]=reg_data;
    }
    break;
    case 4:
    {
        if(dev->io!=NULL)
        {
            {
                uint8_t out2=(((reg_data&0x08)!=0)?1:0);
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_OUT2,&out2);
            }
            {
                uint8_t out1=(((reg_data&0x04)!=0)?1:0);
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_OUT1,&out1);
            }
            {
                uint8_t rts=(((reg_data&0x02)!=0)?1:0);
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_RTS,&rts);
            }
            {
                uint8_t dtr=(((reg_data&0x01)!=0)?1:0);
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_DTR,&dtr);
            }
        }
        //高3位为保留位，始终为0
        reg_data&=0x1F;
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MCR]=reg_data;
    }
    break;
    case 5:
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]=reg_data;
    }
    break;
    case 6:
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MSR]=reg_data;
    }
    break;
    case 7:
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_SCR]=reg_data;
    }
    break;
    default:
    {

    }
    break;
    }
}
void hs_common_serial_8250_bus_read(hs_common_serial_8250_t *dev,uint8_t address,uint8_t *reg_data)
{
    if(dev==NULL)
    {
        return;
    }
    //地址只允许0～7
    address%=8;
    bool DLAB=((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR] &0x80)!=0);
    switch(address)
    {
    case 0:
    {
        if(DLAB)
        {
            if(reg_data!=NULL)
            {
                (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLL];
            }
        }
        else
        {
            if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MCR]&0x10)!=0)
            {
                //Loop Back
                if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&0x01)!=0)
                {
                    //有数据
                    if(reg_data!=NULL)
                    {
                        (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_RBR];
                    }
                    //自动清除，数据准备好标志
                    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&=0xFE;
                    break;
                }
            }
            //非Loopback模式或者Loopback模式下无数据
            if(dev->io!=NULL)
            {
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_RECEIVE_BYTE,&dev->registers[HS_COMMON_SERIAL_8250_REGISTER_RBR]);
            }
            if(reg_data!=NULL)
            {
                (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_RBR];
            }
            //自动清除，数据准备好标志
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&=0xFE;
        }
    }
    break;
    case 1:
    {
        if(DLAB)
        {
            if(reg_data!=NULL)
            {
                (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLM];
            }
        }
        else
        {
            if(reg_data!=NULL)
            {
                (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER];
            }
        }
    }
    break;
    case 2:
    {
        if(reg_data!=NULL)
        {
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR];
        }
    }
    break;
    case 3:
    {
        if(reg_data!=NULL)
        {
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
        }
    }
    break;
    case 4:
    {
        if(reg_data!=NULL)
        {
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MCR];
        }
    }
    break;
    case 5:
    {
        if(reg_data!=NULL)
        {
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR];
        }
    }
    break;
    case 6:
    {
        if(reg_data!=NULL)
        {
            if(dev->io!=NULL)
            {
                uint8_t new_msr=0;
                uint8_t value=0;
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_DCD,&value);
                if(value!=0)
                {
                    new_msr|=0x80;
                }
                value=0;
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_RI,&value);
                if(value!=0)
                {
                    new_msr|=0x40;
                }
                value=0;
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_DSR,&value);
                if(value!=0)
                {
                    new_msr|=0x20;
                }
                value=0;
                dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_CTS,&value);
                if(value!=0)
                {
                    new_msr|=0x10;
                }

                new_msr |= ((new_msr^dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MSR])>>4);
                dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MSR]=new_msr;
            }
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MSR];
        }
    }
    break;
    case 7:
    {
        if(reg_data!=NULL)
        {
            (*reg_data)=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_SCR];
        }
    }
    break;
    default:
    {

    }
    break;
    }
}

void hs_common_serial_8250_bus_write32(hs_common_serial_8250_t *dev,uint8_t address,uint32_t reg_data)
{
    if(address%4!=0)
    {
        return;
    }
    hs_common_serial_8250_bus_write(dev,address/4,(uint8_t)reg_data);
}

void hs_common_serial_8250_bus_read32(hs_common_serial_8250_t *dev,uint8_t address,uint32_t *reg_data)
{
    if(address%4!=0)
    {
        return;
    }
    uint8_t data=0;
    hs_common_serial_8250_bus_read(dev,address/4,&data);
    if(reg_data!=NULL)
    {
        (*reg_data)=data;
    }
}

void hs_common_serial_8250_bus_tick(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return;
    }
    if(dev->io!=NULL)
    {
        uint8_t data=0;
        dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_TICK,&data);
    }

    if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER]&0x0F)==0)
    {
        //无中断使能，直接清除现有中断等待标志
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]|=0x01;
    }
    else
    {
        if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&0x01)!=0)
        {
            //当前无中断，按照优先级顺序进行中断扫描
            for(size_t i=0; i<8; i++)
            {
                switch(i)
                {
                case 0:
                {
                    //接收器线中断（INTID=0x3）
                }
                break;
                case 1:
                {
                    //接收数据有效中断（INTID=0x2）
                    if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER]&0x01)!=0)
                    {
                        if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&0x01)!=0)
                        {
                            //指示等待接收数据中断
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&=0xF0;
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]|=((0x2)<<1);
                        }
                    }
                }
                break;
                case 2:
                {
                    //字符超时中断（INTID=0x6）
                }
                break;
                case 3:
                {
                    //THR空中断（INTID=0x1）
                    if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER]&0x02)!=0)
                    {
                        if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&0x20)!=0)
                        {
                            //THR空中断
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&=0xF0;
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]|=((0x1)<<1);
                        }
                    }
                }
                break;
                case 4:
                {
                    //Modem状态（INTID=0x0）
                    if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IER]&0x08)!=0)
                    {
                        if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_MSR]&0x0F)!=0)
                        {
                            //Modem状态中断(注意：中断过程中应当读取MSR寄存器以清空相应标志)
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&=0xF0;
                            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]|=((0x0)<<1);
                        }
                    }
                }
                break;
                case 5:
                {

                }
                break;
                case 6:
                {

                }
                break;
                case 7:
                {

                }
                break;
                default:
                {

                }
                break;
                }
                if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&0x01)==0)
                {
                    //已有中断等待处理，退出中断扫描。
                    break;
                }
            }
        }
    }

    if((dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&0x01)==0)
    {
        //有中断正在等待
        if(dev->io!=NULL)
        {
            //调用中断,在中断中应该清除中断标志，否则会再次中断。
            dev->io(dev,HS_COMMON_SERIAL_8250_IO_OPERATE_IRQ,&dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]);
            //清除保留位
            dev->registers[HS_COMMON_SERIAL_8250_REGISTER_IIR]&=(~0x30);
        }
    }

    //TODO:进行8250内部处理
}

size_t hs_common_serial_8250_config_baud_get(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return 0;
    }
    size_t divisor_latch=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLM]*256UL+dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLL];
    if(divisor_latch==0)
    {
        divisor_latch=1;
    }
    return dev->clk_freq/(16*divisor_latch);
}

void hs_common_serial_8250_config_baud_set(hs_common_serial_8250_t *dev,size_t baud)
{
    if(dev==NULL)
    {
        return ;
    }
    if(baud==0)
    {
        //默认波特率115200
        baud=115200;
    }
    size_t divisor_latch=dev->clk_freq/(16*baud);
    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLM]=((divisor_latch>>8)&0xFF);
    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_DLL]=(divisor_latch&0xFF);
}


hs_common_serial_8250_config_parity_t hs_common_serial_8250_config_parity_get(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return HS_COMMON_SERIAL_8250_CONFIG_PARITY_NONE;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    if((LCR&0x08)==0)   //PEN
    {
        return HS_COMMON_SERIAL_8250_CONFIG_PARITY_NONE;
    }
    else
    {
        if((LCR&0x20)!=0)    //Stick Parity
        {
            if((LCR&0x10)!=0) //EPS
            {
                return HS_COMMON_SERIAL_8250_CONFIG_PARITY_SPACE;
            }
            else
            {
                return HS_COMMON_SERIAL_8250_CONFIG_PARITY_MARK;
            }
        }
        else
        {
            if((LCR&0x10)!=0) //EPS
            {
                return HS_COMMON_SERIAL_8250_CONFIG_PARITY_EVEN;
            }
            else
            {
                return HS_COMMON_SERIAL_8250_CONFIG_PARITY_ODD;
            }
        }
    }
}

void hs_common_serial_8250_config_parity_set(hs_common_serial_8250_t *dev,hs_common_serial_8250_config_parity_t parity)
{
    if(dev==NULL)
    {
        return;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    switch(parity)
    {
    case HS_COMMON_SERIAL_8250_CONFIG_PARITY_NONE:
    {
        LCR&=(~0x08);//PEN
    }
    break;
    case HS_COMMON_SERIAL_8250_CONFIG_PARITY_EVEN:
    {
        LCR|=(0x08);    //PEN
        LCR|=(0x10);    //EPS
        LCR&=(~0x20);   //Stick Parity
    }
    break;
    case HS_COMMON_SERIAL_8250_CONFIG_PARITY_ODD:
    {
        LCR|=(0x08);    //PEN
        LCR&=(~0x10);    //EPS
        LCR&=(~0x20);   //Stick Parity
    }
    break;
    case HS_COMMON_SERIAL_8250_CONFIG_PARITY_SPACE:
    {
        LCR|=(0x08);    //PEN
        LCR|=(0x10);    //EPS
        LCR|=(0x20);   //Stick Parity
    }
    break;
    case HS_COMMON_SERIAL_8250_CONFIG_PARITY_MARK:
    {
        LCR|=(0x08);    //PEN
        LCR&=(~0x10);    //EPS
        LCR|=(0x20);   //Stick Parity
    }
    break;
    default:
    {
    }
    break;
    }
    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR]=LCR;
}

size_t hs_common_serial_8250_config_stopbits_get(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return 1;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    if((LCR&0x04)!=0)
    {
        return 2;
    }
    return 1;
}


void hs_common_serial_8250_config_stopbits_set(hs_common_serial_8250_t *dev,size_t stopbits)
{
    if(dev==NULL)
    {
        return;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    if(stopbits >= 2)
    {
        LCR|=(0x04);
    }
    else
    {
        LCR&=(~0x04);
    }
    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR]=LCR;
}


size_t hs_common_serial_8250_config_databits_get(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return 8;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    return 5+(LCR&0x03);
}

void hs_common_serial_8250_config_databits_set(hs_common_serial_8250_t *dev,size_t databits)
{
    if(dev==NULL)
    {
        return ;
    }
    if(databits<5)
    {
        databits=5;
    }
    if(databits>8)
    {
        databits=8;
    }
    uint8_t LCR=dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR];
    LCR&=(~0x03);
    LCR|=(databits-5);
    dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LCR]=LCR;
}


bool hs_common_serial_8250_status_dataready_get(hs_common_serial_8250_t *dev)
{
    if(dev==NULL)
    {
        return false;
    }

    return (dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]&0x01)!=0;
}

bool hs_common_serial_8250_status_dataready_set(hs_common_serial_8250_t *dev,uint8_t data)
{
    if(dev==NULL)
    {
        return false;
    }

    if(!hs_common_serial_8250_status_dataready_get(dev))
    {
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_RBR]=data;
        dev->registers[HS_COMMON_SERIAL_8250_REGISTER_LSR]|=0x01;
        return true;
    }
    return false;
}
