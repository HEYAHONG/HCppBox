#include "hbox_config.h"
#include "hbox.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED

hdefaults_tick_t hbox_tick_get(void)
{
    return 0;
}

void hbox_enter_critical()
{

}

void hbox_exit_critical()
{

}

void * hbox_malloc(size_t bytes)
{
    return malloc(bytes);
}

void hbox_free(void *ptr)
{
    free(ptr);
}

