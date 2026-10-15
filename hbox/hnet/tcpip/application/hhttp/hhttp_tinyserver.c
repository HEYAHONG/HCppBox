/***************************************************************
 * Name:      hhttp_tinyserver.c
 * Purpose:   实现hhttp_tinyserver接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
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
        connection->body.body_chunked.usr=connection;
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
            ret=hhttp_body_chunked_parse_input(&connection->body.body_chunked,data,&single_data_length);
        }
        else
        {
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
                    }
                    else
                    {
                        /*
                         * 头部结束
                         */

                    }
                }
            }
        }
    }

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


    return ret;
}





