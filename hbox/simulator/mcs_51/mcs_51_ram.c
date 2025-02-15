/***************************************************************
 * Name:      mcs_51_ram.c
 * Purpose:   实现mcs_51_ram接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_ram.h"
void hs_mcs_51_ram_model_tiny_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_tiny_t*  ram)
{
    if(ram!=NULL)
    {
        switch(opt)
        {
        case HS_MCS_51_IO_RESET:                //IO复位
        {
            //清空内存内容
            memset(ram->ram,0,sizeof(ram->ram));
        };
        break;
        case HS_MCS_51_IO_READ_RAM_SFR:          //读取内部低128字节RAM与SFR
        {
            memcpy(data,&ram->ram[address],length);
        }
        break;
        case HS_MCS_51_IO_WRITE_RAM_SFR:         //写入内部低128字节RAM与SFR
        {
            memcpy(&ram->ram[address],data,length);
        }
        break;
        case HS_MCS_51_IO_READ_HIGH_RAM:
        {
            if(address < 128)
            {
                memcpy(data,&ram->ram[address],length);
            }
        }
        break;
        case HS_MCS_51_IO_WRITE_HIGH_RAM:
        {
            if(address < 128)
            {
                memcpy(&ram->ram[address],data,length);
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void hs_mcs_51_ram_model_small_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_small_t*  ram)
{
    if(ram!=NULL)
    {
        hs_mcs_51_ram_model_tiny_bus_io(core,opt,address,data,length,usr,&ram->base);
        switch(opt)
        {
        case HS_MCS_51_IO_RESET:                //IO复位
        {
            //清空内存内容
            memset(ram->ram,0,sizeof(ram->ram));
        };
        break;
        case HS_MCS_51_IO_READ_HIGH_RAM:
        {
            if(address >= 128)
            {
                memcpy(data,&ram->ram[address-128],length);
            }
        }
        break;
        case HS_MCS_51_IO_WRITE_HIGH_RAM:
        {
            if(address >= 128)
            {
                memcpy(&ram->ram[address-128],data,length);
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}


void hs_mcs_51_ram_model_medium_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_medium_t*  ram)
{
    if(ram!=NULL)
    {
        hs_mcs_51_ram_model_small_bus_io(core,opt,address,data,length,usr,&ram->base);
        switch(opt)
        {
        case HS_MCS_51_IO_RESET:                //IO复位
        {
            //清空内存内容
            memset(ram->xram,0,sizeof(ram->xram));
        };
        break;
        case HS_MCS_51_IO_READ_EXTERNAL_RAM:
        {
            if(address < 256)
            {
                memcpy(data,&ram->xram[address],length);
            }
        }
        break;
        case HS_MCS_51_IO_WRITE_EXTERNAL_RAM:
        {
            if(address < 256)
            {
                memcpy(&ram->xram[address],data,length);
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void hs_mcs_51_ram_model_large_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_large_t*  ram)
{
    if(ram!=NULL)
    {
        hs_mcs_51_ram_model_medium_bus_io(core,opt,address,data,length,usr,&ram->base);
        switch(opt)
        {
        case HS_MCS_51_IO_RESET:                //IO复位
        {
            //清空内存内容
            memset(ram->xram,0,sizeof(ram->xram));
        };
        break;
        case HS_MCS_51_IO_READ_EXTERNAL_RAM:
        {
            if(address >= 256)
            {
                memcpy(data,&ram->xram[address-256],length);
            }
        }
        break;
        case HS_MCS_51_IO_WRITE_EXTERNAL_RAM:
        {
            if(address >= 256)
            {
                memcpy(&ram->xram[address-256],data,length);
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void hs_mcs_51_ram_model_huge_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_ram_model_huge_t*  ram)
{
    if(ram!=NULL)
    {
        hs_mcs_51_ram_model_large_bus_io(core,opt,address,data,length,usr,&ram->base);
    }
}


