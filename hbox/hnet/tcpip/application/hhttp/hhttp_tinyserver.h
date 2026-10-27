/***************************************************************
 * Name:      hhttp_tinyserver.h
 * Purpose:   声明hhttp_tinyserver接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHTTP_TINYSERVER_H__
#define __HHTTP_TINYSERVER_H__

#include "hdefaults.h"
#include "hhttp_common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 文件，用于响应主体
 */
struct hhttp_tinyserver_file;
typedef struct hhttp_tinyserver_file hhttp_tinyserver_file_t;
struct hhttp_tinyserver_file
{
    uint64_t (*file_content_read)(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size);                      /**< 文件读取，当返回值为（(uint64_t)-1）时表示已结束  */
    bool (*file_field_process_alloc)(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);                                  /**< 头处理，is_internal为true表示传入的是内置头，否则为空，一般用于处理内置的头,返回false时且is_internal为false时表示结束头处理 */
    bool (*file_field_process_free)(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);                                   /**< 头释放，返回true表示处理完成 */
    void *usr;
};


#define HHTTP_TINYSERVER_FILE_CONTENT_READ_END                      ((uint64_t)-1)


typedef uint64_t (*hhttp_tinyserver_file_content_read_t)(hhttp_tinyserver_file_t *file,uint64_t offset,uint8_t *buffer,uint64_t buffer_size);
typedef bool (*hhttp_tinyserver_file_field_process_t)(hhttp_tinyserver_file_t *file,hhttp_field_t *field,bool is_internal);
/** \brief 文件初始化
 *
 * \param file hhttp_tinyserver_file_t* 文件
 * \param file_read hhttp_tinyserver_file_content_read_t 内容读取
 * \param field_alloc hhttp_tinyserver_file_field_process_t 头部字段分配
 * \param field_free hhttp_tinyserver_file_field_process_t 头部字段释放
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_file_init(hhttp_tinyserver_file_t *file,hhttp_tinyserver_file_content_read_t file_read,hhttp_tinyserver_file_field_process_t field_alloc,hhttp_tinyserver_file_field_process_t field_free,void *usr);

struct hhttp_tinyserver_file_buffer;
typedef struct hhttp_tinyserver_file_buffer hhttp_tinyserver_file_buffer_t;
struct hhttp_tinyserver_file_buffer
{
    hhttp_tinyserver_file_t base;
    hhttp_string_t buffer;
};

/** \brief (缓冲)文件初始化
 *
 * \param file hhttp_tinyserver_file_buffer_t* (缓冲)文件
 * \param buffer hhttp_string_t 缓冲,需要注意生命周期
 * \param field_alloc hhttp_tinyserver_file_field_process_t 头部字段分配
 * \param field_free hhttp_tinyserver_file_field_process_t 头部字段分配
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_file_buffer_init(hhttp_tinyserver_file_buffer_t *file,hhttp_string_t buffer,hhttp_tinyserver_file_field_process_t field_alloc,hhttp_tinyserver_file_field_process_t field_free,void *usr);


struct hhttp_tinyserver_file_cfile;
typedef struct hhttp_tinyserver_file_cfile hhttp_tinyserver_file_cfile_t;
struct hhttp_tinyserver_file_cfile
{
    hhttp_tinyserver_file_t base;
    FILE *stream;
};

/** \brief (C)文件初始化
 *
 * \param file hhttp_tinyserver_file_cfile_t* (C)文件
 * \param stream FILE* C文件指针
 * \param field_alloc hhttp_tinyserver_file_field_process_t 头部字段分配
 * \param field_free hhttp_tinyserver_file_field_process_t 头部字段分配
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_file_cfile_init(hhttp_tinyserver_file_cfile_t *file,FILE * stream,hhttp_tinyserver_file_field_process_t field_alloc,hhttp_tinyserver_file_field_process_t field_free,void *usr);


/*
 * 会话
 */
struct hhttp_tinyserver_session;
typedef struct hhttp_tinyserver_session hhttp_tinyserver_session_t;
struct hhttp_tinyserver_session
{
    int  (*session_target_match)(hhttp_tinyserver_session_t *session,hhttp_request_line_t request_line);                                                            /**< 匹配target，返回HTTP状态码 */
    void (*session_process_field)(hhttp_tinyserver_session_t *session,hhttp_field_t field);                                                                         /**< 处理请求头 */
    void (*session_process_body)(hhttp_tinyserver_session_t *session,uint64_t offset,hhttp_string_t part_data);                                                     /**< 会话写入数据,注意:offset无效，仅用于兼容 */
    void (*session_process_body_end)(hhttp_tinyserver_session_t *session);                                                                                          /**< 会话写入结束 */
    hhttp_tinyserver_file_t  *(*session_target_file_open)(hhttp_tinyserver_session_t *session,int http_status_code,hhttp_request_line_t request_line);              /**< 打开文件,用于响应请求 */
    void (*session_target_file_close)(hhttp_tinyserver_session_t *session,hhttp_tinyserver_file_t *file);                                                           /**< 关闭文件 */
    void *usr;
};


typedef int  (*hhttp_tinyserver_session_target_match_t)(hhttp_tinyserver_session_t *session,hhttp_request_line_t request_line);
typedef void (*hhttp_tinyserver_session_process_field_t)(hhttp_tinyserver_session_t *session,hhttp_field_t field);
typedef void (*hhttp_tinyserver_session_process_body_t)(hhttp_tinyserver_session_t *session,uint64_t offset,hhttp_string_t part_data);
typedef void (*hhttp_tinyserver_session_process_body_end_t)(hhttp_tinyserver_session_t *session);
typedef hhttp_tinyserver_file_t  *(*hhttp_tinyserver_session_target_file_open_t)(hhttp_tinyserver_session_t *session,int http_status_code,hhttp_request_line_t request_line);
typedef void (*hhttp_tinyserver_session_target_file_close_t)(hhttp_tinyserver_session_t *session,hhttp_tinyserver_file_t *file);

/** \brief 会话初始化
 *
 * \param session hhttp_tinyserver_session_t* 会话
 * \param session_match hhttp_tinyserver_session_target_match_t 匹配
 * \param session_field hhttp_tinyserver_session_process_field_t 头部字段
 * \param session_body hhttp_tinyserver_session_process_body_t 主体
 * \param session_body_end hhttp_tinyserver_session_process_body_end_t 主体结束
 * \param session_file_open hhttp_tinyserver_session_target_file_open_t 文件打开
 * \param session_file_close hhttp_tinyserver_session_target_file_close_t 文件关闭
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_session_init(hhttp_tinyserver_session_t *session,hhttp_tinyserver_session_target_match_t session_match,hhttp_tinyserver_session_process_field_t session_field,hhttp_tinyserver_session_process_body_t session_body,hhttp_tinyserver_session_process_body_end_t session_body_end,hhttp_tinyserver_session_target_file_open_t session_file_open,hhttp_tinyserver_session_target_file_close_t session_file_close,void *usr);

/*
 * 虚拟主机，用于匹配不同的Host（通过头部的Host字段确定）
 */
struct hhttp_tinyserver_vhost;
typedef struct hhttp_tinyserver_vhost hhttp_tinyserver_vhost_t;
struct hhttp_tinyserver_vhost
{
    bool (*vhost_match)(const hhttp_tinyserver_vhost_t *vhost,hhttp_string_t host);                                                             /**< 匹配主机,为NULL时默认为false,除默认虚拟主机不可为NULL  */
    hhttp_tinyserver_session_t *(*vhost_session_open)(const hhttp_tinyserver_vhost_t *vhost,hhttp_request_line_t request_line);                 /**< 打开会话,不可为空 */
    void (*vhost_session_close)(const hhttp_tinyserver_vhost_t *vhost,hhttp_tinyserver_session_t *session);                                     /**< 关闭会话 */
    void *usr;
};

typedef bool (*hhttp_tinyserver_vhost_match_t)(const hhttp_tinyserver_vhost_t *vhost,hhttp_string_t host);
typedef hhttp_tinyserver_session_t *(*hhttp_tinyserver_vhost_session_open_t)(const hhttp_tinyserver_vhost_t *vhost,hhttp_request_line_t request_line);
typedef void (*hhttp_tinyserver_vhost_session_close_t)(const hhttp_tinyserver_vhost_t *vhost,hhttp_tinyserver_session_t *session);

/** \brief 虚拟主机初始化
 *
 * \param vhost hhttp_tinyserver_vhost_t* 虚拟主机
 * \param vhost_match hhttp_tinyserver_vhost_match_t 虚拟主机匹配
 * \param vhost_session_open hhttp_tinyserver_vhost_session_open_t 打开会话
 * \param vhost_session_close hhttp_tinyserver_vhost_session_close_t 关闭会话
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_vhost_init(hhttp_tinyserver_vhost_t *vhost,hhttp_tinyserver_vhost_match_t vhost_match,hhttp_tinyserver_vhost_session_open_t vhost_session_open,hhttp_tinyserver_vhost_session_close_t vhost_session_close,void *usr);

/*
 * 服务
 */
struct hhttp_tinyserver_service;
typedef struct hhttp_tinyserver_service hhttp_tinyserver_service_t;
struct hhttp_tinyserver_service
{
    const hhttp_tinyserver_vhost_t *default_vhost;                  /**< 默认虚拟主机 */
    const hhttp_tinyserver_vhost_t *vhost;                          /**< 虚拟主机数组起始地址 */
    size_t vhost_count;                                             /**< 虚拟主机个数 */
    void *usr;
};

/** \brief 服务初始化
 *
 * \param service hhttp_tinyserver_service_t* 服务
 * \param default_vhost const hhttp_tinyserver_vhost_t* 默认虚拟主机
 * \param vhost const hhttp_tinyserver_vhost_t* 虚拟主机
 * \param vhost_count size_t 虚拟主机数量
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_service_init(hhttp_tinyserver_service_t *service,const hhttp_tinyserver_vhost_t *default_vhost,const hhttp_tinyserver_vhost_t *vhost,size_t vhost_count,void *usr);

/** \brief 服务查找虚拟主机
 *
 * \param service const hhttp_tinyserver_service_t* 服务
 * \param host hhttp_string_t 主机名（通过头部的Host字段确定）
 * \return const hhttp_tinyserver_vhost_t* 虚拟主机
 *
 */
const hhttp_tinyserver_vhost_t * hhttp_tinyserver_service_find_vhost(const hhttp_tinyserver_service_t *service,hhttp_string_t host);


/*
 * 连接，每个连接对应一个tcp（普通与ssl）连接
 */
#ifndef HHTTP_TINYSERVER_CONNECTION_MIN_HEADER_BUFFER_SIZE
#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_UNIX)
#define HHTTP_TINYSERVER_CONNECTION_MIN_HEADER_BUFFER_SIZE (16384)
#else
#define HHTTP_TINYSERVER_CONNECTION_MIN_HEADER_BUFFER_SIZE (1024)
#endif
#endif // HHTTP_TINYSERVER_CONNECTION_MIN_HEADER_BUFFER_SIZE
struct hhttp_tinyserver_connection;
typedef struct hhttp_tinyserver_connection hhttp_tinyserver_connection_t;
struct hhttp_tinyserver_connection
{
    const hhttp_tinyserver_service_t * service;                                                                                 /**< 服务 */
    const hhttp_tinyserver_vhost_t * connection_vhost;                                                                          /**< 虚拟主机 */
    hhttp_tinyserver_session_t * connection_session;                                                                            /**< 会话 */
    hhttp_tinyserver_file_t * connection_file;                                                                                  /**< 文件 */
    void (*connection_write)(hhttp_tinyserver_connection_t *connection,uint64_t offset,hhttp_string_t part_data);               /**< 连接写入数据 */
    void (*connection_close)(hhttp_tinyserver_connection_t *connection);                                                        /**< 连接关闭（服务器主动关闭，被动关闭请调用清理函数） */
    void *usr;
    struct
    {
        uint32_t keep_alive:1;                                                                                                  /**< 连接保活 */
        uint32_t request_line:1;                                                                                                /**< 请求行 */
        uint32_t body:1;                                                                                                        /**< 是否为主体 */
        uint32_t body_chunked:1;                                                                                                /**< 主体是否为chunked */
        uint32_t bad_request:1;                                                                                                 /**< 请求错误，对应HHTTP_STATUS_CODE_400,仅当状态未定义时有效*/
        uint32_t not_found:1;                                                                                                   /**< 找不到，对应HHTTP_STATUS_CODE_404,仅当状态未定义时有效*/
        uint32_t internal_error:1;                                                                                              /**< 内部错误,对应HHTTP_STATUS_CODE_500,仅当状态未定义时有效 */
    } flags;                                                                                                                    /**< 标志 */
    hhttp_request_line_t request_line;                                                                                          /**< 请求行 */
    hhttp_status_line_t status_line;                                                                                            /**< 状态行 */
    union
    {
        hhttp_body_normal_parse_t  body_normal;                                                                                /**< 正常主体，用户不应当使用 */
        hhttp_body_chunked_parse_t body_chunked;                                                                               /**< chunked主体，用户不应当使用 */
    } body;                                                                                                                     /**< 主体，用户不应当使用 */
    size_t header_buffer_external_size;                                                                                         /**< 头(请求行+头部字段)缓冲区长度，不包含本结构体已有的header_buffer，附加的缓冲区一定要在本结构体之后  */
    size_t header_buffer_offset;                                                                                                /**< 当前已解析的offset */
    size_t header_buffer_index;                                                                                                 /**< 待写入的数据的引索，也表示已写入的长度 */
    uint8_t header_buffer[HHTTP_TINYSERVER_CONNECTION_MIN_HEADER_BUFFER_SIZE];                                                  /**< 内置缓冲区 */
};


/** \brief 清理连接。当客户端断开连接时需要手动调用
 *
 * \param connection hhttp_tinyserver_connection_t* 连接
 *
 */
void hhttp_tinyserver_connection_cleanup(hhttp_tinyserver_connection_t *connection);


/** \brief 连接输入数据
 *
 * \param connection hhttp_tinyserver_connection_t* 连接
 * \param data uint8-t* 数据
 * \param datalen size_t 数据长度
 * \return bool 是否正确处理
 *
 */
bool hhttp_tinyserver_connection_input(hhttp_tinyserver_connection_t *connection,uint8_t *data,size_t datalen);

typedef void (*hhttp_tinyserver_connection_write_t)(hhttp_tinyserver_connection_t *connection,uint64_t offset,hhttp_string_t part_data);
typedef void (*hhttp_tinyserver_connection_close_t)(hhttp_tinyserver_connection_t *connection);

/** \brief 连接初始化
 *
 * \param connection hhttp_tinyserver_connection_t* 连接
 * \param service const hhttp_tinyserver_service_t* 服务
 * \param con_write hhttp_tinyserver_connection_write_t 连接写入
 * \param con_close hhttp_tinyserver_connection_close_t 连接关闭
 * \param usr void* 用户参数
 *
 */
void hhttp_tinyserver_connection_init(hhttp_tinyserver_connection_t *connection,const hhttp_tinyserver_service_t * service,hhttp_tinyserver_connection_write_t con_write,hhttp_tinyserver_connection_close_t con_close,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHTTP_TINYSERVER_H__
