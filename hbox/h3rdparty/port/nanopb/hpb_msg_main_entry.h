/***************************************************************
 * Name:      hpb_msg_main_entry.h
 * Purpose:   声明hpb_msg_main_entry接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPB_MSG_MAIN_ENTRY_H_INCLUDE__
#define __HPB_MSG_MAIN_ENTRY_H_INCLUDE__
#include "h3rdparty_port_nanopb.h"
#include "stdbool.h"
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 编码main_entry消息
 *
 * \param buffer void* 待写入的缓冲
 * \param buffer_length size_t  待写入的缓冲长度
 * \param argc int main_entry的argc字段
 * \param argv[] const char* main_entry的argv字段
 * \return size_t 已写入的字节
 *
 */
size_t hnanopb_msg_main_entry_encode(void *buffer,size_t buffer_length,int argc,const char *argv[]);


/** \brief 解码argc回调
 *
 * \param argc int argc值
 * \param usr void* 用户参数
 *
 */
typedef bool (*hnanobp_msg_main_entry_on_argc_read_t)(int argc,void *usr);

/** \brief argv读取函数
 *
 * \param ctx void* 上下文（由解码回调函数的函数参数提供）
 * \param buff void* 缓冲区
 * \param buff_length size_t 缓冲区长度
 * \return bool 是否成功
 *
 */
typedef bool (*hnanobp_msg_main_entry_argv_read_t)(void *ctx,void *buff,size_t buff_length);

/** \brief 解码argv回调
 *
 * \param ctx void* 上下文
 * \param readfunc hnanobp_msg_main_entry_argv_read_t 读取函数，用户掉取此函数读取数据
 * \param length size_t 待读取的长度
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
typedef bool (*hnanobp_msg_main_entry_on_argv_read_t)(void *ctx,hnanobp_msg_main_entry_argv_read_t readfunc,size_t length,void *usr);

/** \brief 解码main_entry消息
 *
 * \param buffer const void* 缓冲区
 * \param buffer_length size_t 缓冲区长度
 * \param argc_cb hnanobp_msg_main_entry_on_argc_read_t 解码argc回调
 * \param argv_cb hnanobp_msg_main_entry_on_argv_read_t 解码argv回调
 * \param usr void* 用户参数
 * \return bool 是否解码成功
 *
 */
bool hnanobp_msg_main_entry_decode(const void *buffer,size_t buffer_length,hnanobp_msg_main_entry_on_argc_read_t  argc_cb,hnanobp_msg_main_entry_on_argv_read_t argv_cb,void *usr);


/** \brief 编码main_entry消息(带envp变量)
 *
 * \param buffer void* 待写入的缓冲
 * \param buffer_length size_t  待写入的缓冲长度
 * \param argc int main_entry的argc字段
 * \param argv[] const char* main_entry的argv字段
 * \param envp[] const char* main_entry的envp字段
 * \return size_t 已写入的字节
 *
 */
size_t hnanopb_msg_main_entry_with_envp_encode(void *buffer,size_t buffer_length,int argc,const char *argv[],const char *envp[]);

#define hnanobp_msg_main_entry_with_envp_on_argc_read_t hnanobp_msg_main_entry_on_argc_read_t
#define hnanobp_msg_main_entry_with_envp_on_argv_read_t hnanobp_msg_main_entry_on_argv_read_t
#define hnanobp_msg_main_entry_with_envp_on_envp_read_t hnanobp_msg_main_entry_on_argv_read_t

/** \brief  解码main_entry消息(带envp变量)
 *
 * \param buffer const void* 待写入的缓冲
 * \param buffer_length size_t 待写入的缓冲长度
 * \param argc_cb hnanobp_msg_main_entry_with_envp_on_argc_read_t  解码argc回调
 * \param argv_cb hnanobp_msg_main_entry_with_envp_on_argv_read_t  解码argv回调
 * \param envp_cb hnanobp_msg_main_entry_with_envp_on_envp_read_t  解码envp回调
 * \param usr void*
 * \return bool
 *
 */
bool hnanobp_msg_main_entry_with_envp_decode(const void *buffer,size_t buffer_length,hnanobp_msg_main_entry_with_envp_on_argc_read_t  argc_cb,hnanobp_msg_main_entry_with_envp_on_argv_read_t argv_cb,hnanobp_msg_main_entry_with_envp_on_envp_read_t envp_cb,void *usr);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPB_MSG_MAIN_ENTRY_H_INCLUDE__
