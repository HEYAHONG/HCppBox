/***************************************************************
 * Name:      hed25519_getrandom.c
 * Purpose:   实现hed25519_getrandom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-08-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hed25519_getrandom.h"

int hed25519_rand(void)
{
    int ret=0;
    HED25519_GETRANDOM(&ret,sizeof(ret),0);
    return ((unsigned int)ret)%(HED25519_RAND_MAX);
}


