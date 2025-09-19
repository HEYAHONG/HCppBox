#include "stdio.h"
#include "stdlib.h"
#include "hbox.h"
#include "opensbi_port.h"
#include "ctype.h"


int main()
{

    hshell_printf(NULL,"main enter!\r\n");

    hruntime_init_lowlevel();

    hruntime_init();

    while(true)
    {
        hruntime_loop();
    }
}

static void print_blank(size_t n)
{
    while(n--)
    {
        hshell_printf(NULL,"    ");
    }
}

static void print_fdt(const void *fdt,int offset,int depth)
{
    if(offset < 0 || depth < 0)
    {
        return;
    }
    /*
     * 遍历属性
     */
    {
        if(offset >= 0)
        {
            const char *name=fdt_get_name(fdt,offset,NULL);
            if(name!=NULL)
            {
                if(name[0]=='\0')
                {
                    name="/";
                }
                print_blank(depth);
                hshell_printf(NULL,"%s\r\n",name);
            }
            {
                int prop_offset=0;
                fdt_for_each_property_offset(prop_offset,fdt,offset)
                {
                    int prop_size=0;
                    const char *prop_name=NULL;
                    const uint8_t *prop_value = (const uint8_t *)fdt_getprop_by_offset(fdt, prop_offset, &prop_name, &prop_size);
                    if(name!=NULL)
                    {
                        bool is_printable_string=(prop_value!=NULL) && (prop_size > 0) && (prop_value[prop_size-1]=='\0');
                        for(size_t i=0; i<prop_size; i++)
                        {
                            if((i+1)==prop_size)
                            {
                                break;
                            }
                            if(((!isprint((char)prop_value[i])  && (strcmp(prop_name,"model")!=0)) && !(prop_value[i]=='\0' && (strcmp(prop_name,"compatible")==0))))
                            {
                                is_printable_string=false;
                                break;
                            }
                        }
                        if(is_printable_string)
                        {
                            print_blank(depth+1);
                            const char *value=(const char *)prop_value;
                            hshell_printf(NULL,"%-32s:",prop_name);
                            size_t value_base=0;
                            while(value_base+strlen(&value[value_base])+1 <= prop_size)
                            {
                                hshell_printf(NULL,"%s ",&value[value_base]);
                                value_base+=(strlen(&value[value_base])+1);
                            }
                            hshell_printf(NULL,"\r\n");
                        }
                        else
                        {
                            print_blank(depth+1);
                            switch(prop_size)
                            {
                            case 0:
                            {
                                hshell_printf(NULL,"%-32s\r\n",prop_name);
                            }
                            break;
                            case 1:
                            {
                                uint8_t value=prop_value[0];
                                hshell_printf(NULL,"%-32s:%02X\r\n",prop_name,(int)value);
                            }
                            break;
                            case 2:
                            {
                                uint16_t value=fdt16_ld((const fdt16_t *)prop_value);
                                hshell_printf(NULL,"%-32s:%04X\r\n",prop_name,(int)value);
                            }
                            break;
                            case 4:
                            {
                                uint32_t value=fdt32_ld((const fdt32_t *)prop_value);
                                hshell_printf(NULL,"%-32s:%08X\r\n",prop_name,(int)value);
                            }
                            break;
                            case 8:
                            {
                                uint64_t value=fdt64_ld((const fdt64_t *)prop_value);
                                hshell_printf(NULL,"%-32s:%08X%08X\r\n",prop_name,(int)((value>>32)&0xFFFFFFFF),(int)((value>>0)&0xFFFFFFFF));
                            }
                            break;
                            default:
                            {
                                char value[512]= {0};
                                hbase16_encode_with_null_terminator(value,sizeof(value),prop_value,prop_size);
                                hshell_printf(NULL,"%-32s:%s\r\n",prop_name,value);
                            }
                            break;
                            }
                        }
                    }
                }
            }
        }
    }
    /*
     * 遍历子节点
     */
    {
        if(offset >= 0)
        {
            int child=0;
            fdt_for_each_subnode(child,fdt,offset)
            {
                print_fdt(fdt,child,depth+1);
            }
        }
    }
}

/*
 * 主初始化
 */
void  main_init(const hruntime_function_t *func)
{
    //注册命令
    HSHELL_COMMANDS_REGISTER(NULL);
    {
        //打印陷入地址(中断与异常处理的地址)
        uintptr_t stvec=0;
        asm volatile ("csrr %0,stvec":"+r"(stvec)::"memory");
        hshell_printf(NULL,"stvec=%p\r\n",stvec);
    }
    hshell_printf(NULL,"fdt addr=0x%p,len=%d bytes,header check %s!\r\n",sbi_entry_para_data.a1,fdt_totalsize((void *)sbi_entry_para_data.a1),fdt_check_header((void *)sbi_entry_para_data.a1)==0?"ok":"failed");
    if(fdt_check_header((void *)sbi_entry_para_data.a1)==0)
    {
        const void *fdt=(void *)sbi_entry_para_data.a1;
        size_t fdt_total_size=fdt_totalsize(fdt);
        if(fdt_check_full(fdt,fdt_total_size)==0)
        {
            hshell_printf(NULL,"device tree:\r\n");
            print_fdt(fdt,0,1);
        }
    }
    {
        extern  uint8_t __heap_start[];
        extern  uint8_t __heap_end[];
        hshell_printf(NULL,"Heap:start=%p,end=%p\r\n",__heap_start,__heap_end);
    }
    hshell_printf(NULL,"HBox Init(tick=%llu)!\r\n",(unsigned long long)hdefaults_tick_get());
}
HRUNTIME_INIT_EXPORT(main,255,main_init,NULL);
HRUNTIME_SYMBOL_EXPORT(main_init);

/*
 * 主循环
 */
void  main_loop(const hruntime_function_t *func)
{
    while(EOF!=hshell_loop(NULL));
}
HRUNTIME_LOOP_EXPORT(main,255,main_loop,NULL);
HRUNTIME_SYMBOL_EXPORT(main_loop);
