#include "criticalsection.h"
#include "stdbool.h"
#if !defined(LIBMONO_RUNTIME_MAINLOOP_H)
#define LIBMONO_RUNTIME_MAINLOOP_H

struct libmono_runtime_mainloop_task_context
{
    /** \brief 任务入口，注意此函数会多次调用
     *
     *
     */
    void (* entry)(struct libmono_runtime_mainloop_task_context *ctx);
    /** \brief  事件组，当事件组为0时事件任务不会运行,每一位表示一个事件。用户应当使用相应函数修改此成员，不要直接进行修改
     *              低12位为用户自定义事件
     *              高4位为系统事件
     *
     */

    uint16_t event_group;
};
typedef struct libmono_runtime_mainloop_task_context libmono_runtime_mainloop_task_context_t;


enum
{
    LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_INIT        =   (1UL << 15),    /**< 初始化，此标志由主循环自动设置，由用户清除*/
    LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_SERVICE_RUN =   (1UL << 14),    /**< 服务运行，此标志用于保证入口函数一直被调用，此标志需要用户手动设置或清除 */
};


/** \brief 设置事件
 *
 * \param ctx struct libmono_runtime_mainloop_task_context* 任务上下文
 * \param events uint16_t 事件或者事件的组合（按位或）
 *
 */
void libmono_runtime_mainloop_task_event_set(struct libmono_runtime_mainloop_task_context *ctx,uint16_t events) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;


/** \brief 清除事件
 *
 * \param ctx struct libmono_runtime_mainloop_task_context* 任务上下文
 * \param events uint16_t 事件或者事件的组合（按位或）
 *
 */
void libmono_runtime_mainloop_task_event_clear(struct libmono_runtime_mainloop_task_context *ctx,uint16_t events) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

/** \brief 获取事件组
 *
 * \param ctx struct libmono_runtime_mainloop_task_context* 任务上下文
 * \return uint16_t 事件组
 *
 */
uint16_t libmono_runtime_mainloop_task_event(struct libmono_runtime_mainloop_task_context *ctx) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

/** \brief 主循环处理
 *
 * 注意：在多处中调用主循环处理可能导致异常
 *
 */
void libmono_runtime_mainloop_process(void) LIBMONO_FUNCTION_ATTRIBUTE;

/** \brief 判断主循环是否空闲
 *
 * 注意：此函数在任务中调用时需要先将当前任务所有事件处理完毕。
 *
 * \return bool 是否空闲,当空闲时,可进入低功耗模式（如果有电源管理相关接口）
 *
 */
bool libmono_runtime_mainloop_is_idle(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

/** \brief 主循环当前任务
 *
 * \return libmono_runtime_mainloop_task_context_t* 当前任务指针，失败返回NULL
 *
 */
libmono_runtime_mainloop_task_context_t * libmono_runtime_mainloop_current_task(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

