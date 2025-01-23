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


#if defined(WIN32) && !defined(HAVE_SDL)
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
    bool fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(hdc!=NULL)
        {
            HDC memdc=CreateCompatibleDC(hdc);
            if(memdc==NULL)
            {
                return false;
            }
            HBITMAP hbitmap=CreateCompatibleBitmap(hdc,w,h);
            if(hbitmap==NULL)
            {
                DeleteDC(memdc);
                return false;
            }
            SelectObject(memdc,hbitmap);
            for(size_t i=0; i<w; i++)
            {
                for(size_t j=0; j<h; j++)
                {
                    //每个像素都进行一次转换
                    hgui_pixel_t final_pixel=hgui_pixel_bits_get(pixel,i+x,j+y);
                    COLORREF color = RGB((final_pixel.pixel_32_bits >> 16) & 0xFF, (final_pixel.pixel_32_bits >> 8) & 0xFF, (final_pixel.pixel_32_bits) & 0xFF);
                    SetPixel(memdc, i, j, color);
                }
            }
            BitBlt(hdc, x, y, w, h, memdc, 0, 0, SRCCOPY);
            SelectObject(memdc,NULL);
            DeleteObject(hbitmap);
            DeleteDC(memdc);
            return true;
        }
        return false;
    }
    static  bool g_fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        return obj.fill_rectangle(driver,x,y,w,h,pixel);
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
                       WS_POPUPWINDOW | WS_CAPTION,  /* popup window */
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
    static hgui_pixel_mode_t  g_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t *new_mode)
    {
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
            if(GetClientRect(hwnd,&rect))
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

            ssize_t new_w = (*w);
            ssize_t new_h = (*h);

            bool ret = (MoveWindow(hwnd,rect.left,rect.top,new_w,new_h,TRUE)!=0);

            if(GetClientRect(hwnd,&rect))
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

            ret = (MoveWindow(hwnd,rect.left,rect.top,2*new_w-(*w),2*new_h-(*h),TRUE)!=0);

            if(GetClientRect(hwnd,&rect))
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
            while(PeekMessage (&messages, hwnd, 0, 0,PM_REMOVE))
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
        driver.fill_rectangle=g_fill_rectangle;
        driver.reset=g_reset;
        driver.resize=g_resize;
        driver.is_ok=g_is_ok;
        driver.pixel_mode = g_pixel_mode;
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
    {
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        if (g_HCPPGuiDriver.hdc != NULL)
        {
            ReleaseDC(g_HCPPGuiDriver.hwnd, g_HCPPGuiDriver.hdc);
            g_HCPPGuiDriver.hdc = NULL;
        }
        g_HCPPGuiDriver.hwnd = NULL;
    }
    break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        hgui_gui_event_key_t key = { 0 };
        key.key_press_or_release = (((message == WM_KEYDOWN) || (message == WM_SYSKEYDOWN)) ? 1 : 0);
        if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
        {
            key.key_mode |= HGUI_GUI_EVENT_KEY_MODE_ALT;
        }
        if (wParam >= '0' && wParam <= '9')
        {
            //数字键
            key.key_value = wParam-'0'+ HGUI_GUI_EVENT_KEY_VALUE_0;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        if (wParam >= 'A' && wParam <= 'Z')
        {
            //字母键
            key.key_value = wParam-'A'+HGUI_GUI_EVENT_KEY_VALUE_a;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        switch (wParam)
        {
        case VK_INSERT:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_INSERT;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_HOME:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_HOME;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_PRIOR:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_PAGEUP;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_END:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_END;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_NEXT:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_PAGEDOWN;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_DELETE:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_DELETE;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_RIGHT:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_RIGHT;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_LEFT:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_LEFT;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_DOWN:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_DOWN;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_UP:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_UP;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_F1:
        case VK_F2:
        case VK_F3:
        case VK_F4:
        case VK_F5:
        case VK_F6:
        case VK_F7:
        case VK_F8:
        case VK_F9:
        case VK_F10:
        case VK_F11:
        case VK_F12:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_F1+wParam-VK_F1;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_NUMPAD1:
        case VK_NUMPAD2:
        case VK_NUMPAD3:
        case VK_NUMPAD4:
        case VK_NUMPAD5:
        case VK_NUMPAD6:
        case VK_NUMPAD7:
        case VK_NUMPAD8:
        case VK_NUMPAD9:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP1+wParam-VK_NUMPAD1;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_NUMPAD0:
        {
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP0+wParam-VK_NUMPAD0;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_DECIMAL:
        {
            //小键盘 .
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_PERIOD;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_SEPARATOR:
        {
            //小键盘 ENTER
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_ENTER;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_ADD:
        {
            //小键盘 +
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_PLUS;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_SUBTRACT:
        {
            //小键盘 -
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_MINUS;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_MULTIPLY:
        {
            //小键盘 *
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_MULTIPLY;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        case VK_DIVIDE:
        {
            //小键盘 /
            key.key_value = HGUI_GUI_EVENT_KEY_VALUE_KP_DIVIDE;
            hgui_gui_event_key_emit(&key, hgui_driver_event_input_helper, &driver);
        }
        break;
        default:
        {
        }
        break;
        }
    }
    break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

#elif defined(HAVE_SDL)
#include <SDL.h>
#include <SDL_version.h>
#ifdef WIN32
#include <windows.h>
#endif // WIN32
hgui_driver &driver=*hgui_driver_default_get();
static uint32_t sdl_keysym_convert(uint32_t keycode)
{
#if ((SDL_MAJOR_VERSION)>=1) && ((SDL_MINOR_VERSION)>2)
    uint32_t ret=keycode;
    switch(keycode)
    {
    case SDLK_INSERT:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_INSERT;
    }
    break;
    case SDLK_HOME:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_HOME;
    }
    break;
    case SDLK_PAGEUP:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_PAGEUP;
    }
    break;
    case SDLK_END:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_END;
    }
    break;
    case SDLK_PAGEDOWN:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_PAGEDOWN;
    }
    break;
    case SDLK_DELETE:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_DELETE;
    }
    break;
    case SDLK_RIGHT:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_RIGHT;
    }
    break;
    case SDLK_LEFT:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_LEFT;
    }
    break;
    case SDLK_DOWN:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_DOWN;
    }
    break;
    case SDLK_UP:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_UP;
    }
    break;
    case SDLK_F1:
    case SDLK_F2:
    case SDLK_F3:
    case SDLK_F4:
    case SDLK_F5:
    case SDLK_F6:
    case SDLK_F7:
    case SDLK_F8:
    case SDLK_F9:
    case SDLK_F10:
    case SDLK_F11:
    case SDLK_F12:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_F1+keycode-SDLK_F1;
    }
    break;
    case SDLK_KP_1:
    case SDLK_KP_2:
    case SDLK_KP_3:
    case SDLK_KP_4:
    case SDLK_KP_5:
    case SDLK_KP_6:
    case SDLK_KP_7:
    case SDLK_KP_8:
    case SDLK_KP_9:
    {
        ret=HGUI_GUI_EVENT_KEY_VALUE_KP1+keycode-SDLK_KP_1;
    }
    break;
    case SDLK_KP_0:
    {
        ret= HGUI_GUI_EVENT_KEY_VALUE_KP0+keycode-SDLK_KP_0;
    }
    break;
    case SDLK_KP_PERIOD:
    {
        //小键盘 .
        ret=HGUI_GUI_EVENT_KEY_VALUE_KP_PERIOD;
    }
    break;
    case SDLK_KP_ENTER:
    {
        //小键盘 ENTER
        ret= HGUI_GUI_EVENT_KEY_VALUE_KP_ENTER;
    }
    break;
    case SDLK_KP_PLUS:
    {
        //小键盘 +
        ret=HGUI_GUI_EVENT_KEY_VALUE_KP_PLUS;
    }
    break;
    case SDLK_KP_MINUS:
    {
        //小键盘 -
        ret=HGUI_GUI_EVENT_KEY_VALUE_KP_MINUS;
    }
    break;
    case SDLK_KP_MULTIPLY:
    {
        //小键盘 *
        ret=HGUI_GUI_EVENT_KEY_VALUE_KP_MULTIPLY;
    }
    break;
    case SDLK_KP_DIVIDE:
    {
        //小键盘 /
        ret = HGUI_GUI_EVENT_KEY_VALUE_KP_DIVIDE;
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
#else
    return keycode;
#endif
}
static class HCPPGuiDriver
{
    std::recursive_mutex m_lock;
    SDL_Surface *screen;
public:
    bool fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
    {
        (void)driver;
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(screen!=NULL)
        {
            for(size_t i=x; i<x+w; i++)
            {
                for(size_t j=y; j<y+h; j++)
                {
                    //每个像素都进行一次转换
                    hgui_pixel_t final_pixel=hgui_pixel_bits_get(pixel,i,j);
                    SDL_Rect fill_rect= {(Sint16)i,(Sint16)j,(Uint16)1,(Uint16)1};
                    SDL_FillRect(screen, &fill_rect, final_pixel.pixel_32_bits);
                }
            }
            SDL_Flip(screen);
            return true;
        }
        return false;
    }
    static bool g_fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
    {
        if(driver==NULL || driver->usr==NULL)
        {
            return false;
        }
        HCPPGuiDriver &obj=*(HCPPGuiDriver *)driver->usr;
        return obj.fill_rectangle(driver,x,y,w,h,pixel);
    }
    bool reset(hgui_driver_t *driver)
    {
        (void)driver;
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if((SDL_WasInit(SDL_INIT_EVERYTHING)&SDL_INIT_VIDEO)==0)
        {
            if(SDL_Init(SDL_INIT_EVERYTHING)<0)
            {
                return false;
            }
        }
        if(screen==NULL)
        {
            screen=SDL_SetVideoMode(320,240,32,SDL_HWSURFACE);
            SDL_WM_SetCaption("","sdl");
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
        (void)driver;
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        return screen!=NULL;
    }
    static hgui_pixel_mode_t  g_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t *new_mode)
    {
        (void)driver;
        (void)new_mode;
        return HGUI_PIXEL_MODE_32_BITS;
    }
    bool resize(hgui_driver_t *driver,ssize_t *w,ssize_t *h)
    {
        (void)driver;
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(w==NULL || h == NULL)
        {
            return false;
        }
        bool ret=false;
        bool need_resize=false;
        if((*w) > 0 || (*h) > 0)
        {
            need_resize=true;
        }
        if((*w) <0)
        {
            (*w)=screen->w;
            ret=true;
        }
        if((*h) <0)
        {
            (*h)=screen->h;
            ret=true;
        }
        if(need_resize)
        {
            if(screen!=NULL)
            {
                SDL_FreeSurface(screen);
                //重新创建窗口，需要重新绘制
                screen=SDL_SetVideoMode((*w),(*h),32,SDL_HWSURFACE);
                SDL_WM_SetCaption("","sdl");
                ret=true;
            }
        }

        (*w)=screen->w;
        (*h)=screen->h;

        return ret;
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
            SDL_Event event= {0};
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                case SDL_QUIT:
                {
                    screen=NULL;
                }
                break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                {
                    SDL_KeyboardEvent *key = (SDL_KeyboardEvent*)&event;
                    hgui_gui_event_key_t hkey= {0};
                    hkey.key_value=sdl_keysym_convert(key->keysym.sym);
                    hkey.key_mode=key->keysym.mod;
                    hkey.key_press_or_release=((event.type==SDL_KEYDOWN)?1:0);
                    hgui_gui_event_key_emit(&hkey,hgui_driver_event_input_helper,driver);
                }
                break;
                default:
                {

                }
                break;
                }
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
    HCPPGuiDriver():screen(NULL)
    {
        driver.usr=this;
        driver.fill_rectangle=g_fill_rectangle;
        driver.reset=g_reset;
        driver.resize=g_resize;
        driver.is_ok=g_is_ok;
        driver.pixel_mode = g_pixel_mode;
#ifdef WIN32
#ifndef __CYGWIN__
        SDL_SetModuleHandle(GetModuleHandle(NULL));
#endif // __CYGWIN__
#endif // WIN32
    }
    ~HCPPGuiDriver()
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);
        if(screen!=NULL)
        {
            SDL_FreeSurface(screen);
        }
        screen=NULL;
        SDL_Quit();
    }
} g_HCPPGuiDriver;

void *HCPPGuiInit(void)
{
    return &g_HCPPGuiDriver;
}

#else
//不支持的平台
void *HCPPGuiInit(void)
{
    return NULL;
}

#endif // WIN32
