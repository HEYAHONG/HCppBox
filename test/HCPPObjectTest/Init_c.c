#include "HCPPInit.h"
#include "stdio.h"
static void init_blank(void)
{
    printf("Init_c init (blank)\r\n");
}

static void init_0(void)
{
    printf("Init_c init 0\r\n");
}

static void init_1(void)
{
    printf("Init_c init 1\r\n");
}

static void init_2(void)
{
    printf("Init_c init 2\r\n");
}

/*
此函数由Init.cpp调用
*/
void Init_c_early_init()
{
    HCPPInitRegister(NULL,init_blank);
    HCPPInitRegister("1",init_1);
    HCPPInitRegister("2",init_2);
    HCPPInitRegister("0",init_0);
}

