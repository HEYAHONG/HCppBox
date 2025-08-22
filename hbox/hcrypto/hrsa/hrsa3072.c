/***************************************************************
 * Name:      hrsa3072.c
 * Purpose:   实现hrsa3072接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrsa3072.h"

void hrsa3072_public_key_load_n(hrsa3072_public_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_load_be((huint3072_t *)&key->n,bytes,bytes_len);
}
void hrsa3072_public_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa3072_public_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_store_be((huint3072_t *)&key->n,bytes,bytes_len);
}
void hrsa3072_public_key_load_e(hrsa3072_public_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_load_be((huint3072_t *)&key->e,bytes,bytes_len);
}
void hrsa3072_public_key_store_e(uint8_t *bytes,size_t bytes_len,const hrsa3072_public_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_store_be((huint3072_t *)&key->e,bytes,bytes_len);
}

bool hrsa3072_public_key_is_ok(const hrsa3072_public_key_t *key)
{
    bool ret=false;
    if(key!=NULL)
    {
        ret=(!huint3072_t_is_zero(&key->e) && !huint3072_t_is_zero(&key->n));
    }
    return ret;
}

void hrsa3072_private_key_load_n(hrsa3072_private_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_load_be((huint3072_t *)&key->n,bytes,bytes_len);
}
void hrsa3072_private_key_store_n(uint8_t *bytes,size_t bytes_len,const hrsa3072_private_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_store_be((huint3072_t *)&key->n,bytes,bytes_len);
}
void hrsa3072_private_key_load_d(hrsa3072_private_key_t *key,const uint8_t *bytes,size_t bytes_len)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_load_be((huint3072_t *)&key->d,bytes,bytes_len);
}
void hrsa3072_private_key_store_d(uint8_t *bytes,size_t bytes_len,const hrsa3072_private_key_t *key)
{
    if(key==NULL || bytes==NULL)
    {
        return;
    }
    huint3072_store_be((huint3072_t *)&key->d,bytes,bytes_len);
}
bool hrsa3072_private_key_is_ok(const hrsa3072_private_key_t *key)
{
    bool ret=false;
    if(key!=NULL)
    {
        ret=(!huint3072_t_is_zero(&key->d) && !huint3072_t_is_zero(&key->n));
    }
    return ret;
}

bool hrsa3072_encipher(hrsa3072_context_t *ctx,hrsa3072_cipher_message_t cipher_message,const hrsa3072_data_block_t data_block,const hrsa3072_private_key_t *key)
{
    if(ctx==NULL || cipher_message==NULL || data_block==NULL || key == NULL || !hrsa3072_private_key_is_ok(key))
    {
        return false;
    }

    huint6144_load_be(&ctx->state.state[5],data_block,sizeof(hrsa3072_data_block_t));
    huint6144_load_uint32(&ctx->state.state[6],0);
    memcpy(&ctx->state.state[6],&key->d,sizeof(key->d));
    huint6144_load_uint32(&ctx->state.state[7],0);
    memcpy(&ctx->state.state[7],&key->n,sizeof(key->n));
    huint6144_power_mod_with_external_state(&ctx->state,&ctx->state.state[4],&ctx->state.state[5],&ctx->state.state[6],&ctx->state.state[7]);
    huint6144_store_be(&ctx->state.state[4],cipher_message,sizeof(hrsa3072_cipher_message_t));

    return true;
}

bool hrsa3072_decipher(hrsa3072_context_t *ctx,hrsa3072_data_block_t data_block,const hrsa3072_cipher_message_t cipher_message,const hrsa3072_public_key_t *key)
{
    if(ctx==NULL || cipher_message==NULL || data_block==NULL || key == NULL || !hrsa3072_public_key_is_ok(key))
    {
        return false;
    }

    huint6144_load_be(&ctx->state.state[5],cipher_message,sizeof(hrsa3072_cipher_message_t));
    huint6144_load_uint32(&ctx->state.state[6],0);
    memcpy(&ctx->state.state[6],&key->e,sizeof(key->e));
    huint6144_load_uint32(&ctx->state.state[7],0);
    memcpy(&ctx->state.state[7],&key->n,sizeof(key->n));
    huint6144_power_mod_with_external_state(&ctx->state,&ctx->state.state[4],&ctx->state.state[5],&ctx->state.state[6],&ctx->state.state[7]);
    huint6144_store_be(&ctx->state.state[4],data_block,sizeof(hrsa3072_data_block_t));

    return true;
}
