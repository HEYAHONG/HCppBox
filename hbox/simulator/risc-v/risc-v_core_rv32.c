/***************************************************************
 * Name:      risc-v_core_rv32.c
 * Purpose:   实现risc-v_core_rv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_core_rv32.h"


struct hs_risc_v_core_rv32
{
    hs_risc_v_core_rv32_io_t io;
    void*                    usr;
};


size_t hs_risc_v_core_rv32_size(void)
{
    return sizeof(hs_risc_v_core_rv32_t);
}


hs_risc_v_core_rv32_t *hs_risc_v_core_rv32_init(void *mem,hs_risc_v_core_rv32_io_t io,void *usr)
{
    if(mem==NULL)
    {
        return NULL;
    }
    hs_risc_v_core_rv32_t *ret=(hs_risc_v_core_rv32_t *)mem;
    ret->io=io;
    ret->usr=usr;
    hs_risc_v_core_rv32_reset(ret);
    return ret;
}

static uint32_t hs_risc_v_core_rv32_pc_read(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=0;
        core->io(core,HS_RISC_V_CORE_RV32_IO_PC_REGISTER_READ,0,pc.bytes,sizeof(pc.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        return pc.value;
    }
    return 0;
}

static void hs_risc_v_core_rv32_pc_write(hs_risc_v_core_rv32_t *core,uint32_t pc_value)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t pc;
        pc.value=pc_value;
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
        core->io(core,HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE,0,pc.bytes,sizeof(pc.bytes),core->usr);
    }
}

static uint32_t  hs_risc_v_core_rv32_instruction_read(hs_risc_v_core_rv32_t *core)
{
    if(core!=NULL && core->io!=NULL)
    {
        hs_risc_v_common_memory_word_t instruction;
        instruction.value=0;
        core->io(core,HS_RISC_V_CORE_RV32_IO_MEMORY_READ,hs_risc_v_core_rv32_pc_read(core),instruction.bytes,sizeof(instruction.bytes),core->usr);
        HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(instruction);
        return instruction.value;
    }
    return 0;
}


static void hs_risc_v_core_rv32_exec(hs_risc_v_core_rv32_t * core)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }


    uint32_t instruction= hs_risc_v_core_rv32_instruction_read(core);
    uint32_t pc=hs_risc_v_core_rv32_pc_read(core);
    core->io(core,HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ENTER,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
    pc+=hs_risc_v_common_instruction_length(instruction);
    hs_risc_v_core_rv32_pc_write(core,pc);

    //指令是否正确处理，当指令被正确处理时，需要将此标志置位true
    bool is_instruction_processed=false;



    if(!is_instruction_processed)
    {
        //指令未处理，可能是custom指令
        core->io(core,HS_RISC_V_CORE_RV32_IO_CUSTOM_INSTRUCTION_EXEC,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
    }

    core->io(core,HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EXIT,pc,(uint8_t*)&instruction,sizeof(instruction),core->usr);
}


void hs_risc_v_core_rv32_tick(hs_risc_v_core_rv32_t * core,size_t cycles)
{
    if(core==NULL || core->io==NULL)
    {
        return;
    }
    while(cycles--)
    {
        core->io(core,HS_RISC_V_CORE_RV32_IO_TICK_ENTER,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);

        hs_risc_v_core_rv32_exec(core);

        core->io(core,HS_RISC_V_CORE_RV32_IO_TICK_EXIT,hs_risc_v_core_rv32_pc_read(core),(uint8_t*)&cycles,sizeof(cycles),core->usr);
    };
}


void hs_risc_v_core_rv32_reset(hs_risc_v_core_rv32_t * core)
{
    if(core!=NULL && core->io!=NULL)
    {
        uint32_t val=0;
        core->io(core,HS_RISC_V_CORE_RV32_IO_RESET,0,(uint8_t *)&val,sizeof(val),core->usr);

        {
            //初始化PC值
            hs_risc_v_common_memory_word_t pc;
            pc.value=0;
            core->io(core,HS_RISC_V_CORE_RV32_IO_ENTRY_PC_READ,0,pc.bytes,sizeof(pc.bytes),core->usr);
            HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(pc);
            hs_risc_v_core_rv32_pc_write(core,pc.value);
        }
    }
}

bool hs_risc_v_core_rv32_io(hs_risc_v_core_rv32_t *core,hs_risc_v_core_rv32_io_opt_t opt,uint32_t address,uint8_t *data,size_t len)
{
    if(core!=NULL && core->io!=NULL)
    {
        return core->io(core,opt,address,data,len,core->usr);
    }
    return false;
}
