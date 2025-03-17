#include "hbox.h"
#include <thread>
int main(int argc,const char *argv[])
{
#ifndef WIN32
    hshell_printf(NULL,"press Ctrl-D to exit!\r\n");
#endif
    //一般终端中无需开启回显，串口终端才开启回显
    hshell_echo_set(NULL,false);
    while(hshell_loop(NULL)==0)
    {
        std::this_thread::yield();
    }
    hshell_printf(NULL,"hshell exit!\r\n");
    return 0;
}
