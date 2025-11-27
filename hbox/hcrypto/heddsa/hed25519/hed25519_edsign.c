/***************************************************************
 * Name:      hed25519_edsign.c
 * Purpose:   实现hed25519_edsign接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_edsign.h"

#define HED25519_EXPANDED_SIZE      (64)

static const hed25519_fprime_t hed25519_ed25519_order=
{
    0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58,
    0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
};

static void hed25519_expand_key(hed25519_sha512_config_t  *cfg,uint8_t *expanded, const uint8_t *secret)
{
    hed25519_sha512_context_t s;

    hed25519_sha512_starts(cfg,&s);
    hed25519_sha512_update(cfg,&s, secret, HED25519_EDSIGN_SECRET_KEY_SIZE);
    hed25519_sha512_finish(cfg,&s, expanded);

    hed25519_ed25519_prepare(expanded);
}

static uint8_t hed25519_upp(hed25519_ed25519_pt_t *p, const uint8_t *packed)
{
    uint8_t x[HED25519_FPRIME_SIZE];
    uint8_t y[HED25519_FPRIME_SIZE];
    uint8_t ok = hed25519_ed25519_try_unpack(x, y, packed);

    hed25519_ed25519_project(p, x, y);
    return ok;
}

static void hed25519_pp(uint8_t *packed, const hed25519_ed25519_pt_t *p)
{
    uint8_t x[HED25519_FPRIME_SIZE];
    uint8_t y[HED25519_FPRIME_SIZE];

    hed25519_ed25519_unproject(x, y, p);
    hed25519_ed25519_pack(packed, x, y);
}

static void hed25519_sm_pack(uint8_t *r, const uint8_t *k)
{
    hed25519_ed25519_pt_t p;

    hed25519_ed25519_smult(&p, &hed25519_ed25519_base, k);
    hed25519_pp(r, &p);
}

void hed25519_edsign_sec_generate(hed25519_edsign_secret_key_t secret)
{
    if(secret==NULL)
    {
        return;
    }
    for(size_t i=0; i<sizeof(hed25519_edsign_secret_key_t); i++)
    {
        secret[i]=((unsigned)hed25519_rand()%0x100);
    }
}

void hed25519_edsign_sec_to_pub(hed25519_sha512_config_t  *cfg,hed25519_edsign_public_key_t pub, const hed25519_edsign_public_key_t secret)
{
    uint8_t expanded[HED25519_EXPANDED_SIZE];

    hed25519_expand_key(cfg,expanded, secret);
    hed25519_sm_pack(pub, expanded);
}

static void hed25519_save_hash(hed25519_sha512_config_t  *cfg,hed25519_sha512_context_t *s, uint8_t *out)
{
    hed25519_sha512_t sha512= {0};
    hed25519_sha512_finish(cfg,s,sha512);
    hed25519_fprime_from_bytes(out, sha512, sizeof(hed25519_sha512_t), hed25519_ed25519_order);
}

static void hed25519_generate_k(hed25519_sha512_config_t  *cfg,uint8_t *k, const uint8_t *kgen_key,const uint8_t *message, size_t len)
{
    hed25519_sha512_context_t s;

    hed25519_sha512_starts(cfg,&s);
    hed25519_sha512_update(cfg,&s, kgen_key, 32);
    hed25519_sha512_update(cfg,&s, message, len);
    hed25519_save_hash(cfg,&s, k);
}

static void hed25519_hash_message(hed25519_sha512_config_t  *cfg,uint8_t *z, const uint8_t *r, const uint8_t *a,const uint8_t *m, size_t len)
{
    hed25519_sha512_context_t s;

    hed25519_sha512_starts(cfg,&s);
    hed25519_sha512_update(cfg,&s, r, 32);
    hed25519_sha512_update(cfg,&s, a, 32);
    hed25519_sha512_update(cfg,&s, m, len);
    hed25519_save_hash(cfg,&s, z);
}

void hed25519_edsign_sign(hed25519_edsign_signature_t signature,hed25519_sha512_config_t  *cfg,const hed25519_edsign_public_key_t pub,const hed25519_edsign_secret_key_t secret,const void *message, size_t len)
{
    uint8_t expanded[HED25519_EXPANDED_SIZE];
    uint8_t e[HED25519_FPRIME_SIZE];
    uint8_t s[HED25519_FPRIME_SIZE];
    uint8_t k[HED25519_FPRIME_SIZE];
    uint8_t z[HED25519_FPRIME_SIZE];

    hed25519_expand_key(cfg,expanded, secret);

    /* Generate k and R = kB */
    hed25519_generate_k(cfg,k, expanded + 32, message, len);
    hed25519_sm_pack(signature, k);

    /* Compute z = H(R, A, M) */
    hed25519_hash_message(cfg,z, signature, pub, message, len);

    /* Obtain e */
    hed25519_fprime_from_bytes(e, expanded, 32, hed25519_ed25519_order);

    /* Compute s = ze + k */
    hed25519_fprime_mul(s, z, e, hed25519_ed25519_order);
    hed25519_fprime_add(s, k, hed25519_ed25519_order);
    memcpy(signature + 32, s, 32);
}


static void hed25519_generate_k2(hed25519_sha512_config_t  *cfg,uint8_t *k, const uint8_t *kgen_key,hed25519_edsign_sign2_callback_t cb, void *usr)
{
    hed25519_sha512_context_t s;

    hed25519_sha512_starts(cfg,&s);
    hed25519_sha512_update(cfg,&s, kgen_key, 32);
    if(cb != NULL)
    {
        uint8_t buffer[4096];
        size_t  offset=0;
        while(true)
        {
            int byte_read=cb(usr,offset,buffer,sizeof(buffer));
            if(byte_read > 0)
            {
                hed25519_sha512_update(cfg,&s, buffer, byte_read);
                offset+=byte_read;
                if(byte_read!=sizeof(buffer))
                {
                    /*
                     * 到达尾部
                     */
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    hed25519_save_hash(cfg,&s, k);
}

static void hed25519_hash_message2(hed25519_sha512_config_t  *cfg,uint8_t *z, const uint8_t *r, const uint8_t *a,hed25519_edsign_sign2_callback_t cb, void *usr)
{
    hed25519_sha512_context_t s;

    hed25519_sha512_starts(cfg,&s);
    hed25519_sha512_update(cfg,&s, r, 32);
    hed25519_sha512_update(cfg,&s, a, 32);
    if(cb != NULL)
    {
        uint8_t buffer[4096];
        size_t  offset=0;
        while(true)
        {
            int byte_read=cb(usr,offset,buffer,sizeof(buffer));
            if(byte_read > 0)
            {
                hed25519_sha512_update(cfg,&s, buffer, byte_read);
                offset+=byte_read;
                if(byte_read!=sizeof(buffer))
                {
                    /*
                     * 到达尾部
                     */
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    hed25519_save_hash(cfg,&s, z);
}

void hed25519_edsign_sign2(hed25519_edsign_signature_t signature,hed25519_sha512_config_t  *cfg,const hed25519_edsign_public_key_t pub,const hed25519_edsign_secret_key_t secret,hed25519_edsign_sign2_callback_t cb, void *usr)
{
    uint8_t expanded[HED25519_EXPANDED_SIZE];
    uint8_t e[HED25519_FPRIME_SIZE];
    uint8_t s[HED25519_FPRIME_SIZE];
    uint8_t k[HED25519_FPRIME_SIZE];
    uint8_t z[HED25519_FPRIME_SIZE];

    hed25519_expand_key(cfg,expanded, secret);

    /* Generate k and R = kB */
    hed25519_generate_k2(cfg,k, expanded + 32, cb, usr);
    hed25519_sm_pack(signature, k);

    /* Compute z = H(R, A, M) */
    hed25519_hash_message2(cfg,z, signature, pub, cb, usr);

    /* Obtain e */
    hed25519_fprime_from_bytes(e, expanded, 32, hed25519_ed25519_order);

    /* Compute s = ze + k */
    hed25519_fprime_mul(s, z, e, hed25519_ed25519_order);
    hed25519_fprime_add(s, k, hed25519_ed25519_order);
    memcpy(signature + 32, s, 32);
}

void hed25519_edsign_verify_init(hed25519_edsign_verify_state_t *st,hed25519_sha512_config_t  *cfg,const hed25519_edsign_signature_t sig,const hed25519_edsign_public_key_t pub)
{
    if(st==NULL)
    {
        return;
    }
    st->cfg=cfg;
    hed25519_sha512_starts(cfg,&st->sha);
    hed25519_sha512_update(cfg,&st->sha, sig, 32);
    hed25519_sha512_update(cfg,&st->sha, pub, 32);
}

bool hed25519_edsign_verify(hed25519_edsign_verify_state_t *st, const hed25519_edsign_signature_t sig, const hed25519_edsign_public_key_t pub)
{
    hed25519_ed25519_pt_t p;
    hed25519_ed25519_pt_t q;
    uint8_t lhs[HED25519_FPRIME_SIZE];
    uint8_t rhs[HED25519_FPRIME_SIZE];
    uint8_t z[HED25519_FPRIME_SIZE];
    uint8_t ok = 1;

    /* Compute z = H(R, A, M) */
    hed25519_save_hash(st->cfg,&st->sha, z);

    /* sB = (ze + k)B = ... */
    hed25519_sm_pack(lhs, sig + 32);

    /* ... = zA + R */
    ok &= hed25519_upp(&p, pub);
    hed25519_ed25519_smult(&p, &p, z);
    ok &= hed25519_upp(&q, sig);
    hed25519_ed25519_add(&p, &p, &q);
    hed25519_pp(rhs, &p);

    /* Equal? */
    return ok & hed25519_f25519_eq(lhs, rhs);
}




