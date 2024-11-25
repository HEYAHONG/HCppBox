/***************************************************************
 * Name:      HCPPGui.cpp
 * Purpose:   声明HCPPGui对外接口。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hbox.h"
#include "HCPPGui.h"
#include "mutex"
#include "thread"
#include "chrono"


#ifdef WIN32
hgui_driver &driver=*hgui_driver_default_get();
#include "windows.h"
#include "tchar.h"
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


static class HCPPGuiDriver
{
    bool IsRegisterClass;
    HWND hwnd;
    HDC hdc;
    friend LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    std::recursive_mutex m_lock;
public:
    bool draw_pixel(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(hdc !=NULL)
        {
            SetPixel(hdc,x,y,pixel.pixel_32_bits);
            return true;
        }
        return false;
    }
    static  bool g_draw_pixel(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        return obj.draw_pixel(driver,x,y,pixel);
    }
    bool reset(hgui_driver_t *driver)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(hwnd!=NULL)
        {
            if(hdc!=NULL)
            {
                ReleaseDC(hwnd,hdc);
                hdc=NULL;
            }
            DestroyWindow(hwnd);
            hwnd=NULL;
        }
        const TCHAR szClassName[]=_T("HCPPGuiDriverWin32");
        if(!IsRegisterClass)
        {
            WNDCLASSEX wincl;        /* Data structure for the windowclass */

            /* The Window structure */
            wincl.hInstance = GetModuleHandleA(NULL);
            wincl.lpszClassName = szClassName;
            wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
            wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
            wincl.cbSize = sizeof (WNDCLASSEX);

            /* Use default icon and mouse-pointer */
            wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
            wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
            wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
            wincl.lpszMenuName = NULL;                 /* No menu */
            wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
            wincl.cbWndExtra = 0;                      /* structure or the window instance */
            /* Use Windows's default colour as the background of the window */
            wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

            IsRegisterClass=(RegisterClassEx (&wincl)!=0);
        }
        if(IsRegisterClass && hwnd==NULL)
        {
            /* The class is registered, let's create the program*/
            hwnd = CreateWindowEx (
                       0,                   /* Extended possibilites for variation */
                       szClassName,         /* Classname */
                       _T(""),       /* Title Text */
                       WS_OVERLAPPEDWINDOW, /* default window */
                       CW_USEDEFAULT,       /* Windows decides the position */
                       CW_USEDEFAULT,       /* where the window ends up on the screen */
                       320,                 /* The programs width */
                       240,                 /* and height in pixels */
                       HWND_DESKTOP,        /* The window is a child-window to desktop */
                       NULL,                /* No menu */
                       GetModuleHandleA(NULL),       /* Program Instance handler */
                       NULL                 /* No Window Creation data */
                   );
            if(hwnd!=NULL)
            {
                /* Make the window visible on the screen */
                ShowWindow (hwnd,SW_SHOW);

                hdc=GetDC(hwnd);
            }
        }


        return true;
    }
    static bool g_reset(hgui_driver_t *driver)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        return obj.reset(driver);
    }
    bool is_ok(hgui_driver_t *driver)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        return hwnd!=NULL;
    }
    hgui_pixel_mode_t  g_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t *new_mode)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        (void)driver;
        (void)new_mode;
        return HGUI_PIXEL_MODE_32_BITS;
    }
    bool resize(hgui_driver_t *driver,ssize_t *w,ssize_t *h)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(hwnd==NULL || w==NULL || h==NULL)
        {
            return false;
        }
        {
            RECT rect= {0};
            if(GetWindowRect(hwnd,&rect))
            {
                if((*w) < 0 )
                {
                    (*w) = rect.right - rect.left;
                }

                if((*h) < 0 )
                {
                    (*h) = rect.bottom - rect.top;
                }
            }

            bool ret = (MoveWindow(hwnd,rect.left,rect.top,(*w),(*h),TRUE)!=0);

            if(GetWindowRect(hwnd,&rect))
            {
                (*w) = rect.right - rect.left;
                if((*w) < 0)
                {
                    (*w)=-(*w);
                }
                (*h) = rect.bottom - rect.top;
                if((*h) < 0)
                {
                    (*h)=-(*h);
                }
            }

            return ret;
        }
        return false;
    }

    static bool g_resize(hgui_driver_t *driver,ssize_t *w,ssize_t *h)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        return obj.resize(driver,w,h);
    }

    bool update(hgui_driver_t *driver)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        {
            MSG messages;
            if(PeekMessage (&messages, hwnd, 0, 0,PM_REMOVE))
            {
                /* Translate virtual-key messages into character messages */
                TranslateMessage(&messages);
                /* Send message to WindowProcedure */
                DispatchMessage(&messages);
            }
        }
        return true;
    }
    static bool g_is_ok(hgui_driver_t *driver)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        obj.update(driver);
        return obj.is_ok(driver);
    }
    HCPPGuiDriver():IsRegisterClass(false),hwnd(NULL),hdc(NULL)
    {
        driver.usr=this;
        driver.draw_pixel=g_draw_pixel;
        driver.reset=g_reset;
        driver.resize=g_resize;
        driver.is_ok=g_is_ok;
    }
    ~HCPPGuiDriver()
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(hwnd!=NULL)
        {
            if(hdc!=NULL)
            {
                ReleaseDC(hwnd,hdc);
                hdc=NULL;
            }
            DestroyWindow(hwnd);
            hwnd=NULL;
        }
    }
}
g_HCPPGuiDriver;

void *HCPPGuiInit(void)
{
    return &g_HCPPGuiDriver;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        if(g_HCPPGuiDriver.hdc!=NULL)
        {
            ReleaseDC(g_HCPPGuiDriver.hwnd,g_HCPPGuiDriver.hdc);
            g_HCPPGuiDriver.hdc=NULL;
        }
        g_HCPPGuiDriver.hwnd=NULL;
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

#else

void *HCPPGuiInit(void)
{
    return NULL;
}

#endif // WIN32
