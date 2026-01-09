/***************************************************************
 * Name:      hrng_linearcongruential.c
 * Purpose:   实现hrng_linearcongruential接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrng_linearcongruential.h"

hrng_linearcongruential_context_t hrng_linearcongruential_context_rand48=
{
    0x00001234abcd330e
};

const hrng_linearcongruential_config_t hrng_linearcongruential_config_rand48=
{
   0x00000005deece66d,
   0x000000000000000b,
   (1ULL<<(48))
};

uint64_t hrng_linearcongruential_update(const hrng_linearcongruential_config_t *cfg,hrng_linearcongruential_context_t *ctx)
{
    if(cfg==NULL)
    {
        cfg=&hrng_linearcongruential_config_rand48;
    }

    if(ctx==NULL)
    {
        ctx=&hrng_linearcongruential_context_rand48;
    }

    /*
     * 线性同余法
     */
    ctx->seed_number=((ctx->seed_number*cfg->mult_number+cfg->add_number)%cfg->mod_number);

    return ctx->seed_number;
}

long hrng_linearcongruential_rand48_rand(void)
{
    return hrng_linearcongruential_update(NULL,NULL)%HRNG_LINEARCONGRUENTIAL_RAND48_RANDMAX;
}

void hrng_linearcongruential_rand48_srand(uint64_t seed_number)
{
    hrng_linearcongruential_context_rand48.seed_number=seed_number;
}
