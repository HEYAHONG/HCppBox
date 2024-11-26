
#include "HCPPBox.h"
#include "hbox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

int main()
{
    HCPPGuiInit();

    if(hgui_driver_reset(NULL))
    {
        size_t i=0;
        while(hgui_driver_is_ok(NULL))
        {
            i++;
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            if(i==100)
            {
                ssize_t w=320,h=240;
                {
                    hgui_driver_resize(NULL,&w,&h);
                }
                hgui_pixel_t pixel;
                if(hgui_driver_pixel_mode(NULL,NULL)==HGUI_PIXEL_MODE_24_BITS || hgui_driver_pixel_mode(NULL,NULL)==HGUI_PIXEL_MODE_32_BITS)
                {
                    pixel.pixel_32_bits=0xFF0000FF;
                    pixel.mode=hgui_driver_pixel_mode(NULL,NULL);
                }
                hgui_driver_fill_rectangle(NULL,0,0,w,h,pixel);
            }
        }
    }

    return 0;
}
