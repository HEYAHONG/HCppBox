/***************************************************************
 * Name:      hstacklesscoroutine_v2.h
 * Purpose:   声明hstacklesscoroutine_v2接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-04-04
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTACKLESSCOROUTINE_V2_H_INCLUDED__
#define __HSTACKLESSCOROUTINE_V2_H_INCLUDED__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "limits.h"
#include "hdefaults.h"
#include "hcompiler.h"

/*
本文件用于辅助编写简易无栈协程。
*/
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


struct hstacklesscoroutine2_ccb;            /**< 协程控制块 */
typedef struct hstacklesscoroutine2_ccb hstacklesscoroutine2_ccb_t;
struct hstacklesscoroutine2_scheduler;      /**< 调度器 */
typedef struct hstacklesscoroutine2_scheduler hstacklesscoroutine2_scheduler_t;

/** \brief 获取协程控制块大小
 *
 * \return size_t 协程控制块大小
 *
 */
size_t hstacklesscoroutine2_ccb_size(void);


/** \brief 初始化协程控制块
 *
 * \param mem void* 协程控制块内存地址（对于裸机而言，协程控制块，后接备份栈空间）
 * \param length size_t 内存长度（对于裸机而言，由协程控制块长度+备份栈长度）
 * \return hstacklesscoroutine2_ccb_t* 协程控制块指针
 *
 */
hstacklesscoroutine2_ccb_t *hstacklesscoroutine2_ccb_init(void *mem,size_t length);

/** \brief 协程任务入口
 *
 * \param scheduler hstacklesscoroutine2_scheduler_t* 协程调度器
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块
 * \param usr void* 用户参数
 *
 */
typedef void (*hstacklesscoroutine2_task_entry_t)(hstacklesscoroutine2_scheduler_t *scheduler,hstacklesscoroutine2_ccb_t *ccb,void *usr);

typedef struct hstacklesscoroutine2_task
{
    hstacklesscoroutine2_task_entry_t entry;        /**< 任务入口 */
    void *usr;                                      /**< 用户参数 */
} hstacklesscoroutine2_task_t;                      /**< 协程任务 */

/** \brief 协程任务初始化
 *
 * \param entry hstacklesscoroutine2_task_entry_t 任务入口
 * \param usr void* 用户参数
 * \return hstacklesscoroutine2_task_t 协程任务
 *
 */
hstacklesscoroutine2_task_t hstacklesscoroutine2_task_init(hstacklesscoroutine2_task_entry_t entry,void *usr);

/** \brief 设置协程任务
 *
 * \param ccb hstacklesscoroutine2_ccb_t*  协程控制块指针
 * \param task hstacklesscoroutine2_task_t 协程任务
 * \return bool 是否设置成功，仅当任务有效且协程处于停止状态或者创建状态时才可设置成功。
 *
 */
bool hstacklesscoroutine2_ccb_set(hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_task_t task);

typedef enum
{
    HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE=0,            /**< 创建状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_READY=1,             /**< 就绪状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING=2,           /**< 运行状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCK=3,             /**< 阻塞状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_STOPPED=4,           /**< 停止状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_RESERVED_0=5,        /**< 保留状态0,休眠状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_RESERVED_1=6,        /**< 保留状态1,唤醒状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_UNKOWN=7,            /**< 未知状态,通常用于返回错误 */
} hstacklesscoroutine2_running_state_t;                     /**< 运行状态  */

/** \brief 获取协程控制块状态
 *
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \return hstacklesscoroutine2_running_state_t 协程运行状态
 *
 */
hstacklesscoroutine2_running_state_t hstacklesscoroutine2_ccb_running_state_get(hstacklesscoroutine2_ccb_t *ccb);

struct hstacklesscoroutine2_scheduler
{
    hstacklesscoroutine2_ccb_t * ccb_list_head;    /**< 协程控制块列表 */
};

/** \brief 启动协程调度器
 *
 * \param scheduler hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针,为NULL时使用默认协程调度器
 * \return int 当所有协程完成后,返回0，出现错误返回-1。
 *
 */
int hstacklesscoroutine2_scheduler_start(hstacklesscoroutine2_scheduler_t * scheduler);


/** \brief 协程调度器注册协程控制块
 *
 * \param scheduler hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针,为NULL时使用默认协程调度器
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \return int 正常返回0，失败返回-1
 *
 */
int hstacklesscoroutine2_scheduler_ccb_register(hstacklesscoroutine2_scheduler_t * scheduler,hstacklesscoroutine2_ccb_t *ccb);

/** \brief 协程让出时间片，注意：此函数只能在任务中调用
 *
 * \param sch hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 *
 */
void hstacklesscoroutine2_yield(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb);

/** \brief 延时至某个节拍，注意：此函数只能在任务中调用
 *
 * \param sch hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \param tick hdefaults_tick_t 节拍数
 *
 */
void hstacklesscoroutine2_delay_util(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hdefaults_tick_t  tick);

/** \brief 延时，注意：此函数只能在任务中调用
 *
 * \param sch hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \param tick hdefaults_tick_t 节拍数
 *
 */
void hstacklesscoroutine2_delay(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hdefaults_tick_t  tick);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSTACKLESSCOROUTINE_V2_H_INCLUDED__
