/***************************************************************
 * Name:      mcs_51_serial.c
 * Purpose:   实现mcs_51_serial接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_serial.h"

void hs_mcs_51_serial_init(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_callback_t io,void *usr)
{
    hs_mcs_51_serial_t new_serial= {io,usr,0,0};
    if(serial!=NULL)
    {
        (*serial)=new_serial;
    }
}

void hs_mcs_51_serial_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_serial_t *serial)
{
    if(serial==NULL || core == NULL)
    {
        //无效，无io操作
        return;
    }
    switch(opt)
    {
    case HS_MCS_51_IO_TICK_ENTER:
    {
        if(serial->io!=NULL)
        {
            uint16_t core_pc= hs_mcs_51_pc_get(core);
            serial->io(serial,HS_MCS_51_SERIAL_IO_TICK,&core_pc);
        }

        if((serial->RB&((1ULL<<8)))!=0)
        {
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_RB8,true);
        }
        else
        {
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_RB8,false);
        }

        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EA) && hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_ES))
        {
            //中断已使能

            if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RI) || hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TI))
            {
                //需要中断

                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PS))
                {
                    //高优先级

                    if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                    {
                        hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_SI0,true);
                    }

                }
                else
                {
                    //低优先级

                    if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                    {
                        hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_SI0,false);
                    }
                }
            }
        }
    }
    break;
    case HS_MCS_51_IO_TICK_EXIT:
    {

    }
    break;
    case HS_MCS_51_IO_READ_RAM_SFR:          //读取内部低128字节RAM与SFR
    {
        switch(address)
        {
        case HS_MCS_51_SFR_SBUF:
        {
            if(serial->io!=NULL)
            {
                serial->io(serial,HS_MCS_51_SERIAL_IO_RECEIVE,&serial->RB);
            }
            if(data!=NULL)
            {
                (*data)=(serial->RB&0xFF);
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
    break;
    case HS_MCS_51_IO_WRITE_RAM_SFR:         //写入内部低128字节RAM与SFR
    {
        switch(address)
        {
        case HS_MCS_51_SFR_SBUF:
        {
            if(serial->io==NULL)
            {
                //当io函数为空时，默认成功
                hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TI,true);
            }
            else
            {
                if(data!=NULL)
                {
                    serial->TH=(*data);
                }
                serial->TH&=(~((1ULL<<8)));
                if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_TB8))
                {
                    serial->TH|=((1ULL<<8));
                }
                if(serial->io(serial,HS_MCS_51_SERIAL_IO_TRANSMIT,&serial->TH))
                {
                    //当io函数成功时,默认成功。当IO函数失败时，默认由用户设置TI标志
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TI,true);
                }
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
    break;
    default:
    {

    }
    break;
    }
}


size_t hs_mcs_51_serial_config_mode_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial)
{
    (void)serial;
    uint8_t scon=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SCON,&scon);
    return scon>>6;//高2位表示模式
}

size_t hs_mcs_51_serial_config_baud_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial)
{
    size_t baud=0;
    switch(hs_mcs_51_serial_config_mode_get(core,serial))
    {
    case 0:
    {
        baud=HS_MCS_51_COMMON_CLK_FREQ/12;
    }
    break;
    case 1:
    case 3:
    {
        uint8_t pcon=0;
        hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_PCON,&pcon);
        bool smod=pcon&0x80;
        uint8_t th1=0;
        hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_TH1,&th1);
        baud=HS_MCS_51_COMMON_CLK_FREQ*(smod?2:1)/32/((256-th1)*12);
    }
    break;
    case 2:
    {
        uint8_t pcon=0;
        hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_PCON,&pcon);
        bool smod=pcon&0x80;
        baud=HS_MCS_51_COMMON_CLK_FREQ*(smod?2:1)/64;
    }
    break;
    default:
    {

    }
    break;
    }
    return baud;
}

void hs_mcs_51_serial_config_ti_set(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial)
{
    (void)serial;
    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_TI,true);
}


bool hs_mcs_51_serial_config_ri_get(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial)
{
    (void)serial;
    return  hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RI);
}

bool hs_mcs_51_serial_status_dataready_set(hs_mcs_51_core_t *core,hs_mcs_51_serial_t *serial,uint16_t data)
{
    if(core==0 || serial==0)
    {
        //参数错误
        return false;
    }

    if(!hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_REN))
    {
        //程序未启用串口
        return false;
    }

    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_RI))
    {
        //程序未清除RI标志
        return false;
    }
    bool RB8=((data&(1ULL<<8))!=0);
    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_RB8,RB8);
    serial->RB=data;
    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_SM2)&&!RB8)
    {
        //启用SM2不接收RB8为0的数据
        return false;
    }

    //设置RI标志
    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_RI,true);

    return true;
}
