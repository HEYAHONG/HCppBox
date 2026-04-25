/***************************************************************
 * Name:      hshell_context.h
 * Purpose:   声明hshell_context接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHELL_CONTEXT_H__
#define __HSHELL_CONTEXT_H__
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "hcompiler.h"
#include "hdefaults.h"
#include "hshell_defaults.h"
#include "hshell_ctlseq.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct
{
    int (*getchar)(void);                                   /**< 获取字符，返回8位字符，失败返回EOF */
    int (*putchar)(int ch);                                 /**< 输出字符，参数为8位字符 */
    int (*invoke_command)(int argc,const char *argv[]);     /**< 启动命令。注意：此api的优先级是最低的，仅当其它执行方式失败时才使用此api */
} hshell_context_external_api_t;         /**< 外部API */

/** \brief hshell 获取默认的外部API
 *
 * \return hshell_context_external_api_t hshell外部api
 *
 */
hshell_context_external_api_t hshell_context_default_external_api(void);


/** \brief hshell 获取默认的提示字符串
 *
 * \return const char* 提示字符串
 *
 */
const char * hshell_context_default_prompt_string(void);

struct hshell_command;
typedef struct hshell_command hshell_command_t;

struct hshell_context;
typedef struct hshell_context hshell_context_t;
struct hshell_context
{
    const char *prompt;                         /**< 提示字符串 */
    hshell_context_external_api_t api;          /**< 外部api表 */
    struct
    {
        uint32_t init:1;                        /**< 是否初始化，当未初始化时，调用上下文相关API时会执行初始化 */
        uint32_t login:1;                       /**< 是否已登录 */
        uint32_t prompt:1;                      /**< 当prompt为0时，将打印提示字符串并置为1 */
        uint32_t escape:1;                      /**< 收到特殊转义序列，为1将进入转义序列处理过程 */
        uint32_t return_newline_compatible:1;   /**< 兼容\r\n,由第一个\r触发执行,为1时表示刚刚通过\r执行 */
        uint32_t input_complete:1;              /**< 当此值为1时，输入检查函数将返回true并将此值设置为0 */
        uint32_t insert_mode:1;                 /**< 输入时是否采用插入模式，为1时为插入模式（即新字符插入当前字符串），默认为覆盖模式（即直接覆盖当前字符） */
        uint32_t echo:1;                        /**< 是否回显 */
        uint32_t show_banner:1;                 /**< 是否显示banner */
        uint32_t command_name_shortcut:1;       /**< 当此值为1时，当用户输入的命令名称匹配某个命令的前几个字母时视为匹配成功 */
    } flags;                                    /**< 标志 */
    uint8_t buffer[HSHELL_CONTEXT_BUFFER_SIZE]; /**< 缓冲 */
    size_t  buffer_ptr;                         /**< 缓冲指针 */
    struct
    {
        const hshell_command_t *array_base;     /**< 命令数组首地址 */
        size_t array_count;                     /**< 命令数组中命令的个数 */
    } command;                                  /**< 命令 */
    int command_exit_code;                      /**< 最近一次命令的退出代码 */
    uint8_t  escape_sequence[12];               /**< 转义序列 */
    struct
    {
        hshell_context_t *next;                 /**< 下一个上下文，当此指针不为空时，循环将直接进入此指针所指的上下文 */
        hshell_context_t *prev;                 /**< 原上下文，当此指针不为空时，表示现在的上下文是作为子上下文存在的 */
    } sub_context;                              /**< 子上下文，允许进入子上下文(此时原上下文处于不活跃状态)*/
#if HSHELL_MAX_HISTORY_COUNT > 0
    struct
    {
        uint8_t history[HSHELL_MAX_HISTORY_COUNT][HSHELL_CONTEXT_BUFFER_SIZE];  /**< 历史记录 */
        size_t store_ptr;                                                       /**< 当前保存的历史记录的指针 */
        size_t load_ptr;                                                        /**< 当前加载的历史记录的指针 */
    } history;
#endif // HSHELL_MAX_HISTORY_COUNT
};

/** \brief hshell 获取获取默认上下文
 *
 * \return hshell_context_t* hshell默认上下文
 *
 */
hshell_context_t *hshell_context_default_context(void);


/** \brief hshell上下文初始化
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 *
 */
void hshell_context_init(hshell_context_t *ctx);

/** \brief hshell设置提示字符串
 *
 * \param ctx hshell_context_t * hshell上下文,为NULL时使用默认上下文
 * \param prompt const char* 提示字符串，为NULL时无效。注意：字符串不能存储在栈上。
 * \return const char* 原提示字符串
 *
 */
const char *hshell_prompt_string_set(hshell_context_t *ctx,const char *prompt);

/** \brief hshell获取提示字符串
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return const char* 提示字符串
 *
 */
const char *hshell_prompt_string_get(hshell_context_t *ctx);

/** \brief hshell设置外部api
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param api hshell_context_external_api_t 外部api
 * \return hshell_context_external_api_t 原外部api
 *
 */
hshell_context_external_api_t hshell_external_api_set(hshell_context_t *ctx,hshell_context_external_api_t api);

/** \brief hshell获取外部api
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return hshell_context_external_api_t 外部api
 *
 */
hshell_context_external_api_t hshell_external_api_get(hshell_context_t *ctx);


/** \brief hshell设置回显设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param echo bool 回显设置
 * \return bool 原回显设置
 *
 */
bool hshell_echo_set(hshell_context_t *ctx,bool echo);


/** \brief hshell获取回显设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return bool 回显设置
 *
 */
bool hshell_echo_get(hshell_context_t *ctx);


/** \brief hshell设置banner显示设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param  show_banner bool banner显示设置
 * \return bool banner显示设置
 *
 */
bool hshell_show_banner_set(hshell_context_t *ctx,bool show_banner);


/** \brief hshell获取banner显示设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return bool banner显示设置
 *
 */
bool hshell_show_banner_get(hshell_context_t *ctx);

/** \brief hshell命令名称shortcut设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param  command_name_shortcut bool 命令名称shortcut设置
 * \return bool 命令名称shortcut设置
 *
 */
bool hshell_command_name_shortcut_set(hshell_context_t *ctx,bool command_name_shortcut);


/** \brief hshell获取命令名称shortcut设置
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return bool 命令名称shortcut设置
 *
 */
bool hshell_command_name_shortcut_get(hshell_context_t *ctx);

/** \brief hshell设置命令数组
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param array_base const hshell_command_t* 命令数组基地址
 * \param array_count size_t 命令数组中命令个数
 *
 */
void hshell_command_array_set(hshell_context_t *ctx,const hshell_command_t *array_base,size_t array_count);


/** \brief hshell读取字符
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return int 8位字符，失败返回EOF
 *
 */
int hshell_context_getchar(hshell_context_t *ctx);


/** \brief hshell写入字符
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param ch int 待写入的字符
 * \return int 已写入8位字符
 *
 */
int hshell_context_putchar(hshell_context_t *ctx,int ch);


/** \brief  hshell格式化输出,注意：组件外部格式化输出推荐采用hshell_printf函数
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param fmt const char* 格式字符串
 * \param ...   参数
 * \return int 返回值
 *
 */
int hshell_context_printf(hshell_context_t *ctx,const char *fmt,...);

/** \brief hshell进入上下文
 *
 * \param ctx hshell_context_t* 当前上下文
 * \param next_ctx hshell_context_t* 子上下文，不可为NULL，注意：子上下文一般不能在栈上分配。
 *
 */
void hshell_subcontext_enter(hshell_context_t *ctx,hshell_context_t *next_ctx);

/** \brief hshell退出子上下文（由原上下文调用）
 *
 * \param ctx hshell_context_t* 原上下文
 *
 */
void hshell_subcontext_exit(hshell_context_t *ctx);

/** \brief hshell退出子上下文（由子上下文调用）
 *
 * \param sub_ctx hshell_context_t* 子上下文，不可为NULL
 *
 */
void hshell_subcontext_exit_from_sub(hshell_context_t *sub_ctx);


/** \brief hshell 从argv参数中获取上下文指针
 *
 * \param argc int argc参数
 * \param argv[] const char* argv参数
 * \return hshell_context_t* hshell上下文
 *
 */
hshell_context_t * hshell_context_get_from_main_argv(int argc,const char *argv[]);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSHELL_CONTEXT_H__
