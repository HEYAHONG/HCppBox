#include "common_wxwidgets.h"
#include <wx/image.h>
#include "thread"
#include <atomic>
#include <chrono>

static std::atomic_bool hcppbox_thread_flag=true;
static void hcppbox_thread_entry(void)
{
    while(hcppbox_thread_flag)
    {
        hcpprt_loop();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    hcppbox_thread_flag=true;
}

void hcppbox_init(void)
{
    if(hcpprt_init_done())
    {
        return;
    }


    hcpprt_init();

    hruntime_loop_enable_softwaretimer(false);

    {

        hcppbox_thread_flag=true;
        std::thread hcppbox_thread(hcppbox_thread_entry);
        hcppbox_thread.detach();
    }

    HCPPInit();

    wxInitAllImageHandlers();
}

void hcppbox_deinit(void)
{
    if(!hcppbox_thread_flag)
    {
        return;
    }

    hcppbox_thread_flag=false;

    std::chrono::time_point tp_start=std::chrono::steady_clock::now();
    while((!hcppbox_thread_flag) && ((std::chrono::steady_clock::now()-tp_start) < std::chrono::seconds(3)))
    {
        std::this_thread::yield();
    }
}

void hcppbox_softwaretimer_isr(void)
{
    if(hcpprt_init_done())
    {
        hsoftwaretimer_mainloop();
    }
}
