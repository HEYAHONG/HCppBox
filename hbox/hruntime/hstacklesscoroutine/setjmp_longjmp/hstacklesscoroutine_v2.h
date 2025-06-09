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

/*
 * longjmp可用于多层返回。
 * 使用longjmp跳转至被调用的函数是未定义行为，一般只有裸机可支持（裸机一般是通过直接恢复寄存器(如PC与SP)的方式实现 longjmp）
 * 注意:本组件使用了setjmp与longjmp ,可能与C++某些特性兼容性不好，使用局部变量与C++异常时尤其需要注意。
 */


#if defined(HDEFAULTS_OS_NONE)
#if defined(HDEFAULTS_LIBC_ARMCLIB)
#define HSTACKLESSCOROUTINE2_BARE_MACHINE    1
#endif
#ifdef HDEFAULTS_LIBC_NEWLIB
#define HSTACKLESSCOROUTINE2_BARE_MACHINE    1
#endif
#ifdef HDEFAULTS_LIBC_PICOLIBC
#define HSTACKLESSCOROUTINE2_BARE_MACHINE    1
#endif
#endif

#ifdef HDEFAULTS_OS_UNIX
#ifdef HDEFAULTS_LIBC_NEWLIB
#define HSTACKLESSCOROUTINE2_BARE_MACHINE    1
#endif
#endif

/*
 * 某些情况下，需要启用非裸机的写法，可使用HSTACKLESSCOROUTINE2_NO_BARE_MACHINE强行使用非裸机的写法
 */
#ifdef  HSTACKLESSCOROUTINE2_NO_BARE_MACHINE
#ifdef  HSTACKLESSCOROUTINE2_BARE_MACHINE
#undef  HSTACKLESSCOROUTINE2_BARE_MACHINE
#endif
#endif


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
#ifndef HSTACKLESSCOROUTINE2_BARE_MACHINE
    int core_value;                                 /**< 核心值，用于任务入口函数内部调度 */
#endif // HSTACKLESSCOROUTINE2_BARE_MACHINE
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


/** \brief 获取协程任务
 *
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \return hstacklesscoroutine2_task_t 协程任务
 *
 */
hstacklesscoroutine2_task_t hstacklesscoroutine2_ccb_get(hstacklesscoroutine2_ccb_t *ccb);


/** \brief 获取协程是否被暂停
 *
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \return bool 是否被暂停
 *
 */
bool hstacklesscoroutine2_ccb_is_suspend(hstacklesscoroutine2_ccb_t *ccb);

/** \brief 协程暂停
 *
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 *
 */
void hstacklesscoroutine2_ccb_suspend(hstacklesscoroutine2_ccb_t *ccb);

/** \brief 协程从暂停中恢复
 *
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 *
 */
void hstacklesscoroutine2_ccb_resume(hstacklesscoroutine2_ccb_t *ccb);

typedef enum
{
    HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE=0,            /**< 创建状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_READY=1,             /**< 就绪状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING=2,           /**< 运行状态 */
    HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCKED=3,           /**< 阻塞状态 */
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

struct  hstacklesscoroutine2_awaiter;
typedef struct  hstacklesscoroutine2_awaiter hstacklesscoroutine2_awaiter_t;
/** \brief 协程挂起点资源检查函数
 *
 * \param sch hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \param awaiter hstacklesscoroutine2_awaiter_t* 协程挂起点指针
 * \return bool true表示资源已就绪
 *
 */
typedef bool (*hstacklesscoroutine2_awaiter_wait_for_ready_t)(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_awaiter_t *awaiter);
struct hstacklesscoroutine2_awaiter
{
    hstacklesscoroutine2_awaiter_wait_for_ready_t  wait_for_ready;       /**< 等待准备好,返回true退出等待*/
    void *usr;                                                          /**< 用户参数 */
};

/** \brief 协程挂起点初始化
 *
 * \param wait_for_ready hstacklesscoroutine2_awaiter_wait_for_ready_t 资源检查函数
 * \param usr void* 用户参数
 * \return hstacklesscoroutine2_awaiter_t
 *
 */
hstacklesscoroutine2_awaiter_t hstacklesscoroutine_awaiter2_init(hstacklesscoroutine2_awaiter_wait_for_ready_t wait_for_ready,void *usr);


/** \brief 等待挂起点,注意：此函数只能在任务中调用
 *
 * \param sch hstacklesscoroutine2_scheduler_t* 协程调度器调度器指针
 * \param ccb hstacklesscoroutine2_ccb_t* 协程控制块指针
 * \param awaiter hstacklesscoroutine2_awaiter_t 协程挂起点
 *
 */
void hstacklesscoroutine2_await(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_awaiter_t awaiter);

#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
/*
 * 裸机模式下无需定义协程块，留空以兼容代码
 */
#define  HSTACKLESSCOROUTINE2_BLOCK_START(CCB_NAME)
#define  HSTACKLESSCOROUTINE2_BLOCK_POINT(CCB_NAME)
#define  HSTACKLESSCOROUTINE2_BLOCK_LABEL(CCB_NAME,N)
#define  HSTACKLESSCOROUTINE2_BLOCK_GOTO(SCH,CCB_NAME,N)
#define  HSTACKLESSCOROUTINE2_BLOCK_END(CCB_NAME)

/*
 * 兼容代码,无作用
 */
int __hstacklesscoroutine2_core_value_get(hstacklesscoroutine2_ccb_t *ccb);
void __hstacklesscoroutine2_core_value_set(hstacklesscoroutine2_ccb_t *ccb,int core_value);

#else
/*
 * 非裸机模式下，协程下一次进入时在协程块中将保存的点执行
 */


int __hstacklesscoroutine2_core_value_get(hstacklesscoroutine2_ccb_t *ccb);
void __hstacklesscoroutine2_core_value_set(hstacklesscoroutine2_ccb_t *ccb,int core_value);

/** \brief 协程块起始，只能在协程任务入口函数中使用。参数为协程控制块指针。
 *
 *
 */
#define HSTACKLESSCOROUTINE2_BLOCK_START(CCB_NAME) \
    if((CCB_NAME!=NULL))\
    {\
        if(__hstacklesscoroutine2_core_value_get(CCB_NAME)==0)\
        {\
            __hstacklesscoroutine2_core_value_set((CCB_NAME), __LINE__ );\
        }\
        switch(__hstacklesscoroutine2_core_value_get(CCB_NAME))\
        {\
        case __LINE__:

/** \brief 协程块点（下一次进入协程将从此进入），只能在协程任务入口函数中使用。参数为协程控制块指针。只能在协程块中使用。
 *
 *
 */

#define HSTACKLESSCOROUTINE2_BLOCK_POINT(CCB_NAME) \
        if((CCB_NAME)!=NULL)   \
        {\
            __hstacklesscoroutine2_core_value_set((CCB_NAME), __LINE__ );\
        }\
        case __LINE__ :

/** \brief 协程标签（配合GOTO使用），只能在协程任务入口函数中使用。参数CCB_NAME为协程控制块指针,参数N为大于0的整数。只能在协程块中使用。
 *
 *
 */
#define  HSTACKLESSCOROUTINE2_BLOCK_LABEL(CCB_NAME,N) \
        case -(N) :


/** \brief 协程GOTO（配合标签使用），只能在协程任务入口函数中使用。参数SCH为调度器指针，参数CCB_NAME为协程控制块指针,参数N为大于0的整数。只能在协程块中使用。
 *
 *
 */
#define  HSTACKLESSCOROUTINE2_BLOCK_GOTO(SCH,CCB_NAME,N) \
        if((CCB_NAME)!=NULL)   \
        {\
            __hstacklesscoroutine2_core_value_set((CCB_NAME), -(N) );\
        }\
        hstacklesscoroutine2_yield(SCH,CCB_NAME);


/** \brief 协程块结束,与协程块起始配对，只能在协程任务入口函数中使用。参数为协程控制块指针。
 *
 *
 */
#define HSTACKLESSCOROUTINE2_BLOCK_END(CCB_NAME)\
        default:\
        {\
            if(__hstacklesscoroutine2_core_value_get(CCB_NAME) < __LINE__ )\
            {\
                __hstacklesscoroutine2_core_value_set((CCB_NAME), 0 );\
            }\
        }\
        break;\
        }\
    }

#endif // HSTACKLESSCOROUTINE2_BARE_MACHINE


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSTACKLESSCOROUTINE_V2_H_INCLUDED__
