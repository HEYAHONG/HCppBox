/***************************************************************
 * Name:      hchacha20.c
 * Purpose:   实现hchacha20接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-03
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hchacha20.h"

#define HCHACHA20_GET_UINT32_LE( data, offset )              \
    (                                                      \
          ( (uint32_t) ( data )[( offset )    ]       )    \
        | ( (uint32_t) ( data )[( offset ) + 1] <<  8 )    \
        | ( (uint32_t) ( data )[( offset ) + 2] << 16 )    \
        | ( (uint32_t) ( data )[( offset ) + 3] << 24 )    \
    )

#ifndef HCHACHA20_PUT_UINT32_LE
#define HCHACHA20_PUT_UINT32_LE(n,b,i)                                \
    do {                                                            \
        (b)[(i)    ] = (unsigned char) ( ( (n)       ) & 0xFF );    \
        (b)[(i) + 1] = (unsigned char) ( ( (n) >>  8 ) & 0xFF );    \
        (b)[(i) + 2] = (unsigned char) ( ( (n) >> 16 ) & 0xFF );    \
        (b)[(i) + 3] = (unsigned char) ( ( (n) >> 24 ) & 0xFF );    \
    } while( 0 )

#endif

#define HCHACHA20_ROTL32(value, amount) \
    ((uint32_t) ((value) << (amount)) | ((value) >> (32 - (amount))))

#define HCHACHA20_CTR_INDEX (12U)
#define HCHACHA20_BLOCK_SIZE_BYTES (4U * 16U)

static inline void hchacha20_xor(unsigned char *r,const unsigned char *a,const unsigned char *b,size_t n)
{
    if(r==NULL || a == NULL || b == NULL)
    {
        return;
    }
    for(size_t i=0; i<n; i++)
    {
        r[i]=a[i]^b[i];
    }
}


/**
 * \brief           ChaCha20 quarter round operation.
 *
 *                  The quarter round is defined as follows (from RFC 7539):
 *                      1.  a += b; d ^= a; d <<<= 16;
 *                      2.  c += d; b ^= c; b <<<= 12;
 *                      3.  a += b; d ^= a; d <<<= 8;
 *                      4.  c += d; b ^= c; b <<<= 7;
 *
 * \param state     ChaCha20 state to modify.
 * \param a         The index of 'a' in the state.
 * \param b         The index of 'b' in the state.
 * \param c         The index of 'c' in the state.
 * \param d         The index of 'd' in the state.
 */
static inline void hchacha20_quarter_round(uint32_t state[16],size_t a,size_t b,size_t c,size_t d)
{
    /* a += b; d ^= a; d <<<= 16; */
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = HCHACHA20_ROTL32(state[d], 16);

    /* c += d; b ^= c; b <<<= 12 */
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = HCHACHA20_ROTL32(state[b], 12);

    /* a += b; d ^= a; d <<<= 8; */
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = HCHACHA20_ROTL32(state[d], 8);

    /* c += d; b ^= c; b <<<= 7; */
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = HCHACHA20_ROTL32(state[b], 7);
}

/**
 * \brief           Perform the ChaCha20 inner block operation.
 *
 *                  This function performs two rounds: the column round and the
 *                  diagonal round.
 *
 * \param state     The ChaCha20 state to update.
 */
static void hchacha20_inner_block(uint32_t state[16])
{
    hchacha20_quarter_round(state, 0, 4, 8,  12);
    hchacha20_quarter_round(state, 1, 5, 9,  13);
    hchacha20_quarter_round(state, 2, 6, 10, 14);
    hchacha20_quarter_round(state, 3, 7, 11, 15);

    hchacha20_quarter_round(state, 0, 5, 10, 15);
    hchacha20_quarter_round(state, 1, 6, 11, 12);
    hchacha20_quarter_round(state, 2, 7, 8,  13);
    hchacha20_quarter_round(state, 3, 4, 9,  14);
}

/**
 * \brief               Generates a keystream block.
 *
 * \param initial_state The initial ChaCha20 state (key, nonce, counter).
 * \param keystream     Generated keystream bytes are written to this buffer.
 */
static void hchacha20_block(const uint32_t initial_state[16],uint8_t keystream[64])
{
    uint32_t working_state[16];
    size_t i;

    memcpy(working_state,initial_state,HCHACHA20_BLOCK_SIZE_BYTES);

    for (i = 0U; i < 10U; i++)
    {
        hchacha20_inner_block(working_state);
    }

    working_state[0] += initial_state[0];
    working_state[1] += initial_state[1];
    working_state[2] += initial_state[2];
    working_state[3] += initial_state[3];
    working_state[4] += initial_state[4];
    working_state[5] += initial_state[5];
    working_state[6] += initial_state[6];
    working_state[7] += initial_state[7];
    working_state[8] += initial_state[8];
    working_state[9] += initial_state[9];
    working_state[10] += initial_state[10];
    working_state[11] += initial_state[11];
    working_state[12] += initial_state[12];
    working_state[13] += initial_state[13];
    working_state[14] += initial_state[14];
    working_state[15] += initial_state[15];

    for (i = 0U; i < 16; i++)
    {
        size_t offset = i * 4U;

        HCHACHA20_PUT_UINT32_LE(working_state[i], keystream, offset);
    }

}


int hchacha20_setkey(hchacha20_context_t *ctx,const uint8_t key[32])
{
    int ret=-1;
    if(ctx == NULL ||key == NULL)
    {
        return ret;
    }
    memset(ctx,0,sizeof(hchacha20_context_t));
    ctx->keystream_bytes_used = HCHACHA20_BLOCK_SIZE_BYTES;
    /* ChaCha20 constants - the string "expand 32-byte k" */
    ctx->state[0] = 0x61707865;
    ctx->state[1] = 0x3320646e;
    ctx->state[2] = 0x79622d32;
    ctx->state[3] = 0x6b206574;
    /* Set key */
    ctx->state[4]  = HCHACHA20_GET_UINT32_LE(key, 0);
    ctx->state[5]  = HCHACHA20_GET_UINT32_LE(key, 4);
    ctx->state[6]  = HCHACHA20_GET_UINT32_LE(key, 8);
    ctx->state[7]  = HCHACHA20_GET_UINT32_LE(key, 12);
    ctx->state[8]  = HCHACHA20_GET_UINT32_LE(key, 16);
    ctx->state[9]  = HCHACHA20_GET_UINT32_LE(key, 20);
    ctx->state[10] = HCHACHA20_GET_UINT32_LE(key, 24);
    ctx->state[11] = HCHACHA20_GET_UINT32_LE(key, 28);

    ret=0;

    return ret;
}

int hchacha20_starts(hchacha20_context_t *ctx,const uint8_t nonce[12],uint32_t counter)
{
    int ret=-1;
    if(ctx == NULL || nonce == NULL)
    {
        return ret;
    }

    /* Counter */
    ctx->state[12] = counter;

    /* Nonce */
    ctx->state[13] = HCHACHA20_GET_UINT32_LE(nonce, 0);
    ctx->state[14] = HCHACHA20_GET_UINT32_LE(nonce, 4);
    ctx->state[15] = HCHACHA20_GET_UINT32_LE(nonce, 8);

    memset(ctx->keystream8,0,sizeof(ctx->keystream8));

    /* Initially, there's no keystream bytes available */
    ctx->keystream_bytes_used = HCHACHA20_BLOCK_SIZE_BYTES;

    ret=0;
    return ret;
}

int hchacha20_update(hchacha20_context_t *ctx,size_t length,const uint8_t *input,uint8_t  *output)
{
    int ret=-1;
    if(ctx==NULL)
    {
        return ret;
    }
    ret=0;
    if(length == 0 || input == NULL || output == NULL)
    {
        return ret;
    }

    size_t offset = 0U;
    /* Use leftover keystream bytes, if available */
    while (length > 0U && ctx->keystream_bytes_used < HCHACHA20_BLOCK_SIZE_BYTES)
    {
        output[offset] = input[offset]^ ctx->keystream8[ctx->keystream_bytes_used];

        ctx->keystream_bytes_used++;
        offset++;
        length--;
    }

    /* Process full blocks */
    while (length >= HCHACHA20_BLOCK_SIZE_BYTES)
    {
        /* Generate new keystream block and increment counter */
        hchacha20_block(ctx->state, ctx->keystream8);
        ctx->state[HCHACHA20_CTR_INDEX]++;

        hchacha20_xor(output + offset, input + offset, ctx->keystream8, 64U);

        offset += HCHACHA20_BLOCK_SIZE_BYTES;
        length   -= HCHACHA20_BLOCK_SIZE_BYTES;
    }

    /* Last (partial) block */
    if (length > 0U)
    {
        /* Generate new keystream block and increment counter */
        hchacha20_block(ctx->state, ctx->keystream8);
        ctx->state[HCHACHA20_CTR_INDEX]++;

        hchacha20_xor(output + offset, input + offset, ctx->keystream8, length);

        ctx->keystream_bytes_used = length;

    }

    return ret;
}

int hchacha20_crypt(const uint8_t key[32],const uint8_t nonce[12],uint32_t counter,size_t length,const uint8_t *input,uint8_t *output)
{
    int ret=-1;
    hchacha20_context_t ctx;

    if((ret=hchacha20_setkey(&ctx,key)) < 0)
    {
        return ret;
    }

    if((ret=hchacha20_starts(&ctx,nonce,counter)) < 0)
    {
        return ret;
    }

    if((ret=hchacha20_update(&ctx,length,input,output)) < 0)
    {
        return ret;
    }

    return ret;
}

