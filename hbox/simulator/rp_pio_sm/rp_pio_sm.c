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
                sm->io(sm,HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_0+index,&val,sm->usr);
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
                sm->io(sm,HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_0+index,&val,sm->usr);
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
    if(sm!=0&&sm->io!=NULL)
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

