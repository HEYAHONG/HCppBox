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


typedef struct
{
    struct
    {
        uint32_t be_init:1;
    };
} hgui_scene1_app_status_t;//APP状态，包含所有APP可修改的属性

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

static bool  hgui_scene1_app_init_callback(const hgui_scene1_app_t *app,void *usr)
{
    if(hgui_scene1_app_was_init(app))
    {
        return true;
    }
    if(hgui_driver_reset(hgui_scene1_app_driver_get(app)))
    {
        {
            HGUI_SCENE1_APP_ON_INIT_SUCCESS;
        }
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
        HGUI_SCENE1_APP_ON_UPDATE_SUCCESS;
        ret=true;
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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __HGUI_SCENE1_APP_C__
