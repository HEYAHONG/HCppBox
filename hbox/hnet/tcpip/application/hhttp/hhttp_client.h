/***************************************************************
 * Name:      hhttp_client.h
 * Purpose:   声明hhttp_client接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHTTP_CLIENT_H__
#define __HHTTP_CLIENT_H__
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"
#include "hhttp_common.h"
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * HTTP客户端 IO
 */
struct hhttp_client_io;
typedef struct hhttp_client_io hhttp_client_io_t;
struct hhttp_client_io
{
    void (*send)(hhttp_client_io_t *io,hhttp_string_t part_data);           /**< 发送数据 */
    void (*close)(hhttp_client_io_t *io);                                   /**< 关闭连接，用于发生异常时关闭连接，此时is_ok应该返回flase */
    bool (*is_ok)(hhttp_client_io_t *io);                                   /**< IO是否正常， */
    intptr_t usr;                                                           /**< 用户参数 */
};

typedef void (*hhttp_client_io_send_t)(hhttp_client_io_t *io,hhttp_string_t part_data);
typedef void (*hhttp_client_io_close_t)(hhttp_client_io_t *io);
typedef bool (*hhttp_client_io_is_ok_t)(hhttp_client_io_t *io);

/** \brief IO初始化
 *
 * \param io hhttp_client_io_t* IO
 * \param io_send hhttp_client_io_send_t 发送
 * \param io_close hhttp_client_io_close_t 关闭
 * \param io_is_ok hhttp_client_io_is_ok_t 是否正常
 * \param usr void* 用户参数
 *
 */
void hhttp_client_io_init(hhttp_client_io_t *io,hhttp_client_io_send_t io_send,hhttp_client_io_close_t io_close,hhttp_client_io_is_ok_t io_is_ok,void *usr);

/** \brief IO数据发送
 *
 * \param io hhttp_client_io_t* IO
 * \param part_data hhttp_string_t 部分数据
 *
 */
void hhttp_client_io_send(hhttp_client_io_t *io,hhttp_string_t part_data);

/** \brief IO关闭连接
 *
 * \param io hhttp_client_io_t* IO
 *
 */
void hhttp_client_io_close(hhttp_client_io_t *io);

/** \brief IO是否正常运行
 *
 * \param io hhttp_client_io_t* IO
 * \return bool 是否正常
 *
 */
bool hhttp_client_io_is_ok(hhttp_client_io_t *io);


#if !defined(HHTTP_CLIENT_MIN_HEADER_BUFFER_SIZE)
#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_UNIX)
#define HHTTP_CLIENT_MIN_HEADER_BUFFER_SIZE (8192)
#else
#define HHTTP_CLIENT_MIN_HEADER_BUFFER_SIZE (1024)
#endif
#endif

struct hhttp_client;
typedef struct hhttp_client hhttp_client_t;
struct hhttp_client
{
    hhttp_client_io_t io;                                                                                               /**< io,用户不要修改内部成员 */
    struct
    {
        bool (*field_process_alloc)(hhttp_client_t *client,hhttp_field_t *field,bool is_internal);                      /**< 头处理，is_internal为true表示传入的是内置头，否则为空，一般用于处理内置的头,返回false时且is_internal为false时表示结束头处理 */
        bool (*field_process_free)(hhttp_client_t *client,hhttp_field_t *field,bool is_internal);                       /**< 头释放，返回true表示处理完成 */
        void *request_usr;
    } request;                                                                                                          /**< 请求参数，用于修改请求 */
    struct
    {
        void (*process_status)(hhttp_client_t *client,hhttp_status_line_t line);
        void (*process_field)(hhttp_client_t *client,hhttp_field_t field);                                              /**< 请求头 */
        void (*process_body)(hhttp_client_t *client,uint64_t offset,hhttp_string_t part_data);                          /**< 主体数据数据, */
        void (*process_body_end)(hhttp_client_t *client);                                                               /**< 主体数据结束 */
        void *response_usr;
    } response;
    struct
    {
        struct
        {
            uint32_t request:1;
            uint32_t status:1;
            uint32_t field:1;
            uint32_t body:1;
            uint32_t body_chunked:1;
            uint32_t body_end:1;
        } status;                                                                                                       /**< 状态 */
        union
        {
            struct
            {
                uint32_t param_error:1;
                uint32_t request_error:1;
                uint32_t internal_error:1;
            } bits;                                                                                                     /**< 错误位 */
            uint32_t word;                                                                                              /**< 字，非0表示出错，具体含义根据大小端不同而不同 */
        } error;                                                                                                        /**< 错误 */
    } flags;                                                                                                            /**< 标志 */
    void *usr;                                                                                                          /**< 用户参数 */
    union
    {
        hhttp_body_normal_parse_t  body_normal;                                                                         /**< 正常主体，用户不应当使用 */
        hhttp_body_chunked_parse_t body_chunked;                                                                        /**< chunked主体，用户不应当使用 */
    } body;                                                                                                             /**< 主体，用户不应当使用 */
    size_t header_buffer_external_size;                                                                                 /**< 缓冲区长度，不包含本结构体已有的header_buffer，附加的缓冲区一定要在本结构体之后  */
    size_t header_buffer_index;                                                                                         /**< 待写入的数据的引索，也表示已写入的长度 */
    uint8_t header_buffer[HHTTP_CLIENT_MIN_HEADER_BUFFER_SIZE];                                                         /**< 内置缓冲区（存储行） */
};

/** \brief 初始化客户端，初始化后可填写成员，一般情况下只初始化一次
 *
 * \param client hhttp_client_t* 客户端
 *
 */
void hhttp_client_init(hhttp_client_t *client);


/** \brief 设置客户端IO
 *
 * \param client hhttp_client_t* 客户端
 * \param io hhttp_client_io_t* IO
 * \param usr void* 用户参数
 *
 */
void hhttp_client_io_set(hhttp_client_t *client,hhttp_client_io_t *io,void *usr);


typedef bool (*hhttp_client_field_process_t)(hhttp_client_t *client,hhttp_field_t *field,bool is_internal);
/** \brief 客户端设置请求
 *
 * \param client hhttp_client_t* 客户端
 * \param field_alloc hhttp_client_field_process_t 头部字段分配
 * \param field_free hhttp_client_field_process_t 头部字段释放
 * \param request_usr void* 请求参数用户参数
 *
 */
void hhttp_client_request_set(hhttp_client_t *client,hhttp_client_field_process_t field_alloc,hhttp_client_field_process_t field_free,void *request_usr);


typedef void (*hhttp_client_process_status_t)(hhttp_client_t *client,hhttp_status_line_t line);
typedef void (*hhttp_client_process_field_t)(hhttp_client_t *client,hhttp_field_t field);
typedef void (*hhttp_client_process_body_t)(hhttp_client_t *client,uint64_t offset,hhttp_string_t part_data);
typedef void (*hhttp_client_process_body_end_t)(hhttp_client_t *client);
/** \brief 客户端设置响应
 *
 * \param client hhttp_client_t* 客户端
 * \param resp_status hhttp_client_process_status_t 状态
 * \param resp_field hhttp_client_process_field_t 头部字段
 * \param resp_body hhttp_client_process_body_t 主体
 * \param resp_body_end hhttp_client_process_body_end_t 主体结束
 * \param usr void * 用户参数
 *
 */
void hhttp_client_response_set(hhttp_client_t *client,hhttp_client_process_status_t resp_status,hhttp_client_process_field_t resp_field,hhttp_client_process_body_t resp_body,hhttp_client_process_body_end_t resp_body_end,void *usr);

/** \brief 清理客户端，当连接异常终止时请调用此函数清理客户端，清理之前需要完成错误处理,清理后可发起新的请求
 *，
 * \param client hhttp_client_t* 客户端
 *
 */
void hhttp_client_cleanup(hhttp_client_t *client);

/** \brief 客户端是否出错
 *
 * \param client hhttp_client_t* 客户端
 * \return uint32_t 非0表示出错
 *
 */
uint32_t hhttp_client_is_error(hhttp_client_t *client);

/** \brief 客户端是否结束
 *
 * \param client hhttp_client_t* 客户端
 * \return bool 是否结束
 *
 */
bool hhttp_client_is_end(hhttp_client_t *client);

/** \brief 客户端发送请求
 *
 * \param client hhttp_client_t* 客户端
 * \param method hhttp_method_t 方法
 * \param uri_string const char* 字符串,特殊字符与中文字符需要先使用%组编码
 * \param body hhttp_string_t 主体
 * \return bool 是否发送成功
 *
 */
bool hhttp_client_send_request(hhttp_client_t *client,hhttp_method_t method,const char *uri_string,hhttp_string_t body);


/** \brief 客户端输入数据
 *
 * \param client hhttp_client_t* 客户端
 * \param data const uint8_t* 数据
 * \param datalen size_t 数据长度
 * \return bool 是否成功
 *
 */
bool hhttp_client_input(hhttp_client_t *client,const uint8_t *data,size_t datalen);

/** \brief 客户端输入数据结束，仅当连接被服务端断开时调用
 *
 * \param client hhttp_client_t* 客户端
 * \return bool 是否成功
 *
 */
bool hhttp_client_input_end(hhttp_client_t *client);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHTTP_CLIENT_H__
