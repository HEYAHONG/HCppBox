/***************************************************************
 * Name:      hrsa2048.c
 * Purpose:   实现hrsa2048接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrsa2048.h"

void hrsa2048_public_key_load_n(hrsa2048_public_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_load_be((huint2048_t *)&key->n,bytes,bytes_len);
}
void hrsa2048_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa2048_public_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_store_be((huint2048_t *)&key->n,bytes,bytes_len);
}
void hrsa2048_public_key_load_e(hrsa2048_public_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_load_be((huint2048_t *)&key->e,bytes,bytes_len);
}
void hrsa2048_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa2048_public_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_store_be((huint2048_t *)&key->e,bytes,bytes_len);
}

bool hrsa2048_public_key_is_ok(const hrsa2048_public_key_t *key)
{
    bool ret=false;
    if(key!=NULL)
    {
        ret=(!huint2048_t_is_zero(&key->e) && !huint2048_t_is_zero(&key->n));
    }
    return ret;
}

void hrsa2048_private_key_load_n(hrsa2048_private_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_load_be((huint2048_t *)&key->n,bytes,bytes_len);
}
void hrsa2048_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa2048_private_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_store_be((huint2048_t *)&key->n,bytes,bytes_len);
}
void hrsa2048_private_key_load_d(hrsa2048_private_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_load_be((huint2048_t *)&key->d,bytes,bytes_len);
}
void hrsa2048_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa2048_private_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint2048_store_be((huint2048_t *)&key->d,bytes,bytes_len);
}
bool hrsa2048_private_key_is_ok(const hrsa2048_private_key_t *key)
{
    bool ret=false;
    if(key!=NULL)
    {
        ret=(!huint2048_t_is_zero(&key->d) && !huint2048_t_is_zero(&key->n));
    }
    return ret;
}

bool hrsa2048_encipher(hrsa2048_context_t *ctx,hrsa2048_cipher_message_t cipher_message,const hrsa2048_plain_message_t plain_message,const hrsa2048_private_key_t *key)
{
    if(ctx==NULL || cipher_message==NULL || plain_message==NULL || key == NULL || !hrsa2048_private_key_is_ok(key))
    {
        return false;
    }

    huint4096_load_be(&ctx->state.state[5],plain_message,sizeof(hrsa2048_plain_message_t));
    huint4096_load_uint32(&ctx->state.state[6],0);
    memcpy(&ctx->state.state[6],&key->d,sizeof(key->d));
    huint4096_load_uint32(&ctx->state.state[7],0);
    memcpy(&ctx->state.state[7],&key->n,sizeof(key->n));
    huint4096_power_mod_with_external_state(&ctx->state,&ctx->state.state[4],&ctx->state.state[5],&ctx->state.state[6],&ctx->state.state[7]);
    huint4096_store_be(&ctx->state.state[4],cipher_message,sizeof(hrsa2048_cipher_message_t));

    return true;
}

bool hrsa2048_decipher(hrsa2048_context_t *ctx,hrsa2048_plain_message_t plain_message,const hrsa2048_cipher_message_t cipher_message,const hrsa2048_public_key_t *key)
{
    if(ctx==NULL || cipher_message==NULL || plain_message==NULL || key == NULL || !hrsa2048_public_key_is_ok(key))
    {
        return false;
    }

    huint4096_load_be(&ctx->state.state[5],cipher_message,sizeof(hrsa2048_cipher_message_t));
    huint4096_load_uint32(&ctx->state.state[6],0);
    memcpy(&ctx->state.state[6],&key->e,sizeof(key->e));
    huint4096_load_uint32(&ctx->state.state[7],0);
    memcpy(&ctx->state.state[7],&key->n,sizeof(key->n));
    huint4096_power_mod_with_external_state(&ctx->state,&ctx->state.state[4],&ctx->state.state[5],&ctx->state.state[6],&ctx->state.state[7]);
    huint4096_store_be(&ctx->state.state[4],plain_message,sizeof(hrsa2048_plain_message_t));

    return true;
}
