#include "hbox.h"
#include "stdio.h"

//无栈协程对局部变量的使用有限制，使用全局变量
static int co1_step=0;
HSTACKLESSCOROUTINE_BLOCK_START(co1_c)
//协程语句块
printf("co1_c:ccb=%08X,event=%08X\r\n",(uint32_t)(uintptr_t)HSTACKLESSCOROUTINE_GET_CURRENT_CCB(),(uint32_t)(uintptr_t)HSTACKLESSCOROUTINE_GET_CURRENT_EVENT());
hstacklesscoroutine_yield();//让出控制权
printf("co1_c:step 1\r\n");
if(co1_step++!=1)
{
    //未达到条件，不执行下一步。
    hstacklesscoroutine_return();
}
hstacklesscoroutine_yield_with_label(2);
printf("co1_c:step 2\r\n");
co1_step++;
hstacklesscoroutine_yield_with_label(3);
printf("co1_c:step 3\r\n");
co1_step++;
hstacklesscoroutine_yield_with_label(4);
printf("co1_c:step 4\r\n");
co1_step++;
if(co1_step <=5)
{
    //返回标签2
    hstacklesscoroutine_goto_label(2);
}
hstacklesscoroutine_yield_with_label(5);
printf("co1_c:step 5\r\n");
HSTACKLESSCOROUTINE_BLOCK_END(co1_c)




