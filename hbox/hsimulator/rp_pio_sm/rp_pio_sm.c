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
        uint32_t stall:1;//Stall
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
        hs_rp_pio_sm_reset(sm);
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
        case HS_RP_PIO_SM_STALL_STATUS:
        {
            (*val)=sm->stall;
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

static void hs_rp_pio_sm_exec_bigendian_fix(hs_rp_pio_sm_instruction_t *instruction)
{
    if(instruction!=NULL)
    {
        uint16_t instruction_value=instruction->Instruction;
        instruction->Instruction=0x8000;
        if(instruction->Instruction_Class!=0)
        {
            //小端模式，复原指令
            instruction->Instruction=instruction_value;
        }
        else
        {
            //大端模式，重新解析指令
            instruction->Instruction=0;
            instruction->Instruction_Class=((instruction_value>>13)&0x7);
            instruction->Delay_SideSet=((instruction_value>>8)&0x1F);
            switch(instruction->Instruction_Class)
            {
            case HS_RP_PIO_SM_INS_CLASS_JMP:           //JMP指令
            {
                instruction->INS_JMP.Condition=((instruction_value>>5)&0x7);
                instruction->INS_JMP.Address=((instruction_value)&0x1F);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_WAIT:          //WAIT指令
            {
                instruction->INS_WAIT.Pol=((instruction_value>>7)&0x1);
                instruction->INS_WAIT.Source=((instruction_value>>5)&0x3);
                instruction->INS_WAIT.Index=((instruction_value)&0x1F);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_IN:            //IN指令
            {
                instruction->INS_IN.Source=((instruction_value>>5)&0x7);
                instruction->INS_IN.Bit_count=((instruction_value)&0x1F);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_OUT:           //OUT指令
            {
                instruction->INS_OUT.Destination=((instruction_value>>5)&0x7);
                instruction->INS_OUT.Bit_count=((instruction_value)&0x1F);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_PUSH_MOV_PULL: //PUSH、MOV、PULL指令
            {
                instruction->INS_PUSH_MOV_PULL.Pull=((instruction_value>>7)&0x1);
                instruction->INS_PUSH_MOV_PULL.ifF_ifE=((instruction_value>>6)&0x1);
                instruction->INS_PUSH_MOV_PULL.Blk=((instruction_value>>5)&0x1);
                instruction->INS_PUSH_MOV_PULL.Mov=((instruction_value>>4)&0x1);
                instruction->INS_PUSH_MOV_PULL.Idxl=((instruction_value>>3)&0x1);
                instruction->INS_PUSH_MOV_PULL.Index=((instruction_value)&0x3);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_MOV:           //MOV指令
            {
                instruction->INS_MOV.Destination=((instruction_value>>5)&0x7);
                instruction->INS_MOV.Op=((instruction_value>>3)&0x3);
                instruction->INS_MOV.Source=((instruction_value)&0x7);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_IRQ:           //IRQ指令
            {
                instruction->INS_IRQ.Clr=((instruction_value>>6)&0x1);
                instruction->INS_IRQ.Wait=((instruction_value>>5)&0x1);
                instruction->INS_IRQ.IdxMode=((instruction_value>>3)&0x3);
                instruction->INS_IRQ.Index=((instruction_value)&0x7);
            }
            break;
            case HS_RP_PIO_SM_INS_CLASS_SET:           //SET指令
            {
                instruction->INS_SET.Destination=((instruction_value>>5)&0x7);
                instruction->INS_SET.Data=((instruction_value)&0x1F);
            }
            break;
            }
        }
    }
}

void hs_rp_pio_sm_exec(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_instruction_t instruction)
{
    if(sm==NULL || sm->io==NULL)
    {
        return;
    }

    //大端模式修复
    hs_rp_pio_sm_exec_bigendian_fix(&instruction);

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
                sm->stall=0;
            }
            else
            {
                sm->stall=1;
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
                sm->stall=0;
            }
            else
            {
                sm->stall=1;
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
                sm->stall=0;
            }
            else
            {
                sm->stall=1;
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
                sm->stall=0;
            }
            else
            {
                sm->stall=1;
            }
        }
        break;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_IN:            //IN指令
    {
        uint8_t bit_count=instruction.INS_IN.Bit_count;
        if(bit_count==0)
        {
            bit_count=32;
        }
        //输入数据掩码
        uint32_t bit_mask=((((uint64_t)1ULL)<<(bit_count))-1);
        uint32_t in_data=0;
        switch(instruction.INS_IN.Source)
        {
        case 0:
        {
            //PINS
            sm->io(sm,HS_RP_PIO_SM_IO_READ_PINS,&in_data,sm->usr);
            in_data&=bit_mask;
        }
        break;
        case 1:
        {
            //X
            in_data=sm->x;
            in_data&=bit_mask;
        }
        break;
        case 2:
        {
            //Y
            in_data=sm->y;
            in_data&=bit_mask;
        }
        break;
        case 3:
        {
            //NULL
            in_data=0;
            in_data&=bit_mask;
        }
        break;
        case 6:
        {
            //ISR
            in_data=sm->isr;
            in_data&=bit_mask;
        }
        break;
        case 7:
        {
            //OSR
            in_data=sm->osr;
            in_data&=bit_mask;
        }
        break;
        default:
        {

        }
        break;
        }

        if(sm->autopush)
        {
            if(sm->stall)
            {
                //自动PUSH
                uint32_t val=sm->isr;
                if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&val,sm->usr))
                {
                    //PUSH成功
                    sm->isr=0;
                    sm->isr_shift_cnt=0;
                    sm->pc++;
                    sm->stall=0;
                }
            }
            else
            {
                uint8_t new_isr_shift_cnt=sm->isr_shift_cnt;
                new_isr_shift_cnt+=bit_count;
                if(sm->in_shiftdir)
                {
                    //右移
                    sm->isr >>= bit_count;
                    sm->isr|=(in_data<<(32-bit_count));
                }
                else
                {
                    //左移
                    sm->isr <<= bit_count;
                    sm->isr|=in_data;
                }
                if(new_isr_shift_cnt>32)
                {
                    new_isr_shift_cnt=32;
                }
                sm->isr_shift_cnt=new_isr_shift_cnt;
                uint8_t thresh=sm->push_thresh;
                if(thresh==0)
                {
                    thresh=32;
                }
                if(sm->isr_shift_cnt >= thresh)
                {
                    //自动PUSH
                    uint32_t val=sm->isr;
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&val,sm->usr))
                    {
                        //PUSH成功
                        sm->isr=0;
                        sm->isr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                    else
                    {
                        sm->stall=1;
                    }

                }
                else
                {
                    sm->pc++;
                    sm->stall=0;
                }
            }
        }
        else
        {
            uint8_t new_isr_shift_cnt=sm->isr_shift_cnt;
            new_isr_shift_cnt+=bit_count;
            if(sm->in_shiftdir)
            {
                //右移
                sm->isr >>= bit_count;
                sm->isr|=(in_data<<(32-bit_count));
            }
            else
            {
                //左移
                sm->isr <<= bit_count;
                sm->isr|=in_data;
            }
            if(new_isr_shift_cnt>32)
            {
                new_isr_shift_cnt=32;
            }
            sm->isr_shift_cnt=new_isr_shift_cnt;
            sm->pc++;
            sm->stall=0;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_OUT:           //OUT指令
    {
        uint8_t bit_count=instruction.INS_OUT.Bit_count;
        if(bit_count==0)
        {
            bit_count=32;
        }
        //输入数据掩码
        uint32_t bit_mask=((((uint64_t)1ULL)<<(bit_count))-1);

        if(sm->autopull==0)
        {
            uint32_t out_data=0;
            {
                uint8_t new_osr_shift_cnt=sm->osr_shift_cnt;
                new_osr_shift_cnt+=bit_count;
                if(new_osr_shift_cnt > 32)
                {
                    new_osr_shift_cnt=32;
                }
                sm->osr_shift_cnt=new_osr_shift_cnt;
            }
            if(sm->out_shiftdir)
            {
                //右移
                out_data = (bit_mask &  sm->osr);
                sm->osr >>=  bit_count;
            }
            else
            {
                //左移
                bit_mask <<= (32-bit_count);
                out_data = ((sm->osr & bit_mask)>> (32-bit_count));
                sm->osr <<=  bit_count;
            }
            switch(instruction.INS_OUT.Destination)
            {
            case 0:
            {
                //PINS
                sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&out_data,sm->usr);
                sm->pc++;
            }
            break;
            case 1:
            {
                //X
                sm->x=out_data;
                sm->pc++;
            }
            break;
            case 2:
            {
                //Y
                sm->y=out_data;
                sm->pc++;
            }
            break;
            case 3:
            {
                //NULL
                sm->pc++;
            }
            break;
            case 4:
            {
                //PINDIR
                sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&out_data,sm->usr);
                sm->pc++;
            }
            break;
            case 5:
            {
                sm->pc=out_data;
            }
            break;
            case 6:
            {
                //ISR
                if(sm->in_shiftdir)
                {
                    //右移
                    sm->isr >>= bit_count;
                    sm->isr|=(out_data<<(32-bit_count));
                }
                else
                {
                    //左移
                    sm->isr <<= bit_count;
                    sm->isr|=out_data;
                }
                sm->isr_shift_cnt=bit_count;
                sm->pc++;
            }
            break;
            case 7:
            {
                //EXEC
                hs_rp_pio_sm_instruction_t new_instruction;
                new_instruction.Instruction=out_data;
                hs_rp_pio_sm_exec(sm,new_instruction);
            }
            break;
            default:
            {

            }
            break;
            }
        }
        else
        {
            if(sm->stall)
            {
                uint32_t val=0;
                if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
                {
                    {
                        uint32_t out_data=0;
                        {
                            uint8_t new_osr_shift_cnt=sm->osr_shift_cnt;
                            new_osr_shift_cnt+=bit_count;
                            if(new_osr_shift_cnt > 32)
                            {
                                new_osr_shift_cnt=32;
                            }
                            sm->osr_shift_cnt=new_osr_shift_cnt;
                        }
                        if(sm->out_shiftdir)
                        {
                            //右移
                            out_data = (bit_mask &  sm->osr);
                            sm->osr >>=  bit_count;
                        }
                        else
                        {
                            //左移
                            bit_mask <<= (32-bit_count);
                            out_data = ((sm->osr & bit_mask)>> (32-bit_count));
                            sm->osr <<=  bit_count;
                        }
                        switch(instruction.INS_OUT.Destination)
                        {
                        case 0:
                        {
                            //PINS
                            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&out_data,sm->usr);
                            sm->pc++;
                        }
                        break;
                        case 1:
                        {
                            //X
                            sm->x=out_data;
                            sm->pc++;
                        }
                        break;
                        case 2:
                        {
                            //Y
                            sm->y=out_data;
                            sm->pc++;
                        }
                        break;
                        case 3:
                        {
                            //NULL
                            sm->pc++;
                        }
                        break;
                        case 4:
                        {
                            //PINDIR
                            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&out_data,sm->usr);
                            sm->pc++;
                        }
                        break;
                        case 5:
                        {
                            sm->pc=out_data;
                        }
                        break;
                        case 6:
                        {
                            //ISR
                            if(sm->in_shiftdir)
                            {
                                //右移
                                sm->isr >>= bit_count;
                                sm->isr|=(out_data<<(32-bit_count));
                            }
                            else
                            {
                                //左移
                                sm->isr <<= bit_count;
                                sm->isr|=out_data;
                            }
                            sm->isr_shift_cnt=bit_count;
                            sm->pc++;
                        }
                        break;
                        case 7:
                        {
                            //EXEC
                            hs_rp_pio_sm_instruction_t new_instruction;
                            new_instruction.Instruction=out_data;
                            hs_rp_pio_sm_exec(sm,new_instruction);
                        }
                        break;
                        default:
                        {

                        }
                        break;
                        }
                    }
                    sm->osr=val;
                    sm->osr_shift_cnt=0;
                    sm->stall=0;
                }
            }
            else
            {
                uint32_t thresh=sm->pull_thresh;
                if(thresh==0)
                {
                    thresh=32;
                }
                if((sm->osr_shift_cnt+bit_count) < thresh)
                {
                    //无需PULL
                    uint32_t out_data=0;
                    {
                        uint8_t new_osr_shift_cnt=sm->osr_shift_cnt;
                        new_osr_shift_cnt+=bit_count;
                        if(new_osr_shift_cnt > 32)
                        {
                            new_osr_shift_cnt=32;
                        }
                        sm->osr_shift_cnt=new_osr_shift_cnt;
                    }
                    if(sm->out_shiftdir)
                    {
                        //右移
                        out_data = (bit_mask &  sm->osr);
                        sm->osr >>=  bit_count;
                    }
                    else
                    {
                        //左移
                        bit_mask <<= (32-bit_count);
                        out_data = ((sm->osr & bit_mask)>> (32-bit_count));
                        sm->osr <<=  bit_count;
                    }
                    switch(instruction.INS_OUT.Destination)
                    {
                    case 0:
                    {
                        //PINS
                        sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&out_data,sm->usr);
                        sm->pc++;
                    }
                    break;
                    case 1:
                    {
                        //X
                        sm->x=out_data;
                        sm->pc++;
                    }
                    break;
                    case 2:
                    {
                        //Y
                        sm->y=out_data;
                        sm->pc++;
                    }
                    break;
                    case 3:
                    {
                        //NULL
                        sm->pc++;
                    }
                    break;
                    case 4:
                    {
                        //PINDIR
                        sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&out_data,sm->usr);
                        sm->pc++;
                    }
                    break;
                    case 5:
                    {
                        sm->pc=out_data;
                    }
                    break;
                    case 6:
                    {
                        //ISR
                        if(sm->in_shiftdir)
                        {
                            //右移
                            sm->isr >>= bit_count;
                            sm->isr|=(out_data<<(32-bit_count));
                        }
                        else
                        {
                            //左移
                            sm->isr <<= bit_count;
                            sm->isr|=out_data;
                        }
                        sm->isr_shift_cnt=bit_count;
                        sm->pc++;
                    }
                    break;
                    case 7:
                    {
                        //EXEC
                        hs_rp_pio_sm_instruction_t new_instruction;
                        new_instruction.Instruction=out_data;
                        hs_rp_pio_sm_exec(sm,new_instruction);
                    }
                    break;
                    default:
                    {

                    }
                    break;
                    }
                }
                else
                {
                    //设置stall,准备pull
                    sm->stall=1;
                }
            }
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_PUSH_MOV_PULL: //PUSH、MOV、PULL指令
    {
        if(instruction.INS_PUSH_MOV_PULL.Mov)
        {
            if(instruction.INS_PUSH_MOV_PULL.Pull)
            {
                //MOV(From RX)
                uint8_t index=0;
                if(instruction.INS_PUSH_MOV_PULL.Idxl)
                {
                    index=(instruction.INS_PUSH_MOV_PULL.Index&0x3);
                }
                else
                {
                    index=(sm->y&0x3);
                }
                uint32_t val=0;
                sm->io(sm,(hs_rp_pio_sm_io_opt_t)((int)HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_0+index),&val,sm->usr);
                sm->osr=val;
                sm->osr_shift_cnt=0;
            }
            else
            {
                //MOV(To RX)
                uint8_t index=0;
                if(instruction.INS_PUSH_MOV_PULL.Idxl)
                {
                    index=(instruction.INS_PUSH_MOV_PULL.Index&0x3);
                }
                else
                {
                    index=(sm->y&0x3);
                }
                uint32_t val=sm->isr;
                sm->io(sm,(hs_rp_pio_sm_io_opt_t)((int)HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_0+index),&val,sm->usr);
            }
            sm->pc++;
        }
        else
        {
            //Pull Push
            if(instruction.INS_PUSH_MOV_PULL.Pull)
            {
                //Pull
                if(instruction.INS_PUSH_MOV_PULL.ifF_ifE)
                {
                    uint8_t thresh=sm->pull_thresh;
                    if(thresh==0)
                    {
                        thresh=32;
                    }
                    if(sm->osr_shift_cnt < thresh)
                    {
                        sm->pc++;
                        //do nothing
                        break;
                    }
                }

                if(instruction.INS_PUSH_MOV_PULL.Blk)
                {
                    //block
                    uint32_t val=0;
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
                    {
                        sm->osr=val;
                        sm->osr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                    else
                    {
                        sm->stall=1;
                    }
                }
                else
                {
                    //noblock
                    uint32_t val=0;
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
                    {
                        sm->osr=val;
                        sm->osr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                    else
                    {
                        //读取X的值
                        sm->osr=sm->x;
                        sm->osr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                }

            }
            else
            {
                //Push
                if(instruction.INS_PUSH_MOV_PULL.ifF_ifE)
                {
                    uint8_t thresh=sm->push_thresh;
                    if(thresh==0)
                    {
                        thresh=32;
                    }
                    if(sm->isr_shift_cnt < thresh)
                    {
                        sm->pc++;
                        //do nothing
                        break;
                    }
                }

                if(instruction.INS_PUSH_MOV_PULL.Blk)
                {
                    //block
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&sm->isr,sm->usr))
                    {
                        sm->isr=0;
                        sm->isr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                    else
                    {
                        sm->stall=1;
                    }
                }
                else
                {
                    //noblock
                    sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&sm->isr,sm->usr);
                    {
                        sm->isr=0;
                        sm->isr_shift_cnt=0;
                        sm->pc++;
                        sm->stall=0;
                    }
                }
            }
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_MOV:           //MOV指令
    {
        uint32_t data=0;
        switch(instruction.INS_MOV.Source)
        {
        case 0:
        {
            //PINS
            sm->io(sm,HS_RP_PIO_SM_IO_READ_PINS,&data,sm->usr);
        }
        break;
        case 1:
        {
            //X
            data=sm->x;
        }
        break;
        case 2:
        {
            //Y
            data=sm->y;
        }
        break;
        case 3:
        {
            //NULL
            data=0;
        }
        break;
        case 5:
        {
            //STATUS
            sm->io(sm,HS_RP_PIO_SM_IO_READ_STATUS,&data,sm->usr);
        }
        break;
        case 6:
        {
            //ISR
            data=sm->isr;
        }
        break;
        case 7:
        {
            //OSR
            data=sm->osr;
        }
        default:
        {

        }
        break;
        }

        switch(instruction.INS_MOV.Op)
        {
        case 0:
        {
            //None
        }
        break;
        case 1:
        {
            //Invert (bitwise complement)
            data=~data;
        }
        break;
        case 2:
        {
            //Bit-reverse
            uint32_t new_data=0;
            for(size_t i=0; i<32; i++)
            {
                if((data&(1ULL<<i))!=0)
                {
                    new_data|=(1ULL << (31-i));
                }
            }
            data=new_data;
        }
        break;
        default:
        {

        }
        break;
        }

        switch(instruction.INS_MOV.Destination)
        {
        case 0:
        {
            //PINS
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&data,sm->usr);
            sm->pc++;
        }
        break;
        case 1:
        {
            //X
            sm->x=data;
            sm->pc++;
        }
        break;
        case 2:
        {
            //Y
            sm->y=data;
            sm->pc++;
        }
        break;
        case 3:
        {
            //PINDIR
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&data,sm->usr);
            sm->pc++;
        }
        break;
        case 4:
        {
            //Exec
            {
                hs_rp_pio_sm_instruction_t new_instruction;
                new_instruction.Instruction=data;
                hs_rp_pio_sm_exec(sm,new_instruction);
            }
        }
        break;
        case 5:
        {
            //PC
            sm->pc=data;
        }
        break;
        case 6:
        {
            //ISR
            sm->isr=data;
            sm->isr_shift_cnt=0;
            sm->pc++;
        }
        break;
        case 7:
        {
            //OSR
            sm->osr=data;
            sm->osr_shift_cnt=0;
            sm->pc++;
        }
        break;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_IRQ:           //IRQ指令
    {
        //两个字段组合
        uint8_t index=(((uint32_t)instruction.INS_IRQ.IdxMode) << 3)+instruction.INS_IRQ.Index;
        if(instruction.INS_IRQ.Clr)
        {
            uint32_t irq=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&irq,sm->usr);
            irq&=~(0x1ULL << index);
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_IRQ,&irq,sm->usr);
            sm->pc++;
        }
        else
        {
            uint32_t irq=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&irq,sm->usr);
            if(!sm->stall)
            {
                irq|=(0x1ULL << index);
                sm->io(sm,HS_RP_PIO_SM_IO_WRITE_IRQ,&irq,sm->usr);
            }
            if(instruction.INS_IRQ.Wait)
            {
                if((irq&(1ULL<<index))==0)
                {
                    sm->pc++;
                    sm->stall=0;
                }
                else
                {
                    sm->stall=1;
                }
            }
            else
            {
                sm->pc++;
                sm->stall=0;
            }
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_SET:            //SET指令
    {
        switch(instruction.INS_SET.Destination)
        {
        case 0:
        {
            //PINS
            uint32_t val=instruction.INS_SET.Data;
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&val,sm->usr);
        }
        break;
        case 1:
        {
            //X
            uint32_t val=instruction.INS_SET.Data;
            sm->x=val;
        }
        break;
        case 2:
        {
            //Y
            uint32_t val=instruction.INS_SET.Data;
            sm->y=val;
        }
        break;
        case 4:
        {
            //PINDIRS
            uint32_t val=instruction.INS_SET.Data;
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&val,sm->usr);
        }
        break;
        default:
        {

        }
        break;
        }
        sm->pc++;
    }
    break;
    }
}

void hs_rp_pio_sm_tick(hs_rp_pio_sm_t *sm,size_t cycles)
{
    if(sm!=NULL&&sm->io!=NULL)
    {

        while((cycles--)!=0)
        {
            hs_rp_pio_sm_instruction_t instruction;
            {
                uint32_t val=sm->pc;
                sm->io(sm,HS_RP_PIO_SM_IO_READ_INSTRUCTION,&val,sm->usr);
                instruction.Instruction=val;
            }

            //若出现Stall，则等待Stall结束后再进行延时操作。
            if(sm->stall==0)
            {
                if(sm->delay!=0)
                {
                    sm->delay--;
                    //等待延时结束
                    continue;
                }
            }

            hs_rp_pio_sm_exec(sm,instruction);
        }
    }
}


void hs_rp_pio_sm_reset(hs_rp_pio_sm_t *sm)
{
    if(sm!=NULL)
    {
        hs_rp_pio_io_t io=sm->io;
        void *usr=sm->usr;
        memset(sm,0,hs_rp_pio_sm_size());
        sm->io=io;
        sm->usr=usr;
        //默认为空
        sm->osr_shift_cnt=32;
        //默认右移
        sm->out_shiftdir=1;
        //默认右移
        sm->in_shiftdir=1;

        //复位IO
        if(sm->io!=NULL)
        {
            uint32_t val=0;
            sm->io(sm,HS_RP_PIO_SM_IO_RESET,&val,sm->usr);
        }
    }
}


void hs_rp_pio_sm_fifo_init(hs_rp_pio_sm_fifo_t *sm_fifo)
{
    if(sm_fifo!=NULL)
    {
        memset(sm_fifo,0,sizeof(hs_rp_pio_sm_fifo_t));
        sm_fifo->is_empty=1;
    }
}

bool hs_rp_pio_sm_fifo_is_empty(hs_rp_pio_sm_fifo_t *sm_fifo)
{
    if(sm_fifo!=NULL)
    {
        if(sm_fifo->read_ptr==sm_fifo->write_ptr)
        {
            return sm_fifo->is_empty!=0;
        }
    }
    return false;
}

bool hs_rp_pio_sm_fifo_is_full(hs_rp_pio_sm_fifo_t *sm_fifo)
{
    if(sm_fifo!=NULL)
    {
        if(sm_fifo->read_ptr==sm_fifo->write_ptr)
        {
            return sm_fifo->is_full!=0;
        }
    }
    return true;
}

bool hs_rp_pio_sm_fifo_push(hs_rp_pio_sm_fifo_t *sm_fifo,uint32_t data)
{
    if(sm_fifo!=NULL)
    {
        if(sm_fifo->disable_fifo)
        {
            //已关闭FIFO
            return false;
        }


        if(sm_fifo->read_ptr==sm_fifo->write_ptr)
        {
            if(hs_rp_pio_sm_fifo_is_full(sm_fifo))
            {
                return false;
            }
            if(hs_rp_pio_sm_fifo_is_empty(sm_fifo))
            {
                //正常写入
                sm_fifo->fifo[sm_fifo->write_ptr++]=data;
                sm_fifo->is_empty=0;
            }
            else
            {
                //写入最后一个空位
                sm_fifo->fifo[sm_fifo->write_ptr]=data;
                sm_fifo->is_full=1;
            }
        }
        else
        {
            //读写指针不相等，直接写入
            sm_fifo->fifo[sm_fifo->write_ptr++]=data;
            if(sm_fifo->read_ptr==sm_fifo->write_ptr)
            {
                sm_fifo->is_full=1;
            }
        }

        return true;
    }
    return false;
}

bool hs_rp_pio_sm_fifo_pull(hs_rp_pio_sm_fifo_t *sm_fifo,uint32_t* data)
{
    if(sm_fifo!=NULL && data!=NULL)
    {
        if(sm_fifo->disable_fifo)
        {
            //已关闭FIFO
            return false;
        }



        if(sm_fifo->read_ptr==sm_fifo->write_ptr)
        {
            if(hs_rp_pio_sm_fifo_is_empty(sm_fifo))
            {
                return false;
            }

            if(hs_rp_pio_sm_fifo_is_full(sm_fifo))
            {
                //正常读取
                (*data)=sm_fifo->fifo[sm_fifo->read_ptr++];
                sm_fifo->is_full=0;
            }
            else
            {
                //读取最后一个有效数据
                (*data)=sm_fifo->fifo[sm_fifo->read_ptr];
                sm_fifo->is_empty=1;
            }
        }
        else
        {
            //读写指针不相等，直接读取
            (*data)=sm_fifo->fifo[sm_fifo->read_ptr++];
            if(sm_fifo->read_ptr==sm_fifo->write_ptr)
            {
                sm_fifo->is_empty=1;
            }
        }

        return true;
    }
    return false;
}

void hs_rp_pio_sm_memory_init(hs_rp_pio_sm_memory_t *sm_mem)
{
    if(sm_mem!=NULL)
    {
        memset(sm_mem,0,sizeof(hs_rp_pio_sm_memory_t));
        //默认设置
        sm_mem->in_shiftdir=1;
        sm_mem->out_shiftdir=1;
    }
}

void hs_rp_pio_sm_load_memory_cfg(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_fifo_t *sm_rxfifo,const hs_rp_pio_sm_memory_t *sm_mem)
{
    if(sm_mem!=NULL)
    {
        if(sm_rxfifo!=NULL)
        {
            sm_rxfifo->disable_fifo=sm_mem->disable_rxfifo;
        }

        if(sm!=NULL)
        {
            sm->pull_thresh=sm_mem->pull_thresh;
            sm->push_thresh=sm_mem->push_thresh;
            sm->out_shiftdir=sm_mem->out_shiftdir;
            sm->in_shiftdir=sm_mem->in_shiftdir;
            sm->autopull=sm_mem->autopull;
            sm->autopush=sm_mem->autopush;
            sm->sideset_cnt=(sm_mem->sideset_cnt%6);
        }
    }
}

static bool hs_rp_pio_io_process(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_io_opt_t opt,uint32_t *val,void *usr)
{
    bool ret=true;
    hs_rp_pio_t *pio=(hs_rp_pio_t *)sm;
    if(pio==NULL || val==NULL)
    {
        return false;
    }
    switch(opt)
    {
    case HS_RP_PIO_SM_IO_RESET:                 //IO复位，无参数
    {
        hs_rp_pio_sm_fifo_init(&pio->txfifo);
        hs_rp_pio_sm_fifo_init(&pio->rxfifo);
        hs_rp_pio_sm_load_memory_cfg(sm,&pio->rxfifo,&pio->memory);
    }
    break;
    case HS_RP_PIO_SM_IO_READ_INSTRUCTION:       //指令读取，传入PC的值(通过*val访问)，传出指令（低16位）
    {
        (*val)=pio->memory.code[(*val)];
    }
    break;
    case HS_RP_PIO_SM_IO_PUSH_RX_FIFO:           //写RX_FIFO，写32位数据
    {
        ret=hs_rp_pio_sm_fifo_push(&pio->rxfifo,(*val));
    }
    break;
    case HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_0:     //读RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            (*val)=pio->rxfifo.fifo[0];
        }
    }
    break;
    case HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_1:    //读RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            (*val)=pio->rxfifo.fifo[1];
        }
    }
    break;
    case HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_2:    //读RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            (*val)=pio->rxfifo.fifo[2];
        }
    }
    break;
    case HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_3:    //读RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            (*val)=pio->rxfifo.fifo[3];
        }
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_0:   //写RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            pio->rxfifo.fifo[0]=(*val);
        }
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_1:   //写RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            pio->rxfifo.fifo[1]=(*val);
        }
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_2:   //写RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            pio->rxfifo.fifo[2]=(*val);
        }
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_3:   //写RX_FIFO，写32位数据
    {
        if(pio->rxfifo.disable_fifo)
        {
            pio->rxfifo.fifo[3]=(*val);
        }
    }
    break;
    case HS_RP_PIO_SM_IO_PULL_TX_FIFO:          //读TX_FIFO，读32位数据
    {
        ret=hs_rp_pio_sm_fifo_pull(&pio->txfifo,val);
    }
    break;
    case HS_RP_PIO_SM_IO_READ_PINS:             //读PINS,32位数据，每一位表示一个PIN
    {
        (*val)=pio->pins;
    }
    break;
    case HS_RP_PIO_SM_IO_READ_JMP_PIN:          //读取用于JMP的PIN,32位数据，每一位表示一个PIN，位0表示JMP指令使用的PIN
    {
        (*val)=pio->jmp_pin;
    }
    break;
    case HS_RP_PIO_SM_IO_READ_GPIO:             //读GPIO(未经映射)，32位数据，每一位表示一个GPIO。
    {
        (*val)=pio->gpio;
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_PINS:            //写PINS,32位数据，每一位表示一个PIN
    {
        pio->pins=(*val);
    }
    break;
    case HS_RP_PIO_SM_IO_READ_PINDIRS:          //读PINDIR,32位数据，每一位表示一个PINDIR
    {
        (*val)=pio->pindirs;
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_PINDIRS:         //写PINDIR,32位数据，每一位表示一个PINDIR
    {
        pio->pindirs=(*val);
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_SIDESET:         //SIDESET可参考HS_RP_PIO_SM_IO_WRITE_PINS,低位有效
    {
        pio->sideset=(*val);
    }
    break;
    case HS_RP_PIO_SM_IO_READ_STATUS:           //读STATUS,32位数据，通常是全0或者全1，由MOV指令使用
    {
        (*val)=pio->status;
    }
    break;
    case HS_RP_PIO_SM_IO_READ_IRQ:              //读IRQ,32位数据，位地址由IdxMode(2bit)+Index(3bit)组成
    {
        (*val)=pio->irq;
    }
    break;
    case HS_RP_PIO_SM_IO_WRITE_IRQ:             //写IRQ,32位数据，位地址由IdxMode(2bit)+Index(3bit)组成,通常用于清除中断。
    {
        pio->irq=(*val);
    }
    break;
    default:
    {

    }
    break;
    }
    if(pio->hook!=NULL)
    {
        pio->hook(sm,opt,val,usr);
    }
    return ret;
}

void hs_rp_pio_init(hs_rp_pio_t *pio,hs_rp_pio_io_t hook,void *usr)
{
    if(pio!=NULL)
    {
        memset(pio,0,sizeof(hs_rp_pio_t));
        hs_rp_pio_sm_memory_init(&pio->memory);
        pio->hook=hook;
        hs_rp_pio_sm_init(pio,hs_rp_pio_io_process,usr);
    }
}

void hs_rp_pio_tick(hs_rp_pio_t *pio,size_t cycles)
{
    hs_rp_pio_sm_tick((hs_rp_pio_sm_t *)pio,cycles);
}

void hs_rp_pio_reset(hs_rp_pio_t *pio)
{
    hs_rp_pio_sm_reset((hs_rp_pio_sm_t *)pio);
}

bool hs_rp_pio_push(hs_rp_pio_t *pio,uint32_t data)
{
    if(pio!=NULL)
    {
        return hs_rp_pio_sm_fifo_push(&pio->txfifo,data);
    }
    return false;
}

bool hs_rp_pio_pull(hs_rp_pio_t *pio,uint32_t* data)
{
    if(pio!=NULL)
    {
        return hs_rp_pio_sm_fifo_pull(&pio->rxfifo,data);
    }
    return false;
}

bool  hs_rp_pio_rxfifo_set(hs_rp_pio_t *pio,uint32_t addr,uint32_t data)
{
    if((pio!=NULL) && (pio->rxfifo.disable_fifo==1))
    {
        pio->rxfifo.fifo[addr%4]=data;
        return true;
    }
    return false;
}

bool  hs_rp_pio_rxfifo_get(hs_rp_pio_t *pio,uint32_t addr,uint32_t* data)
{
    if((pio!=NULL) && (pio->rxfifo.disable_fifo==1))
    {
        if(data!=NULL)
        {
            (*data)=pio->rxfifo.fifo[addr%4];
        }
        return true;
    }
    return false;
}


