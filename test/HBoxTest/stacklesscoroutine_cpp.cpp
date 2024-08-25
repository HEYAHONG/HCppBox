#include "hbox.h"
#include "stdio.h"

//无栈协程对局部变量的使用有限制，使用全局变量
static int co1_step=0;
static bool wait_for_ready(void *usr,hstacklesscoroutine_event_t *event)
{
    printf("co1_cpp:await %d\r\n",co1_step++);
    if(co1_step >=10)
    {
        return true;
    }
    return false;
}
HSTACKLESSCOROUTINE_BLOCK_START(co1_cpp)
//协程语句块
co1_step=0;
printf("co1_cpp:ccb=%08X,event=%08X\r\n",(uint32_t)(uintptr_t)HSTACKLESSCOROUTINE_GET_CURRENT_CCB(),(uint32_t)(uintptr_t)HSTACKLESSCOROUTINE_GET_CURRENT_EVENT());
hstacklesscoroutine_yield();//让出控制权
printf("co1_cpp:step 1\r\n");
if(co1_step++!=1)
{
    //未达到条件，不执行下一步。
    hstacklesscoroutine_return();
}
hstacklesscoroutine_yield_with_label(2);
printf("co1_cpp:step 2\r\n");
co1_step++;
hstacklesscoroutine_yield_with_label(3);
printf("co1_cpp:step 3\r\n");
co1_step++;
hstacklesscoroutine_yield_with_label(4);
printf("co1_cpp:step 4\r\n");
co1_step++;
if(co1_step <=5)
{
    //返回标签2
    hstacklesscoroutine_goto_label(2);
}
else
{
    //等待条件
    hstacklesscoroutine_await((hstacklesscoroutine_awaiter_t{wait_for_ready,NULL}));
}
hstacklesscoroutine_yield_with_label(5);
printf("co1_cpp:step 5\r\n");
HSTACKLESSCOROUTINE_BLOCK_END(co1_cpp)
