
#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

void IsInHeap_IsInThread_main();
void Parent_main();
void Thread_main();
void Timer_main();
void Socket_main();
void Init_main();

int main()
{

    IsInHeap_IsInThread_main();

    Parent_main();

    Thread_main();

    Timer_main();

    Socket_main();

    Init_main();

#ifdef WIN32
    printf("press any key to exit\n");
    getchar();
#endif // WIN32

    return 0;
}
