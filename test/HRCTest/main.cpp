#include "hrc.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
    {
        const char * banner=(const char *)RCGetHandle((const char *)"banner");
        if(banner!=NULL)
        {
            printf("banner:\r\n%s\r\n",banner);
        }
    }
    {
        //枚举测试
        printf("RCEnum:\r\n");
        printf("\t| %-64s\t| %s\r\n","Name","Length");
        RCEnum([](const unsigned char *Name,size_t NameLength,const unsigned char *Resource,size_t ResourceLength)
        {
            printf("\t| %-64s\t| %d\r\n",Name,(int)ResourceLength);
        });
    }
    return 0;
}
