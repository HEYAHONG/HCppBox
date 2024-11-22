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

                            uint8_t sp=0;
                            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
                            //压栈PC高字节
                            sp++;
                            {
                                if(core->io!=NULL)
                                {
                                    uint8_t val=(core->pc>>8);
                                    core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                                }
                            }
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

                            //跳转至中断入口地址
                            core->pc=address;
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
                            uint8_t sp=0;
                            hs_mcs_51_sfr_read(core,HS_MCS_51_SFR_SP,&sp);
                            //压栈PC高字节
                            sp++;
                            {
                                if(core->io!=NULL)
                                {
                                    uint8_t val=(core->pc>>8);
                                    core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,sp,&val,sizeof(val),core->usr);
                                }
                            }
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

                            //跳转至中断入口地址
                            core->pc=address;
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
    size_t ret=1;
    switch(instruction)
    {
    case 0x02:
    case 0x10:
    case 0x12:
    case 0x20:
    case 0x30:
    case 0x43:
    case 0x53:
    case 0x63:
    case 0x75:
    case 0x85:
    case 0x90:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB7:
    case 0xB8:
    case 0xB9:
    case 0xBA:
    case 0xBB:
    case 0xBC:
    case 0xBD:
    case 0xBE:
    case 0xBF:
    case 0xD5:
    {
        ret=3;
    }
    break;
    case 0x01:
    case 0x05:
    case 0x11:
    case 0x15:
    case 0x21:
    case 0x24:
    case 0x25:
    case 0x31:
    case 0x34:
    case 0x35:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x44:
    case 0x45:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x54:
    case 0x55:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x64:
    case 0x65:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x74:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0x8C:
    case 0x8D:
    case 0x8E:
    case 0x8F:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0xA0:
    case 0xA1:
    case 0xA2:
    case 0xA6:
    case 0xA7:
    case 0xA8:
    case 0xA9:
    case 0xAA:
    case 0xAB:
    case 0xAC:
    case 0xAD:
    case 0xAE:
    case 0xAF:
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xC0:
    case 0xC1:
    case 0xC2:
    case 0xC5:
    case 0xD0:
    case 0xD1:
    case 0xD2:
    case 0xD8:
    case 0xD9:
    case 0xDA:
    case 0xDB:
    case 0xDC:
    case 0xDD:
    case 0xDE:
    case 0xDF:
    case 0xE1:
    case 0xE5:
    case 0xF1:
    case 0xF5:
    {
        ret=2;
    }
    break;
    default:
    {
        ret=1;
    }
    break;
    }
    return ret;
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

static void hs_mcs_51_core_exec(hs_mcs_51_core_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        uint8_t instruction[HS_MCS_51_INSTRUCTION_MAX_LENGTH]= {0};
        core->io(core,HS_MCS_51_IO_READ_ROM,core->pc,instruction,sizeof(instruction),core->usr);
        //TODO:执行指令
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
            core->pc+=2;
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
            core->pc+=1;
        }
        break;
        case 0x04://INC A
        {
            uint8_t acc=hs_mcs_51_sfr_acc_read(core);
            //自增
            acc++;
            hs_mcs_51_sfr_acc_write(core,acc);
            core->pc+=1;
        }
        break;
        case 0x05://INC addr
        {
            uint8_t addr=instruction[1];
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,addr,&val,sizeof(val),core->usr);
            val++;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,addr,&val,sizeof(val),core->usr);
            core->pc+=2;
        }
        break;
        case 0x06://INC @R0
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+0;
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            val++;
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            core->pc+=1;
        }
        break;
        case 0x07://INC @R1
        {
            uint8_t psw=hs_mcs_51_sfr_psw_read(core);
            uint8_t Rn_address=8*((psw>>3)&0x3)+1;
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            uint8_t val=0;
            core->io(core,HS_MCS_51_IO_READ_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            val++;
            core->io(core,HS_MCS_51_IO_WRITE_HIGH_RAM,Rn,&val,sizeof(val),core->usr);
            core->pc+=1;
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
            uint8_t Rn_address=8*((psw>>3)&0x3)+instruction[0]-0x08;
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            Rn++;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            core->pc+=1;
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
            core->pc+=2;
            core->delay_tick=1;
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
            core->pc+=2;
            core->delay_tick=1;
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
            uint8_t Rn_address=8*((psw>>3)&0x3)+instruction[0]-0xE8;
            uint8_t Rn=0;
            core->io(core,HS_MCS_51_IO_READ_RAM_SFR,Rn_address,&Rn,sizeof(Rn),core->usr);
            hs_mcs_51_sfr_acc_write(core,Rn);
            core->pc+=1;
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
            uint8_t Rn_address=8*((psw>>3)&0x3)+instruction[0]-0xF8;
            core->io(core,HS_MCS_51_IO_WRITE_RAM_SFR,Rn_address,&acc,sizeof(acc),core->usr);
            core->pc+=1;
        }
        break;
        default:
        {
            //默认行为,类似NOP指令，啥也不做
            core->pc+=hs_mcs_51_core_instruction_length(instruction[0]);
        }
        break;
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

            hs_mcs_51_core_exec(core);

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
