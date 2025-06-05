/***************************************************************
 * Name:      hbignum.c
 * Purpose:   实现hbignum接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbignum.h"

/*
 * 使能常用的位数
 */
#ifndef HUINT128
#define HUINT128  1
#endif // HUINT128
#ifndef HUINT160
#define HUINT160  1
#endif // HUINT160

#ifndef HUINT256
#define HUINT256  1
#endif // HUINT256
#ifndef HUINT288
#define HUINT288  1
#endif // HUINT288


#ifndef HUINT512
#define HUINT512  1
#endif // HUINT512
#ifndef HUINT544
#define HUINT544  1
#endif // HUINT544

#ifndef HUINT1024
#define HUINT1024  1
#endif // HUINT1024
#ifndef HUINT1056
#define HUINT1056  1
#endif // HUINT1056

#ifndef HUINT2048
#define HUINT2048  1
#endif // HUINT2048
#ifndef HUINT2080
#define HUINT2080  1
#endif // HUINT2080

#ifndef HUINT4096
#define HUINT4096  1
#endif // HUINT4096
#ifndef HUINT4128
#define HUINT4128 1
#endif // HUINT4128

#ifndef HUINT8192
#define HUINT8192  1
#endif // HUINT8192
#ifndef HUINT8224
#define HUINT8224 1
#endif // HUINT8224

#include "hbignum/huint.c"

