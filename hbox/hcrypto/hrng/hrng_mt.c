/***************************************************************
 * Name:      hrng_mt.c
 * Purpose:   实现hrng_mt接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrng_mt.h"

static hrng_mt_context_t hrng_mt_context_default=HRNG_MT_CONTEXT_INIT;

void hrng_mt_srand(hrng_mt_context_t *ctx,uint32_t seed)
{
    if(ctx==NULL)
    {
        ctx=&hrng_mt_context_default;
    }

    ctx->mt[0]= seed;
    for (size_t mti=1; mti<sizeof(ctx->mt)/sizeof(ctx->mt[0]); mti++)
    {
        ctx->mt[mti] =(1812433253UL * (ctx->mt[mti-1] ^ (ctx->mt[mti-1] >> 30)) + mti);
    }
}

uint32_t hrng_mt_rand(hrng_mt_context_t *ctx)
{
    uint32_t y=0;
    if(ctx==NULL)
    {
        ctx=&hrng_mt_context_default;
    }

    if(ctx->mag01[1]==ctx->mag01[0] || ctx->mti > (sizeof(ctx->mt)/sizeof(ctx->mt[0])))
    {
        /*
         * 未初始化，进行初始化
         */
        hrng_mt_srand(ctx,5489);

        if(ctx->mag01[0]!=0)
        {
            ctx->mag01[0]=0;
        }
        if(ctx->mag01[1]==0)
        {
            ctx->mag01[1]=HRNG_MT_MATRIX_A;
        }
    }

    if(ctx->mti >= (sizeof(ctx->mt)/sizeof(ctx->mt[0])))
    {
        size_t kk=0;
        for (kk=0; kk<HRNG_MT_N-HRNG_MT_M; kk++)
        {
            y = (ctx->mt[kk]&HRNG_MT_UPPER_MASK)|(ctx->mt[kk+1]&HRNG_MT_LOWER_MASK);
            ctx->mt[kk] = ctx->mt[kk+HRNG_MT_M] ^ (y >> 1) ^ ctx->mag01[y & 0x1UL];
        }
        for (; kk<HRNG_MT_N-1; kk++)
        {
            y = (ctx->mt[kk]&HRNG_MT_UPPER_MASK)|(ctx->mt[kk+1]&HRNG_MT_LOWER_MASK);
            ctx->mt[kk] = ctx->mt[kk+(HRNG_MT_M-HRNG_MT_N)] ^ (y >> 1) ^ ctx->mag01[y & 0x1UL];
        }
        y = (ctx->mt[HRNG_MT_N-1]&HRNG_MT_UPPER_MASK)|(ctx->mt[0]&HRNG_MT_LOWER_MASK);
        ctx->mt[HRNG_MT_N-1] = ctx->mt[HRNG_MT_M-1] ^ (y >> 1) ^ ctx->mag01[y & 0x1UL];
        ctx->mti=0;
    }

    y = ctx->mt[ctx->mti++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);


    return y;
}

