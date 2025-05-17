

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
    hprintf("helloworld coff!\r\n");
    return 0;
}

int coff_exit()
{
    coff_data.refcnt--;
    return 0;
}


