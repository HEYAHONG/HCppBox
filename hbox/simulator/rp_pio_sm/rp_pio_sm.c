/***************************************************************
 * Name:      rp_pio_sm.c
 * Purpose:   实现rp_pio_sm接口,注意：rp_pio_sm主要用于模拟pio的状态机而非硬件，即主要用于模拟运行pio程序(视具体实现可能和实际硬件有所有差异)。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "rp_pio_sm.h"
#include "string.h"

struct hs_rp_pio_sm
{
    hs_rp_pio_io_t io;
    void *usr;
    uint32_t osr;//OSR寄存器
    uint32_t isr;//ISR寄存器
    uint32_t x;//X寄存器
    uint32_t y;//Y寄存器
    struct
    {
        uint32_t pc:5;//只支持5位指令地址(共32条指令)
        uint32_t delay:5;//用于指令的Delay字段
        uint32_t osr_shift_cnt:6;//OSR寄存器移动计数器,0~32
        uint32_t isr_shift_cnt:6;//移动寄存器移动计数器，0~32
        uint32_t pull_thresh:5;//PULL指令阈值,0代表阈值为32.
        uint32_t push_thresh:5;//PUSH指令阈值,0代表阈值为32.
        uint32_t out_shiftdir:1;//1=右移，0=左移
        uint32_t in_shiftdir:1;//1=右移，0=左移
        uint32_t autopull:1;//1=自动pull
        uint32_t autopush:1;//1=自动push
        uint32_t sideset_cnt:3;//0~5
    };

};

size_t hs_rp_pio_sm_size(void)
{
    return sizeof(hs_rp_pio_sm_t);
}

hs_rp_pio_sm_t *hs_rp_pio_sm_init(void *mem,hs_rp_pio_io_t io,void *usr)
{
    if(mem!=NULL)
    {
        memset(mem,0,hs_rp_pio_sm_size());
        hs_rp_pio_sm_t *sm=(hs_rp_pio_sm_t *)mem;
        sm->io=io;
        sm->usr=usr;
        sm->osr_shift_cnt=32;
        sm->out_shiftdir=1;
        sm->in_shiftdir=1;
        return sm;
    }
    return NULL;
}

void hs_rp_pio_sm_cfg(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_cfg_opt_t opt,uint32_t val)
{
    if(sm!=NULL)
    {
        switch(opt)
        {
        case HS_RP_PIO_SM_PULL_THRESH:
        {
            sm->pull_thresh=val;
        }
        break;
        case HS_RP_PIO_SM_PUSH_THRESH:
        {
            sm->push_thresh=val;
        }
        break;
        case HS_RP_PIO_SM_OUT_SHIFTDIR:
        {
            sm->out_shiftdir=val;
        }
        break;
        case HS_RP_PIO_SM_IN_SHIFTDIR:
        {
            sm->in_shiftdir=val;
        }
        break;
        case HS_RP_PIO_SM_AUTOPULL:
        {
            sm->autopull=val;
        }
        break;
        case HS_RP_PIO_SM_AUTOPUSH:
        {
            sm->autopush=val;
        }
        break;
        case HS_RP_PIO_SM_SIDESET_CNT:
        {
            sm->sideset_cnt=(val%6);
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void hs_rp_pio_sm_status(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_status_opt_t opt,uint32_t *val)
{
    if(sm!=NULL && val!=NULL)
    {
        switch(opt)
        {
        case HS_RP_PIO_SM_OSR_STATUS:
        {
            (*val)=sm->osr;
        }
        break;
        case HS_RP_PIO_SM_ISR_STATUS:
        {
            (*val)=sm->isr;
        }
        break;
        case HS_RP_PIO_SM_X_STATUS:
        {
            (*val)=sm->x;
        }
        break;
        case HS_RP_PIO_SM_Y_STATUS:
        {
            (*val)=sm->y;
        }
        break;
        case HS_RP_PIO_SM_PC_STATUS:
        {
            (*val)=sm->pc;
        }
        break;
        case HS_RP_PIO_SM_DELAY_STATUS:
        {
            (*val)=sm->delay;
        }
        break;
        case HS_RP_PIO_SM_OSR_SHIFT_CNT_STATUS:
        {
            (*val)=sm->osr_shift_cnt;
        }
        break;
        case HS_RP_PIO_SM_ISR_SHIFT_CNT_STATUS:
        {
            (*val)=sm->isr_shift_cnt;
        }
        break;
        case HS_RP_PIO_SM_PULL_THRESH_STATUS:
        {
            (*val)=sm->pull_thresh;
        }
        break;
        case HS_RP_PIO_SM_PUSH_THRESH_STATUS:
        {
            (*val)=sm->push_thresh;
        }
        break;
        case HS_RP_PIO_SM_OUT_SHIFTDIR_STATUS:
        {
            (*val)=sm->out_shiftdir;
        }
        break;
        case HS_RP_PIO_SM_IN_SHIFTDIR_STATUS:
        {
            (*val)=sm->in_shiftdir;
        }
        break;
        case HS_RP_PIO_SM_AUTOPULL_STATUS:
        {
            (*val)=sm->autopull;
        }
        break;
        case HS_RP_PIO_SM_AUTOPUSH_STATUS:
        {
            (*val)=sm->autopush;
        }
        break;
        case HS_RP_PIO_SM_SIDESET_CNT_STATUS:
        {
            (*val)=sm->sideset_cnt;
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void hs_rp_pio_sm_exec(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_instruction_t instruction)
{
    if(sm==NULL || sm->io==NULL)
    {
        return;
    }
    //Delay Sideset特性
    if(sm->sideset_cnt > 0)
    {
        uint32_t val=instruction.Delay_SideSet>>(5-sm->sideset_cnt);
        sm->io(sm,HS_RP_PIO_SM_IO_WRITE_SIDESET,&val,sm->usr);
        sm->delay=instruction.Delay_SideSet&((1ULL << (5-sm->sideset_cnt))-1);
    }
    else
    {
        sm->delay=instruction.Delay_SideSet;
    }

    switch(instruction.Instruction_Class)
    {
    case HS_RP_PIO_SM_INS_CLASS_JMP:           //JMP指令
    {
        switch(instruction.INS_JMP.Condition)
        {
        case 0:
        {
            //无条件跳转
            sm->pc=instruction.INS_JMP.Address;
        }
        break;
        case 1:
        {
            if(!sm->x)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 2:
        {
            if(sm->x--)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 3:
        {
            if(!sm->y)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 4:
        {
            if(sm->y--)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 5:
        {
            if(sm->x!=sm->y)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 6:
        {
            uint32_t pins=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_JMP_PIN,&pins,sm->usr);
            if((pins &0x1)!=0)
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        case 7:
        {
            uint8_t cnt=sm->osr_shift_cnt;
            uint8_t thresh=sm->pull_thresh;
            if(thresh==0)
            {
                thresh=32;
            }
            if(!(cnt >= thresh))
            {
                sm->pc=instruction.INS_JMP.Address;
            }
            else
            {
                sm->pc++;
            }
        }
        break;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_WAIT:          //WAIT指令
    {
        uint32_t Pol=instruction.INS_WAIT.Pol;
        switch(instruction.INS_WAIT.Source)
        {
        case 0:
        {
            //GPIO (未经映射的)
            uint32_t val=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_GPIO,&val,sm->usr);
            if(Pol==(0x1&(val>>instruction.INS_WAIT.Index)))
            {
                sm->pc++;
            }
        }
        break;
        case 1:
        {
            //PIN
            uint32_t val=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_PINS,&val,sm->usr);
            if(Pol==(0x1&(val>>instruction.INS_WAIT.Index)))
            {
                sm->pc++;
            }
        }
        break;
        case 2:
        {
            //IRQ
            uint32_t val=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&val,sm->usr);
            if(Pol==(0x1&(val>>instruction.INS_WAIT.Index)))
            {
                sm->pc++;
                if(Pol==1)
                {
                    //自动清除中断
                    val&=~(1ULL<<instruction.INS_WAIT.Index);
                    sm->io(sm,HS_RP_PIO_SM_IO_WRITE_IRQ,&val,sm->usr);
                }
            }
        }
        break;
        case 3:
        {
            //JMPPIN
            uint32_t val=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_JMP_PIN,&val,sm->usr);
            if(Pol==(0x1&(val>>instruction.INS_WAIT.Index)))
            {
                sm->pc++;
            }
        }
        break;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_IN:            //IN指令
    {

    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_OUT:           //OUT指令
    {

    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_PUSH_MOV_PULL: //PUSH、MOV、PULL指令
    {

    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_MOV:           //MOV指令
    {

    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_IRQ:           //IRQ指令
    {

    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_SET:            //SET指令
    {

    }
    break;
    }
}

void hs_rp_pio_sm_tick(hs_rp_pio_sm_t *sm,size_t cycles)
{
    if(sm!=0&&sm->io!=NULL)
    {

        while(cycles--!=0)
        {
            hs_rp_pio_sm_instruction_t instruction;
            {
                uint32_t val=sm->pc;
                sm->io(sm,HS_RP_PIO_SM_IO_READ_INSTRUCTION,&val,sm->usr);
                instruction.Instruction=val;
            }


            if(sm->delay!=0)
            {
                sm->delay--;
                //等待延时结束
                continue;
            }

            hs_rp_pio_sm_exec(sm,instruction);
        }
    }
}

