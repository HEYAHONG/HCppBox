/***************************************************************
 * Name:      libmono_template_config.h
 * Purpose:   MCS-51 libmono 配置头文件
 * Author:    opencode+deepseek
 * Created:   2026-10-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

/** \brief MCS-51 libmono 配置头文件
 *
 * 本文件包含所有可外部配置的宏定义。
 * 由opencode+deepseek自动生成，请勿手动修改。
 *
 * \note 更新方法：
 * - 扫描源代码目录下lib目录中的ReadMe.md文件
 * - 提取"可外部配置的宏定义"表格
 * - 更新libmono_template_config.h文件（保留原文件风格）
 * - 文件格式：带BOM的UTF-8编码，Unix换行符(LF)
 * - 注释风格：Doxygen格式
 *
 * 使用方法：
 * - 将本文件复制为实际使用的配置头文件，并按需取消注释相关宏定义
 * - 在包含libmono.c之前包含该配置文件
 */

#ifndef LIBMONO_TEMPLATE_CONFIG_H
#define LIBMONO_TEMPLATE_CONFIG_H

/* ========== base ========== */

/** \brief (普通)数据属性
 *
 * 根据模型不同有不同的默认值
 */
/* #define LIBMONO_DATA_ATTRIBUTE */

/** \brief 常量数据属性
 */
/* #define LIBMONO_CONST_DATA_ATTRIBUTE */

/** \brief (普通)函数属性
 */
/* #define LIBMONO_FUNCTION_ATTRIBUTE */

/** \brief 可重入函数属性
 */
/* #define LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE */

/** \brief 关中断
 *
 * 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为
 */
/* #define LIBMONO_BASE_CORE_DISABLE_INTERRUPT */

/** \brief 开中断
 *
 * 通常用于实现临界区，用户可重新定义此宏定义用于控制libmono库行为
 */
/* #define LIBMONO_BASE_CORE_ENABLE_INTERRUPT */

/** \brief 不实现core
 */
/* #define LIBMONO_BASE_CORE_NO_IMPLEMENT */

/* ========== runtime ========== */

/** \brief 不移植getchar
 *
 * 若用户不实现getchar可能会在某些情况下遇到链接错误
 */
/* #define LIBMONO_RUNTIME_LIBC_NO_GETCHAR */

/** \brief 用户实现的getchar
 */
/* #define GETCHAR */

/** \brief 不移植putchar
 *
 * 若用户不实现putchar可能会在某些情况下遇到链接错误
 */
/* #define LIBMONO_RUNTIME_LIBC_NO_PUTCHAR */

/** \brief 用户实现的putchar
 */
/* #define PUTCHAR */

/** \brief 使用串口作为标准输入输出
 */
/* #define LIBMONO_RUNTIME_LIBC_STDIO_SERIAL */

/** \brief 不移植time
 *
 * 若用户不实现time将使用C库的版本
 */
/* #define LIBMONO_RUNTIME_LIBC_NO_TIME */

/** \brief 用户实现的time
 */
/* #define TIME */

/** \brief 时区偏移
 *
 * UTC时间-当前时区时间，如北京时间的时区偏移为-8*3600
 */
/* #define LIBMONO_RUNTIME_TIMEZONE_OFFSET */

/** \brief 任务数组名称
 *
 * 定义任务数组时类型采用libmono_runtime_mainloop_task_context_t * const且需要以NULL结尾。
 */
/* #define LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME */

/** \brief 不实现criticalsection
 */
/* #define LIBMONO_RUNTIME_CRITICALSECTION_NO_IMPLEMENT */

/** \brief 不实现tick
 */
/* #define LIBMONO_RUNTIME_TICK_NO_IMPLEMENT */

/** \brief 不实现mainloop
 */
/* #define LIBMONO_RUNTIME_MAINLOOP_NO_IMPLEMENT */

/** \brief 不实现libc
 */
/* #define LIBMONO_RUNTIME_LIBC_NO_IMPLEMENT */

/* ========== shell ========== */

/** \brief shell缓冲大小
 *
 * 一般不宜过大
 */
/* #define LIBMONO_SHELL_BUFFER_LENGTH */

/** \brief shell命令列表
 *
 * 具体格式见libmono_shell_command_t
 */
/* #define LIBMONO_SHELL_COMMAND_LIST */

/** \brief 不实现shell
 */
/* #define LIBMONO_SHELL_SHELL_NO_IMPLEMENT */

#endif /* LIBMONO_TEMPLATE_CONFIG_H */
