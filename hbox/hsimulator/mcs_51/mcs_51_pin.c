/***************************************************************
 * Name:      mcs_51_pin.c
 * Purpose:   实现mcs_51_pin接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_core.h"
#include "mcs_51_pin.h"

void hs_mcs_51_pin_init(hs_mcs_51_pin_t *pin,hs_mcs_51_pin_io_callback_t io,void *usr)
{
    if(pin!=NULL)
    {
        pin->io=io;
        pin->usr=usr;
    }
}

void hs_mcs_51_pin_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_pin_t *pin)
{
    if(core==NULL || pin ==NULL)
    {
        return;
    }

    switch(opt)
    {
    case HS_MCS_51_IO_RESET:
    {
        for(size_t i=0; i<(sizeof(pin->port)/sizeof(pin->port[0])); i++)
        {
            //所有端口都初始化为0xFF
            pin->port[i]=0xFF;
        }
        memset(&pin->flag,0,sizeof(pin->flag));
    }
    break;
    case HS_MCS_51_IO_INSTRUCTION_ENTER:
    {
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P0,pin->port[HS_MCS_51_PIN_PORT_0]);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P1,pin->port[HS_MCS_51_PIN_PORT_1]);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P2,pin->port[HS_MCS_51_PIN_PORT_2]);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P3,pin->port[HS_MCS_51_PIN_PORT_3]);
    }
    break;
    case HS_MCS_51_IO_INSTRUCTION_EXIT:
    {
        {
            uint8_t value=pin->port[HS_MCS_51_PIN_PORT_0];
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_P0,&pin->port[HS_MCS_51_PIN_PORT_0]);
            value^=pin->port[HS_MCS_51_PIN_PORT_0];
            if(value!=0)
            {
                for(size_t i=0; i<8; i++)
                {
                    if((value&(1ULL<<(i)))!=0)
                    {
                        if(pin->io!=NULL)
                        {
                            pin->io(pin,HS_MCS_51_PIN_IO_PIN_CHANGE,HS_MCS_51_PIN_PORT_0,i);
                        }
                    }
                }
            }
        }
        {
            uint8_t value=pin->port[HS_MCS_51_PIN_PORT_1];
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_P1,&pin->port[HS_MCS_51_PIN_PORT_1]);
            value^=pin->port[HS_MCS_51_PIN_PORT_1];
            if(value!=0)
            {
                for(size_t i=0; i<8; i++)
                {
                    if((value&(1ULL<<(i)))!=0)
                    {
                        if(pin->io!=NULL)
                        {
                            pin->io(pin,HS_MCS_51_PIN_IO_PIN_CHANGE,HS_MCS_51_PIN_PORT_1,i);
                        }
                    }
                }
            }
        }
        {
            uint8_t value=pin->port[HS_MCS_51_PIN_PORT_2];
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_P2,&pin->port[HS_MCS_51_PIN_PORT_2]);
            value^=pin->port[HS_MCS_51_PIN_PORT_2];
            if(value!=0)
            {
                for(size_t i=0; i<8; i++)
                {
                    if((value&(1ULL<<(i)))!=0)
                    {
                        if(pin->io!=NULL)
                        {
                            pin->io(pin,HS_MCS_51_PIN_IO_PIN_CHANGE,HS_MCS_51_PIN_PORT_2,i);
                        }
                    }
                }
            }
        }
        {
            uint8_t value=pin->port[HS_MCS_51_PIN_PORT_3];
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_P3,&pin->port[HS_MCS_51_PIN_PORT_3]);
            value^=pin->port[HS_MCS_51_PIN_PORT_3];
            if(value!=0)
            {
                for(size_t i=0; i<8; i++)
                {
                    if((value&(1ULL<<(i)))!=0)
                    {
                        if(pin->io!=NULL)
                        {
                            pin->io(pin,HS_MCS_51_PIN_IO_PIN_CHANGE,HS_MCS_51_PIN_PORT_3,i);
                        }
                    }
                }
            }
        }
    }
    break;
    case HS_MCS_51_IO_TICK_ENTER:
    {
        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EA))
        {
            {
                //INT0中断
                bool int0=((pin->port[HS_MCS_51_PIN_PORT_3]&(1ULL<<2))!=0);
                if(!int0 &&!hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_IT0))
                {
                    //低电平中断
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE0,true);
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EX0))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PX0))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE0,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE0,false);
                            }
                        }

                    }
                }

            }

            {
                //INT1中断
                bool int1=((pin->port[HS_MCS_51_PIN_PORT_3]&(1ULL<<3))!=0);
                if(!int1 &&!hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_IT1))
                {
                    //低电平中断
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE1,true);
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EX1))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PX1))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE1,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE1,false);
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
        if(address==HS_MCS_51_INTERRUPT_8051_IE0)
        {
            pin->flag.ie0=1;
        }

        if(address==HS_MCS_51_INTERRUPT_8051_IE1)
        {
            pin->flag.ie1=1;
        }
    }
    break;
    case HS_MCS_51_IO_INTERRUPT_EXIT:
    {
        //清除部分中断标志

        if(pin->flag.ie0)
        {
            pin->flag.ie0=0;
            /*
             *  若为低电平中断，当电平未恢复正常时又会进入中断
             */
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE0,false);
        }

        if(pin->flag.ie1)
        {
            pin->flag.ie1=0;
            /*
             *  若为低电平中断，当电平未恢复正常时又会进入中断
             */
            hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE1,false);
        }
    }
    break;
    default:
    {

    }
    break;
    }
}

uint8_t hs_mcs_51_pin_port_get(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port)
{
    if(core==NULL || pin==NULL)
    {
        return 0;
    }
    return pin->port[((size_t)port)%(sizeof(pin->port)/sizeof(pin->port[0]))];
}


void hs_mcs_51_pin_port_set(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t value)
{
    if(core==NULL || pin==NULL)
    {
        return;
    }
    {
        uint8_t old_value=pin->port[((size_t)port)%(sizeof(pin->port)/sizeof(pin->port[0]))];
        uint8_t diff_value=(old_value^value);
        for(size_t i=0; i<8; i++)
        {
            if((diff_value & (1ULL << i))!=0)
            {
                if(pin->io!=NULL)
                {
                    pin->io(pin,HS_MCS_51_PIN_IO_USR_PIN_CHANGE,port,i);
                }
            }
        }
    }
    switch(port)
    {
    case HS_MCS_51_PIN_PORT_3:  //P3口
    {
        {
            //对特殊引脚的操作执行回调函数
            uint8_t old_value =pin->port[HS_MCS_51_PIN_PORT_3];
            uint8_t diff_value=(old_value^value);
            if((diff_value&(1ULL<<(2)))!=0)
            {
                if(pin->io!=NULL)
                {
                    pin->io(pin,HS_MCS_51_PIN_IO_INT0_PIN_CHANGE,HS_MCS_51_PIN_PORT_3,2);
                }
            }
            if((diff_value&(1ULL<<(3)))!=0)
            {
                if(pin->io!=NULL)
                {
                    pin->io(pin,HS_MCS_51_PIN_IO_INT1_PIN_CHANGE,HS_MCS_51_PIN_PORT_3,3);
                }
            }
            if((diff_value&(1ULL<<(4)))!=0)
            {
                if(pin->io!=NULL)
                {
                    pin->io(pin,HS_MCS_51_PIN_IO_T0_PIN_CHANGE,HS_MCS_51_PIN_PORT_3,4);
                }
            }
            if((diff_value&(1ULL<<(5)))!=0)
            {
                if(pin->io!=NULL)
                {
                    pin->io(pin,HS_MCS_51_PIN_IO_T1_PIN_CHANGE,HS_MCS_51_PIN_PORT_3,5);
                }
            }
        }

        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EA))
        {
            {
                bool int0=(((pin->port[HS_MCS_51_PIN_PORT_3]&(1ULL<<2))!=0) && ((value&(1ULL<<2))==0));
                if(int0 &&hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_IT0))
                {
                    //下降边沿中断
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE0,true);
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EX0))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PX0))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE0,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE0,false);
                            }
                        }

                    }
                }
            }

            {
                bool int1=(((pin->port[HS_MCS_51_PIN_PORT_3]&(1ULL<<3))!=0) && ((value&(1ULL<<3))==0));
                if(int1 &&hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_IT1))
                {
                    //下降边沿中断
                    hs_mcs_51_bit_write(core,HS_MCS_51_BIT_ADDRESS_IE1,true);
                    if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_EX1))
                    {
                        if(hs_mcs_51_bit_read(core,HS_MCS_51_BIT_ADDRESS_PX1))
                        {
                            //高优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 2)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE1,true);
                            }

                        }
                        else
                        {
                            //低优先级

                            if(hs_mcs_51_core_interrupt_nested_get(core) < 1)
                            {
                                hs_mcs_51_core_interrupt_set(core,HS_MCS_51_INTERRUPT_8051_IE1,false);
                            }
                        }

                    }
                }
            }
        }
    }
    break;
    default:
        break;
    }
    pin->port[((size_t)port)%(sizeof(pin->port)/sizeof(pin->port[0]))]=value;
}

bool hs_mcs_51_pin_port_pin_get(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t pinnum)
{
    return (hs_mcs_51_pin_port_get(core,pin,port)&(1ULL<<(pinnum%8)))!=0;
}

void hs_mcs_51_pin_port_pin_set(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,hs_mcs_51_pin_port_t port,uint8_t pinnum,bool value)
{
    uint8_t new_value=hs_mcs_51_pin_port_get(core,pin,port);
    new_value&=(~((1ULL)<<(pinnum%8)));
    if(value)
    {
        new_value|=((1ULL)<<(pinnum%8));
    }
    hs_mcs_51_pin_port_set(core,pin,port,new_value);
}

bool hs_mcs_51_pin_port_pin_get_by_bitaddr(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,uint8_t bit_addr)
{
    return hs_mcs_51_pin_port_pin_get(core,pin,(hs_mcs_51_pin_port_t)(HS_MCS_51_PIN_PORT_0+((bit_addr-HS_MCS_51_BIT_ADDRESS_P0_0)/0x10)),bit_addr%8);
}

void hs_mcs_51_pin_port_pin_set_by_bitaddr(hs_mcs_51_core_t *core,hs_mcs_51_pin_t *pin,uint8_t bit_addr,bool value)
{
    hs_mcs_51_pin_port_pin_set(core,pin,(hs_mcs_51_pin_port_t)(HS_MCS_51_PIN_PORT_0+((bit_addr-HS_MCS_51_BIT_ADDRESS_P0_0)/0x10)),bit_addr%8,value);
}
