/***************************************************************
 * Name:      native-lib.cpp
 * Purpose:   monochromescreen的JNI接口。
 * Author:    HYH (hyhsystem.cn)
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include <jni.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <string.h>

#include "hbox.h"
#include "android_hgui_driver.h"
#include "app_main.h"

namespace
{

std::mutex g_app_mutex;
std::atomic<bool> g_app_running(false);
std::thread g_app_thread;
bool g_app_inited=false;

void app_update_loop(void)
{
    while(g_app_running.load())
    {
        {
            std::lock_guard<std::mutex> lock(g_app_mutex);
            app_gui_update();
            /*
             * C++层的本次操作完成后才提交到显示缓冲区，避免闪烁。
             */
            android_hgui_driver_present();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void app_stop(void)
{
    /*
     * 注意：此处不能在持有g_app_mutex时join工作线程，
     * 否则工作线程可能在等待g_app_mutex时与join相互死锁。
     */
    g_app_running.store(false);
    if(g_app_thread.joinable())
    {
        g_app_thread.join();
    }
}

} // namespace

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_init(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    if(g_app_inited)
    {
        return;
    }
    android_hgui_driver_register();
    if(!app_gui_init())
    {
        return;
    }
    android_hgui_driver_present();
    g_app_inited=true;
    g_app_running.store(true);
    g_app_thread=std::thread(app_update_loop);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_deinit(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;
    app_stop();
    std::lock_guard<std::mutex> lock(g_app_mutex);
    if(g_app_inited)
    {
        app_gui_deinit();
        g_app_inited=false;
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_getWidth(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;
    return android_hgui_driver_width();
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_getHeight(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;
    return android_hgui_driver_height();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_getFrameId(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;
    return (jlong)android_hgui_driver_frame_id();
}

extern "C"
JNIEXPORT jint JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_copyFrame(JNIEnv *env, jclass clazz, jintArray pixels)
{
    (void)clazz;
    if(pixels==NULL)
    {
        return 0;
    }
    jsize length=env->GetArrayLength(pixels);
    if(length<=0)
    {
        return 0;
    }
    void *buffer=env->GetPrimitiveArrayCritical(pixels,NULL);
    if(buffer==NULL)
    {
        return 0;
    }
    size_t copied=android_hgui_driver_copy_frame((uint32_t *)buffer,(size_t)length);
    env->ReleasePrimitiveArrayCritical(pixels,buffer,0);
    return (jint)copied;
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_keyEvent(JNIEnv *env, jclass clazz, jint key_value, jboolean pressed)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    hgui_gui_event_key_t key;
    memset(&key,0,sizeof(key));
    key.key_value=(uint32_t)key_value & 0x7FFF;
    key.key_press_or_release=pressed?1:0;
    hgui_gui_event_key_emit(&key,hgui_driver_event_input_helper,hgui_driver_default_get());
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_touchEvent(JNIEnv *env, jclass clazz, jint action, jint id, jint x, jint y, jint dx, jint dy, jint pressure)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_touch_event((uint8_t)action,(uint32_t)id,(int32_t)x,(int32_t)y,(int32_t)dx,(int32_t)dy,(uint16_t)pressure);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_mouseMotion(JNIEnv *env, jclass clazz, jint x, jint y, jint xrel, jint yrel, jint button_state)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_mouse_motion_event((int32_t)x,(int32_t)y,(int32_t)xrel,(int32_t)yrel,(uint16_t)button_state);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_mouseButton(JNIEnv *env, jclass clazz, jint button, jboolean pressed, jint x, jint y)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_mouse_button_event((uint8_t)button,pressed?1:0,(int32_t)x,(int32_t)y);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_mouseWheel(JNIEnv *env, jclass clazz, jint x, jint y, jint delta)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_mouse_wheel_event((int32_t)x,(int32_t)y,(int32_t)delta);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_joystickAxis(JNIEnv *env, jclass clazz, jint which, jint axis, jint value)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_joystick_axis_event((uint8_t)which,(uint8_t)axis,(int16_t)value);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_joystickHat(JNIEnv *env, jclass clazz, jint which, jint hat, jint value)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_joystick_hat_event((uint8_t)which,(uint8_t)hat,(uint8_t)value);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_monochromescreen_Native_joystickButton(JNIEnv *env, jclass clazz, jint which, jint button, jboolean pressed)
{
    (void)env;
    (void)clazz;
    std::lock_guard<std::mutex> lock(g_app_mutex);
    android_hgui_driver_joystick_button_event((uint8_t)which,(uint8_t)button,pressed?1:0);
}
