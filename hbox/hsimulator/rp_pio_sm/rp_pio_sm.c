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
        uint32_t sideset_cnt:3;//0~5(包含可选使能位)
        uint32_t stall:1;//Stall
        uint32_t side_en:1;//sideset可选使能位，1=最高位作为使能
        uint32_t wrap_bottom:5;//程序回绕起始地址
        uint32_t wrap_top:5;//程序回绕结束地址
        uint32_t wrap_enable:1;//程序回绕使能
        uint32_t sm_id:2;//状态机编号(用于IRQ相对寻址)
        uint32_t exec_pending:1;//存在由EXEC产生的待执行指令
        uint32_t exec_no_advance:1;//执行EXEC产生的指令时不再自动递增PC
    };
    uint32_t exec_instruction;//EXEC产生的待执行指令(低16位有效)

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
            sm->sideset_cnt=(val>5?5:val);
        }
        break;
        case HS_RP_PIO_SM_SIDE_EN:
        {
            sm->side_en=(val!=0);
        }
        break;
        case HS_RP_PIO_SM_WRAP_BOTTOM:
        {
            sm->wrap_bottom=(val&0x1F);
        }
        break;
        case HS_RP_PIO_SM_WRAP_TOP:
        {
            sm->wrap_top=(val&0x1F);
        }
        break;
        case HS_RP_PIO_SM_WRAP_ENABLE:
        {
            sm->wrap_enable=(val!=0);
        }
        break;
        case HS_RP_PIO_SM_SM_ID:
        {
            sm->sm_id=(val&0x3);
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
        case HS_RP_PIO_SM_SIDE_EN_STATUS:
        {
            (*val)=sm->side_en;
        }
        break;
        case HS_RP_PIO_SM_WRAP_BOTTOM_STATUS:
        {
            (*val)=sm->wrap_bottom;
        }
        break;
        case HS_RP_PIO_SM_WRAP_TOP_STATUS:
        {
            (*val)=sm->wrap_top;
        }
        break;
        case HS_RP_PIO_SM_WRAP_ENABLE_STATUS:
        {
            (*val)=sm->wrap_enable;
        }
        break;
        case HS_RP_PIO_SM_SM_ID_STATUS:
        {
            (*val)=sm->sm_id;
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

/** \brief 获取实际使用的阈值(寄存器值0表示32)
 */
static uint32_t hs_rp_pio_sm_threshold(uint32_t thresh)
{
    return (thresh==0)?32:thresh;
}

/** \brief 按EXECCTRL_WRAP_BOTTOM/WRAP_TOP更新PC
 *
 * 由EXEC(MOV EXEC/OUT EXEC)执行的指令不再重复更新PC，因为外层指令已经完成PC更新。
 */
static void hs_rp_pio_sm_advance_pc(hs_rp_pio_sm_t *sm)
{
    if(sm==NULL || sm->exec_no_advance)
    {
        return;
    }
    if(sm->wrap_enable && sm->pc==sm->wrap_top)
    {
        sm->pc=sm->wrap_bottom;
        return;
    }
    sm->pc=(sm->pc+1)&0x1F;
}

/** \brief 解码IRQ/WAIT IRQ的索引(IdxMode+Index)
 *
 * RP2040: bit4为REL标志; RP2350: bit4~bit3为IdxMode(00=THIS,01=PREV,10=REL,11=NEXT)。
 * 当前实现仅支持本PIO块以及REL相对寻址，PREV/NEXT退化为本PIO块。
 */
static uint8_t hs_rp_pio_sm_irq_index(hs_rp_pio_sm_t *sm,uint32_t raw_index)
{
    uint8_t index=(raw_index&0x7);
    uint8_t mode=((raw_index>>3)&0x3);
    if(mode==2)
    {
        //REL:状态机编号以模4方式加到低2位
        index=(index&(~0x3))|((index+sm->sm_id)&0x3);
    }
    return index;
}

/** \brief 将数据按IN移位方向移入ISR，并更新输入移位计数器
 */
static void hs_rp_pio_sm_isr_shift_in(hs_rp_pio_sm_t *sm,uint32_t data,uint32_t bit_count)
{
    if(bit_count>=32)
    {
        sm->isr=data;
    }
    else if(sm->in_shiftdir)
    {
        //右移,新数据从高位进入
        sm->isr=(sm->isr>>bit_count)|((data&((1UL<<bit_count)-1))<<(32-bit_count));
    }
    else
    {
        //左移,新数据从低位进入
        sm->isr=(sm->isr<<bit_count)|(data&((1UL<<bit_count)-1));
    }

    {
        uint32_t shift_cnt=(uint32_t)sm->isr_shift_cnt+bit_count;
        if(shift_cnt>32)
        {
            shift_cnt=32;
        }
        sm->isr_shift_cnt=shift_cnt;
    }
}

/** \brief 按OUT移位方向从OSR移出数据，并更新输出移位计数器
 *
 * \return uint32_t 移出的数据(高位补0)
 */
static uint32_t hs_rp_pio_sm_osr_shift_out(hs_rp_pio_sm_t *sm,uint32_t bit_count)
{
    uint32_t out_data=0;
    if(bit_count>=32)
    {
        out_data=sm->osr;
        sm->osr=0;
    }
    else if(sm->out_shiftdir)
    {
        //右移,输出最低位
        out_data=sm->osr&((1UL<<bit_count)-1);
        sm->osr>>=bit_count;
    }
    else
    {
        //左移,输出最高位
        out_data=sm->osr>>(32-bit_count);
        sm->osr<<=bit_count;
    }

    {
        uint32_t shift_cnt=(uint32_t)sm->osr_shift_cnt+bit_count;
        if(shift_cnt>32)
        {
            shift_cnt=32;
        }
        sm->osr_shift_cnt=shift_cnt;
    }
    return out_data;
}

/** \brief 非OUT周期上的自动PULL
 *
 * 参考数据手册3.5.4.2:在非OUT周期，只要OSR移位计数器达到阈值且TX FIFO非空，
 * 硬件就会自动将TX FIFO中的数据加载到OSR。
 */
static void hs_rp_pio_sm_autopull_refill(hs_rp_pio_sm_t *sm)
{
    if(sm==NULL || sm->io==NULL || !sm->autopull)
    {
        return;
    }
    if(sm->osr_shift_cnt>=hs_rp_pio_sm_threshold(sm->pull_thresh))
    {
        uint32_t val=0;
        if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
        {
            sm->osr=val;
            sm->osr_shift_cnt=0;
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
        uint32_t delay_bits=5-sm->sideset_cnt;
        uint32_t side_val=(instruction.Delay_SideSet>>delay_bits)&((1UL<<sm->sideset_cnt)-1);
        bool do_sideset=true;
        if(sm->side_en)
        {
            //最高位作为使能位,低位为实际的sideset值
            uint32_t enable_mask=(1UL<<(sm->sideset_cnt-1));
            do_sideset=((side_val&enable_mask)!=0);
            side_val&=~(enable_mask);
        }
        if(do_sideset)
        {
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_SIDESET,&side_val,sm->usr);
        }
        sm->delay=instruction.Delay_SideSet&((1UL<<delay_bits)-1);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
                hs_rp_pio_sm_advance_pc(sm);
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
            uint8_t index=hs_rp_pio_sm_irq_index(sm,instruction.INS_WAIT.Index);
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&val,sm->usr);
            if(Pol==(0x1&(val>>index)))
            {
                hs_rp_pio_sm_advance_pc(sm);
                if(Pol==1)
                {
                    //自动清除中断
                    val&=~(1UL<<index);
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
            //JMPPIN(PINCTRL_JMP_PIN + Index(0~3))
            uint32_t val=0;
            uint8_t index=(instruction.INS_WAIT.Index&0x3);
            sm->io(sm,HS_RP_PIO_SM_IO_READ_JMP_PIN,&val,sm->usr);
            if(Pol==(0x1&(val>>index)))
            {
                hs_rp_pio_sm_advance_pc(sm);
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
        uint32_t bit_count=instruction.INS_IN.Bit_count;
        if(bit_count==0)
        {
            bit_count=32;
        }
        //输入数据掩码
        uint32_t bit_mask=(bit_count>=32)?0xFFFFFFFFUL:((1UL<<bit_count)-1);
        uint32_t in_data=0;

        if(sm->autopush && sm->stall)
        {
            /*
             * 上一次IN的自动PUSH因为RX FIFO已满而stall。
             * 移位操作已经完成，本周期只需重试PUSH，不能再次移位。
             */
            uint32_t val=sm->isr;
            if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&val,sm->usr))
            {
                sm->isr=0;
                sm->isr_shift_cnt=0;
                hs_rp_pio_sm_advance_pc(sm);
                sm->stall=0;
            }
            break;
        }

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

        //移位并更新输入移位计数器
        hs_rp_pio_sm_isr_shift_in(sm,in_data,bit_count);

        if(sm->autopush && sm->isr_shift_cnt>=hs_rp_pio_sm_threshold(sm->push_thresh))
        {
            //自动PUSH
            uint32_t val=sm->isr;
            if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&val,sm->usr))
            {
                sm->isr=0;
                sm->isr_shift_cnt=0;
                hs_rp_pio_sm_advance_pc(sm);
                sm->stall=0;
            }
            else
            {
                //RX FIFO已满,等待下一个周期重试PUSH
                sm->stall=1;
            }
        }
        else
        {
            hs_rp_pio_sm_advance_pc(sm);
            sm->stall=0;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_OUT:           //OUT指令
    {
        uint32_t bit_count=instruction.INS_OUT.Bit_count;
        if(bit_count==0)
        {
            bit_count=32;
        }

        uint32_t thresh=hs_rp_pio_sm_threshold(sm->pull_thresh);

        if(sm->autopull && sm->osr_shift_cnt>=thresh)
        {
            /*
             * OUT周期开始时OSR移位计数器已达到PULL阈值。
             * 本周期的OUT必须stall，但可以同时把TX FIFO中的数据装入OSR。
             * 下一个周期再使用新的OSR数据执行OUT(不能在同一周期完成填充与移出)。
             */
            uint32_t val=0;
            if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
            {
                sm->osr=val;
                sm->osr_shift_cnt=0;
            }
            sm->stall=1;
            break;
        }

        //从OSR移出数据,同时更新输出移位计数器
        uint32_t out_data=hs_rp_pio_sm_osr_shift_out(sm,bit_count);

        //OUT完成后检查是否达到阈值,若达到则自动PULL(与移出操作同时完成)
        if(sm->autopull && sm->osr_shift_cnt>=thresh)
        {
            uint32_t val=0;
            if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
            {
                sm->osr=val;
                sm->osr_shift_cnt=0;
            }
        }

        switch(instruction.INS_OUT.Destination)
        {
        case 0:
        {
            //PINS
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINS,&out_data,sm->usr);
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 1:
        {
            //X
            sm->x=out_data;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 2:
        {
            //Y
            sm->y=out_data;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 3:
        {
            //NULL
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 4:
        {
            //PINDIR
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&out_data,sm->usr);
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 5:
        {
            //PC:无条件跳转
            sm->pc=out_data;
        }
        break;
        case 6:
        {
            /*
             * ISR:写入的是零扩展后的32位值(低bit_count位来自OSR,高位为0),
             * 不移动ISR的已有内容;输入移位计数器被设置为bit_count。
             */
            sm->isr=out_data;
            sm->isr_shift_cnt=bit_count;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 7:
        {
            //EXEC:OUT本身在本周期完成,EXEC指令在下一个周期执行
            sm->exec_instruction=out_data&0xFFFF;
            sm->exec_pending=1;
            sm->delay=0;//忽略外层OUT的delay
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        default:
        {

        }
        break;
        }
        sm->stall=0;
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
            hs_rp_pio_sm_advance_pc(sm);
        }
        else
        {
            //Pull Push
            if(instruction.INS_PUSH_MOV_PULL.Pull)
            {
                //PULL
                uint32_t thresh=hs_rp_pio_sm_threshold(sm->pull_thresh);
                bool no_op=false;

                if(sm->autopull && sm->osr_shift_cnt<thresh)
                {
                    //autopull使能时,OSR未空则PULL为空操作(作为数据屏障)
                    no_op=true;
                }
                if(instruction.INS_PUSH_MOV_PULL.ifF_ifE && sm->osr_shift_cnt<thresh)
                {
                    //IfEmpty:输出移位计数器未达到阈值时不做任何事
                    no_op=true;
                }

                if(no_op)
                {
                    hs_rp_pio_sm_advance_pc(sm);
                    sm->stall=0;
                }
                else if(instruction.INS_PUSH_MOV_PULL.Blk)
                {
                    //block
                    uint32_t val=0;
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PULL_TX_FIFO,&val,sm->usr))
                    {
                        sm->osr=val;
                        sm->osr_shift_cnt=0;
                        hs_rp_pio_sm_advance_pc(sm);
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
                    }
                    else
                    {
                        //FIFO为空时,将X复制到OSR
                        sm->osr=sm->x;
                        sm->osr_shift_cnt=0;
                    }
                    hs_rp_pio_sm_advance_pc(sm);
                    sm->stall=0;
                }
            }
            else
            {
                //PUSH
                uint32_t thresh=hs_rp_pio_sm_threshold(sm->push_thresh);
                if(instruction.INS_PUSH_MOV_PULL.ifF_ifE && sm->isr_shift_cnt<thresh)
                {
                    //IfFull:输入移位计数器未达到阈值时不做任何事
                    hs_rp_pio_sm_advance_pc(sm);
                    sm->stall=0;
                }
                else if(instruction.INS_PUSH_MOV_PULL.Blk)
                {
                    //block
                    if(sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&sm->isr,sm->usr))
                    {
                        sm->isr=0;
                        sm->isr_shift_cnt=0;
                        hs_rp_pio_sm_advance_pc(sm);
                        sm->stall=0;
                    }
                    else
                    {
                        sm->stall=1;
                    }
                }
                else
                {
                    //noblock:FIFO满时数据丢失,但ISR仍然被清空并继续执行
                    sm->io(sm,HS_RP_PIO_SM_IO_PUSH_RX_FIFO,&sm->isr,sm->usr);
                    sm->isr=0;
                    sm->isr_shift_cnt=0;
                    hs_rp_pio_sm_advance_pc(sm);
                    sm->stall=0;
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
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 1:
        {
            //X
            sm->x=data;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 2:
        {
            //Y
            sm->y=data;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 3:
        {
            //PINDIR
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_PINDIRS,&data,sm->usr);
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 4:
        {
            //EXEC:MOV本身在本周期完成,EXEC指令在下一个周期执行
            sm->exec_instruction=data&0xFFFF;
            sm->exec_pending=1;
            sm->delay=0;//忽略外层MOV的delay
            hs_rp_pio_sm_advance_pc(sm);
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
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        case 7:
        {
            //OSR
            sm->osr=data;
            sm->osr_shift_cnt=0;
            hs_rp_pio_sm_advance_pc(sm);
        }
        break;
        }
    }
    break;
    case HS_RP_PIO_SM_INS_CLASS_IRQ:           //IRQ指令
    {
        //IdxMode(bit4~bit3)+Index(bit2~bit0)
        uint32_t raw_index=(((uint32_t)instruction.INS_IRQ.IdxMode) << 3)|instruction.INS_IRQ.Index;
        uint8_t index=hs_rp_pio_sm_irq_index(sm,raw_index);
        if(instruction.INS_IRQ.Clr)
        {
            uint32_t irq=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&irq,sm->usr);
            irq&=~(1UL << index);
            sm->io(sm,HS_RP_PIO_SM_IO_WRITE_IRQ,&irq,sm->usr);
            hs_rp_pio_sm_advance_pc(sm);
        }
        else
        {
            uint32_t irq=0;
            sm->io(sm,HS_RP_PIO_SM_IO_READ_IRQ,&irq,sm->usr);
            if(!sm->stall)
            {
                irq|=(1UL << index);
                sm->io(sm,HS_RP_PIO_SM_IO_WRITE_IRQ,&irq,sm->usr);
            }
            if(instruction.INS_IRQ.Wait)
            {
                if((irq&(1UL<<index))==0)
                {
                    hs_rp_pio_sm_advance_pc(sm);
                    sm->stall=0;
                }
                else
                {
                    sm->stall=1;
                }
            }
            else
            {
                hs_rp_pio_sm_advance_pc(sm);
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
        hs_rp_pio_sm_advance_pc(sm);
    }
    break;
    }
}

void hs_rp_pio_sm_exec_immediate(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_instruction_t instruction)
{
    if(sm==NULL || sm->io==NULL)
    {
        return;
    }

    //新的立即执行指令替换尚未完成的立即执行/EXEC指令(与SMx_INSTR行为一致)
    sm->exec_pending=0;
    sm->stall=0;

    bool old_exec_no_advance=sm->exec_no_advance;
    sm->exec_no_advance=1;
    hs_rp_pio_sm_exec(sm,instruction);
    sm->exec_no_advance=old_exec_no_advance;

    if(sm->stall && !sm->exec_pending)
    {
        /*
         * 指令未完成(例如PULL block等待FIFO),保存该指令,
         * 由后续hs_rp_pio_sm_tick()继续执行;执行时仍然不自动递增PC。
         */
        sm->exec_instruction=instruction.Instruction;
        sm->exec_pending=1;
    }
}

void hs_rp_pio_sm_tick(hs_rp_pio_sm_t *sm,size_t cycles)
{
    if(sm!=NULL&&sm->io!=NULL)
    {

        while((cycles--)!=0)
        {
            hs_rp_pio_sm_instruction_t instruction;
            bool is_pending=(sm->exec_pending!=0);

            if(is_pending)
            {
                //执行由OUT EXEC/MOV EXEC产生的指令(在下一个周期执行)
                instruction.Instruction=(uint16_t)sm->exec_instruction;
            }
            else
            {
                uint32_t val=sm->pc;
                sm->io(sm,HS_RP_PIO_SM_IO_READ_INSTRUCTION,&val,sm->usr);
                instruction.Instruction=val;
            }

            //非OUT周期上的自动PULL(参考数据手册3.5.4.2)
            if(((instruction.Instruction>>13)&0x7)!=HS_RP_PIO_SM_INS_CLASS_OUT)
            {
                hs_rp_pio_sm_autopull_refill(sm);
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

            if(is_pending)
            {
                //EXEC产生的指令不再自动更新PC(外层指令已经完成PC更新)
                sm->exec_no_advance=1;
                sm->exec_pending=0;
            }

            hs_rp_pio_sm_exec(sm,instruction);

            if(is_pending)
            {
                if(sm->stall)
                {
                    //EXEC产生的指令stall,下一个周期继续执行同一条指令
                    sm->exec_pending=1;
                }
                if(!sm->exec_pending)
                {
                    sm->exec_no_advance=0;
                }
            }
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
        //默认回绕地址(关闭回绕时与PC自然回绕等价)
        sm->wrap_bottom=0;
        sm->wrap_top=31;

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
        //默认回绕地址(回绕未使能时与PC自然回绕等价)
        sm_mem->wrap_bottom=0;
        sm_mem->wrap_top=31;
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
            sm->sideset_cnt=(sm_mem->sideset_cnt>5?5:sm_mem->sideset_cnt);
            sm->side_en=sm_mem->side_en;
            sm->wrap_bottom=sm_mem->wrap_bottom;
            sm->wrap_top=sm_mem->wrap_top;
            sm->wrap_enable=sm_mem->wrap_enable;
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


