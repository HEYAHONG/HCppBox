/***************************************************************
 * Name:      hgui_scene1_app.h
 * Purpose:   实现app接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_scene1_app.h"
#ifndef __HGUI_SCENE1_APP_C__
#define __HGUI_SCENE1_APP_C__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef HGUI_SCENE1_APP_DRIVER
#define HGUI_SCENE1_APP_DRIVER      NULL
#endif // HGUI_SCENE1_APP_DRIVER

#ifndef HGUI_SCENE1_APP_WIDTH
#define HGUI_SCENE1_APP_WIDTH       128
#endif // HGUI_SCENE1_APP_WIDTH

#ifndef HGUI_SCENE1_APP_HEIGHT
#define HGUI_SCENE1_APP_HEIGHT      64
#endif // HGUI_SCENE1_APP_HEIGHT


#ifndef HGUI_SCENE1_APP_SCREEN_STACK_DEPTH
#define HGUI_SCENE1_APP_SCREEN_STACK_DEPTH      4
#endif // HGUI_SCENE1_APP_SCREEN_STACK_DEPTH

#if (HGUI_SCENE1_APP_SCREEN_STACK_DEPTH) < 4
#undef  HGUI_SCENE1_APP_SCREEN_STACK_DEPTH
#define HGUI_SCENE1_APP_SCREEN_STACK_DEPTH      4
#endif

typedef struct
{
    struct                                  //状态信息
    {
        uint32_t be_init:1;                 //是否初始化
        uint32_t need_refresh:1;            //是否需要刷新
    };
    struct                                  //屏幕栈信息
    {
        size_t   screen_stack_pointer;                                                  // 栈指针
        hgui_scene1_screen_base_t *screen_stack[HGUI_SCENE1_APP_SCREEN_STACK_DEPTH];    //屏幕栈
    };

    hgui_gui_event_callback_t event_cb;//事件回调

} hgui_scene1_app_status_t;                 //APP状态，包含所有APP可修改的属性

struct hgui_scene1_app
{
    hgui_driver_t * driver;
    size_t          w;
    size_t          h;
    bool  (*init)(const hgui_scene1_app_t *app,void *usr);
    bool  (*update)(const hgui_scene1_app_t *app,void *usr);
    hgui_scene1_app_status_t *status;
};

#ifndef HGUI_SCENE1_APP_ON_INIT_SUCCESS
#define HGUI_SCENE1_APP_ON_INIT_SUCCESS {}
#endif // HGUI_SCENE1_APP_ON_INIT_SUCCESS

#ifndef HGUI_SCENE1_APP_ON_INIT_FAILURE
#define HGUI_SCENE1_APP_ON_INIT_FAILURE {}
#endif // HGUI_SCENE1_APP_ON_INIT_FAILURE

static bool hgui_scene1_app_event_input_helper(uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    const hgui_scene1_app_t *app=&g_hgui_scene1_app;
    {
        hgui_scene1_screen_base_t *screen=app->status->screen_stack[app->status->screen_stack_pointer];
        if(screen!=NULL && screen->event_cb!=NULL)
        {
            if(app->status->event_cb==NULL)
            {
                return screen->event_cb(screen,app,type,eventparam,eventparam_length,usr);
            }
            else
            {
                bool ret=screen->event_cb(screen,app,type,eventparam,eventparam_length,usr);
                ret = (app->status->event_cb(type,eventparam,eventparam_length,usr) && ret);
                return ret;
            }
        }
        else
        {
            if(app->status->event_cb!=NULL)
            {
                return app->status->event_cb(type,eventparam,eventparam_length,usr);
            }
        }
    }
    return false;
}

static bool  hgui_scene1_app_init_callback(const hgui_scene1_app_t *app,void *usr)
{
    (void)usr;
    if(hgui_scene1_app_was_init(app))
    {
        return true;
    }
    if(hgui_driver_reset(hgui_scene1_app_driver_get(app)))
    {
        {
            HGUI_SCENE1_APP_ON_INIT_SUCCESS;
        }
        {
            hgui_driver_event_callback_set(hgui_scene1_app_driver_get(app),hgui_scene1_app_event_input_helper);
        }
        hgui_scene1_app_need_refresh(app);
        if(app!=NULL)
        {
            if(app->status!=NULL)
            {
                app->status->be_init=1;
            }
        }
        else
        {
            g_hgui_scene1_app.status->be_init=1;
        }
        return true;
    }
    {
        HGUI_SCENE1_APP_ON_INIT_FAILURE;
    }
    return false;
}


#ifndef HGUI_SCENE1_APP_ON_UPDATE_BEGIN
#define HGUI_SCENE1_APP_ON_UPDATE_BEGIN {}
#endif // HGUI_SCENE1_APP_ON_UPDATE_BEGIN

#ifndef HGUI_SCENE1_APP_ON_UPDATE_END
#define HGUI_SCENE1_APP_ON_UPDATE_END {}
#endif // HGUI_SCENE1_APP_ON_UPDATE_END

#ifndef HGUI_SCENE1_APP_ON_UPDATE_SUCCESS
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS {}
#endif // HGUI_SCENE1_APP_ON_UPDATE_SUCCESS

#ifndef HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH {}
#endif // HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH

#ifndef HGUI_SCENE1_APP_ON_UPDATE_FAILURE
#define HGUI_SCENE1_APP_ON_UPDATE_FAILURE {}
#endif // HGUI_SCENE1_APP_ON_UPDATE_FAILURE

static bool  hgui_scene1_app_update_callback(const hgui_scene1_app_t *app,void *usr)
{
    if(app!=NULL)
    {
        if(!hgui_scene1_app_was_init(app))
        {
            hgui_scene1_app_init(app,usr);
        }
        if(!hgui_scene1_app_was_init(app))
        {
            return false;
        }
    }
    bool ret=false;
    {
        HGUI_SCENE1_APP_ON_UPDATE_BEGIN;
    }

    if(hgui_driver_update(hgui_scene1_app_driver_get(app)))
    {
        {
            HGUI_SCENE1_APP_ON_UPDATE_SUCCESS;
        }
        {
            hgui_scene1_screen_base_t *screen=app->status->screen_stack[app->status->screen_stack_pointer];
            if(screen!=NULL)
            {
                hgui_scene1_screen_base_update(screen,app);
            }
        }
        ret=true;
        {
            bool need_refresh=false;
            {
                if(app!=NULL)
                {
                    if(app->status!=NULL)
                    {
                        need_refresh=(app->status->need_refresh==1);
                        app->status->need_refresh=0;
                    }
                }
                else
                {
                    need_refresh=(g_hgui_scene1_app.status->need_refresh==1);
                    g_hgui_scene1_app.status->need_refresh=0;
                }
            }
            if(need_refresh)
            {
                {
                    HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH;
                }
            }
        }
    }
    else
    {
        HGUI_SCENE1_APP_ON_UPDATE_FAILURE;
        ret=false;
    }

    {
        HGUI_SCENE1_APP_ON_UPDATE_END;
    }

    return ret;
}

static hgui_scene1_app_status_t m_hgui_scene1_app_status= {0};
const hgui_scene1_app_t g_hgui_scene1_app=
{
    (HGUI_SCENE1_APP_DRIVER),
    (HGUI_SCENE1_APP_WIDTH),
    (HGUI_SCENE1_APP_HEIGHT),
    hgui_scene1_app_init_callback,
    hgui_scene1_app_update_callback,
    &m_hgui_scene1_app_status
};


hgui_driver_t * hgui_scene1_app_driver_get(const hgui_scene1_app_t *app)
{
    if(app!=NULL)
    {
        return app->driver;
    }
    return g_hgui_scene1_app.driver;
}

size_t hgui_scene1_app_width_get(const hgui_scene1_app_t *app)
{
    if(app!=NULL)
    {
        return app->w;
    }
    return g_hgui_scene1_app.w;
}


size_t hgui_scene1_app_height_get(const hgui_scene1_app_t *app)
{
    if(app!=NULL)
    {
        return app->h;
    }
    return g_hgui_scene1_app.h;
}

bool hgui_scene1_app_init(const hgui_scene1_app_t *app,void *usr)
{
    if(app!=NULL)
    {
        if(app->init!=NULL)
        {
            return app->init(app,usr);
        }
        //当没有初始化时默认为true.
        return true;
    }
    return g_hgui_scene1_app.init(app,usr);
}

bool hgui_scene1_app_was_init(const hgui_scene1_app_t *app)
{
    if(app!=NULL)
    {
        if(g_hgui_scene1_app.status!=NULL)
        {
            return app->status->be_init==1;
        }
        return false;
    }
    return g_hgui_scene1_app.status->be_init==1;
}

bool hgui_scene1_app_update(const hgui_scene1_app_t *app,void *usr)
{
    if(app!=NULL)
    {
        if(app->update!=NULL)
        {
            return app->update(app,usr);
        }
        //当没有更新时默认为true.
        return true;
    }
    return g_hgui_scene1_app.update(app,usr);
}

void hgui_scene1_app_need_refresh(const hgui_scene1_app_t *app)
{
    if(app==NULL)
    {
        app=&g_hgui_scene1_app;
    }

    if(app->status!=NULL)
    {
        app->status->need_refresh=1;
    }
}

bool hgui_scene1_app_screen_stack_push(const hgui_scene1_app_t *app,hgui_scene1_screen_base_t *screen)
{
    if(app==NULL)
    {
        app=&g_hgui_scene1_app;
    }

    if(screen==NULL)
    {
        return false;
    }

    if(app->status==NULL)
    {
        return false;
    }
    bool ret=false;

    if(app->status->screen_stack_pointer == ((sizeof(app->status->screen_stack)/sizeof(app->status->screen_stack[0]))-1))
    {
        return false;
    }
    hgui_scene1_screen_base_t *old_screen=app->status->screen_stack[app->status->screen_stack_pointer];
    if(old_screen==NULL)
    {
        app->status->screen_stack[app->status->screen_stack_pointer]=screen;
        hgui_scene1_screen_base_enter(screen,app);
        ret=true;
    }
    else
    {
        app->status->screen_stack_pointer++;
        app->status->screen_stack[app->status->screen_stack_pointer]=screen;
        hgui_scene1_screen_base_leave(old_screen,app);
        hgui_scene1_screen_base_enter(screen,app);
        ret=true;
    }

    return ret;
}

bool hgui_scene1_app_screen_stack_pop(const hgui_scene1_app_t *app)
{
    if(app==NULL)
    {
        app=&g_hgui_scene1_app;
    }

    if(app->status==NULL)
    {
        return false;
    }
    bool ret=false;
    hgui_scene1_screen_base_t *old_screen=app->status->screen_stack[app->status->screen_stack_pointer];
    if(app->status->screen_stack_pointer!=0)
    {
        hgui_scene1_screen_base_leave(old_screen,app);
        ret=true;
        app->status->screen_stack_pointer--;
        hgui_scene1_screen_base_t *new_screen=app->status->screen_stack[app->status->screen_stack_pointer];
        hgui_scene1_screen_base_enter(new_screen,app);
    }
    else
    {
        if(old_screen!=NULL)
        {
            app->status->screen_stack[app->status->screen_stack_pointer]=NULL;
            hgui_scene1_screen_base_leave(old_screen,app);
            ret=true;
        }
    }

    return ret;
}

hgui_scene1_screen_base_t * hgui_scene1_app_current_screen_get(const hgui_scene1_app_t *app)
{
    if(app==NULL)
    {
        app=&g_hgui_scene1_app;
    }

    if(app->status==NULL)
    {
        return NULL;
    }

    return app->status->screen_stack[app->status->screen_stack_pointer];
}


void hgui_scene1_app_event_callback_set(const hgui_scene1_app_t *app,hgui_gui_event_callback_t cb)
{
    if(app==NULL)
    {
        app=&g_hgui_scene1_app;
    }

    if(app->status==NULL)
    {
        return;
    }

    app->status->event_cb=cb;
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __HGUI_SCENE1_APP_C__
