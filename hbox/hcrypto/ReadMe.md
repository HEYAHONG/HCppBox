# 说明

本组件主要实现一些软件实现的加密算法，用于在没有硬件专门支持的情况下使用相关算法。

# 数据编码/解码

## base16

16进制字符串，类似base64，可用于将二进制编码为字符串。

- [hbase16.h](hbase16.h)
- [hbase16.c](hbase16.c)

## base64

- [hbase64.h](hbase64.h)
- [hbase64.c](hbase64.c)

## ASN.1

- [hasn1.c](hasn1.c)
- [hasn1.h](hasn1.h)
- [hasn1](hasn1)

# 数据校验

## BCC

- [hbcc.h](hbcc.h)
- [hbcc.c](hbcc.c)

## LRC

- [hlrc.h](hlrc.h)
- [hlrc.c](hlrc.c)

## CRC

- [hcrc.h](hcrc.h)
- [hcrc.c](hcrc.c)

# 对称加密

## 分组密码

### AES

- [haes.h](haes.h)
- [haes.c](haes.c)

### SM4

- [hsm4.h](hsm4.h)
- [hsm4.c](hsm4.c)

# 消息摘要

## MD4

- [hmd4.h](hmd4.h)
- [hmd4.c](hmd4.c)

## MD5

- [hmd5.h](hmd5.h)
- [hmd5.c](hmd5.c)

## SHA

- [hsha.h](hsha.h)
- [hsha.c](hsha.c)

## RIPEMD160

- [hripemd160.h](hripemd160.h)
- [hripemd160.c](hripemd160.c)

## SM3

- [hsm3.h](hsm3.h)
- [hsm3.c](hsm3.c)

# 消息认证码

## HMAC

基于哈希运算的消息认证码。

注意：仅仅实现了本组件支持的消息摘要算法的HMAC算法。

- [hhmac.h](hhmac.h)
- [hhmac.c](hhmac.c)

