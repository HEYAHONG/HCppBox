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


/*
 * 读数据（功能码:HDLT645_FRAME_CONTROL_FCT_READ）
 */
struct hdlt645_master_ctx_cmd_read;
typedef struct hdlt645_master_ctx_cmd_read hdlt645_master_ctx_cmd_read_t;
typedef void (*hdlt645_master_ctx_cmd_read_callback_t)(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_data_di_t *di,const uint8_t *data,size_t datalen);
typedef void (*hdlt645_master_ctx_cmd_read_error_callback_t)(hdlt645_master_ctx_cmd_read_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_read
{
    hdlt645_bcd_addr_t addr;                                                    /**< 目标地址，读取回复后将修改为回复的地址 */
    uint8_t request_buffer[10];
    uint8_t request_buffer_length;
    bool    need_readext;                                                       /**< 需要读取后续数据 */
    hdlt645_master_ctx_cmd_read_callback_t read;                                /**< 读取成功回调 */
    hdlt645_master_ctx_cmd_read_error_callback_t error;                         /**< 读取失败回调 */
    uintptr_t usr;                                                              /**< 用户参数 */
};

/** \brief 读取命令初始化（格式1）
 *
 * \param cmd hdlt645_master_ctx_cmd_read_t* 读取命令
 * \param dst_addr hdlt645_bcd_addr_t * 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param read hdlt645_master_ctx_cmd_read_callback_t 读取成功回调
 * \param error hdlt645_master_ctx_cmd_read_error_callback_t 失败回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_read_init1(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr);


/** \brief 读取命令初始化（格式2）
 *
 * \param cmd hdlt645_master_ctx_cmd_read_t* 读取命令
 * \param dst_addr hdlt645_bcd_addr_t * 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param n uint8_t 负荷记录块数
 * \param read hdlt645_master_ctx_cmd_read_callback_t 读取成功回调
 * \param error hdlt645_master_ctx_cmd_read_error_callback_t 失败回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_read_init2(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t n,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr);


/** \brief 读取命令初始化（格式3）
 *
 * \param cmd hdlt645_master_ctx_cmd_read_t* 读取命令
 * \param dst_addr hdlt645_bcd_addr_t * 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param n uint8_t 负荷记录块数
 * \param timestamp htime_t 指定时间
 * \param read hdlt645_master_ctx_cmd_read_callback_t 读取成功回调
 * \param error hdlt645_master_ctx_cmd_read_error_callback_t 失败回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_read_init3(hdlt645_master_ctx_cmd_read_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t n,htime_t timestamp,hdlt645_master_ctx_cmd_read_callback_t read,hdlt645_master_ctx_cmd_read_error_callback_t error,void *usr);


/*
 * 读后续数据（功能码:HDLT645_FRAME_CONTROL_FCT_READEXT）
 */
struct hdlt645_master_ctx_cmd_readext;
typedef struct hdlt645_master_ctx_cmd_readext hdlt645_master_ctx_cmd_readext_t;
typedef void (*hdlt645_master_ctx_cmd_readext_callback_t)(hdlt645_master_ctx_cmd_readext_t *cmd,hdlt645_data_di_t *di,const uint8_t *data,size_t datalen,uint8_t seq);
typedef void (*hdlt645_master_ctx_cmd_readext_error_callback_t)(hdlt645_master_ctx_cmd_readext_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_readext
{
    hdlt645_bcd_addr_t addr;                                                    /**< 目标地址，读取回复后将修改为回复的地址 */
    hdlt645_data_di_t  di;                                                      /**< 数据标识 */
    uint8_t seq;                                                                /**< 序号，1～255 */
    bool    need_readext;                                                       /**< 需要读取后续数据 */
    hdlt645_master_ctx_cmd_readext_callback_t read;                             /**< 读取成功回调 */
    hdlt645_master_ctx_cmd_readext_error_callback_t error;                      /**< 读取失败回调 */
    uintptr_t usr;                                                              /**< 用户参数 */
};


/** \brief 读取后续数据命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_readext_t* 读取命令
 * \param dst_addr hdlt645_bcd_addr_t * 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param seq uint8_t 序号，1～255
 * \param read hdlt645_master_ctx_cmd_readext_callback_t 读取成功回调
 * \param error hdlt645_master_ctx_cmd_readext_error_callback_t 失败回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_readext_init(hdlt645_master_ctx_cmd_readext_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,uint8_t seq,hdlt645_master_ctx_cmd_readext_callback_t read,hdlt645_master_ctx_cmd_readext_error_callback_t error,void *usr);


/*
 * 读通信地址（功能码:HDLT645_FRAME_CONTROL_FCT_READADDR）
 */
struct hdlt645_master_ctx_cmd_readaddr;
typedef struct hdlt645_master_ctx_cmd_readaddr hdlt645_master_ctx_cmd_readaddr_t;
struct hdlt645_master_ctx_cmd_readaddr
{
    hdlt645_bcd_addr_t addr;                                            /**< 通信地址，用于存储读取的地址 */
};


/*
 * 写数据（功能码:HDLT645_FRAME_CONTROL_FCT_WRITE）
 */
struct hdlt645_master_ctx_cmd_write;
typedef struct hdlt645_master_ctx_cmd_write hdlt645_master_ctx_cmd_write_t;
typedef void (*hdlt645_master_ctx_cmd_write_error_callback_t)(hdlt645_master_ctx_cmd_write_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_write
{
    hdlt645_bcd_addr_t addr;                                            /**< 目标地址，回复后将修改为回复的地址*/
    hdlt645_data_di_t  di;                                              /**< 数据标识 */
    hdlt645_data_p_t   p;                                               /**< 密码 */
    hdlt645_data_c_t   c;                                               /**< 操作者 */
    uint8_t * data;                                                     /**< 数据 */
    size_t    data_length;                                              /**< 数据长度 */
    hdlt645_master_ctx_cmd_write_error_callback_t error;                /**< 错误回调 */
    uintptr_t usr;                                                      /**< 用户参数 */
};

/** \brief 写入数据命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_write_t* 写数据命令
 * \param dst_addr hdlt645_bcd_addr_t* 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param p hdlt645_data_p_t* 密码
 * \param c hdlt645_data_c_t* 操作者
 * \param data uint8_t* 数据
 * \param data_length size_t 数据长度
 * \param error hdlt645_master_ctx_cmd_write_error_callback_t 错误回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_write_init(hdlt645_master_ctx_cmd_write_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t  *di,hdlt645_data_p_t *p,hdlt645_data_c_t   *c,uint8_t *data,size_t data_length,hdlt645_master_ctx_cmd_write_error_callback_t error,void *usr);

/*
 * 写通信地址（功能码:HDLT645_FRAME_CONTROL_FCT_WRITEADDR）
 */
struct hdlt645_master_ctx_cmd_writeaddr;
typedef struct hdlt645_master_ctx_cmd_writeaddr hdlt645_master_ctx_cmd_writeaddr_t;
struct hdlt645_master_ctx_cmd_writeaddr
{
    hdlt645_bcd_addr_t addr;                                            /**< 待写入的地址，回复后将修改为回复的地址*/
};

/** \brief 写入地址命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_writeaddr_t* 写入地址命令
 * \param addr hdlt645_bcd_addr_t* 待写入的地址
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_writeaddr_init(hdlt645_master_ctx_cmd_writeaddr_t *cmd,hdlt645_bcd_addr_t *addr);

/*
 * 冻结命令（功能码:HDLT645_FRAME_CONTROL_FCT_FREEZE）
 */
struct hdlt645_master_ctx_cmd_freeze;
typedef struct hdlt645_master_ctx_cmd_freeze hdlt645_master_ctx_cmd_freeze_t;
typedef void (*hdlt645_master_ctx_cmd_freeze_error_callback_t)(hdlt645_master_ctx_cmd_freeze_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_freeze
{
    hdlt645_bcd_addr_t addr;                                            /**< 目标地址，回复后将修改为回复的地址*/
    uint8_t mm;                                                         /**< 分，BCD码，0x99用作通配符 */
    uint8_t hh;                                                         /**< 时，BCD码，0x99用作通配符 */
    uint8_t DD;                                                         /**< 天，BCD码，0x99用作通配符*/
    uint8_t MM;                                                         /**< 月，BCD码，0x99用作通配符 */
    hdlt645_master_ctx_cmd_freeze_error_callback_t error;               /**< 错误回调 */
    uintptr_t usr;                                                      /**< 用户参数 */
};

/** \brief 冻结命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_freeze_t* 冻结命令
 * \param dst_addr hdlt645_bcd_addr_t* 目标地址
 * \param mm uint8_t 分，BCD码
 * \param hh uint8_t 小时，BCD码
 * \param DD uint8_t 天，BCD码
 * \param MM uint8_t 月，BCD码
 * \param error hdlt645_master_ctx_cmd_freeze_error_callback_t 错误回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_freeze_init(hdlt645_master_ctx_cmd_freeze_t *cmd,hdlt645_bcd_addr_t *dst_addr,uint8_t mm,uint8_t hh,uint8_t DD,uint8_t MM,hdlt645_master_ctx_cmd_freeze_error_callback_t error,void *usr);

/*
 * 修改通信速率（功能码:HDLT645_FRAME_CONTROL_FCT_CHCOM）
 */
struct hdlt645_master_ctx_cmd_chcom;
typedef struct hdlt645_master_ctx_cmd_chcom hdlt645_master_ctx_cmd_chcom_t;
typedef void (*hdlt645_master_ctx_cmd_chcom_error_callback_t)(hdlt645_master_ctx_cmd_chcom_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_chcom
{
    hdlt645_bcd_addr_t addr;                                            /**< 目标地址，回复后将修改为回复的地址*/
    uint8_t z;                                                          /**< 通信特征字 */
    hdlt645_master_ctx_cmd_chcom_error_callback_t error;                /**< 错误回调 */
    uintptr_t usr;                                                      /**< 用户参数 */
};

/** \brief 更改通信速率命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_chcom_t* 更改通信速率命令
 * \param dst_addr hdlt645_bcd_addr_t* 目标地址
 * \param z uint8_t 通信特征字
 * \param error hdlt645_master_ctx_cmd_chcom_error_callback_t 错误回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_chcom_init(hdlt645_master_ctx_cmd_chcom_t *cmd,hdlt645_bcd_addr_t *dst_addr,uint8_t z,hdlt645_master_ctx_cmd_chcom_error_callback_t error,void *usr);


/*
 * 修改密码（功能码:HDLT645_FRAME_CONTROL_FCT_CHPASS）
 */
struct hdlt645_master_ctx_cmd_chpass;
typedef struct hdlt645_master_ctx_cmd_chpass hdlt645_master_ctx_cmd_chpass_t;
typedef void (*hdlt645_master_ctx_cmd_chpass_error_callback_t)(hdlt645_master_ctx_cmd_chpass_t *cmd,uint8_t err);
struct hdlt645_master_ctx_cmd_chpass
{
    hdlt645_bcd_addr_t addr;                                            /**< 目标地址，回复后将修改为回复的地址*/
    hdlt645_data_di_t  di;                                              /**< 数据标识 */
    hdlt645_data_p_t   p;                                               /**< 密码 */
    hdlt645_data_p_t   new_p;                                           /**< 新密码 */
    hdlt645_master_ctx_cmd_chpass_error_callback_t error;               /**< 错误回调 */
    uintptr_t usr;                                                      /**< 用户参数 */
};

/** \brief 修改密码命令初始化
 *
 * \param cmd hdlt645_master_ctx_cmd_chpass_t* 修改密码命令
 * \param dst_addr hdlt645_bcd_addr_t* 目标地址
 * \param di hdlt645_data_di_t* 数据标识
 * \param p hdlt645_data_p_t* 密码
 * \param new_p hdlt645_data_p_t* 新密码
 * \param error hdlt645_master_ctx_cmd_chpass_error_callback_t 错误回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hdlt645_master_ctx_cmd_chpass_init(hdlt645_master_ctx_cmd_chpass_t *cmd,hdlt645_bcd_addr_t *dst_addr,hdlt645_data_di_t *di,hdlt645_data_p_t *p,hdlt645_data_p_t *new_p,hdlt645_master_ctx_cmd_chpass_error_callback_t error,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_MASTER_H_INCLUDED
