/***************************************************************
 * Name:      hdlt645_slave.h
 * Purpose:   声明hdlt645_slave接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-31
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_SLAVE_H_INCLUDED__
#define __HDLT645_SLAVE_H_INCLUDED__

#include "hdlt645_common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    HDLT645_SLAVE_IO_STATUS_IDLE=0,
    HDLT645_SLAVE_IO_STATUS_SOF1,
    HDLT645_SLAVE_IO_STATUS_ADDR,
    HDLT645_SLAVE_IO_STATUS_SOF2,
    HDLT645_SLAVE_IO_STATUS_C,
    HDLT645_SLAVE_IO_STATUS_DATALEN,
    HDLT645_SLAVE_IO_STATUS_DATA,
    HDLT645_SLAVE_IO_STATUS_CKSUM,
    HDLT645_SLAVE_IO_STATUS_EOF,
    HDLT645_SLAVE_IO_STATUS_TIMEOUT,
    HDLT645_SLAVE_IO_STATUS_ERROR=-1,
} hdlt645_slave_io_status_t;

/*
 * IO端口（每一个物理端口需要一个io端口）
 */
struct hdlt645_slave_io;
typedef size_t (*hdlt645_slave_io_cb_timeout_t)(struct hdlt645_slave_io *io);
typedef size_t (*hdlt645_slave_io_cb_reply_t)(struct hdlt645_slave_io *io,uint8_t *frame,size_t frame_len);
typedef struct hdlt645_slave_io
{
    uint8_t rx_buffer[12+HDLT645_FRAME_DATALENGTH_MAX_WRITE];                                   /**< 从机接收数据缓冲（受限于最大写入长度） */
    size_t  rx_buffer_index;                                                                    /**< 从机接收缓冲引锁 */
#if !defined(HDLT645_SLAVE_IO_NO_TX_BUFFER)
    uint8_t buffer[12+HDLT645_FRAME_DATALENGTH_MAX_READ];                                       /**< 从机发送数据缓冲（受限于最大读取长度） */
#endif
    hdlt645_slave_io_cb_timeout_t timeout;                                                      /**< 从机超时,将恢复空闲状态 */
    hdlt645_slave_io_cb_reply_t reply;                                                          /**< 从机回复回调不可为空 */
    void *usr;                                                                                  /**< 用户参数 */
} hdlt645_slave_io_t;

/** \brief 初始化从机IO端口
 *
 * \param io hdlt645_slave_io_t* IO端口
 * \param timeout hdlt645_slave_io_cb_timeout_t 超时检查
 * \param reply hdlt645_slave_io_cb_reply_t 从机回复
 * \param usr void* 用户参数
 *
 */
void hdlt645_slave_io_init(hdlt645_slave_io_t *io,hdlt645_slave_io_cb_timeout_t timeout,hdlt645_slave_io_cb_reply_t reply,void *usr);

/** \brief 从机接收状态
 *
 * \param io hdlt645_slave_io_t* IO端口
 * \return hdlt645_slave_io_status_t 状态
 *
 */
hdlt645_slave_io_status_t hdlt645_slave_io_status(hdlt645_slave_io_t *io);

/** \brief 从机IO复位接收缓冲区（应用层处理完成后应当调用此函数复位）
 *
 * \param io hdlt645_slave_io_t* IO端口
 *
 */
void hdlt645_slave_io_rx_reset(hdlt645_slave_io_t *io);

/** \brief 从机IO接收缓冲区输入
 *
 * \param io hdlt645_slave_io_t* IO端口
 * \param data uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return size_t 已输入的长度
 *
 */
size_t hdlt645_slave_io_rx_input(hdlt645_slave_io_t *io,uint8_t *data,size_t datalen);

/*
 * 上下文，多个IO端口可共用一个上下文
 */
struct hdlt645_slave_io_ctx_cmd;
typedef struct hdlt645_slave_io_ctx_cmd hdlt645_slave_io_ctx_cmd_t;
typedef struct hdlt645_slave_io_ctx
{
    hdlt645_bcd_addr_t addr;                                        /**< 本机地址 */
    const hdlt645_slave_io_ctx_cmd_t *cmd_table;                    /**< 支持的命令表，以控制码0作为结尾 */
    void *usr;                                                      /**< 用户参数 */
} hdlt645_slave_io_ctx_t;

/** \brief 从机上下文初始化
 *
 * \param ctx hdlt645_slave_io_ctx_t* 从机上下文
 * \param cmd_table hdlt645_slave_io_ctx_cmd_t* 命令表
 * \param usr void* 用户参数
 *
 */
void hdlt645_slave_io_ctx_init(hdlt645_slave_io_ctx_t *ctx,hdlt645_bcd_addr_t *addr,hdlt645_slave_io_ctx_cmd_t *cmd_table,void *usr);

/** \brief 从机处理IO
 *
 * \param ctx hdlt645_slave_io_ctx_t* 从机上下文
 * \param io hdlt645_slave_io_t* IO端口
 *
 */
void hdlt645_slave_io_ctx_process_io(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io);

typedef bool (*hdlt645_slave_io_ctx_cmd_cb_process_t)(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);
struct hdlt645_slave_io_ctx_cmd
{
    uint8_t fct;                                        /**< 功能码，0表示结束 */
    hdlt645_slave_io_ctx_cmd_cb_process_t process;      /**< 处理函数，返回true回复 */
    uintptr_t usr[2];                                     /**< 用户参数 */
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_SLAVE_H_INCLUDED
