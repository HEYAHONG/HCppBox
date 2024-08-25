/***************************************************************
 * Name:      hstacklesscoroutine.h
 * Purpose:   声明hstacklesscoroutine接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTACKLESSCOROUTINE_H_INCLUDED__
#define __HSTACKLESSCOROUTINE_H_INCLUDED__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "limits.h"
/*
本文件用于辅助编写简易无栈协程。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef struct
{
    int event; /**< 事件 */
    void *eventparam; /**< 事件参数 */
} hstacklesscoroutine_event_t;

struct hstacklesscoroutine_control_block;
typedef struct hstacklesscoroutine_control_block hstacklesscoroutine_control_block_t;
struct hstacklesscoroutine_control_block
{
    int corevalue;/**< 核心值，此值用于协程调度，用户不可直接更改*/
};

#define HSTACKLESSCOROUTINE_CONTROL_BLOCK_INIT_VALUE {0}

#define __HSTACKLESSCOROUTINE_BLOCK_START(NAME) \
hstacklesscoroutine_control_block_t g_hstacklesscoroutine_##NAME##_ccb = HSTACKLESSCOROUTINE_CONTROL_BLOCK_INIT_VALUE;\
void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event);\
void hstacklesscoroutine_##NAME##_entry_with_event(hstacklesscoroutine_event_t *event)\
{\
    hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(&g_hstacklesscoroutine_##NAME##_ccb,event);\
}\
void hstacklesscoroutine_##NAME##_entry(void)\
{\
    hstacklesscoroutine_##NAME##_entry_with_event(NULL);\
}\
void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event)\
{\
    if(ccb==NULL || ccb->corevalue == -1)\
    {\
        return;\
    }\
    switch(ccb->corevalue)\
    {\
        case 0:\


#define __HSTACKLESSCOROUTINE_BLOCK_END(NAME) \
        ccb->corevalue=-1;\
        default:\
        {\
            ccb->corevalue=-1;\
        }\
    }\
}\


/** \brief 无栈协程块起始，需要放在全局（不可放在函数体或命名空间内）
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#ifdef __cplusplus
#define HSTACKLESSCOROUTINE_BLOCK_START(NAME)   extern "C" {\
                                                __HSTACKLESSCOROUTINE_BLOCK_START(NAME)
#else
#define HSTACKLESSCOROUTINE_BLOCK_START(NAME)   __HSTACKLESSCOROUTINE_BLOCK_START(NAME)
#endif


/** \brief 无栈协程块结束，需要放在全局（不可放在函数体或命名空间内）
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#ifdef __cplusplus
#define HSTACKLESSCOROUTINE_BLOCK_END(NAME) __HSTACKLESSCOROUTINE_BLOCK_END(NAME)\
                                            }
#else
#define HSTACKLESSCOROUTINE_BLOCK_END(NAME) __HSTACKLESSCOROUTINE_BLOCK_END(NAME)
#endif

#define __HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME) \
extern hstacklesscoroutine_control_block_t g_hstacklesscoroutine_##NAME##_ccb;\
extern void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event);\
extern void hstacklesscoroutine_##NAME##_entry_with_event(hstacklesscoroutine_event_t *event);\
extern void hstacklesscoroutine_##NAME##_entry(void);

/** \brief 无栈协程块声明，若协程定义与调用不在同一个文件，则需要将此声明放在调用方文件的全局。
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#ifdef __cplusplus
#define HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME) extern "C"\
                                                    {\
                                                        __HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME)\
                                                    }
#else
#define HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME) __HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME)
#endif

/** \brief 获取全局控制块。
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#define HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(NAME)    &g_hstacklesscoroutine_##NAME##_ccb\


/** \brief 获取当前协程控制块指针，需要在协程块内使用
 *
 *
 */
#define HSTACKLESSCOROUTINE_GET_CURRENT_CCB() ccb

/** \brief 获取当前协程事件指针，需要在协程块内使用
 *
 *
 */
#define HSTACKLESSCOROUTINE_GET_CURRENT_EVENT() event

#define __HSTACKLESSCOROUTINE_YIELD(N)  \
                                        {\
                                            HSTACKLESSCOROUTINE_GET_CURRENT_CCB()->corevalue=N;\
                                        }\
                                        break;\
                                        case N:\
/** \brief 协程让出控制权,，需要在协程块内使用
 *
 *
 */
#define hstacklesscoroutine_yield() __HSTACKLESSCOROUTINE_YIELD(__LINE__)

/** \brief 协程让出控制权(带标签),，需要在协程块内使用
 *
 *\param N 标签，大于0小于(INT_MAX-1000)的正整数
 *
 */
#define hstacklesscoroutine_yield_with_label(N) __HSTACKLESSCOROUTINE_YIELD((-((N)+1000)))

/** \brief 协程返回,，需要在协程块内使用
 *
 *
 */
#define hstacklesscoroutine_return() break

/** \brief 进入指定的标签,，需要在协程块内使用
 *
 *\param N 标签，大于0小于(INT_MAX-1000)的正整数
 *
 */
#define hstacklesscoroutine_goto_label(N)  {\
                                                HSTACKLESSCOROUTINE_GET_CURRENT_CCB()->corevalue=(-((N)+1000));\
                                           }\
                                           hstacklesscoroutine_return();


/** \brief 协程是否完成
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 * \return bool 是否完成协程的运行，参数为NULL时也返回true
 *
 */
bool hstacklesscoroutine_is_finished(hstacklesscoroutine_control_block_t *ccb);


/** \brief 当协程完成后，重新启动协程
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 *
 */
void hstacklesscoroutine_coroutine_restart(hstacklesscoroutine_control_block_t *ccb);

/** \brief 强制重新启动协程(不管协程运行状态)
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 *
 */
void hstacklesscoroutine_coroutine_force_restart(hstacklesscoroutine_control_block_t *ccb);

/*
 * 协程入口
 */
#define HSTACKLESSCOROUTINE_ENTRY(NAME) {\
                                            hstacklesscoroutine_##NAME##_entry();\
                                        }\

#define HSTACKLESSCOROUTINE_ENTRY_WITH_EVENT(NAME,EVENT)    {\
                                                                hstacklesscoroutine_##NAME##_entry_with_event(EVENT);\
                                                            }\


#define HSTACKLESSCOROUTINE_ENTRY_WITH_CCB_AND_EVENT(NAME,CCB,EVENT)    {\
                                                                            hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(CCB,EVENT);\
                                                                        }\



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSTACKLESSCOROUTINE_H_INCLUDED__
