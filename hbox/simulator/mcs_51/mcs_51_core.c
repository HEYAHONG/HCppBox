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

            //TODO:执行指令
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
    }
}
