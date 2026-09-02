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

/*
 * 从机命令，最多可定义32个命令（包括功能码0），其中DL/T 645协议已使用13个命令
 */
typedef bool (*hdlt645_slave_io_ctx_cmd_cb_process_t)(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);
struct hdlt645_slave_io_ctx_cmd
{
    uint8_t fct;                                        /**< 功能码，0表示结束 */
    hdlt645_slave_io_ctx_cmd_cb_process_t process;      /**< 处理函数，返回true回复 */
    uintptr_t usr[2];                                   /**< 用户参数 */
};

#define HDLT645_SLAVE_IO_CTX_CMD_TIME(PROCESS,TIME_SYNC) \
            {HDLT645_FRAME_CONTROL_FCT_TIME,PROCESS,{(uintptr_t)(TIME_SYNC),0}}                                 /**< 广播校时 */
#define HDLT645_SLAVE_IO_CTX_CMD_READ(PROCESS,DI_TABLE,DI_TABLE_SIZE) \
            {HDLT645_FRAME_CONTROL_FCT_READ,PROCESS,{(uintptr_t)(DI_TABLE),DI_TABLE_SIZE}}                      /**< 读取数据 */
#define HDLT645_SLAVE_IO_CTX_CMD_READEXT(PROCESS,DI_TABLE,DI_TABLE_SIZE) \
            {HDLT645_FRAME_CONTROL_FCT_READEXT,PROCESS,{(uintptr_t)(DI_TABLE),DI_TABLE_SIZE}}                   /**< 读取后续数据 */
#define HDLT645_SLAVE_IO_CTX_CMD_WRITE(PROCESS,DI_TABLE,DI_TABLE_SIZE) \
            {HDLT645_FRAME_CONTROL_FCT_WRITE,PROCESS,{(uintptr_t)(DI_TABLE),DI_TABLE_SIZE}}                     /**< 读取后续数据 */
#define HDLT645_SLAVE_IO_CTX_CMD_END() \
            {0,NULL,{0,0}}                                                                                      /**< 命令结束，命令表最后一个成员必须是命令结束 */

/*
 * 时间，用于广播校时
 */
struct hdlt645_slave_time;
typedef struct hdlt645_slave_time hdlt645_slave_time_t;
struct hdlt645_slave_time
{
    void (*time_sync)(const hdlt645_slave_time_t *time,uint8_t ss,uint8_t mm,uint8_t hh,uint8_t DD,uint8_t MM,uint8_t YY);                       /**< 时间同步，参数一般为BCD码 */
    uintptr_t usr;
};

extern const hdlt645_slave_time_t hdlt645_slave_time_default;

/*
 * 数据标识，可用于读数据、读剩余数据、写数据
 */
struct hdlt645_slave_di;
typedef struct hdlt645_slave_di hdlt645_slave_di_t;
struct hdlt645_slave_di
{
    uint32_t di_num;                                                                                                    /**< 数据标识 */
    void (*set_time)(const hdlt645_slave_di_t *di,uint8_t mm,uint8_t hh,uint8_t DD,uint8_t MM,uint8_t YY);              /**< 设定查询时间,可选 */
    void (*reset_time)(const hdlt645_slave_di_t *di);                                                                   /**< 取消查询时间限制，可选 */
    void (*write_enable)(const hdlt645_slave_di_t *di,hdlt645_data_p_t *p,hdlt645_data_c_t *c);                         /**< 写使能，可选 */
    void (*write_disable)(const hdlt645_slave_di_t *di);                                                                /**< 写使能关闭,可选 */
    size_t (*getlen)(const hdlt645_slave_di_t *di);                                                                     /**< 获取长度，一般情况下，长度是固定值,不可为空 */
    size_t (*write)(const hdlt645_slave_di_t *di,const uint8_t *buff,size_t length);                                    /**< 写数据，返回已写入的值,不可为空 */
    size_t (*read)(const hdlt645_slave_di_t *di,uint8_t *buff,size_t length);                                           /**< 读数据，返回已读取的值，不可为空 */
    uintptr_t usr;                                                                                                      /**< 用户参数 */
};

#define HDLT645_SLAVE_DI_DEFINE(DI0,DI1,DI2,DI3)                                        ((DI0)+(DI1)*(1UL << (8))+(DI2)*(1UL << (16))+(DI3)*(1UL << (24)))
#define HDLT645_SLAVE_DI_DEFINE2(DI3,DI2,DI1,DI0)                                       HDLT645_SLAVE_DI_DEFINE(DI0,DI1,DI2,DI3)

/** \brief 获取数据操作计数
 *
 * \param di_table const hdlt645_slave_di_t* 数据表
 * \param di_table_len size_t 数据表长度
 * \param di_dst uint32_t 目标数据标识
 * \param max_len 单次操作长度
 * \return size_t 操作计数
 *
 */
size_t hdlt645_slave_di_count(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,size_t max_len);

/** \brief 读取数据
 *
 * \param di_table const hdlt645_slave_di_t* 数据表
 * \param di_table_len size_t 数据表长度
 * \param di_dst_num uint32_t 目标数据标识
 * \param index size_t 从0开始的引索，小于操作计数
 * \param data uint8_t* 数据
 * \param datalen size_t 数据缓冲指针
 * \return size_t 读取的数据长度
 *
 */
size_t hdlt645_slave_di_read(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,size_t index,uint8_t *data,size_t datalen);

/** \brief 写入数据
 *
 * \param di_table const hdlt645_slave_di_t* 数据表
 * \param di_table_len size_t 数据表长度
 * \param di_dst_num uint32_t 目标数据标识
 * \param data const uint8_t* 数据
 * \param datalen size_t 数据长度
 * \return size_t 写入的数据长度
 *
 */
size_t hdlt645_slave_di_write(const hdlt645_slave_di_t *di_table,size_t di_table_len,uint32_t di_dst_num,const uint8_t *data,size_t datalen);

/*
 * 广播校时、数据、读后续数据、写数据
 */
bool hdlt645_slave_io_ctx_cmd_time_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);
bool hdlt645_slave_io_ctx_cmd_read_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);
bool hdlt645_slave_io_ctx_cmd_readext_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);
bool hdlt645_slave_io_ctx_cmd_write_process(hdlt645_slave_io_ctx_t *ctx,hdlt645_slave_io_t *io,const hdlt645_slave_io_ctx_cmd_t *cmd,uint8_t *data,size_t datalen,uint8_t *reply_buffer,size_t reply_buffer_len);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_SLAVE_H_INCLUDED
