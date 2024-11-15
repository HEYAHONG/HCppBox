#include "HCPPInit.h"
#include "stdio.h"
static void init_blank()
{
    printf("Init_cpp init (blank)\r\n");
}

static void init_0()
{
    printf("Init_cpp init 0\r\n");
}

static void init_1()
{
    printf("Init_cpp init 1\r\n");
}

static void init_2()
{
    printf("Init_cpp init 1\r\n");
}

static void Init_cpp_early_init()
{
    HCPPInitRegister("",init_blank);
    HCPPInitRegister("1",init_1);
    HCPPInitRegister("2",init_2);
    HCPPInitRegister("0",init_0);
    HCPPInitRegister("3",[]()
    {
        printf("Init_cpp init 3\r\n");
    });
    HCPPInitRegister("5",[]()
    {
        printf("Init_cpp init 5\r\n");
    });
    HCPPInitRegister("4",[]()
    {
        printf("Init_cpp init 4\r\n");
    });
}

static class Init_cpp_early_init_class
{
public:
    Init_cpp_early_init_class()
    {
        Init_cpp_early_init();
    }
} g_Init_cpp_early_init_class;

