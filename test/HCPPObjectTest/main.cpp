
#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

void IsInHeap_IsInThread_main();
void Parent_main();

int main()
{

    IsInHeap_IsInThread_main();

    Parent_main();
#ifdef WIN32
    printf("press any key to exit\n");
    getchar();
#endif // WIN32

    return 0;
}
