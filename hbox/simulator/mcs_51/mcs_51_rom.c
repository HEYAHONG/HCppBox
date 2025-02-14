/***************************************************************
 * Name:      mcs_51_rom.c
 * Purpose:   实现mcs_51_rom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_rom.h"

void hs_mcs_51_rom_bus_io(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr,hs_mcs_51_rom_t *rom)
{
    if(opt==HS_MCS_51_IO_READ_ROM)
    {
        if(rom!=NULL)
        {
            if((rom->len >= (address+length)) && (rom->code!=NULL))
            {
                memcpy(data,&rom->code[address],length);
                //成功读取指令
                return;
            }
        }
        {
            //失败跳转至0地址
            uint8_t ljmp_zero[]= {0x02,0x00,0x00};
            memcpy(data,ljmp_zero,(length>sizeof(ljmp_zero))?(sizeof(ljmp_zero)):(length));
        }
    }
}

/*
*  helloworld程序(见rom/helloworld目录)
*/
static const unsigned char hs_mcs_51_rom_helloworld_bin[] = {
    0x02, 0x00, 0x29, 0x02, 0x00, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02,
    0x00, 0x86, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0x00, 0x87, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x02, 0x00, 0x88, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02,
    0x00, 0x89, 0x02, 0x00, 0xd0, 0x75, 0x81, 0x09, 0x12, 0x02, 0xe3, 0xe5,
    0x82, 0x60, 0x03, 0x02, 0x00, 0x26, 0x79, 0x00, 0xe9, 0x44, 0x00, 0x60,
    0x1b, 0x7a, 0x00, 0x90, 0x03, 0x10, 0x78, 0x01, 0x75, 0xa0, 0x00, 0xe4,
    0x93, 0xf2, 0xa3, 0x08, 0xb8, 0x00, 0x02, 0x05, 0xa0, 0xd9, 0xf4, 0xda,
    0xf2, 0x75, 0xa0, 0xff, 0xe4, 0x78, 0x7f, 0xf6, 0xd8, 0xfd, 0x78, 0x00,
    0xe8, 0x44, 0x00, 0x60, 0x0a, 0x79, 0x01, 0x75, 0xa0, 0x00, 0xe4, 0xf3,
    0x09, 0xd8, 0xfc, 0x78, 0x00, 0xe8, 0x44, 0x00, 0x60, 0x0c, 0x79, 0x00,
    0x90, 0x00, 0x01, 0xe4, 0xf0, 0xa3, 0xd8, 0xfc, 0xd9, 0xfa, 0x02, 0x00,
    0x26, 0x32, 0x32, 0x32, 0x32, 0x32, 0x75, 0x98, 0x90, 0x22, 0xae, 0x82,
    0x8e, 0x99, 0x10, 0x99, 0x02, 0x80, 0xfb, 0x22, 0x30, 0x98, 0xfd, 0x85,
    0x99, 0x82, 0xc2, 0x98, 0x22, 0xad, 0x82, 0xae, 0x83, 0xaf, 0xf0, 0x8d,
    0x82, 0x8e, 0x83, 0x8f, 0xf0, 0x12, 0x02, 0x59, 0xfc, 0x60, 0x1c, 0x0d,
    0xbd, 0x00, 0x01, 0x0e, 0x7b, 0x00, 0x8c, 0x82, 0x8b, 0x83, 0xc0, 0x07,
    0xc0, 0x06, 0xc0, 0x05, 0x12, 0x00, 0x8e, 0xd0, 0x05, 0xd0, 0x06, 0xd0,
    0x07, 0x80, 0xd8, 0x22, 0x12, 0x00, 0x8a, 0x90, 0x02, 0xe7, 0x75, 0xf0,
    0x80, 0x12, 0x00, 0xa1, 0x90, 0x02, 0xf5, 0x75, 0xf0, 0x80, 0x12, 0x00,
    0xa1, 0x7f, 0x01, 0xef, 0x24, 0xf6, 0x50, 0x03, 0x02, 0x01, 0xd7, 0x7e,
    0x01, 0xc3, 0xef, 0x9e, 0x50, 0x03, 0x02, 0x01, 0xc9, 0x8f, 0x04, 0x7d,
    0x00, 0x74, 0x30, 0x2c, 0xf5, 0x82, 0xe4, 0x3d, 0xf5, 0x83, 0xc0, 0x07,
    0xc0, 0x06, 0x12, 0x00, 0x8e, 0x90, 0x00, 0x2a, 0x12, 0x00, 0x8e, 0xd0,
    0x06, 0x8e, 0x04, 0x7d, 0x00, 0x74, 0x30, 0x2c, 0xf5, 0x82, 0xe4, 0x3d,
    0xf5, 0x83, 0xc0, 0x06, 0x12, 0x00, 0x8e, 0x90, 0x00, 0x3d, 0x12, 0x00,
    0x8e, 0xd0, 0x06, 0xd0, 0x07, 0x8f, 0xf0, 0xee, 0xa4, 0xfc, 0xad, 0xf0,
    0xc3, 0xec, 0x94, 0x0a, 0xed, 0x64, 0x80, 0x94, 0x80, 0x40, 0x26, 0x75,
    0x08, 0x0a, 0x75, 0x09, 0x00, 0x8c, 0x82, 0x8d, 0x83, 0xc0, 0x07, 0xc0,
    0x06, 0x12, 0x02, 0xab, 0xe5, 0x82, 0x85, 0x83, 0xf0, 0x24, 0x30, 0xf5,
    0x82, 0xe4, 0x35, 0xf0, 0xf5, 0x83, 0x12, 0x00, 0x8e, 0xd0, 0x06, 0xd0,
    0x07, 0x8f, 0xf0, 0xee, 0xa4, 0xfc, 0xad, 0xf0, 0x75, 0x08, 0x0a, 0x75,
    0x09, 0x00, 0x8c, 0x82, 0x8d, 0x83, 0xc0, 0x07, 0xc0, 0x06, 0xc0, 0x05,
    0xc0, 0x04, 0x12, 0x02, 0x75, 0xe5, 0x82, 0x85, 0x83, 0xf0, 0x24, 0x30,
    0xf5, 0x82, 0xe4, 0x35, 0xf0, 0xf5, 0x83, 0x12, 0x00, 0x8e, 0xd0, 0x04,
    0xd0, 0x05, 0xd0, 0x06, 0xd0, 0x07, 0xc3, 0xec, 0x94, 0x0a, 0xed, 0x64,
    0x80, 0x94, 0x80, 0x50, 0x0e, 0x90, 0x00, 0x20, 0xc0, 0x07, 0xc0, 0x06,
    0x12, 0x00, 0x8e, 0xd0, 0x06, 0xd0, 0x07, 0x90, 0x00, 0x20, 0xc0, 0x07,
    0xc0, 0x06, 0x12, 0x00, 0x8e, 0xd0, 0x06, 0xd0, 0x07, 0x0e, 0x02, 0x00,
    0xf1, 0x90, 0x00, 0x0a, 0xc0, 0x07, 0x12, 0x00, 0x8e, 0xd0, 0x07, 0x0f,
    0x02, 0x00, 0xe7, 0x12, 0x00, 0x98, 0x7e, 0x00, 0x8e, 0x83, 0x12, 0x00,
    0x8e, 0x80, 0xf4, 0xe5, 0x08, 0x45, 0x09, 0x60, 0x46, 0x7a, 0x01, 0xe5,
    0x08, 0x25, 0xe0, 0xf5, 0x08, 0xe5, 0x09, 0x33, 0x40, 0x12, 0xf5, 0x09,
    0xe5, 0x82, 0x95, 0x08, 0xe5, 0x83, 0x95, 0x09, 0x40, 0x03, 0x0a, 0x80,
    0xe6, 0xc3, 0xe5, 0x09, 0x13, 0xf5, 0x09, 0xe5, 0x08, 0x13, 0xf5, 0x08,
    0xc3, 0xe5, 0x82, 0x95, 0x08, 0xf5, 0xf0, 0xe5, 0x83, 0x95, 0x09, 0x40,
    0x05, 0xf5, 0x83, 0x85, 0xf0, 0x82, 0xc3, 0xe5, 0x09, 0x13, 0xf5, 0x09,
    0xe5, 0x08, 0x13, 0xf5, 0x08, 0xda, 0xe1, 0x22, 0x7a, 0x10, 0xe4, 0xfb,
    0xfc, 0xe5, 0x82, 0x25, 0xe0, 0xf5, 0x82, 0xe5, 0x83, 0x33, 0xf5, 0x83,
    0xeb, 0x33, 0xfb, 0xec, 0x33, 0xfc, 0xeb, 0x95, 0x08, 0xf5, 0xf0, 0xec,
    0x95, 0x09, 0x40, 0x06, 0xfc, 0xab, 0xf0, 0x43, 0x82, 0x01, 0xda, 0xdd,
    0x22, 0x20, 0xf7, 0x14, 0x30, 0xf6, 0x14, 0x88, 0x83, 0xa8, 0x82, 0x20,
    0xf5, 0x07, 0xe6, 0xa8, 0x83, 0x75, 0x83, 0x00, 0x22, 0xe2, 0x80, 0xf7,
    0xe4, 0x93, 0x22, 0xe0, 0x22, 0xc2, 0xd5, 0xe5, 0x83, 0x30, 0xe7, 0x0d,
    0xd2, 0xd5, 0xe4, 0xc3, 0x95, 0x82, 0xf5, 0x82, 0xe4, 0x95, 0x83, 0xf5,
    0x83, 0xe5, 0x09, 0x30, 0xe7, 0x0b, 0xe4, 0xc3, 0x95, 0x08, 0xf5, 0x08,
    0xe4, 0x95, 0x09, 0xf5, 0x09, 0x12, 0x01, 0xe3, 0x30, 0xd5, 0x0b, 0xe4,
    0xc3, 0x95, 0x82, 0xf5, 0x82, 0xe4, 0x95, 0x83, 0xf5, 0x83, 0x22, 0xc2,
    0xd5, 0xe5, 0x83, 0x30, 0xe7, 0x0d, 0xd2, 0xd5, 0xe4, 0xc3, 0x95, 0x82,
    0xf5, 0x82, 0xe4, 0x95, 0x83, 0xf5, 0x83, 0xe5, 0x09, 0x30, 0xe7, 0x0d,
    0xb2, 0xd5, 0xe4, 0xc3, 0x95, 0x08, 0xf5, 0x08, 0xe4, 0x95, 0x09, 0xf5,
    0x09, 0x12, 0x02, 0x30, 0x30, 0xd5, 0x0b, 0xe4, 0xc3, 0x95, 0x82, 0xf5,
    0x82, 0xe4, 0x95, 0x83, 0xf5, 0x83, 0x22, 0x75, 0x82, 0x00, 0x22, 0x48,
    0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x0a,
    0x00, 0x39, 0x78, 0x39, 0x20, 0x6d, 0x75, 0x6c, 0x74, 0x69, 0x70, 0x6c,
    0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x74, 0x61, 0x62, 0x6c,
    0x65, 0x3a, 0x0a, 0x00
};
#define hs_mcs_51_rom_helloworld_len  784

const hs_mcs_51_rom_t hs_mcs_51_rom_helloworld=
{
    hs_mcs_51_rom_helloworld_bin,
    hs_mcs_51_rom_helloworld_len
};



