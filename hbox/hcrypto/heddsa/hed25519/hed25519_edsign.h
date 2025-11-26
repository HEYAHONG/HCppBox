/***************************************************************
 * Name:      hed25519_edsign.h
 * Purpose:   声明hed25519_edsign接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HED25519_EDSIGN_H__
#define __HED25519_EDSIGN_H__
#include "hed25519_fprime.h"
#include "hed25519_ed25519.h"
#include "hed25519_sha512.h"
#include "hed25519_getrandom.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HED25519_EDSIGN_SECRET_KEY_SIZE     (32)

/*
 * 私钥可使用随机数填充
 */
typedef uint8_t hed25519_edsign_secret_key_t[HED25519_EDSIGN_SECRET_KEY_SIZE];

void hed25519_edsign_sec_generate(hed25519_edsign_secret_key_t secret);

#define HED25519_EDSIGN_PUBLIC_KEY_SIZE     (32)

typedef uint8_t hed25519_edsign_public_key_t[HED25519_EDSIGN_PUBLIC_KEY_SIZE];

void hed25519_edsign_sec_to_pub(hed25519_sha512_config_t  *cfg,hed25519_edsign_public_key_t pub, const hed25519_edsign_public_key_t secret);

#define HED25519_EDSIGN_SIGNATURE_SIZE      (64)

typedef uint8_t hed25519_edsign_signature_t[HED25519_EDSIGN_SIGNATURE_SIZE];

void hed25519_edsign_sign(hed25519_edsign_signature_t signature,hed25519_sha512_config_t  *cfg,const hed25519_edsign_public_key_t pub,const hed25519_edsign_secret_key_t secret,const void *message, size_t len);


typedef struct hed25519_edsign_verify_state hed25519_edsign_verify_state_t;
struct hed25519_edsign_verify_state
{
    hed25519_sha512_config_t  *cfg; //不使用时应该置为NULL
    hed25519_sha512_context_t sha;
};

void hed25519_edsign_verify_init(hed25519_edsign_verify_state_t *st,hed25519_sha512_config_t  *cfg,const hed25519_edsign_signature_t sig,const hed25519_edsign_public_key_t pub);
static inline void hed25519_edsign_verify_add(hed25519_edsign_verify_state_t *st, const void *data, int len)
{
    if(st==NULL)
    {
        return;
    }
    hed25519_sha512_update(st->cfg,&st->sha,data,len);
}
/* Verify a message signature. Returns non-zero if ok. */
bool hed25519_edsign_verify(hed25519_edsign_verify_state_t *st, const hed25519_edsign_signature_t sig, const hed25519_edsign_public_key_t pub);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HED25519_EDSIGN_H__
