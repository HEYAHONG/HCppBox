# 说明

本组件主要实现一些软件实现的加密算法，用于在没有硬件专门支持的情况下使用相关算法。

# 大数计算

大数计算是很多算法的基础。

## hbignum

本组件主要用于软件实现大数计算,与硬件架构无关，但效率一般不高。

- [hbignum.h](hbignum.c)
- [hbignum.c](hbignum.c)
- [hbignum](hbignum)

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
- [hcrc](hcrc)

# 对称加密

## 分组密码

### AES

- [haes.h](haes.h)
- [haes.c](haes.c)

### SM4

- [hsm4.h](hsm4.h)
- [hsm4.c](hsm4.c)

## 流加密

### RC4

- [hrc4.h](hrc4.h)
- [hrc4.c](hrc4.c)

### ChaCha20

- [hchacha20.h](hchacha20.h)
- [hchacha20.c](hchacha20.c)

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

## POLY1305

- [hpoly1305.h](hpoly1305.h)
- [hpoly1305.c](hpoly1305.c)

# 数字签名

## ECDSA

注意：本组件只实现了某些常用位数的版本(如256位)，本组件需要哈希运算的结果作为输入值，不包含哈希过程。

- [hecdsa.h](hecdsa.h)
- [hecdsa.c](hecdsa.c)
- [hecdsa](hecdsa)

## EdDSA

- [heddsa.h](heddsa.h)
- [heddsa.c](heddsa.c)
- [heddsa](heddsa)

# 不对称加密

## RSA

**注意：本组件运算速度极其慢，仍然需要优化。**

- [hrsa.h](hrsa.h)
- [hrsa.c](hrsa.c)
- [hrsa](hrsa)
