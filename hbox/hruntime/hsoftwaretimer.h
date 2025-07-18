/***************************************************************
 * Name:      hsoftwaretimer.h
 * Purpose:   声明hsoftwaretimer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTWARETIMER_H_INCLUDE__
#define __HSOFTWARETIMER_H_INCLUDE__
#include "hmemory.h"
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hsoftwaretimer_lock;
typedef struct hsoftwaretimer_lock hsoftwaretimer_lock_t;
struct hsoftwaretimer_lock
{
    void *usr;
    void (*mutex_lock)(void *);             /**<  锁,参数为用户参数。*/
    void (*mutex_unlock)(void *);           /**<  解锁,参数为用户参数。*/
};

/** \brief 设定定时器锁
 *
 * \param lock const hsoftwaretimer_lock_t* 锁
 *
 */
void hsoftwaretimer_lock_set(const hsoftwaretimer_lock_t *lock);


/** \brief 软件定时器循环，用于内部使用
 *
 *
 */
void hsoftwaretimer_loop_hruntime(void);


/** \brief 定时器循环，在多任务环境下，用于在高优先级任务定时器中调用
 *
 *
 */
void hsoftwaretimer_mainloop(void);



struct hsoftwaretimer_timer;
typedef struct hsoftwaretimer_timer hsoftwaretimer_timer_t;
typedef void (*hsoftwaretimer_timer_callback_t)(hsoftwaretimer_timer_t *timer);
struct hsoftwaretimer_timer
{
    hdoublylinkedlist_head_t node;              /**< 链表节点 */
    hsoftwaretimer_timer_callback_t timeout_cb; /**< 超时回调 */
    void *usr;                                  /**< 用户参数 */
    int flags;                                  /**< 标志 */
    hdefaults_tick_t start;                     /**< 启动时间 */
    hdefaults_tick_t timeout;                   /**< 超时时间 */
    hdefaults_tick_t interval;                  /**< 间隔时间 */
};

enum
{
    HSOFTWARETIMER_FLAG_INIT            =   (1UL << (0)),   /**< 定时器是否初始化 */
    HSOFTWARETIMER_FLAG_START           =   (1UL << (1)),   /**< 定时器是否启动 */
    HSOFTWARETIMER_FLAG_PERIODIC        =   (1UL << (2)),   /**< 定时器是否是周期性的 */
};

/** \brief 检查定时器标志
 *
 * \param timer const hsoftwaretimer_timer_t* 定时器指针
 * \param flags int 标志
 * \return bool 是否检查成功
 *
 */
bool hsoftwaretimer_timer_check_flags(const hsoftwaretimer_timer_t * timer,int flags);

/** \brief 定时器是否在运行
 *
 * \param timer const hsoftwaretimer_timer_t* 定时器指针
 * \return bool 是否运行
 *
 */
bool hsoftwaretimer_timer_is_running(const hsoftwaretimer_timer_t *timer);

/** \brief 启动定时器
 *
 * \param timer hsoftwaretimer_timer_t* 定时器指针
 * \param interval hdefaults_tick_t 间隔
 *
 */
void hsoftwaretimer_timer_start(hsoftwaretimer_timer_t *timer,hdefaults_tick_t interval);


/** \brief 停止定时器
 *
 * \param timer hsoftwaretimer_timer_t* 定时器指针
 *
 */
void hsoftwaretimer_timer_stop(hsoftwaretimer_timer_t *timer);

/** \brief 定时器是否是周期性的
 *
 * \param timer const hsoftwaretimer_timer_t* 定时器指针
 * \return bool 是否是周期性的
 *
 */
bool hsoftwaretimer_timer_is_periodic(const hsoftwaretimer_timer_t *timer);

/** \brief 定时器设置周期性
 *
 * \param timer hsoftwaretimer_timer_t* 定时器指针
 * \param is_periodic bool 是否周期性
 *
 */
void hsoftwaretimer_timer_set_periodic(hsoftwaretimer_timer_t *timer,bool is_periodic);

/** \brief 定时器设置回调
 *
 * \param timer hsoftwaretimer_timer_t* 定时器指针
 * \param callback hsoftwaretimer_timer_callback_t 定时器超时回调
 * \param usr void* 用户参数
 *
 */
void hsoftwaretimer_timer_set_timeout_callback(hsoftwaretimer_timer_t *timer,hsoftwaretimer_timer_callback_t callback,void *usr);

/** \brief 获取默认定时器
 *
 * \return hsoftwaretimer_timer_t* 定时器指针
 *
 */
hsoftwaretimer_timer_t *hsoftwaretimer_timer_get_default_timer(void);

/** \brief 创建一个空定时器
 *
 * \return hsoftwaretimer_timer_t* 定时器指针，失败返回NULL
 *
 */
hsoftwaretimer_timer_t *hsoftwaretimer_timer_new(void);

/** \brief 删除定时器
 *
 * \param timer hsoftwaretimer_timer_t* 定时器指针
 *
 */
void hsoftwaretimer_timer_delete(hsoftwaretimer_timer_t * timer);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTWARETIMER_H_INCLUDE__
