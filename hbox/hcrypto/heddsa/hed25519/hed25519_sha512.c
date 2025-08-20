/***************************************************************
 * Name:      hed25519_sha512.c
 * Purpose:   实现hed25519_sha512接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_sha512.h"
#include "../../hsha.h"

static int hed25519_sha512_default_starts(void *ctx)
{
    return hsha2_sha512_starts((hsha2_sha512_context_t *)ctx);
}

static int hed25519_sha512_default_update(void *ctx,const void *data,size_t datalen)
{
    return hsha2_sha512_update((hsha2_sha512_context_t *)ctx,(const uint8_t *)data,datalen);
}

static int hed25519_sha512_default_finish(void *ctx,hed25519_sha512_t sha512)
{
    return hsha2_sha512_finish((hsha2_sha512_context_t *)ctx,(uint8_t *)sha512);
}

const hed25519_sha512_config_t hed25519_sha512_default_config=
{
    hed25519_sha512_default_starts,
    hed25519_sha512_default_update,
    hed25519_sha512_default_finish
};

int hed25519_sha512_starts(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx)
{
    if(cfg==NULL)
    {
        cfg=&hed25519_sha512_default_config;
    }

    if(cfg->starts!=NULL)
    {
        return cfg->starts(ctx);
    }

    return -1;
}

int hed25519_sha512_update(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx,const void *data,size_t datalen)
{
    if(cfg==NULL)
    {
        cfg=&hed25519_sha512_default_config;
    }

    if(cfg->update!=NULL)
    {
        return cfg->update(ctx,data,datalen);
    }

    return -1;
}

int hed25519_sha512_finish(const hed25519_sha512_config_t *cfg,hed25519_sha512_context_t *ctx,hed25519_sha512_t sha512)
{
    if(cfg==NULL)
    {
        cfg=&hed25519_sha512_default_config;
    }

    if(cfg->finish!=NULL)
    {
        return cfg->finish(ctx,sha512);
    }

    return -1;
}


