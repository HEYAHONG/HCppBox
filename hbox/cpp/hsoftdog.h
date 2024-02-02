/***************************************************************
 * Name:      hsoftdog.h
 * Purpose:   hsoftdog
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-02-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HSOFTDOG_H
#define HSOFTDOG_H

#ifdef __cplusplus

#include "hcpprt.h"
#include "hwatchdog.h"

/*
软狗,通过继承此类方便喂狗,在子类中调用watchdog_feed喂狗
*/
class hsoftdog
{
    hwatchdog_tick_t m_timeout_ms;
    hwatchdog_softdog_t *softdog;
public:
    //默认30S
    hsoftdog(hwatchdog_tick_t timeout_ms=30000):m_timeout_ms(timeout_ms),softdog(NULL)
    {

    }

    hsoftdog(hsoftdog &oths):m_timeout_ms(oths.m_timeout_ms),softdog(NULL)
    {

    }
    hsoftdog(hsoftdog &&oths):m_timeout_ms(oths.m_timeout_ms),softdog(NULL)
    {

    }

    //不允许赋值操作
    hsoftdog &operator=(hsoftdog & oths) =delete;
    hsoftdog &operator=(hsoftdog && oths) =delete;

    virtual ~hsoftdog()
    {

    }
protected:
    //看门狗喂狗，默认只允许子类调用
    void watchdog_feed()
    {
        if(softdog!=NULL)
        {
            hwatchdog_softdog_feed(softdog);
        }
        else
        {
            softdog=hwatchdog_softdog_new(m_timeout_ms);
            if(softdog != NULL)
            {
                hwatchdog_softdog_feed(softdog);
            }
        }
    }
};

#endif // __cplusplus

#endif // HSOFTDOG_H
