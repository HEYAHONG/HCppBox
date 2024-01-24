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
        printf("RCEnum\r\n");
        RCEnum([](const unsigned char *Name,size_t NameLength,const unsigned char *Resource,size_t ResourceLength)
               {
                   printf("name:%s\r\ndata:\r\n%s",Name,Resource);
               });
    }
    return 0;
}
