

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

extern int hprintf(const char *fmt,...);
int coff_entry()
{
    coff_data.refcnt++;
    /*
     * x86_64模式下，编译器生成的32位地址可能会溢出，故而不直接调用外部函数，如需调用应使用函数指针
     */
#if !defined(_M_X64) && !defined(__x86_64) && !defined(__x86_64__)
    hprintf("helloworld coff!\r\n");
#endif
    return 0;
}

int coff_exit()
{
    coff_data.refcnt--;
    return 0;
}


