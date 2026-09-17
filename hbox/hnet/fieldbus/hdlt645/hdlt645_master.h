/***************************************************************
 * Name:      hdlt645_master.h
 * Purpose:   声明hdlt645_master接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_MASTER_H_INCLUDED__
#define __HDLT645_MASTER_H_INCLUDED__

#include "hdefaults.h"
#include "hdlt645_common.h"

#if defined(HDLT645_MASTER_EXTERN_HEADER_FILENAME)
#include HDLT645_MASTER_EXTERN_HEADER_FILENAME
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    HDLT645_MASTER_IO_STATUS_IDLE=0,
    HDLT645_MASTER_IO_STATUS_SOF1,
    HDLT645_MASTER_IO_STATUS_ADDR,
    HDLT645_MASTER_IO_STATUS_SOF2,
    HDLT645_MASTER_IO_STATUS_C,
    HDLT645_MASTER_IO_STATUS_DATALEN,
    HDLT645_MASTER_IO_STATUS_DATA,
    HDLT645_MASTER_IO_STATUS_CKSUM,
    HDLT645_MASTER_IO_STATUS_EOF,
    HDLT645_MASTER_IO_STATUS_TIMEOUT,
    HDLT645_MASTER_IO_STATUS_ERROR=-1,
} hdlt645_master_io_status_t;

/*
 * IO端口（每一个物理端口需要一个io端口）
 */
struct hdlt645_master_io;
typedef size_t (*hdlt645_master_io_cb_timeout_t)(struct hdlt645_master_io *io);
typedef size_t (*hdlt645_master_io_cb_send_t)(struct hdlt645_master_io *io,uint8_t *frame,size_t frame_len);
typedef struct hdlt645_master_io
{
    uint8_t rx_buffer[12+HDLT645_FRAME_DATALENGTH_MAX_READ];                                    /**< 主机接收数据缓冲（受限于最大读取长度） */
    size_t  rx_buffer_index;                                                                    /**< 主机接收缓冲引锁 */
#if !defined(HDLT645_MASTER_IO_NO_TX_BUFFER)
    uint8_t buffer[12+HDLT645_FRAME_DATALENGTH_MAX_WRITE];                                      /**< 主机发送数据缓冲（受限于最大写入长度） */
#endif
    hdlt645_master_io_cb_timeout_t timeout;                                                     /**< 主机超时,将恢复空闲状态 */
    hdlt645_master_io_cb_send_t send;                                                           /**< 主机发送回调不可为空 */
    void *usr;                                                                                  /**< 用户参数 */
} hdlt645_master_io_t;

/** \brief 初始化主机IO端口
 *
 * \param io hdlt645_master_io_t* IO端口
 * \param timeout hdlt645_master_io_cb_timeout_t 超时检查
 * \param send hdlt645_master_io_cb_send_t 主机发送
 * \param usr void* 用户参数
 *
 */
void hdlt645_master_io_init(hdlt645_master_io_t *io,hdlt645_master_io_cb_timeout_t timeout,hdlt645_master_io_cb_send_t send,void *usr);

/** \brief 主机接收状态
 *
 * \param io hdlt645_master_io_t* IO端口
 * \return hdlt645_master_io_status_t 状态
 *
 */
hdlt645_master_io_status_t hdlt645_master_io_status(hdlt645_master_io_t *io);

/** \brief 主机IO复位接收缓冲区（应用层处理完成后应当调用此函数复位）
 *
 * \param io hdlt645_master_io_t* IO端口
 *
 */
void hdlt645_master_io_rx_reset(hdlt645_master_io_t *io);

/** \brief 主机IO接收缓冲区输入
 *
 * \param io hdlt645_master_io_t* IO端口
 * \param data uint8_t* 数据指针
 * \param datalen size_t 数据长度
 * \return size_t 已输入的长度
 *
 */
size_t hdlt645_master_io_rx_input(hdlt645_master_io_t *io,uint8_t *data,size_t datalen);

/*
 * 主机
 */
struct hdlt645_master_ctx;
typedef struct hdlt645_master_ctx hdlt645_master_ctx_t;
struct hdlt645_master_ctx
{
    int status;
    struct
    {
        int fct;
        void *ctx;
        uintptr_t ctx_size;
    } cmd;
};

/** \brief 主机上下文初始化
 *
 * \param ctx hdlt645_master_ctx_t* 主机上下文指针
 * \param fct int 功能码
 * \param cmd_ctx void* 命令上下文指针
 * \param cmd_ctx_size size_t 命令上下文长度
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_init(hdlt645_master_ctx_t *ctx,int fct,void *cmd_ctx,size_t cmd_ctx_size);

typedef enum
{
    HDLT645_MASTER_CTX_STATUS_ERROR=-1,
    HDLT645_MASTER_CTX_STATUS_FINISHED=0,
    HDLT645_MASTER_CTX_STATUS_INIT,
    HDLT645_MASTER_CTX_STATUS_SEND_REQUEST,
    HDLT645_MASTER_CTX_STATUS_WAIT_REPLY,
} hdlt645_master_ctx_status_t;

/** \brief 获取主机上下状态
 *
 * \param ctx hdlt645_master_ctx_t* 主机上下文指针
 * \return hdlt645_master_ctx_status_t 主机上下文状态
 *
 */
hdlt645_master_ctx_status_t hdlt645_master_ctx_status(hdlt645_master_ctx_t *ctx);

/** \brief 处理主机上下文
 *
 * \param ctx hdlt645_master_ctx_t* 主机上下文指针
 * \param io hdlt645_master_io_t*  IO端口指针
 * \return hdlt645_master_ctx_status_t 主机上下文状态
 *
 */
hdlt645_master_ctx_status_t hdlt645_master_ctx_process(hdlt645_master_ctx_t *ctx,hdlt645_master_io_t *io);


/*
 * 广播时间（功能码:HDLT645_FRAME_CONTROL_FCT_TIME）
 */
struct hdlt645_master_ctx_cmd_time;
typedef struct hdlt645_master_ctx_cmd_time hdlt645_master_ctx_cmd_time_t;
typedef htime_t (*hdlt645_master_ctx_cmd_time_callback_t)(hdlt645_master_ctx_cmd_time_t *cmd);
struct hdlt645_master_ctx_cmd_time
{
    hdlt645_master_ctx_cmd_time_callback_t cb;                              /**< 读取时间的回调 */
    uintptr_t usr;                                                          /**< 用户参数 */
};

/** \brief 广播时间命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_time_t* 广播时间命令
 * \param cb hdlt645_master_ctx_cmd_time_callback_t 回调
 * \param usr void* 用户参数
 *
 */
void hdlt645_master_ctx_cmd_time_init(hdlt645_master_ctx_cmd_time_t *cmd,hdlt645_master_ctx_cmd_time_callback_t cb,void *usr);


/** \brief 广播时间命令获取当前时间
 *
 * \param cmd hdlt645_master_ctx_cmd_time_t* 广播时间命令
 * \return htime_t 当前时间
 *
 */
htime_t hdlt645_master_ctx_cmd_time_current(hdlt645_master_ctx_cmd_time_t *cmd);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_MASTER_H_INCLUDED
