/***************************************************************
 * Name:      hsha.h
 * Purpose:   声明hsha接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHA_H__
#define __HSHA_H__
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * SHA1
 */
typedef uint8_t hsha1_sha_t[20];

/*
 * SHA1 消息块
 */
typedef uint8_t hsha1_message_block_t[64];

/*
 * SHA1上下文
 */
typedef struct hsha1_context hsha1_context_t;
struct hsha1_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    hsha1_message_block_t buffer;
};

/** \brief sha1 开始
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \return int 0=未出错
 *
 */
int hsha1_starts(hsha1_context_t *ctx);

/** \brief sha1 更新
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha1_update(hsha1_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha1 完成
 *
 * \param ctx hsha1_context_t* sha1上下文指针
 * \param output hsha1_sha1_t 输出SHA1
 * \return int 0=未出错
 *
 */
int hsha1_finish(hsha1_context_t *ctx,hsha1_sha_t output);

/** \brief sha1计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha1_sha1_t 输出SHA1
 * \return int 0=未出错
 *
 */
int hsha1_sha(const uint8_t *input,size_t ilen,hsha1_sha_t output);


/*
 * SHA2 224
 */
typedef uint8_t hsha2_sha224_t[28];

/*
 * SHA2 消息块
 */
typedef uint8_t hsha2_message_block_t[64];

/*
 * SHA2 SHA224上下文
 */
typedef struct hsha2_sha224_context hsha2_sha224_context_t;
struct hsha2_sha224_context
{
    uint32_t total[2];
    uint32_t A;
    uint32_t B;
    uint32_t C;
    uint32_t D;
    uint32_t E;
    uint32_t F;
    uint32_t G;
    uint32_t H;
    hsha2_message_block_t buffer;
};

/** \brief sha2_sha224 开始
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \return int 0=未出错
 *
 */
int hsha2_sha224_starts(hsha2_sha224_context_t *ctx);

/** \brief sha2_sha224 更新
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha2_sha224_update(hsha2_sha224_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha2_sha224 完成
 *
 * \param ctx hsha2_sha224_context_t* sha2_sha224上下文指针
 * \param output hsha2_sha224_t 输出SHA2 SHA224
 * \return int 0=未出错
 *
 */
int hsha2_sha224_finish(hsha2_sha224_context_t *ctx,hsha2_sha224_t output);

/** \brief sha2_sha224计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha2_sha224_t 输出SHA2 SHA224
 * \return int 0=未出错
 *
 */
int hsha2_sha224(const uint8_t *input,size_t ilen,hsha2_sha224_t output);


/*
 * SHA2 256
 */
typedef uint8_t hsha2_sha256_t[32];

/*
 * SHA2 256上下文（同SHA2 224）
 */
typedef hsha2_sha224_context_t hsha2_sha256_context_t;


/** \brief sha2_sha256 开始
 *
 * \param ctx hsha2_sha256_context_t* sha2_sha256上下文指针
 * \return int 0=未出错
 *
 */
int hsha2_sha256_starts(hsha2_sha256_context_t *ctx);

/** \brief sha2_sha256 更新
 *
 * \param ctx hsha2_sha256_context_t* sha2_sha256上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha2_sha256_update(hsha2_sha256_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha2_sha256 完成
 *
 * \param ctx hsha2_sha256_context_t* sha2_sha256上下文指针
 * \param output hsha2_sha256_t 输出SHA2 SHA256
 * \return int 0=未出错
 *
 */
int hsha2_sha256_finish(hsha2_sha256_context_t *ctx,hsha2_sha256_t output);

/** \brief sha2_sha256计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha2_sha256_t 输出SHA2 SHA256
 * \return int 0=未出错
 *
 */
int hsha2_sha256(const uint8_t *input,size_t ilen,hsha2_sha256_t output);


/*
 * SHA2 384
 */
typedef uint8_t hsha2_sha384_t[48];

/*
 * SHA2 消息块
 */
typedef uint8_t hsha2_message2_block_t[128];

/*
 * SHA2 SHA384上下文
 */
typedef struct hsha2_sha384_context hsha2_sha384_context_t;
struct hsha2_sha384_context
{
    uint64_t total[2];
    uint64_t A;
    uint64_t B;
    uint64_t C;
    uint64_t D;
    uint64_t E;
    uint64_t F;
    uint64_t G;
    uint64_t H;
    hsha2_message2_block_t buffer;
};

/** \brief sha2_sha384 开始
 *
 * \param ctx hsha2_sha384_context_t* sha2_sha384上下文指针
 * \return int 0=未出错
 *
 */
int hsha2_sha384_starts(hsha2_sha384_context_t *ctx);

/** \brief sha2_sha384 更新
 *
 * \param ctx hsha2_sha384_context_t* sha2_sha384上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha2_sha384_update(hsha2_sha384_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha2_sha384 完成
 *
 * \param ctx hsha2_sha384_context_t* sha2_sha384上下文指针
 * \param output hsha2_sha384_t 输出SHA2 SHA384
 * \return int 0=未出错
 *
 */
int hsha2_sha384_finish(hsha2_sha384_context_t *ctx,hsha2_sha384_t output);

/** \brief sha2_sha384计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha2_sha384_t 输出SHA2 SHA384
 * \return int 0=未出错
 *
 */
int hsha2_sha384(const uint8_t *input,size_t ilen,hsha2_sha384_t output);

/*
 * SHA2 512
 */
typedef uint8_t hsha2_sha512_t[64];

/*
 * SHA2 512上下文（同SHA2 384）
 */
typedef hsha2_sha384_context_t hsha2_sha512_context_t;


/** \brief sha2_sha512 开始
 *
 * \param ctx hsha2_sha512_context_t* sha2_sha512上下文指针
 * \return int 0=未出错
 *
 */
int hsha2_sha512_starts(hsha2_sha512_context_t *ctx);

/** \brief sha2_sha512 更新
 *
 * \param ctx hsha2_sha512_context_t* sha2_sha512上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha2_sha512_update(hsha2_sha512_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha2_sha512 完成
 *
 * \param ctx hsha2_sha512_context_t* sha2_sha512上下文指针
 * \param output hsha2_sha512_t 输出SHA2 SHA512
 * \return int 0=未出错
 *
 */
int hsha2_sha512_finish(hsha2_sha512_context_t *ctx,hsha2_sha512_t output);

/** \brief sha2_sha512计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha2_sha512_t 输出SHA2 SHA512
 * \return int 0=未出错
 *
 */
int hsha2_sha512(const uint8_t *input,size_t ilen,hsha2_sha512_t output);

/*
 * SHA3上下文
 */
typedef struct hsha3_context hsha3_context_t;
struct hsha3_context
{
    uint64_t state[25];
    uint32_t index;
    uint16_t olen;
    uint16_t max_block_size;
};

/*
 * SHA3 224上下文
 */
typedef  hsha3_context_t hsha3_sha224_context_t;

/*
 * SHA3 224消息块
 */
typedef uint8_t hsha3_sha224_message_block_t[144];

/*
 * SHA3 224
 */
typedef uint8_t hsha3_sha224_t[28];

/** \brief sha3_sha224 开始
 *
 * \param ctx hsha3_sha224_context_t* sha3_sha224上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_sha224_starts(hsha3_sha224_context_t *ctx);

/** \brief sha3_sha224 更新
 *
 * \param ctx hsha3_sha224_context_t* sha3_sha224上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_sha224_update(hsha3_sha224_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_sha224 完成
 *
 * \param ctx hsha3_sha224_context_t* sha3_sha224上下文指针
 * \param output hsha3_sha224_t 输出SHA3 SHA224
 * \return int 0=未出错
 *
 */
int hsha3_sha224_finish(hsha3_sha224_context_t *ctx,hsha3_sha224_t output);

/** \brief sha3_sha224计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_sha224_t 输出SHA3 SHA224
 * \return int 0=未出错
 *
 */
int hsha3_sha224(const uint8_t *input,size_t ilen,hsha3_sha224_t output);


/*
 * SHA3 256上下文
 */
typedef  hsha3_context_t hsha3_sha256_context_t;

/*
 * SHA3 256消息块
 */
typedef uint8_t hsha3_sha256_message_block_t[136];

/*
 * SHA3 256
 */
typedef uint8_t hsha3_sha256_t[32];

/** \brief sha3_sha256 开始
 *
 * \param ctx hsha3_sha256_context_t* sha3_sha256上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_sha256_starts(hsha3_sha256_context_t *ctx);

/** \brief sha3_sha256 更新
 *
 * \param ctx hsha3_sha256_context_t* sha3_sha256上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_sha256_update(hsha3_sha256_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_sha256 完成
 *
 * \param ctx hsha3_sha256_context_t* sha3_sha256上下文指针
 * \param output hsha3_sha256_t 输出SHA3 SHA256
 * \return int 0=未出错
 *
 */
int hsha3_sha256_finish(hsha3_sha256_context_t *ctx,hsha3_sha256_t output);

/** \brief sha3_sha256计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_sha256_t 输出SHA3 SHA256
 * \return int 0=未出错
 *
 */
int hsha3_sha256(const uint8_t *input,size_t ilen,hsha3_sha256_t output);

/*
 * SHA3 384上下文
 */
typedef  hsha3_context_t hsha3_sha384_context_t;

/*
 * SHA3 384消息块
 */
typedef uint8_t hsha3_sha384_message_block_t[104];

/*
 * SHA3 384
 */
typedef uint8_t hsha3_sha384_t[48];

/** \brief sha3_sha384 开始
 *
 * \param ctx hsha3_sha384_context_t* sha3_sha384上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_sha384_starts(hsha3_sha384_context_t *ctx);

/** \brief sha3_sha384 更新
 *
 * \param ctx hsha3_sha384_context_t* sha3_sha384上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_sha384_update(hsha3_sha384_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_sha384 完成
 *
 * \param ctx hsha3_sha384_context_t* sha3_sha384上下文指针
 * \param output hsha3_sha384_t 输出SHA3 SHA384
 * \return int 0=未出错
 *
 */
int hsha3_sha384_finish(hsha3_sha384_context_t *ctx,hsha3_sha384_t output);

/** \brief sha3_sha384计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_sha384_t 输出SHA3 SHA384
 * \return int 0=未出错
 *
 */
int hsha3_sha384(const uint8_t *input,size_t ilen,hsha3_sha384_t output);


/*
 * SHA3 512上下文
 */
typedef  hsha3_context_t hsha3_sha512_context_t;

/*
 * SHA3 512消息块
 */
typedef uint8_t hsha3_sha512_message_block_t[72];

/*
 * SHA3 512
 */
typedef uint8_t hsha3_sha512_t[64];

/** \brief sha3_sha512 开始
 *
 * \param ctx hsha3_sha512_context_t* sha3_sha512上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_sha512_starts(hsha3_sha512_context_t *ctx);

/** \brief sha3_sha512 更新
 *
 * \param ctx hsha3_sha512_context_t* sha3_sha512上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_sha512_update(hsha3_sha512_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_sha512 完成
 *
 * \param ctx hsha3_sha512_context_t* sha3_sha512上下文指针
 * \param output hsha3_sha512_t 输出SHA3 SHA512
 * \return int 0=未出错
 *
 */
int hsha3_sha512_finish(hsha3_sha512_context_t *ctx,hsha3_sha512_t output);

/** \brief sha3_sha512计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_sha512_t 输出SHA3 SHA512
 * \return int 0=未出错
 *
 */
int hsha3_sha512(const uint8_t *input,size_t ilen,hsha3_sha512_t output);


/*
 * SHA3 shake128上下文
 */
typedef  hsha3_context_t hsha3_shake128_context_t;

/*
 * SHA3 shake128消息块
 */
typedef uint8_t hsha3_shake128_message_block_t[168];

/*
 * SHA3 shake128,shake128可输出更长输出,此处为最小输出
 */
typedef uint8_t hsha3_shake128_t[16];

/** \brief sha3_shake128 开始
 *
 * \param ctx hsha3_shake128_context_t* sha3_shake128上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_shake128_starts(hsha3_shake128_context_t *ctx);

/** \brief sha3_shake128 更新
 *
 * \param ctx hsha3_shake128_context_t* sha3_shake128上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_shake128_update(hsha3_shake128_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_shake128 完成
 *
 * \param ctx hsha3_shake128_context_t* sha3_shake128上下文指针
 * \param output hsha3_shake128_t 输出SHA3 shake128
 * \param output_len size_t 输出长度(需要大于等于最小长度)
 * \return int 0=未出错
 *
 */
int hsha3_shake128_finish(hsha3_shake128_context_t *ctx,hsha3_shake128_t output,size_t output_len);

/** \brief sha3_shake128计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_shake128_t 输出SHA3 shake128
 * \param output_len size_t 输出长度(需要大于等于最小长度)
 * \return int 0=未出错
 *
 */
int hsha3_shake128(const uint8_t *input,size_t ilen,hsha3_shake128_t output,size_t output_len);


/*
 * SHA3 shake256上下文
 */
typedef  hsha3_context_t hsha3_shake256_context_t;

/*
 * SHA3 shake256消息块
 */
typedef uint8_t hsha3_shake256_message_block_t[136];

/*
 * SHA3 shake256,shake256可输出更长输出,此处为最小输出
 */
typedef uint8_t hsha3_shake256_t[32];

/** \brief sha3_shake256 开始
 *
 * \param ctx hsha3_shake256_context_t* sha3_shake256上下文指针
 * \return int 0=未出错
 *
 */
int hsha3_shake256_starts(hsha3_shake256_context_t *ctx);

/** \brief sha3_shake256 更新
 *
 * \param ctx hsha3_shake256_context_t* sha3_shake256上下文指针
 * \param input const uint8_t* 输入数据
 * \param ilen size_t 输入数据长度
 * \return int 0=未出错
 *
 */
int hsha3_shake256_update(hsha3_shake256_context_t *ctx,const uint8_t *input,size_t ilen);

/** \brief sha3_shake256 完成
 *
 * \param ctx hsha3_shake256_context_t* sha3_shake256上下文指针
 * \param output hsha3_shake256_t 输出SHA3 shake256
 * \param output_len size_t 输出长度(需要大于等于最小长度)
 * \return int 0=未出错
 *
 */
int hsha3_shake256_finish(hsha3_shake256_context_t *ctx,hsha3_shake256_t output,size_t output_len);

/** \brief sha3_shake256计算
 *
 * \param input const uint8_t* 输入数据
 * \param ilen size_t    输入数据长度
 * \param output hsha3_shake256_t 输出SHA3 shake256
 * \param output_len size_t 输出长度(需要大于等于最小长度)
 * \return int 0=未出错
 *
 */
int hsha3_shake256(const uint8_t *input,size_t ilen,hsha3_shake256_t output,size_t output_len);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSHA_H__
