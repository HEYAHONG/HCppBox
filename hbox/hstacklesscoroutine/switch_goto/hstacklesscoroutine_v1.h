/***************************************************************
 * Name:      hstacklesscoroutine_v1.h
 * Purpose:   声明hstacklesscoroutine_v1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSTACKLESSCOROUTINE_V1_H_INCLUDED__
#define __HSTACKLESSCOROUTINE_V1_H_INCLUDED__
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

typedef struct
{
    bool (*wait_for_ready)(void *usr,hstacklesscoroutine_event_t *event);/**< 等待准备好,返回true退出等待*/
    void *usr;/**< 用户参数 */
} hstacklesscoroutine_awaiter_t;

typedef enum
{
    HSTACKLESSCOROUTE_HOOK_TYPE_EXEC_PROCESS_ENTER,                         /**< 协程执行过程进入*/
    HSTACKLESSCOROUTE_HOOK_TYPE_EXEC_PROCESS_EXIT,                          /**< 协程执行过程退出*/
    HSTACKLESSCOROUTE_HOOK_TYPE_AWAITER_PROCESS_START,                      /**< 协程Awaiter过程开始*/
    HSTACKLESSCOROUTE_HOOK_TYPE_AWAITER_PROCESS_END,                        /**< 协程Awaiter过程结束*/
    HSTACKLESSCOROUTE_HOOK_TYPE_CODEBLOCK_PROCESS_START,                    /**< 协程代码块过程开始*/
    HSTACKLESSCOROUTE_HOOK_TYPE_CODEBLOCK_PROCESS_END,                      /**< 协程代码块过程结束*/
} hstacklesscoroutine_hook_type_t;

struct hstacklesscoroutine_control_block;
typedef struct hstacklesscoroutine_control_block hstacklesscoroutine_control_block_t;
struct hstacklesscoroutine_control_block
{

    /*
     *  0           协程起始
     *
     *
     */
    int corevalue;/**< 核心值，此值用于协程调度，用户不可直接更改*/
    /*
     *  位0          运行(取0)/暂停位（取1）
     *  位1          协程是否完成，1=完成
     *
     */
    int flags;/**< 运行标志,按位区分功能 */


    int nested;//当前嵌套层数

    int max_nested;//最大嵌套层数

    hstacklesscoroutine_awaiter_t awaiter;/**< 当协程暂停时的等待参数 */

    void (*hook)(hstacklesscoroutine_control_block_t *ccb, hstacklesscoroutine_hook_type_t type, hstacklesscoroutine_event_t *event); /**< 钩子函数 */
};

#define HSTACKLESSCOROUTINE_CONTROL_BLOCK_INIT_VALUE {0,0,0,1,{NULL,NULL},NULL}

#define __HSTACKLESSCOROUTINE_BLOCK_START(NAME) \
hstacklesscoroutine_control_block_t g_hstacklesscoroutine_##NAME##_ccb = HSTACKLESSCOROUTINE_CONTROL_BLOCK_INIT_VALUE;\
static void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event_impl(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event);\
void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event)\
{\
    if(ccb==NULL)\
    {\
        return;\
    }\
    ccb->nested++;\
    if(ccb->nested<=ccb->max_nested)\
    {\
        if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_EXEC_PROCESS_ENTER,event);\
        hstacklesscoroutine_##NAME##_entry_with_ccb_and_event_impl(ccb,event);\
        if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_EXEC_PROCESS_EXIT,event);\
    }\
    ccb->nested--;\
}\
void hstacklesscoroutine_##NAME##_entry_with_event(hstacklesscoroutine_event_t *event)\
{\
    hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(&g_hstacklesscoroutine_##NAME##_ccb,event);\
}\
void hstacklesscoroutine_##NAME##_entry(void)\
{\
    hstacklesscoroutine_##NAME##_entry_with_event(NULL);\
}\
static void hstacklesscoroutine_##NAME##_entry_with_ccb_and_event_impl(hstacklesscoroutine_control_block_t *ccb,hstacklesscoroutine_event_t *event)\
{\
    if(ccb==NULL || (ccb->flags&(0x1ULL<<1))!=0)\
    {\
        return;\
    }\
    if((hstacklesscoroutine_is_suspend(ccb))!=0)\
    {\
        if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_AWAITER_PROCESS_START,event);\
        if(ccb->awaiter.wait_for_ready!=NULL)\
        {\
            if(ccb->awaiter.wait_for_ready(ccb->awaiter.usr,event))\
            {\
                ccb->awaiter.wait_for_ready=NULL;\
                hstacklesscoroutine_coroutine_resume(ccb);\
            }\
        }\
        if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_AWAITER_PROCESS_END,event);\
        return;\
    }\
    if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_CODEBLOCK_PROCESS_START,event);\
    switch(ccb->corevalue)\
    {\
        case 0:\


#define __HSTACKLESSCOROUTINE_BLOCK_END(NAME) \
        ccb->flags|=(0x1ULL<<1);\
        goto hstacklesscoroutine_break;\
        default:\
        {\
            ccb->flags|=(0x1ULL<<1);\
        }\
    }\
    hstacklesscoroutine_break:;\
    if(ccb->hook!=NULL) ccb->hook(ccb,HSTACKLESSCOROUTE_HOOK_TYPE_CODEBLOCK_PROCESS_END,event);\
}\


/** \brief 无栈协程块起始，需要放在全局（不可放在函数体或命名空间内）
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#define HSTACKLESSCOROUTINE_BLOCK_START(NAME)   HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME)\
                                                __HSTACKLESSCOROUTINE_BLOCK_START(NAME)



/** \brief 无栈协程块结束，需要放在全局（不可放在函数体或命名空间内）
 *
 * \param NAME 协程变量名,需要符合C语言中关于变量名命名的条件。
 *
 */
#define HSTACKLESSCOROUTINE_BLOCK_END(NAME) __HSTACKLESSCOROUTINE_BLOCK_END(NAME)


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
                                        hstacklesscoroutine_return();\
                                        case N:\
/** \brief 协程让出控制权,，需要在协程块内使用
 *
 *
 */
#define hstacklesscoroutine_yield() __HSTACKLESSCOROUTINE_YIELD(__LINE__)

/** \brief 协程让出控制权(带标签),，需要在协程块内使用
 *
 *\param N 标签，大于0的正整数
 *
 */
#define hstacklesscoroutine_yield_with_label(N) __HSTACKLESSCOROUTINE_YIELD((-(N)))

/** \brief 协程返回,，需要在协程块内使用
 *
 *
 */
#define hstacklesscoroutine_return() goto hstacklesscoroutine_break

/** \brief 进入指定的标签,，需要在协程块内使用
 *
 *\param N 标签，大于0的正整数
 *
 */
#define hstacklesscoroutine_goto_label(N)  {\
                                                HSTACKLESSCOROUTINE_GET_CURRENT_CCB()->corevalue=(-(N));\
                                           }\
                                           hstacklesscoroutine_return();


#define __HSTACKLESSCOROUTINE_AWAIT(N,AWAITER)  {\
                                                    HSTACKLESSCOROUTINE_GET_CURRENT_CCB()->awaiter=AWAITER;\
                                                    HSTACKLESSCOROUTINE_GET_CURRENT_CCB()->corevalue=N;\
                                                    hstacklesscoroutine_coroutine_suspend(HSTACKLESSCOROUTINE_GET_CURRENT_CCB());\
                                                }\
                                                hstacklesscoroutine_return();\
                                                case N:


#define hstacklesscoroutine_await(AWAITER)  __HSTACKLESSCOROUTINE_AWAIT(__LINE__,(AWAITER))

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

/** \brief 协程是否被暂停
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 * \return bool 是否被暂停
 *
 */
bool hstacklesscoroutine_is_suspend(hstacklesscoroutine_control_block_t *ccb);

/** \brief 暂停协程
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 *
 */
void hstacklesscoroutine_coroutine_suspend(hstacklesscoroutine_control_block_t *ccb);


/** \brief 恢复协程
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 *
 */
void hstacklesscoroutine_coroutine_resume(hstacklesscoroutine_control_block_t *ccb);

/** \brief 协程是否在等待
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 * \return bool 是否在等待
 *
 */
bool hstacklesscoroutine_is_await(hstacklesscoroutine_control_block_t *ccb);

/** \brief 获取协程嵌套层数
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 * \return int 嵌套层数，0为无效
 *
 */
int hstacklesscoroutine_coroutine_get_current_nested(hstacklesscoroutine_control_block_t *ccb);

/** \brief 获取最大嵌套层数
 *
 * \param ccb hstacklesscoroutine_control_block_t* 协程控制块
 * \param max_nested int 最大嵌套层数，0为无效，默认为1（即不允许嵌套）
 *
 */
void hstacklesscoroutine_coroutine_set_max_nested(hstacklesscoroutine_control_block_t *ccb,int max_nested);

/*
 * 协程入口
 */
#define HSTACKLESSCOROUTINE_ENTRY(NAME) \
{\
    hstacklesscoroutine_##NAME##_entry();\
}

#define HSTACKLESSCOROUTINE_ENTRY_WITH_EVENT(NAME,EVENT)  \
{\
    hstacklesscoroutine_##NAME##_entry_with_event(EVENT);\
}


#define HSTACKLESSCOROUTINE_ENTRY_WITH_CCB_AND_EVENT(NAME,CCB,EVENT)    \
{\
    hstacklesscoroutine_##NAME##_entry_with_ccb_and_event(CCB,EVENT);\
}


/*
 * 协程组表示一组协程，它们通常可输入相同的事件。
 */
typedef void (*hstacklesscoroutine_entry_t)(hstacklesscoroutine_event_t *event);
#define __HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME) \
extern hstacklesscoroutine_entry_t g_hstacklesscoroutine_##NAME##__entry_group[];

/** \brief 声明协程组
 *
 * \param NAME 协程组名称
 *
 */
#ifdef __cplusplus
#define HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME) \
extern "C"\
{\
    __HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME)\
}
#else
#define HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME) __HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME)
#endif

/** \brief 协程组块起始,需要放到全局
 *
 * \param NAME 协程组名称
 *
 */
#define HSTACKLESSCOROUTINE_GROUP_BLOCK_START(NAME) \
HSTACKLESSCOROUTINE_DECLARE_COROUTINE_GROUP(NAME)\
hstacklesscoroutine_entry_t g_hstacklesscoroutine_##NAME##__entry_group[]=\
{

/** \brief 协程组块结束，需要放到全局
 *
 * \param NAME 协程组名称
 *
 */
#define HSTACKLESSCOROUTINE_GROUP_BLOCK_END(NAME) \
NULL\
};

/** \brief 协程，需要放在协程组起始与协程组结束之间，注意：放入前需要声明协程。
 *
 * \param COROUTINENAME 协程名称
 *
 */
#define HSTACKLESSCOROUTINE_GROUP_BLOCK_ITEM(COROUTINENAME) \
hstacklesscoroutine_##COROUTINENAME##_entry_with_event,

/** \brief 枚举协程组
 *
 * \param NAME 协程组名称
 * \param ENUM_CB 枚举回调函数
 *
 */
typedef void (*hstacklesscoroutine_group_enum_entry_t)(hstacklesscoroutine_entry_t entry);
#define HSTACKLESSCOROUTINE_GROUP_FOREACH(NAME,ENUM_CB) \
{\
    hstacklesscoroutine_group_enum_entry_t hstacklesscoroutine_group_enum_callback=(ENUM_CB);\
    if(hstacklesscoroutine_group_enum_callback!=NULL)\
    {\
        hstacklesscoroutine_entry_t *current=g_hstacklesscoroutine_##NAME##__entry_group;\
        while((*current)!=NULL)\
        {\
            hstacklesscoroutine_group_enum_callback(*current);\
            current++;\
        }\
    }\
}

/** \brief 协程组入口
 *
 * \param NAME 协程组名称
 * \param EVENT 事件
 *
 */
#define HSTACKLESSCOROUTINE_GROUP_ENTRY(NAME,EVENT) \
{\
    hstacklesscoroutine_entry_t *current=g_hstacklesscoroutine_##NAME##__entry_group;\
    while((*current)!=NULL)\
    {\
            (*current)(EVENT);\
            current++;\
    }\
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSTACKLESSCOROUTINE_V1_H_INCLUDED__
