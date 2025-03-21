/***************************************************************
 * Name:      hshell.h
 * Purpose:   声明shell接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSHELL_H__
#define __HSHELL_H__
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
    } flags;                                    /**< 标志 */
    uint8_t buffer[HSHELL_CONTEXT_BUFFER_SIZE]; /**< 缓冲 */
    size_t  buffer_ptr;
    struct
    {
        hshell_command_t *array_base;           /**< 命令数组首地址 */
        size_t array_count;                     /**< 命令数组中命令的个数 */
    } command;                                  /**< 命令 */
    int command_exit_code;                      /**< 最近一次命令的退出代码 */
    uint8_t  escape_sequence[12];               /**< 转义序列 */
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

/** \brief hshell设置命令数组
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param array_base hshell_command_t* 命令数组基地址
 * \param array_count size_t 命令数组中命令个数
 *
 */
void hshell_command_array_set(hshell_context_t *ctx,hshell_command_t *array_base,size_t array_count);


/** \brief hshell读取字符
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return int 8位字符，失败返回EOF
 *
 */
int hshell_getchar(hshell_context_t *ctx);


/** \brief hshell写入字符
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param ch int 待写入的字符
 * \return int 已写入8位字符
 *
 */
int hshell_putchar(hshell_context_t *ctx,int ch);


/** \brief  hshell格式化输出
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param fmt const char* 格式字符串
 * \param ...   参数
 * \return int 返回值
 *
 */
int hshell_printf(hshell_context_t *ctx,const char *fmt,...);

/** \brief hshell执行命令,注意:执行hshell循环时，此函数不能在其它线程调用，否则将影响执行状态
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param cmdline char* 命令字符串
 * \return int  返回值,命令返回值
 *
 */
int hshell_execute(hshell_context_t *ctx,char *cmdline);

/** \brief hshell进入循环，通常情况下，用户需要不断进入shell循环。
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \return int 返回值，0=正常退出
 *
 */
int hshell_loop(hshell_context_t *ctx);


/** \brief 命令入口
 */
typedef int (*hshell_command_entry_t)(int argc,const char *argv[]);

struct hshell_command
{
    hshell_command_entry_t entry;
    const char *name;
    const char *help;
};

/** \brief hshell 从argv参数中获取上下文指针
 *
 * \param argc int argc参数
 * \param argv[] const char* argv参数
 * \return hshell_context_t* hshell上下文
 *
 */
hshell_context_t * hshell_context_get_from_main_argv(int argc,const char *argv[]);


#if defined(HCOMPILER_ARMCC)
/*
 * armcc,使用名称为HShellCommand的section
 */

/*
 * 导出命令
 */
#define HSHELL_COMMAND_EXPORT(name,entry,help) \
    __SECTION("HShellCommand")\
    static const hshell_command_t hshell_commands_##name = \
    {\
        (hshell_command_entry_t)entry,\
        #name ,\
        #help \
	  }

/*
 * 注册命令（注意:此宏定义会占用hshell上下文的命令数组）
 */
extern const  int HShellCommand$$Base;
extern const  int HShellCommand$$Limit;
#define HSHELL_COMMANDS_REGISTER(context) \
hshell_command_array_set(context,(hshell_command_t *)&HShellCommand$$Base,(((uintptr_t)(hshell_command_t *)&HShellCommand$$Limit)-((uintptr_t)(hshell_command_t *)&HShellCommand$$Base))/sizeof(hshell_command_t))

#else
/*
 * 不支持的编译器（使用相关宏定义将无任何效果，也不会报错）
 */


/*
 * 导出命令
 */
#define HSHELL_COMMAND_EXPORT(name,entry,help)

/*
 * 注册命令（注意:此宏定义会占用hsell上下文的命令数组）
 */
#define HSHELL_COMMANDS_REGISTER(context)


#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSHELL_H__
