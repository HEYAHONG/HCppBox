

struct
{
    int refcnt;
} coff_data = {0};

const struct
{
    const char *name;
} coff_info=
{
    "helloworld",
};
 /*
  * x86_64模式下，编译器生成的32位相对地址可能会溢出，故而不直接调用外部函数，如需调用应使用函数指针,且函数的参数也应当尽量使用指针
  * AARCH64同x86_64 
  */
#if defined(_M_X64) || defined(__x86_64) || defined(__x86_64__) || defined(__aarch64__) || defined(__AARCH64EL__)
int (*hprintf)(const char *fmt,...)=0;
#else
extern int hprintf(const char *fmt,...);
#endif
int coff_entry()
{
    coff_data.refcnt++;
#if defined(_M_X64) || defined(__x86_64) || defined(__x86_64__)
    if(hprintf!=0)
    {
        hprintf("helloworld coff!\r\n");
    }
#else
    hprintf("helloworld coff!\r\n");
#endif
    return 0;
}


int coff_exit()
{
    coff_data.refcnt--;
    return 0;
}



