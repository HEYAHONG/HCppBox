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
#include "hshell_context.h"
#include "hshell_command.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief hshell执行命令,注意:执行hshell循环时，此函数不能在其它线程调用，否则将影响执行状态
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param cmdline char* 命令字符串, 注意：此字符串会被改变
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

/** \brief  hshell格式化输出
 *
 * \param ctx hshell_context_t* hshell上下文,为NULL时使用默认上下文
 * \param fmt const char* 格式字符串
 * \param ...   参数
 * \return int 返回值
 *
 */
int hshell_printf(hshell_context_t *ctx,const char *fmt,...);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSHELL_H__
