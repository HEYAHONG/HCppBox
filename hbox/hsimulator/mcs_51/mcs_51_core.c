/***************************************************************
 * Name:      mcs_51_core.c
 * Purpose:   实现mcs_51_core接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "mcs_51_core.h"
#include "mcs_51_disassembly.h"

struct hs_mcs_51_core
{
    hs_mcs_51_io_t io;
    void *usr;
    struct
    {
        uint16_t delay_tick:2;//MCS-51具有多周期指令。为保证执行效果，对于多周期指令需要延时,最多延时3周期。
        uint16_t interrupt_nested:2;//中断嵌套层数，0=正常运行,3=直接进入高优先级中断
        uint16_t pc;//PC
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

static void hs_mcs_51_core_pc_push(hs_mcs_51_core_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        uint8_t sp=0;
        hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
        //压栈PC低字节
        sp++;
        {
            if(core->io!=NULL)
            {
                uint8_t val=(core->pc);
                core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,sp,&val,sizeof(val),core->usr);
            }
        }
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
        if(sp==0xff)
        {
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_STACK_OVERFLOW,core->pc,&val,sizeof(val),core->usr);
        }
        //压栈PC高字节
        sp++;
        {
            if(core->io!=NULL)
            {
                uint8_t val=(core->pc>>8);
                core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,sp,&val,sizeof(val),core->usr);
            }
        }
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
        if(sp==0xff)
        {
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_STACK_OVERFLOW,core->pc,&val,sizeof(val),core->usr);
        }
    }
}

static void hs_mcs_51_core_pc_pop(hs_mcs_51_core_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        uint16_t pc=0;
        {
            //出栈PC
            uint8_t sp=0;
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
            //出栈PC高字节
            {
                {
                    uint8_t val=0;
                    core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                    pc+=256*val;
                }
            }
            sp--;
            hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
            //出栈PC低字节
            {
                {
                    uint8_t val=0;
                    core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                    pc+=val;
                }
            }
            sp--;
            hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
        }
        core->pc=pc;
    }
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
                        if(core->interrupt_nested==0)
                        {
                            core->interrupt_nested=3;
                        }
                        else
                        {
                            core->interrupt_nested++;//增加中断嵌套，RETI指令时自减1
                        }
                        {
                            hs_mcs_51_core_pc_push(core);

                            //跳转至中断入口地址
                            core->pc=address;
                        }
                        {
                            //调用中断回调
                            if(core->io!=NULL)
                            {
                                uint8_t val=core->interrupt_nested;
                                core->io(core,HS_MCS_51_IO_INTERRUPT_ENTER,i,&val,sizeof(val),core->usr);
                            }
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
                            hs_mcs_51_core_pc_push(core);

                            //跳转至中断入口地址
                            core->pc=address;
                        }
                        {
                            //调用中断回调
                            if(core->io!=NULL)
                            {
                                uint8_t val=core->interrupt_nested;
                                core->io(core,HS_MCS_51_IO_INTERRUPT_ENTER,i,&val,sizeof(val),core->usr);
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
}

static size_t hs_mcs_51_core_instruction_length(uint8_t instruction)
{
    return hs_mcs_51_disassembly_instruction_length(&instruction);
}

static uint8_t hs_mcs_51_sfr_acc_read(hs_mcs_51_core_t * core)
{
    uint8_t ret=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_ACC,&ret);
    return ret;
}

static uint8_t hs_mcs_51_sfr_psw_read(hs_mcs_51_core_t * core);
static void hs_mcs_51_sfr_psw_write(hs_mcs_51_core_t * core,uint8_t psw);
static void hs_mcs_51_sfr_acc_write(hs_mcs_51_core_t * core,uint8_t acc)
{
    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_ACC,acc);
    {
        //更新PSW的P（位0）奇偶校验位
        bool odd=false;
        for(uint8_t i=0; i<8; i++)
        {
            if((acc&(1<<i))!=0)
            {
                odd=!odd;
            }
        }
        uint8_t psw=hs_mcs_51_sfr_psw_read(core);
        if(odd)
        {
            psw|=0x1;
        }
        else
        {
            psw&=(~0x1);
        }
        hs_mcs_51_sfr_psw_write(core,psw);
    }
}

static uint8_t hs_mcs_51_sfr_b_read(hs_mcs_51_core_t * core)
{
    uint8_t ret=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_B,&ret);
    return ret;
}

static void hs_mcs_51_sfr_b_write(hs_mcs_51_core_t * core,uint8_t b)
{
    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_B,b);
}


static uint8_t hs_mcs_51_sfr_psw_read(hs_mcs_51_core_t * core)
{
    uint8_t ret=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_PSW,&ret);
    return ret;
}

static void hs_mcs_51_sfr_psw_write(hs_mcs_51_core_t * core,uint8_t psw)
{
    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_PSW,psw);
}

static bool hs_mcs_51_sfr_bit_read(hs_mcs_51_core_t * core,uint8_t bit_address)
{
    if(core==NULL || core->io==NULL)
    {
        return false;
    }
    if(bit_address < 0x80)
    {
        //内存 0x20~0x2F
        uint8_t byte_address=0x20+bit_address/8;//字节地址
        bit_address%=8;
        uint8_t val=0;
        core->io(core,HS_MCS_51_IO_READ_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
        if((val&(1<<bit_address))!=0)
        {
            return true;
        }
    }
    else
    {
        //特殊寄存器中地址能被8整除的也可位寻址
        uint8_t byte_address=(bit_address/8)*8;//字节地址
        bit_address%=8;
        uint8_t val=0;
        core->io(core,HS_MCS_51_IO_READ_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
        if((val&(1<<bit_address))!=0)
        {
            return true;
        }
    }
    return false;
}

static void hs_mcs_51_sfr_bit_write(hs_mcs_51_core_t * core,uint8_t bit_address,bool bit_value)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }
    if(bit_address < 0x80)
    {
        //内存 0x20~0x2F
        uint8_t byte_address=0x20+bit_address/8;//字节地址
        bit_address%=8;
        uint8_t val=0;
        core->io(core,HS_MCS_51_IO_READ_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
        val &= (~(1<<bit_address));
        if(bit_value)
        {
            val|=(1<<bit_address);
        }
        core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
    }
    else
    {
        //特殊寄存器中地址能被8整除的也可位寻址
        uint8_t byte_address=(bit_address/8)*8;//字节地址
        bit_address%=8;
        uint8_t val=0;
        core->io(core,HS_MCS_51_IO_READ_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
        val &= (~(1<<bit_address));
        if(bit_value)
        {
            val|=(1<<bit_address);
        }
        core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,byte_address,&val,sizeof(val),core->usr);
    }
}

static uint16_t hs_mcs_51_sfr_dptr_read(hs_mcs_51_core_t * core)
{
    uint16_t ret=0;
    uint8_t val=0;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_DPH,&val);
    ret+=256*val;
    hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_DPL,&val);
    ret+=val;
    return ret;
}

static void hs_mcs_51_sfr_dptr_write(hs_mcs_51_core_t * core,uint16_t dptr)
{
    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_DPH,(dptr>>8)&0xFF);
    hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_DPL,dptr&0xFF);
}

static void hs_mcs_51_core_exec(hs_mcs_51_core_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        uint8_t instruction[HS_MCS_51_INSTRUCTION_MAX_LENGTH]= {0};
        core->io(core,HS_MCS_51_IO_READ_ROM,core->pc,instruction,sizeof(instruction),core->usr);
        core->io(core,HS_MCS_51_IO_INSTRUCTION_ENTER,core->pc,instruction,sizeof(instruction),core->usr);
        core->pc+=hs_mcs_51_core_instruction_length(instruction[0]);//自动加PC值
        switch(instruction[0])
        {
        case 0x01://AJMP addr
        case 0x21:
        case 0x41:
        case 0x61:
        case 0x81:
        case 0xA1:
        case 0xC1:
        case 0xE1:
        {
            uint16_t addr11=256*(instruction[0]>>5)+instruction[1];
            core->delay_tick=1;
            core->pc=addr11+(core->pc&0xF800);
        }
        break;
        case 0x02://LJMP addr_hi addr_lo
        {
            core->delay_tick=1;
            core->pc=256*instruction[1]+instruction[2];
        }
        break;
        case 0x03://RR
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            //循环右移
            acc=((acc>>1)+(acc<<7));
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x04://INC A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            //自增
            acc++;
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x05://INC addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val++;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
        }
        break;
        case 0x06://INC @Ri
        case 0x07:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            val++;
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
        }
        break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F://INC Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            Rn++;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
        }
        break;
        case 0x10://JBC bit,addr
        {
            uint8_t bit_address=instruction[1];
            int8_t  rel_addr=instruction[2];
            if(hs_mcs_51_sfr_bit_read(core,bit_address))
            {
                core->pc+=rel_addr;
            }
            hs_mcs_51_sfr_bit_write(core,bit_address,false);
        }
        break;
        case 0x11://ACALL addr
        case 0x31:
        case 0x51:
        case 0x71:
        case 0x91:
        case 0xB1:
        case 0xD1:
        case 0xF1:
        {
            {
                hs_mcs_51_core_pc_push(core);
            }
            uint16_t addr11=256*(instruction[0]>>5)+instruction[1];
            core->delay_tick=1;
            core->pc=addr11+(core->pc&0xF800);
        }
        break;
        case 0x12://LCALL addr
        {
            {
                hs_mcs_51_core_pc_push(core);
            }
            core->delay_tick=1;
            core->pc=256*instruction[1]+instruction[2];
        }
        break;
        case 0x13://RRC A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t cy=(psw>>7);
            uint8_t new_cy=(acc&0x01);
            //循环右移,加上CY
            acc=((acc>>1)+(cy<<7));
            hs_mcs_51_sfr_acc_write(core,acc);
            psw&=0x7F;
            psw|=(new_cy<<7);
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0x14://DEC A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            acc--;
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x15://DEC addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val--;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
        }
        break;
        case 0x16://DEC @Ri
        case 0x17:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            val--;
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
        }
        break;
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F://DEC Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            Rn--;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
        }
        break;
        case 0x20://JB bit,addr
        {
            uint8_t bit_address=instruction[1];
            int8_t  rel_addr=instruction[2];
            if(hs_mcs_51_sfr_bit_read(core,bit_address))
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0x22://RET
        {
            hs_mcs_51_core_pc_pop(core);
            core->delay_tick=1;
        }
        break;
        case 0x23: //RL A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            //循环左移
            acc=((acc<<1)+(acc>>7));
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x24://ADD A,#Data
        {
            uint8_t data=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x25://ADD A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x26://ADD A,@Ri
        case 0x27:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F://ADD A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=Rn;
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x30://JNB bit,addr
        {
            uint8_t bit_address=instruction[1];
            int8_t  rel_addr=instruction[2];
            if(!hs_mcs_51_sfr_bit_read(core,bit_address))
            {
                core->pc+=rel_addr;
            }
        }
        break;
        case 0x32://RETI
        {
            hs_mcs_51_core_pc_pop(core);
            core->delay_tick=1;
            {
                //调用中断回调
                if(core->io!=NULL)
                {
                    uint8_t val=core->interrupt_nested;
                    core->io(core,HS_MCS_51_IO_INTERRUPT_EXIT,0xFFFF,&val,sizeof(val),core->usr);
                }
            }
            //调整中断嵌套级别
            if(core->interrupt_nested!=0)
            {
                if(core->interrupt_nested==3)
                {
                    core->interrupt_nested=0;
                }
                else
                {
                    core->interrupt_nested--;
                }
            }
        }
        break;
        case 0x33://RLC A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t cy=(psw>>7);
            uint8_t new_cy=(acc>>7);
            //循环左移,加上CY
            acc=((acc<<1)+(cy));
            hs_mcs_51_sfr_acc_write(core,acc);
            psw&=0x7F;
            psw|=(new_cy<<7);
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0x34://ADDC A,#Data
        {
            uint8_t data=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)+old_Cy) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)+old_Cy) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)+old_Cy) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=(data+old_Cy);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x35://ADDC A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)+old_Cy) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)+old_Cy) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)+old_Cy) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=(data+old_Cy);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x36://ADDC A,@Ri
        case 0x37:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)+old_Cy) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)+old_Cy) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)+old_Cy) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=(data+old_Cy);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F://ADDC A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=Rn;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                uint8_t Cy=((((uint16_t)acc+(uint16_t)(data)+old_Cy) > 255)?0x01:0);
                uint8_t Ac=((((acc&0x0f)+(data&0x0f)+old_Cy) & 0xf0)?0x01:0);
                uint8_t C6=((((acc&0x7f)+(data&0x7f)+old_Cy) & 0x80)?0x01:00);
                uint8_t Ov=(Cy^C6);
                acc+=(data+old_Cy);
                psw&=(~(0x80|0x40|0x04));
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x40://JC addr
        {
            int8_t rel_addr=instruction[1];
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            if(Cy)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0x42://ORL addr,A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val|=acc;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
        }
        break;
        case 0x43://ORL addr,#data
        {
            uint8_t data=instruction[2];
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val|=data;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x44://ORL A,#Data
        {
            uint8_t data=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                acc|=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x45://ORL A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc|=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x46://ORL A,@Ri
        case 0x47:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc|=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F://ORL A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=Rn;
                acc|=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x50://JNC addr
        {
            int8_t rel_addr=instruction[1];
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            if(!Cy)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0x52://ANL addr,A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val&=acc;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
        }
        break;
        case 0x53://ANL addr,#data
        {
            uint8_t data=instruction[2];
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val&=data;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x54://ANL A,#Data
        {
            uint8_t data=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                acc&=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x55://ANL A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc&=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x56://ANL A,@Ri
        case 0x57:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc&=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F://ANL A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=Rn;
                acc&=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x60://JZ addr
        {
            int8_t rel_addr=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            if(!acc)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0x62://XRL addr,A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val^=acc;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
        }
        break;
        case 0x63://XRL addr,#data
        {
            uint8_t data=instruction[2];
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val^=data;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x64://XRL A,#Data
        {
            uint8_t data=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                acc^=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x65://XRL A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc^=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x66://XRL A,@Ri
        case 0x67:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=val;
                acc^=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6E:
        case 0x6F://XRL A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t data=Rn;
                acc^=data;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0x70://JNZ addr
        {
            int8_t rel_addr=instruction[1];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            if(acc)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0x72://ORL C,addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            uint8_t bit_address=instruction[1];
            uint8_t bit=(hs_mcs_51_sfr_bit_read(core,bit_address)?0x01:0);
            psw|=((bit || Cy)?0x80:0);
            hs_mcs_51_sfr_psw_write(core,psw);
            core->delay_tick=1;
        }
        break;
        case 0x73://JMP @A+DPTR
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint16_t dptr=hs_mcs_51_sfr_dptr_read(core);
            core->pc=dptr+acc;
            core->delay_tick=1;
        }
        break;
        case 0x74://MOV A,#data
        {
            uint8_t data=instruction[1];
            hs_mcs_51_sfr_acc_write(core,data);
        }
        break;
        case 0x75://MOV addr,#data
        {
            uint8_t addr=instruction[1];
            uint8_t data=instruction[2];
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&data,sizeof(data),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x76://MOV @Ri,#data
        case 0x77:
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t data=instruction[1];
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&data,sizeof(data),core->usr);
        }
        break;
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F://MOV Rn,#data
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t data=instruction[1];
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&data,sizeof(data),core->usr);
        }
        break;
        case 0x80://SJMP addr
        {
            int8_t rel_addr=instruction[1];
            core->pc+=rel_addr;
            core->delay_tick=1;
        }
        break;
        case 0x82://ANL C,addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            uint8_t bit_address=instruction[1];
            uint8_t bit=(hs_mcs_51_sfr_bit_read(core,bit_address)?0x01:0);
            psw&=0x7F;
            psw|=((bit && Cy)?0x80:0);
            hs_mcs_51_sfr_psw_write(core,psw);
            core->delay_tick=1;
        }
        break;
        case 0x83://MOVC A,@A+PC
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_READ_ROM,core->pc+acc,&acc,sizeof(acc),core->usr);
            hs_mcs_51_sfr_acc_write(core,acc);
            core->delay_tick=1;
        }
        break;
        case 0x84://DIV AB
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t b=hs_mcs_51_sfr_b_read(core);
            if(b==0)
            {
                //除数为0
                psw|=0x04;
            }
            else
            {
                psw&=(~0x04);
                uint8_t tmp=acc/b;
                b=acc%b;
                acc=tmp;
            }
            hs_mcs_51_sfr_psw_write(core,psw);
            hs_mcs_51_sfr_acc_write(core,acc);
            hs_mcs_51_sfr_b_write(core,b);
            core->delay_tick=3;
        }
        break;
        case 0x85://MOV addr,addr
        {
            uint8_t addr_src=instruction[1];
            uint8_t addr_dst=instruction[2];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr_src,&val,sizeof(val),core->usr);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr_dst,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x86:
        case 0x87://MOV addr @Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F://MOV addr,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&val,sizeof(val),core->usr);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        };
        break;
        case 0x90://MOV DPTR,#data
        {
            uint8_t dph=instruction[1];
            uint8_t dpl=instruction[2];
            hs_mcs_51_sfr_dptr_write(core,256*dph+dpl);
            core->delay_tick=1;
        }
        break;
        case 0x92://MOV addr,C
        {
            uint8_t bit_address=instruction[1];
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            hs_mcs_51_sfr_bit_write(core,bit_address,Cy!=0);
            core->delay_tick=1;
        }
        break;
        case 0x93://MOVC A,@A+DPTR
        {
            uint16_t dptr=hs_mcs_51_sfr_dptr_read(core);
            uint8_t  acc=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_READ_ROM,dptr+acc,&acc,sizeof(acc),core->usr);
            hs_mcs_51_sfr_acc_write(core,acc);
            core->delay_tick=1;
        }
        break;
        case 0x94://SUBB A,#data
        {
            {
                uint8_t data=instruction[1];
                uint8_t acc=hs_mcs_51_sfr_acc_read(core);
                uint8_t result=acc-data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                psw&=(~(0x80|0x40|0x04));
                if(old_Cy)
                {
                    result--;
                }
                uint8_t Cy= (((unsigned int)acc < ((unsigned int)data+old_Cy))?0x1:0);
                uint8_t Ac= (((acc&0x0f) < ((data+old_Cy)&0x0f) || (old_Cy && ((data&0x0f)==0x0f)))?1:0);
                uint8_t Ov= (((acc<0x80 && data>0x7f && result>0x7f) || (acc>0x7f && data<0x80 && result<0x80))?1:0);
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
                hs_mcs_51_sfr_acc_write(core,result);
            }
        }
        break;
        case 0x95://SUBB A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            {
                uint8_t data=val;
                uint8_t acc=hs_mcs_51_sfr_acc_read(core);
                uint8_t result=acc-data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                psw&=(~(0x80|0x40|0x04));
                if(old_Cy)
                {
                    result--;
                }
                uint8_t Cy= (((unsigned int)acc < ((unsigned int)data+old_Cy))?0x1:0);
                uint8_t Ac= (((acc&0x0f) < ((data+old_Cy)&0x0f) || (old_Cy && ((data&0x0f)==0x0f)))?1:0);
                uint8_t Ov= (((acc<0x80 && data>0x7f && result>0x7f) || (acc>0x7f && data<0x80 && result<0x80))?1:0);
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
                hs_mcs_51_sfr_acc_write(core,result);
            }
        }
        break;
        case 0x96:
        case 0x97://SUBB A,@Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            {
                uint8_t data=val;
                uint8_t acc=hs_mcs_51_sfr_acc_read(core);
                uint8_t result=acc-data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                psw&=(~(0x80|0x40|0x04));
                if(old_Cy)
                {
                    result--;
                }
                uint8_t Cy= (((unsigned int)acc < ((unsigned int)data+old_Cy))?0x1:0);
                uint8_t Ac= (((acc&0x0f) < ((data+old_Cy)&0x0f) || (old_Cy && ((data&0x0f)==0x0f)))?1:0);
                uint8_t Ov= (((acc<0x80 && data>0x7f && result>0x7f) || (acc>0x7f && data<0x80 && result<0x80))?1:0);
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
                hs_mcs_51_sfr_acc_write(core,result);
            }
        }
        break;
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
        case 0x9F://SUBB A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            {
                uint8_t data=Rn;
                uint8_t acc=hs_mcs_51_sfr_acc_read(core);
                uint8_t result=acc-data;
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                uint8_t old_Cy=(psw>>7);
                psw&=(~(0x80|0x40|0x04));
                if(old_Cy)
                {
                    result--;
                }
                uint8_t Cy= (((unsigned int)acc < ((unsigned int)data+old_Cy))?0x1:0);
                uint8_t Ac= (((acc&0x0f) < ((data+old_Cy)&0x0f) || (old_Cy && ((data&0x0f)==0x0f)))?1:0);
                uint8_t Ov= (((acc<0x80 && data>0x7f && result>0x7f) || (acc>0x7f && data<0x80 && result<0x80))?1:0);
                psw|=((Cy<<7)|(Ac<<6)|(Ov<<2));
                hs_mcs_51_sfr_psw_write(core,psw);
                hs_mcs_51_sfr_acc_write(core,result);
            }
        }
        break;
        case 0xA0://ORL C,/addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            uint8_t bit_address=instruction[1];
            uint8_t bit=((!hs_mcs_51_sfr_bit_read(core,bit_address))?0x01:0);
            psw|=((bit || Cy)?0x80:0);
            hs_mcs_51_sfr_psw_write(core,psw);
            core->delay_tick=1;
        }
        break;
        case 0xA2://MOV C,bit
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            psw&=0x7F;
            uint8_t bit_address=instruction[1];
            uint8_t bit=(hs_mcs_51_sfr_bit_read(core,bit_address)?0x01:0);
            psw|=(bit<<7);
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0xA3://INC DPTR
        {
            uint16_t dptr=hs_mcs_51_sfr_dptr_read(core);
            dptr++;
            hs_mcs_51_sfr_dptr_write(core,dptr);
            core->delay_tick=1;
        }
        break;
        case 0xA4://MUL AB
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t b=hs_mcs_51_sfr_b_read(core);
            uint16_t result=((uint16_t)acc)*b;
            acc=result&0xFF;
            b=(result>>8)&0xFF;
            psw&=(~(0x80|0x04));
            if(b!=0)
            {
                psw|=0x04;
            }
            hs_mcs_51_sfr_psw_write(core,psw);
            hs_mcs_51_sfr_acc_write(core,acc);
            hs_mcs_51_sfr_b_write(core,b);
            core->delay_tick=3;
        }
        break;
        case 0xA5://0xA5,保留指令
        {
            int8_t rel_addr=0;
            core->io(core,HS_MCS_51_IO_BREAKPOINT,core->pc,(uint8_t *)&rel_addr,sizeof(rel_addr),core->usr);
            core->pc+=rel_addr;
        }
        break;
        case 0xA6:
        case 0xA7://MOV @Ri,addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF://MOV Rn,addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0xB0://ANL C,/addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            uint8_t bit_address=instruction[1];
            uint8_t bit=((!hs_mcs_51_sfr_bit_read(core,bit_address))?0x01:0);
            psw&=0x7F;
            psw|=((bit && Cy)?0x80:0);
            hs_mcs_51_sfr_psw_write(core,psw);
            core->delay_tick=1;
        }
        break;
        case 0xB2://CPL bit
        {
            uint8_t bit_address=instruction[1];
            hs_mcs_51_sfr_bit_write(core,bit_address,!hs_mcs_51_sfr_bit_read(core,bit_address));
            core->delay_tick=1;
        }
        break;
        case 0xB3://CPL C
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Cy=(psw>>7);
            Cy=((Cy==0)?1:0);
            psw&=0x7F;
            psw|=(Cy<<7);
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0xB4://CJNE A,#data,addr
        {
            uint8_t data=instruction[1];
            int8_t rel_addr=instruction[2];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=0x7F;
                if(acc < data)
                {
                    psw |= 0x80;
                }
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            if(acc!=data)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0xB5://CJNE A,addr,addr
        {
            uint8_t data=0;
            uint8_t addr=instruction[1];
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&data,sizeof(data),core->usr);
            int8_t rel_addr=instruction[2];
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            {
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=0x7F;
                if(acc < data)
                {
                    psw |= 0x80;
                }
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            if(acc!=data)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0xB6:
        case 0xB7://CJNE @Ri,#data,addr
        {
            int8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t data=instruction[1];
            int8_t rel_addr=instruction[2];
            {
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=0x7F;
                if(val < data)
                {
                    psw |= 0x80;
                }
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            if(val!=data)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        };
        break;
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
        {
            int8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t data=instruction[1];
            int8_t rel_addr=instruction[2];
            {
                uint8_t psw=hs_mcs_51_sfr_psw_read(core);
                psw&=0x7F;
                if(Rn < data)
                {
                    psw |= 0x80;
                }
                hs_mcs_51_sfr_psw_write(core,psw);
            }
            if(Rn!=data)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0xC0://PUSH
        {
            uint8_t sp=0;
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
            sp++;
            {
                {
                    uint8_t addr=instruction[1];
                    uint8_t val=0;
                    core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
                    core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                }
            }
            hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
            if(sp==0xff)
            {
                uint8_t val=0;
                core->io(core,HS_MCS_51_IO_STACK_OVERFLOW,core->pc,&val,sizeof(val),core->usr);
            }
            core->delay_tick=1;
        }
        break;
        case 0xC2://CLR bit
        {
            uint8_t bit_address=instruction[1];
            hs_mcs_51_sfr_bit_write(core,bit_address,false);
        }
        break;
        case 0xC3://CLR C
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            psw&=0x7F;
            hs_mcs_51_sfr_psw_write(core,psw);;
        }
        break;
        case 0xC4://SWAP A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            acc=(acc>>4)+(acc<<4);
            hs_mcs_51_sfr_acc_write(core,acc);
        }
        break;
        case 0xC5://XCH A,addr
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t addr=instruction[1];
            uint8_t data=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&data,sizeof(data),core->usr);
            hs_mcs_51_sfr_acc_write(core,data);
            data=acc;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&data,sizeof(data),core->usr);
        }
        break;
        case 0xC6:
        case 0xC7://XCH A,@Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            hs_mcs_51_sfr_acc_write(core,val);
            val=acc;
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
        }
        break;
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF://XCH A,Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            hs_mcs_51_sfr_acc_write(core,Rn);
            Rn=acc;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
        }
        break;
        case 0xD0://POP
        {
            uint8_t sp=0;
            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
            {
                {
                    uint8_t addr=instruction[1];
                    uint8_t val=0;
                    core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                    core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
                }
            }
            sp--;
            hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,sp);
            core->delay_tick=1;
        }
        break;
        case 0xD2://SETB addr
        {
            uint8_t bit_address=instruction[1];
            hs_mcs_51_sfr_bit_write(core,bit_address,true);
        }
        break;
        case 0xD3://SETB C
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            psw|=0x80;
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0xD4://DA A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            if ((acc & 0x0f) > 9 || (psw & 0x40))
            {
                if (((uint16_t)acc+(uint16_t)0x06) > 255)
                    psw|= 0x80;
                acc+= 0x06;
            }
            if ((acc & 0xf0) > 0x90 || (psw & 0x80))
            {
                if (((uint16_t)acc+(uint16_t)0x60) > 255)
                    psw|= 0x80;
                acc+= 0x60;
            }
            hs_mcs_51_sfr_acc_write(core,acc);
            hs_mcs_51_sfr_psw_write(core,psw);
        }
        break;
        case 0xD5://DJNZ addr,addr
        {
            uint8_t addr=instruction[1];
            int8_t rel_addr=instruction[2];
            uint8_t data=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&data,sizeof(data),core->usr);
            data--;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&data,sizeof(data),core->usr);
            if(data!=0)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0xD6:
        case 0xD7://XCHD A,@Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            hs_mcs_51_sfr_acc_write(core,(val&0x0F) & (acc & 0xF0));
            val=((acc&0x0F)&(val&0xF0));
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
        }
        break;
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC:
        case 0xDD:
        case 0xDE:
        case 0xDF://DJNZ Rn,addr
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            Rn--;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            int8_t rel_addr=instruction[1];
            if(Rn!=0)
            {
                core->pc+=rel_addr;
            }
            core->delay_tick=1;
        }
        break;
        case 0xE0://MOVX A,@DPTR
        {
            uint16_t dptr=hs_mcs_51_sfr_dptr_read(core);
            uint8_t data=0;
            core->io(core,HS_MCS_51_IO_READ_EXTERNAL_RAM,dptr,&data,sizeof(data),core->usr);
            hs_mcs_51_sfr_acc_write(core,data);
            core->delay_tick=1;
        }
        break;
        case 0xE2:
        case 0xE3://MOVX A,@Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_EXTERNAL_RAM,Rn,&val,sizeof(val),core->usr);
            hs_mcs_51_sfr_acc_write(core,val);
            core->delay_tick=1;
        }
        break;
        case 0xE4://CLR A
        {
            hs_mcs_51_sfr_acc_write(core,0);
        }
        break;
        case 0xE5://MOV A,addr
        {
            uint8_t addr=instruction[1];
            uint8_t data=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&data,sizeof(data),core->usr);
            hs_mcs_51_sfr_acc_write(core,data);
        }
        break;
        case 0xE6:
        case 0xE7://MOV A,@Ri
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            hs_mcs_51_sfr_acc_write(core,val);
        }
        break;
        case 0xE8:
        case 0xE9:
        case 0xEA:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xEE:
        case 0xEF://MOV A,Rn
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            hs_mcs_51_sfr_acc_write(core,Rn);
        }
        break;
        case 0xF0://MOVX @DPTR,A
        {
            uint16_t dptr=hs_mcs_51_sfr_dptr_read(core);
            uint8_t data=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_WRITE_EXTERNAL_RAM,dptr,&data,sizeof(data),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0xF2:
        case 0xF3://MOVX @Ri,A
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_WRITE_EXTERNAL_RAM,Rn,&val,sizeof(val),core->usr);
            core->delay_tick=1;
        }
        break;
        case 0xF4://CPL A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            hs_mcs_51_sfr_acc_write(core,~acc);
        }
        break;
        case 0xF5://MOV addr,A
        {

            uint8_t addr=instruction[1];
            uint8_t data=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&data,sizeof(data),core->usr);
        }
        break;
        case 0xF6:
        case 0xF7://MOV @Ri,A
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x01);
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=hs_mcs_51_sfr_acc_read(core);
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
        }
        break;
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF://MOV Rn,A
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+(instruction[0]&0x07);
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&acc,sizeof(acc),core->usr);
        }
        break;
        default:
        {
            //默认行为,类似NOP指令，啥也不做
        }
        break;
        }
        core->io(core,HS_MCS_51_IO_INSTRUCTION_EXIT,core->pc,instruction,sizeof(instruction),core->usr);
    }
}

void hs_mcs_51_core_tick(hs_mcs_51_core_t * core,size_t cycles)
{
    if(core!=NULL)
    {
        while((cycles--)!=0)
        {
            if(core->io==NULL)
            {
                //io无效，不工作
                continue;
            }

            core->io(core,HS_MCS_51_IO_TICK_ENTER,core->pc,(uint8_t *)&cycles,sizeof(cycles),core->usr);
            if(core->delay_tick!=0)
            {
                core->delay_tick--;
                core->io(core,HS_MCS_51_IO_TICK_EXIT,core->pc,(uint8_t *)&cycles,sizeof(cycles),core->usr);
                continue;
            }

            hs_mcs_51_core_scan_interrupt(core);

            hs_mcs_51_core_exec(core);

            core->io(core,HS_MCS_51_IO_TICK_EXIT,core->pc,(uint8_t *)&cycles,sizeof(cycles),core->usr);
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
        //初始化SP为0x07
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_SP,0x07);
        //初始化P0～P3为0xFF
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P0,0xFF);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P1,0xFF);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P2,0xFF);
        hs_mcs_51_sfr_write(core,HS_MCS_51_SFR_P3,0xFF);

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

int  hs_mcs_51_core_interrupt_nested_get(hs_mcs_51_core_t * core)
{
    if(core!=NULL)
    {
        return core->interrupt_nested;
    }
    return -1;
}

bool hs_mcs_51_sfr_read(hs_mcs_51_core_t * core,hs_mcs_51_sfr_addr_t addr,uint8_t *data)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,data,1,core->usr);
    }
    return false;
}

bool hs_mcs_51_sfr_write(hs_mcs_51_core_t * core,hs_mcs_51_sfr_addr_t addr,uint8_t data)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&data,1,core->usr);
    }
    return false;
}

uint16_t hs_mcs_51_pc_get(hs_mcs_51_core_t * core)
{
    if(core!=NULL)
    {
        return core->pc;
    }
    return 0;
}

bool hs_mcs_51_bit_read(hs_mcs_51_core_t * core,uint8_t bitaddr)
{
    return hs_mcs_51_sfr_bit_read(core,bitaddr);
}

bool hs_mcs_51_bit_write(hs_mcs_51_core_t * core,uint8_t bitaddr,bool value)
{
    hs_mcs_51_sfr_bit_write(core,bitaddr,value);
    return hs_mcs_51_bit_read(core,bitaddr);
}
