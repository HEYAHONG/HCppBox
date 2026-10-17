/***************************************************************
 * Name:      hhttp_tinyserver.c
 * Purpose:   实现hhttp_tinyserver接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime.h"
#include "hhttp_tinyserver.h"

const hhttp_tinyserver_vhost_t * hhttp_tinyserver_service_find_vhost(const hhttp_tinyserver_service_t *service,hhttp_string_t host)
{
    if(service==NULL)
    {
        return NULL;
    }
    const hhttp_tinyserver_vhost_t * vhost=service->default_vhost;
    if(service->vhost!=NULL && service->vhost_count > 0)
    {
        for(size_t i=0; i<service->vhost_count; i++)
        {
            if(service->vhost[i].vhost_match!=NULL && service->vhost[i].vhost_match(&service->vhost[i],host))
            {
                vhost=&service->vhost[i];
                break;
            }
        }
    }
    return vhost;
}

void hhttp_tinyserver_connection_cleanup(hhttp_tinyserver_connection_t *connection)
{
    if(connection == NULL)
    {
        return;
    }

    if(connection->connection_file!=NULL)
    {
        if(connection->connection_session!=NULL && connection->connection_session->session_target_file_close!=NULL)
        {
            connection->connection_session->session_target_file_close(connection->connection_session,connection->connection_file);
        }
        connection->connection_file=NULL;
    }

    if(connection->connection_session!=NULL)
    {
        if(connection->connection_vhost!=NULL && connection->connection_vhost->vhost_session_close!=NULL)
        {
            connection->connection_vhost->vhost_session_close(connection->connection_vhost,connection->connection_session);
        }
        connection->connection_session=NULL;
    }

    connection->connection_vhost=NULL;

    /*
     * 清除所有标志
     */
    memset(&connection->flags,0,sizeof(connection->flags));

    /*
     * 清除状态行
     */
    memset(&connection->status_line,0,sizeof(connection->status_line));

    /*
     * 清除主体
     */
    memset(&connection->body,0,sizeof(connection->body));

    connection->header_buffer_index=0;

    connection->header_buffer_offset=0;
}

static bool hhttp_tinyserver_connection_input_is_body(hhttp_tinyserver_connection_t *connection)
{
    bool ret=false;
    if(connection == NULL)
    {
        return ret;
    }

    ret=(connection->flags.body!=0);

    return ret;
}

static void hhttp_tinyserver_body_parse_data(hhttp_tinyserver_connection_t *connection,uint64_t offset,hhttp_string_t part_data)
{
    if(connection==NULL)
    {
        return;
    }

    if(connection->connection_session!=NULL && connection->connection_session->session_process_body)
    {
        connection->connection_session->session_process_body(connection->connection_session,offset,part_data);
    }
}

static void hhttp_tinyserver_body_parse_data_end(hhttp_tinyserver_connection_t *connection)
{
    if(connection==NULL)
    {
        return;
    }

    if(connection->connection_session!=NULL && connection->connection_session->session_process_body_end)
    {
        connection->connection_session->session_process_body_end(connection->connection_session);
    }
}

static void hhttp_tinyserver_body_chunked_parse_data(hhttp_body_chunked_parse_t *body_chunked,uint64_t offset,hhttp_string_t part_data)
{
    if(body_chunked == NULL)
    {
        return;
    }
    hhttp_tinyserver_body_parse_data((hhttp_tinyserver_connection_t *)body_chunked->usr,offset,part_data);
    if(hhttp_body_chunked_parse_is_end(body_chunked))
    {
        hhttp_tinyserver_body_parse_data_end((hhttp_tinyserver_connection_t *)body_chunked->usr);
    }
}

static void hhttp_tinyserver_body_normal_parse_data(hhttp_body_normal_parse_t *body_normal,uint64_t offset,hhttp_string_t part_data)
{
    if(body_normal == NULL)
    {
        return;
    }
    hhttp_tinyserver_body_parse_data((hhttp_tinyserver_connection_t *)body_normal->usr,offset,part_data);
    if(hhttp_body_normal_parse_is_end(body_normal))
    {
        hhttp_tinyserver_body_parse_data_end((hhttp_tinyserver_connection_t *)body_normal->usr);
    }
}

static bool hhttp_tinyserver_connection_input_is_body_chunked(hhttp_tinyserver_connection_t *connection)
{
    bool ret=false;
    if(connection == NULL)
    {
        return ret;
    }

    ret=(connection->flags.body_chunked!=0);

    if(ret)
    {
        connection->body.body_chunked.data=hhttp_tinyserver_body_chunked_parse_data;
        connection->body.body_chunked.usr=connection;
    }
    else
    {
        connection->body.body_normal.data=hhttp_tinyserver_body_normal_parse_data;
        connection->body.body_normal.usr=connection;
    }

    return ret;
}

static bool hhttp_tinyserver_connection_body_input(hhttp_tinyserver_connection_t *connection,uint8_t *data,size_t datalen)
{
    bool ret=false;

    if(connection == NULL)
    {
        return false;
    }

    ret=true;

    /*
     * 处理请求主体
     */
    while(datalen > 0)
    {
        size_t single_data_length=datalen;
        if(hhttp_tinyserver_connection_input_is_body_chunked(connection))
        {
            if(hhttp_body_chunked_parse_is_end(&connection->body.body_chunked))
            {
                ret=false;
                /*
                 * 请求结束后仍然有数据
                 */
                connection->flags.bad_request=1;
                break;
            }
            ret=hhttp_body_chunked_parse_input(&connection->body.body_chunked,data,&single_data_length);
        }
        else
        {
            if(hhttp_body_normal_parse_is_end(&connection->body.body_normal))
            {
                ret=false;
                /*
                 * 请求结束后仍然有数据
                 */
                connection->flags.bad_request=1;
                break;
            }
            ret=hhttp_body_normal_parse_input(&connection->body.body_normal,data,&single_data_length);
        }
        datalen-=single_data_length;
        data+=single_data_length;
    }

    return ret;
}

static bool hhttp_tinyserver_connection_header_input(hhttp_tinyserver_connection_t *connection,uint8_t *data,size_t datalen)
{
    bool ret=false;
    if(connection==NULL)
    {
        return ret;
    }
    if(data == NULL || datalen == 0)
    {
        return true;
    }

    ret=true;
    for(size_t i=0; i<datalen; i++)
    {
        connection->header_buffer[connection->header_buffer_index]=data[i];
        connection->header_buffer_index++;
        if(connection->header_buffer_index >= connection->header_buffer_external_size+sizeof(connection->header_buffer))
        {
            /*
             * 存储头部的缓冲溢出
             */
            connection->flags.bad_request=1;
            break;
        }
        if(data[i]=='\n')
        {
            hhttp_string_t line=hhttp_string_from_buffer(&connection->header_buffer[connection->header_buffer_offset],connection->header_buffer_index-connection->header_buffer_offset);
            hhttp_string_t line_content=hhttp_string_line_get_content(line);
            if(!hhttp_string_is_empty(line_content))
            {
                connection->header_buffer_offset += line_content.str_len;
                if(connection->flags.request_line==0)
                {
                    if(!hhttp_request_line_parse(&connection->request_line,line_content))
                    {
                        /*
                         * 请求行处理失败
                         */
                        connection->flags.bad_request=1;
                        break;
                    }
                    else
                    {
                        /*
                         * 请求行已处理
                         */
                        connection->flags.request_line=1;
                        connection->status_line.version=connection->request_line.version;
                        if(connection->request_line.version == HHTTP_VERSION_1_0)
                        {
                            connection->flags.keep_alive=0;
                        }
                        else if(connection->request_line.version == HHTTP_VERSION_1_1)
                        {
                            connection->flags.keep_alive=1;
                        }

                        hhttp_body_normal_parse_init(&connection->body.body_normal,hhttp_tinyserver_body_normal_parse_data,0,connection);

                        if(connection->service == NULL)
                        {
                            /*
                             * 请求行处理失败
                             */
                            connection->flags.internal_error=1;
                            break;
                        }
                        else
                        {
                            connection->connection_vhost=connection->service->default_vhost;
                        }
                    }
                }
                else
                {
                    if(!hhttp_body_is_begin(line_content))
                    {
                        /*
                         * 处理头部字段
                         */
                        hhttp_field_t field;
                        if(!hhttp_field_parse(&field,line_content))
                        {
                            /*
                             * 请求行处理失败
                             */
                            connection->flags.bad_request=1;
                            break;
                        }



                        /*
                         * 根据不同的头部字段做一些处理
                         */
                        if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Host")))
                        {
                            /*
                             * Host字段
                             */
                            if(connection->service == NULL)
                            {
                                /*
                                 * 请求行处理失败
                                 */
                                connection->flags.internal_error=1;
                                break;
                            }
                            else
                            {
                                connection->connection_vhost=hhttp_tinyserver_service_find_vhost(connection->service,field.field_values);
                            }
                        }
                        else if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Connection")))
                        {
                            if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("close")))
                            {
                                connection->flags.keep_alive=0;
                            }
                            else if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("keep-alive")))
                            {
                                connection->flags.keep_alive=1;
                            }
                        }
                        else if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Content-Length")))
                        {
                            uint64_t content_length=hhttp_string_dec_to_uint64(field.field_values);
                            hhttp_body_normal_parse_init(&connection->body.body_normal,hhttp_tinyserver_body_normal_parse_data,content_length,connection);
                        }
                        else if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Transfer-Encoding")))
                        {
                            if(!hhttp_string_is_empty(hhttp_string_strstr(field.field_values,hhttp_string_from_cstring("chunked"))))
                            {
                                connection->flags.body_chunked=1;
                                hhttp_body_chunked_parse_init(&connection->body.body_chunked,hhttp_tinyserver_body_chunked_parse_data,connection);
                            }
                        }
                    }
                    else
                    {
                        /*
                         * 头部结束
                         */
                        connection->flags.body=1;
                        if(connection->connection_vhost==NULL)
                        {
                            /*
                             * 内部错误
                             */
                            connection->flags.internal_error=1;
                            break;
                        }
                        if(connection->connection_vhost->vhost_session_open!=NULL)
                        {
                            connection->connection_session=connection->connection_vhost->vhost_session_open(connection->connection_vhost,connection->request_line);
                            if(connection->connection_session==NULL)
                            {
                                /*
                                 * 找不到
                                 */
                                connection->flags.not_found=1;
                                break;
                            }
                            else
                            {
                                size_t header_offset=0;
                                size_t header_length=connection->header_buffer_index;
                                {
                                    /*
                                     * 跳过request_line
                                     */
                                    hhttp_string_t request_line=hhttp_string_line_get_content(hhttp_string_from_buffer(&connection->header_buffer[header_offset],header_length-header_offset));
                                    header_offset += request_line.str_len;
                                }
                                if(connection->connection_session->session_process_field!=NULL)
                                {
                                    /*
                                     * 会话需要重新处理处理头部字段
                                     */
                                    while(header_length > header_offset)
                                    {
                                        hhttp_string_t line=hhttp_string_line_get_content(hhttp_string_from_buffer(&connection->header_buffer[header_offset],header_length-header_offset));
                                        header_offset += line.str_len;
                                        if(hhttp_body_is_begin(line))
                                        {
                                            break;
                                        }
                                        hhttp_field_t field;
                                        if(hhttp_field_parse(&field,line))
                                        {
                                            connection->connection_session->session_process_field(connection->connection_session,field);
                                        }
                                    }
                                }
                                if(connection->connection_session->session_target_match!=NULL)
                                {
                                    connection->status_line.status=connection->connection_session->session_target_match(connection->connection_session,connection->request_line);
                                }
                                else
                                {
                                    /*
                                     * 找不到
                                     */
                                    connection->flags.not_found=1;
                                    break;
                                }
                            }
                        }

                        if((i+1) < datalen)
                        {
                            /*
                             * 主体处理剩余数据
                             */
                            data += (i+1);
                            datalen-= (i+1);
                            hhttp_tinyserver_connection_body_input(connection,data,datalen);
                            break;
                        }

                    }
                }
            }
        }
    }

    return ret;

}

static bool hhttp_tinyserver_connection_is_client_request_error(hhttp_tinyserver_connection_t *connection)
{
    bool ret=false;
    if(connection==NULL)
    {
        return false;
    }

    if(connection->flags.bad_request != 0 || connection->flags.not_found !=0 || connection->flags.internal_error != 0)
    {
        ret=true;
        /*
         * 出错不保活
         */
        connection->flags.keep_alive=0;

        if(connection->status_line.version == HHTTP_VERSION_UNKNOWN)
        {
            connection->status_line.version=HHTTP_VERSION_1_0;
        }
        if(connection->status_line.status < 100 || connection->status_line.status >= 1000)
        {
            if(connection->flags.bad_request!=0)
            {
                connection->status_line.status=HHTTP_STATUS_CODE_400;
            }
            if(connection->flags.not_found!=0)
            {
                connection->status_line.status=HHTTP_STATUS_CODE_404;
            }
            if(connection->flags.internal_error!=0)
            {
                connection->status_line.status=HHTTP_STATUS_CODE_500;
            }
        }
    }

    return ret;
}

static bool hhttp_tinyserver_connection_is_client_request_end(hhttp_tinyserver_connection_t *connection)
{
    bool ret=false;
    if(connection==NULL)
    {
        return false;
    }

    ret=hhttp_tinyserver_connection_is_client_request_error(connection);

    if(!ret && connection->flags.body != 0)
    {
        if(connection->flags.body_chunked!=0)
        {
            ret=hhttp_body_chunked_parse_is_end(&connection->body.body_chunked);
        }
        else
        {
            ret=hhttp_body_normal_parse_is_end(&connection->body.body_normal);
        }
    }

    return ret;
}

static void hhttp_tinyserver_connection_send_server_response_write(void *usr,size_t offset,hhttp_string_t part_string)
{
    if(usr == NULL)
    {
        return;
    }

    hhttp_tinyserver_connection_t *connection=(hhttp_tinyserver_connection_t *)usr;
    if(connection->connection_write!=NULL)
    {
        connection->connection_write(connection,offset,part_string);
    }
}

static bool hhttp_tinyserver_connection_send_server_response(hhttp_tinyserver_connection_t *connection)
{
    if(connection==NULL)
    {
        return false;
    }
    bool ret=true;

    if(!hhttp_tinyserver_connection_is_client_request_error(connection))
    {
        if(connection->connection_session!=NULL &&connection->connection_session->session_target_file_open!=NULL)
        {
            connection->connection_file=connection->connection_session->session_target_file_open(connection->connection_session,connection->status_line.status,connection->request_line);
        }
    }


    /*
     * 状态行
     */
    if(!hhttp_status_line_pack(&connection->status_line,hhttp_tinyserver_connection_send_server_response_write,connection))
    {
        ret=false;
    }

    /*
     * 头
     */
    {
        {
            /*
             * Content-Type
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Content-Type");
            field.field_values=hhttp_string_from_cstring("text/html");
            bool file_field_alloc=false;
            if(connection->connection_file!=NULL && connection->connection_file->file_field_process_alloc!=NULL)
            {
                file_field_alloc=connection->connection_file->file_field_process_alloc(connection->connection_file,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_tinyserver_connection_send_server_response_write,connection))
            {
                ret=false;
            }
            if(file_field_alloc && connection->connection_file!=NULL && connection->connection_file->file_field_process_free!=NULL)
            {
                connection->connection_file->file_field_process_free(connection->connection_file,&field,true);
            }

        }

        {
            /*
             * Connection
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Connection");
            field.field_values=hhttp_string_from_cstring("close");
            if(connection->flags.keep_alive!=0)
            {
                field.field_values=hhttp_string_from_cstring("keep-alive");
            }
            bool file_field_alloc=false;
            if(connection->connection_file!=NULL && connection->connection_file->file_field_process_alloc!=NULL)
            {
                file_field_alloc=connection->connection_file->file_field_process_alloc(connection->connection_file,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_tinyserver_connection_send_server_response_write,connection))
            {
                ret=false;
            }
            if(file_field_alloc && connection->connection_file!=NULL && connection->connection_file->file_field_process_free!=NULL)
            {
                connection->connection_file->file_field_process_free(connection->connection_file,&field,true);
            }
        }

        if(connection->flags.keep_alive!=0)
        {
            /*
             * Transfer-Encoding,保活时默认使用chunked传输
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Transfer-Encoding");
            field.field_values=hhttp_string_from_cstring("chunked");
            bool file_field_alloc=false;
            if(connection->connection_file!=NULL && connection->connection_file->file_field_process_alloc!=NULL)
            {
                file_field_alloc=connection->connection_file->file_field_process_alloc(connection->connection_file,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_tinyserver_connection_send_server_response_write,connection))
            {
                ret=false;
            }
            if(file_field_alloc && connection->connection_file!=NULL && connection->connection_file->file_field_process_free!=NULL)
            {
                connection->connection_file->file_field_process_free(connection->connection_file,&field,true);
            }
        }

        {
            /*
             * Server
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Server");
            char server_name[32]= {0};
            sprintf(server_name,"hhttp/%d.%d.%d",hruntime_version_major,hruntime_version_minor,hruntime_version_patch);
            field.field_values=hhttp_string_from_cstring(server_name);
            bool file_field_alloc=false;
            if(connection->connection_file!=NULL && connection->connection_file->file_field_process_alloc!=NULL)
            {
                file_field_alloc=connection->connection_file->file_field_process_alloc(connection->connection_file,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_tinyserver_connection_send_server_response_write,connection))
            {
                ret=false;
            }
            if(file_field_alloc && connection->connection_file!=NULL && connection->connection_file->file_field_process_free!=NULL)
            {
                connection->connection_file->file_field_process_free(connection->connection_file,&field,true);
            }
        }

        if(connection->connection_file!=NULL && connection->connection_file->file_field_process_alloc!=NULL)
        {
            /*
             * 处理文件自定义的头,由文件自行添加
             */
            hhttp_field_t field;
            bool file_field_alloc=true;
            while(file_field_alloc)
            {
                memset(&field,0,sizeof(field));
                file_field_alloc=connection->connection_file->file_field_process_alloc(connection->connection_file,&field,false);
                if(file_field_alloc)
                {
                    if(!hhttp_field_line_pack(&field,hhttp_tinyserver_connection_send_server_response_write,connection))
                    {
                        ret=false;
                    }

                    if(connection->connection_file->file_field_process_free!=NULL)
                    {
                        connection->connection_file->file_field_process_free(connection->connection_file,&field,false);
                    }
                }
            }
        }

    }


    /*
     *  头与主体分隔
     */
    if(!hhttp_body_begin(hhttp_tinyserver_connection_send_server_response_write,connection))
    {
        ret=false;
    }


    /*
     * 主体
     */
    if(connection->connection_file!=NULL && connection->connection_file->file_content_read!=NULL)
    {
        size_t buffer_size=connection->header_buffer_external_size+sizeof(connection->header_buffer)-connection->header_buffer_index;
        uint8_t *buffer=&connection->header_buffer[connection->header_buffer_index];
        uint8_t statck_buffer[32];
        if(buffer_size < sizeof(statck_buffer))
        {
            buffer_size=sizeof(statck_buffer);
            buffer=statck_buffer;
        }
        uint64_t offset=0;
        uint64_t single_read=0;
        while((single_read=connection->connection_file->file_content_read(connection->connection_file,offset,buffer,buffer_size)) != ((uint64_t)-1))
        {
            hhttp_string_t part_data=hhttp_string_from_buffer(buffer,single_read);
            if(connection->flags.keep_alive == 0)
            {
                /*
                 *非保活模式直接输出数据
                 */
                hhttp_tinyserver_connection_send_server_response_write(connection,offset,part_data);
            }
            else
            {
                /*
                 * 保活模式时默认采用chunked编码
                 */
                if(!hhttp_body_chunked_pack(hhttp_tinyserver_connection_send_server_response_write,connection,part_data))
                {
                    ret=false;
                }

            }
            offset+=single_read;
        }
    }
    else if(hhttp_tinyserver_connection_is_client_request_error(connection))
    {
        if(connection->request_line.method==HHTTP_METHOD_UNKNOWN || connection->request_line.method==HHTTP_METHOD_GET || connection->request_line.method==HHTTP_METHOD_POST)
        {
            const char *html_block_1=
                "<!doctype html>\r\n"
                "<html>\r\n"
                "<head>\r\n"
                "<meta charset=\"utf-8\">\r\n"
                "<title>hhttpd error</title>\r\n"
                "<style>\r\n"
                "body{margin:0;height:100vh;display:grid;place-items:center;color:red}\r\n"
                "h1{margin:0}\r\n"
                "</style>\r\n"
                "</head>\r\n"
                "<body>\r\n"
                "<h1>\r\n";
            hhttp_tinyserver_connection_send_server_response_write(connection,0,hhttp_string_from_cstring(html_block_1));
            {
                uint8_t buffer[32];
                hhttp_tinyserver_connection_send_server_response_write(connection,0,hhttp_string_dec_from_uint64(buffer,sizeof(buffer),connection->status_line.status));
            }
            hhttp_tinyserver_connection_send_server_response_write(connection,0,hhttp_string_from_cstring(" "));
            hhttp_tinyserver_connection_send_server_response_write(connection,0,hhttp_status_code_std_to_reason_string(connection->status_line.status));
            const char *html_block_2=
                "</h1>\r\n"
                "</body>\r\n"
                "</html>\r\n";
            hhttp_tinyserver_connection_send_server_response_write(connection,0,hhttp_string_from_cstring(html_block_2));
        }
    }

    if(connection->flags.keep_alive != 0)
    {
        /*
         * chunked编码结束
         */
        if(!hhttp_body_chunked_end(hhttp_tinyserver_connection_send_server_response_write,connection))
        {
            ret=false;
        }
    }


    if(connection->flags.keep_alive==0)
    {
        if(connection->connection_close!=NULL)
        {
            connection->connection_close(connection);
        }
    }

    /*
     * 清理本次请求
     */
    hhttp_tinyserver_connection_cleanup(connection);

    return ret;
}


bool hhttp_tinyserver_connection_input(hhttp_tinyserver_connection_t *connection,uint8_t *data,size_t datalen)
{
    bool ret=false;

    if(connection==NULL || connection->connection_write == NULL)
    {
        /*
         * 当连接为空时或者写入为空时直接返回
         */
        return ret;
    }

    if(data == NULL || datalen == 0)
    {
        /*
         * 无数据时不处理但返回true
         */
        return true;
    }

    ret=true;

    if(!hhttp_tinyserver_connection_input_is_body(connection))
    {
        ret=hhttp_tinyserver_connection_header_input(connection,data,datalen);
    }
    else
    {
        ret=hhttp_tinyserver_connection_body_input(connection,data,datalen);
    }

    if(hhttp_tinyserver_connection_is_client_request_end(connection))
    {
        hhttp_tinyserver_connection_send_server_response(connection);
    }

    return ret;
}

void hhttp_tinyserver_connection_init(hhttp_tinyserver_connection_t *connection,const hhttp_tinyserver_service_t * service,hhttp_tinyserver_connection_write_t con_write,hhttp_tinyserver_connection_close_t con_close,void *usr)
{
    if(connection==NULL)
    {
        return;
    }

    memset(connection,0,sizeof(*connection));

    connection->service=service;
    connection->connection_write=con_write;
    connection->connection_close=con_close;
    connection->usr=usr;
}



