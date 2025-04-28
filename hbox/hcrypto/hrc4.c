/***************************************************************
 * Name:      hrc4.c
 * Purpose:   实现hrc4接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrc4.h"

void hrc4_set_key(hrc4_context_t *ctx,const uint8_t *key,size_t len)
{
    if(ctx==NULL ||key == NULL || len == 0)
    {
        return;
    }
    HRC4_INT *d = &(ctx->data[0]);
    ctx->x = 0;
    ctx->y = 0;
    int id1=0;
    int id2=0;

#ifdef  SK_LOOP
#undef  SK_LOOP
#endif // SK_LOOP

#define SK_LOOP(d,n)                                            \
    {                                                           \
                HRC4_INT tmp=d[(n)];                            \
                id2 = (key[id1] + tmp + id2) & 0xff;            \
                if (++id1 == len) id1=0;                        \
                d[(n)]=d[id2];                                  \
                d[id2]=tmp;                                     \
    }

    for (size_t i = 0; i < 256; i++)
    {
        d[i] = i;
    }
    for (size_t i = 0; i < 256; i += 4)
    {
        SK_LOOP(d, i + 0);
        SK_LOOP(d, i + 1);
        SK_LOOP(d, i + 2);
        SK_LOOP(d, i + 3);
    }

#undef SK_LOOP

}
void hrc4_rc4(hrc4_context_t *ctx, size_t len, const uint8_t *indata,uint8_t *outdata)
{
    HRC4_INT *d;
    HRC4_INT x, y, tx, ty;
    size_t i;

    x = ctx->x;
    y = ctx->y;
    d = ctx->data;

#ifdef LOOP
#undef LOOP
#endif
#define LOOP(in,out)                \
    x=((x+1)&0xff);                 \
    tx=d[x];                        \
    y=(tx+y)&0xff;                  \
    d[x]=ty=d[y];                   \
    d[y]=tx;                        \
    (out) = d[(tx+ty)&0xff]^ (in);

    i = len >> 3;
    if (i!=0)
    {
        for (;;)
        {
            LOOP(indata[0], outdata[0]);
            LOOP(indata[1], outdata[1]);
            LOOP(indata[2], outdata[2]);
            LOOP(indata[3], outdata[3]);
            LOOP(indata[4], outdata[4]);
            LOOP(indata[5], outdata[5]);
            LOOP(indata[6], outdata[6]);
            LOOP(indata[7], outdata[7]);
            indata += 8;
            outdata += 8;
            if (--i == 0)
                break;
        }
    }
    i = len & 0x07;
    if (i!=0)
    {
        for (;;)
        {
            LOOP(indata[0], outdata[0]);
            if (--i == 0)
                break;
            LOOP(indata[1], outdata[1]);
            if (--i == 0)
                break;
            LOOP(indata[2], outdata[2]);
            if (--i == 0)
                break;
            LOOP(indata[3], outdata[3]);
            if (--i == 0)
                break;
            LOOP(indata[4], outdata[4]);
            if (--i == 0)
                break;
            LOOP(indata[5], outdata[5]);
            if (--i == 0)
                break;
            LOOP(indata[6], outdata[6]);
            if (--i == 0)
                break;
        }
    }
    ctx->x = x;
    ctx->y = y;
#undef LOOP

}
