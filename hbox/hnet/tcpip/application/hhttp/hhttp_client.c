/***************************************************************
 * Name:      hhttp_client.c
 * Purpose:   实现hhttp_client接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hhttp_client.h"
#include "h3rdparty.h"

void hhttp_client_io_send(hhttp_client_io_t *io,hhttp_string_t part_data)
{
    if(io==NULL)
    {
        return;
    }
    if(io->send!=NULL && !hhttp_string_is_empty(part_data))
    {
        io->send(io,part_data);
    }
}

void hhttp_client_io_close(hhttp_client_io_t *io)
{
    if(io==NULL)
    {
        return;
    }
    if(io->close!=NULL)
    {
        io->close(io);
    }
}

bool hhttp_client_io_is_ok(hhttp_client_io_t *io)
{
    if(io==NULL)
    {
        return false;
    }
    if(io->send==NULL||io->is_ok==NULL)
    {
        return false;
    }

    if(!io->is_ok(io))
    {
        return false;
    }

    return true;
}

void hhttp_client_init(hhttp_client_t *client)
{
    if(client==NULL)
    {
        return;
    }

    memset(client,0,sizeof(*client));
}

void hhttp_client_cleanup(hhttp_client_t *client)
{
    if(client==NULL)
    {
        return;
    }

    memset(&client->flags,0,sizeof(client->flags));

    memset(&client->body,0,sizeof(client->body));

    client->header_buffer_index=0;

    memset(client->header_buffer,0,sizeof(client->header_buffer));
}

uint32_t hhttp_client_is_error(hhttp_client_t *client)
{
    if(client==NULL)
    {
        return -1;
    }
    return client->flags.error.word;
}

bool hhttp_client_is_end(hhttp_client_t *client)
{
    if(hhttp_client_is_error(client)!=0)
    {
        return true;
    }

    if(client!=NULL &&client->flags.status.body_end!=0)
    {
        return true;
    }

    return false;
}

static  void hhttp_client_write_callback(void *usr,size_t offset,hhttp_string_t part_string)
{
    if(usr==NULL)
    {
        return;
    }
    hhttp_client_t *client =(hhttp_client_t *)usr;
    hhttp_client_io_send(&client->io,part_string);
}

bool hhttp_client_send_request(hhttp_client_t *client,hhttp_method_t method,const char *uri_string,hhttp_string_t body)
{
    bool ret=false;
    if(client==NULL)
    {
        return ret;
    }
    if(hhttp_client_is_error(client) != 0)
    {
        /*
         * 内部错误返回false
         */
        return ret;
    }
    if(hhttp_client_is_end(client))
    {
        hhttp_client_cleanup(client);
    }
    if(client->flags.status.request!=0)
    {
        /*
         * 已发出请求但未结束，直接返回
         */
        return ret;
    }
    if(!hhttp_client_io_is_ok(&client->io))
    {
        /*
         * 内部错误，无法发送数据
         */
        client->flags.error.bits.internal_error=1;
        return ret;
    }
    if(uri_string==NULL)
    {
        return ret;
    }
    /*
     * 本组件忽略方案（scheme）
     */
    huriparser_string_t http_target=huriparser_parse_http_target(uri_string);
    huriparser_string_t http_host=huriparser_parse_http_host(uri_string);
    if(huriparser_string_is_empty(http_target) || huriparser_string_is_empty(http_host))
    {
        /*
         * 参数有问题
         */
        client->flags.error.bits.param_error=1;
        return ret;
    }
    ret=true;

    {
        /*
         * 发送请求行
         */
        hhttp_request_line_t request_line;
        request_line.method=method;
        request_line.target=hhttp_string_from_buffer(http_target.str,http_target.str_len);
        request_line.version=HHTTP_VERSION_1_1;
        hhttp_request_line_pack(&request_line,hhttp_client_write_callback,client);
    }

    {
        /*
         * 发送头
         */
        if(!hhttp_string_is_empty(body))
        {
            /*
             * Content-Length
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Content-Length");
            uint8_t buffer[16]= {0};
            field.field_values=hhttp_string_dec_from_uint64(buffer,sizeof(buffer),body.str_len);
            bool file_field_alloc=false;
            if(client!=NULL && client->request.field_process_alloc!=NULL)
            {
                file_field_alloc=client->request.field_process_alloc(client,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_client_write_callback,client))
            {
                ret=false;
            }
            if(file_field_alloc && client!=NULL && client->request.field_process_free!=NULL)
            {
                client->request.field_process_free(client,&field,true);
            }
        }

        {
            /*
             * Host
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Host");
            field.field_values=hhttp_string_from_buffer(http_host.str,http_host.str_len);
            bool file_field_alloc=false;
            if(client!=NULL && client->request.field_process_alloc!=NULL)
            {
                file_field_alloc=client->request.field_process_alloc(client,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_client_write_callback,client))
            {
                ret=false;
            }
            if(file_field_alloc && client!=NULL && client->request.field_process_free!=NULL)
            {
                client->request.field_process_free(client,&field,true);
            }
        }

        {
            /*
             * Accept
             */
            hhttp_field_t field;
            field.field_name=hhttp_string_from_cstring("Accept");
            field.field_values=hhttp_string_from_cstring("*/*");
            bool file_field_alloc=false;
            if(client!=NULL && client->request.field_process_alloc!=NULL)
            {
                file_field_alloc=client->request.field_process_alloc(client,&field,true);
            }
            if(!hhttp_field_line_pack(&field,hhttp_client_write_callback,client))
            {
                ret=false;
            }
            if(file_field_alloc && client!=NULL && client->request.field_process_free!=NULL)
            {
                client->request.field_process_free(client,&field,true);
            }
        }

        {
            /*
             * 用户信息
             */
            huriparser_string_t userinfo=huriparser_parse_userinfo(uri_string);
            if(!huriparser_string_is_empty(userinfo))
            {
                hhttp_field_basic_authorization_t basic_authorization;
                basic_authorization.userinfo=hhttp_string_from_buffer(userinfo.str,userinfo.str_len);
                if(!hhttp_field_basic_authorization_pack(&basic_authorization,hhttp_client_write_callback,client))
                {
                    ret=false;
                }
            }

        }

        if(client->request.field_process_alloc!=NULL)
        {
            /*
             * 处理文件自定义的头,由文件自行添加
             */
            hhttp_field_t field;
            bool file_field_alloc=true;
            while(file_field_alloc)
            {
                memset(&field,0,sizeof(field));
                file_field_alloc=client->request.field_process_alloc(client,&field,false);
                if(file_field_alloc)
                {
                    if(!hhttp_field_line_pack(&field,hhttp_client_write_callback,client))
                    {
                        ret=false;
                    }

                    if(client->request.field_process_free!=NULL)
                    {
                        client->request.field_process_free(client,&field,false);
                    }
                }
            }
        }
    }

    hhttp_body_begin(hhttp_client_write_callback,client);

    /*
     * 主体
     */
    if(!hhttp_string_is_empty(body))
    {
        hhttp_client_write_callback(client,0,body);
    }

    client->flags.status.request=1;

    return ret;
}

static void hhttp_client_body_parse_data(hhttp_client_t *client,uint64_t offset,hhttp_string_t part_data)
{
    if(client==NULL)
    {
        return;
    }

    if(client->response.process_body!=NULL)
    {
        client->response.process_body(client,offset,part_data);
    }
}

static void hhttp_client_body_parse_data_end(hhttp_client_t *client)
{
    if(client==NULL)
    {
        return;
    }

    if(client->response.process_body_end!=NULL && client->flags.status.body_end==0)
    {
        client->response.process_body_end(client);
    }

    client->flags.status.body_end=1;
}

static void hhttp_client_body_chunked_parse_data(hhttp_body_chunked_parse_t *body_chunked,uint64_t offset,hhttp_string_t part_data)
{
    if(body_chunked == NULL)
    {
        return;
    }
    hhttp_client_body_parse_data((hhttp_client_t *)body_chunked->usr,offset,part_data);
    if(hhttp_body_chunked_parse_is_end(body_chunked))
    {
        hhttp_client_body_parse_data_end((hhttp_client_t *)body_chunked->usr);
    }
}

static void hhttp_client_body_normal_parse_data(hhttp_body_normal_parse_t *body_normal,uint64_t offset,hhttp_string_t part_data)
{
    if(body_normal == NULL)
    {
        return;
    }
    hhttp_client_body_parse_data((hhttp_client_t *)body_normal->usr,offset,part_data);
    if(hhttp_body_normal_parse_is_end(body_normal))
    {
        hhttp_client_body_parse_data_end((hhttp_client_t *)body_normal->usr);
    }
}

static bool hhttp_client_input_is_body_chunked(hhttp_client_t *client)
{
    bool ret=false;
    if(client == NULL)
    {
        return ret;
    }

    ret=(client->flags.status.body_chunked!=0);

    if(ret)
    {
        client->body.body_chunked.data=hhttp_client_body_chunked_parse_data;
        client->body.body_chunked.usr=client;
    }
    else
    {
        client->body.body_normal.data=hhttp_client_body_normal_parse_data;
        client->body.body_normal.usr=client;
    }

    return ret;
}

static bool hhttp_client_body_input(hhttp_client_t *client,const uint8_t *data,size_t datalen)
{
    bool ret=false;

    if(client == NULL)
    {
        return false;
    }

    if(data==NULL || datalen == 0)
    {
        return true;
    }

    ret=true;

    /*
     * 处理请求主体
     */
    while(datalen > 0)
    {
        size_t single_data_length=datalen;
        if(hhttp_client_input_is_body_chunked(client))
        {
            if(hhttp_body_chunked_parse_is_end(&client->body.body_chunked))
            {
                break;
            }
            ret=hhttp_body_chunked_parse_input(&client->body.body_chunked,data,&single_data_length);
        }
        else
        {
            if(hhttp_body_normal_parse_is_end(&client->body.body_normal))
            {
                break;
            }
            ret=hhttp_body_normal_parse_input(&client->body.body_normal,data,&single_data_length);
        }
        datalen-=single_data_length;
        data+=single_data_length;
    }

    return ret;
}

static bool hhttp_client_header_input(hhttp_client_t *client,const uint8_t *data,size_t datalen)
{
    bool ret=false;

    if(client == NULL)
    {
        return false;
    }

    if(data==NULL || datalen == 0)
    {
        return true;
    }

    ret=true;

    for(size_t i=0; i<datalen; i++)
    {
        client->header_buffer[client->header_buffer_index]=data[i];
        client->header_buffer_index++;
        if(client->header_buffer_index >= client->header_buffer_external_size+sizeof(client->header_buffer))
        {
            /*
             * 存储头部的缓冲溢出
             */
            client->flags.error.bits.internal_error=1;
            ret=false;
            break;
        }
        if(data[i]=='\n')
        {
            hhttp_string_t line=hhttp_string_from_buffer(client->header_buffer,client->header_buffer_index);
            hhttp_string_t line_content=hhttp_string_line_get_content(line);
            if(!hhttp_string_is_empty(line_content))
            {
                if(client->flags.status.status==0)
                {
                    hhttp_status_line_t status_line;
                    ret=hhttp_status_line_parse(&status_line,line_content);
                    if(!ret)
                    {
                        client->flags.error.bits.internal_error=1;
                        ret=false;
                        break;
                    }
                    else
                    {
                        if(client->response.process_status!=NULL)
                        {
                            client->response.process_status(client,status_line);
                        }
                    }
                    client->flags.status.status=1;

                }
                else
                {
                    if(!hhttp_body_is_begin(line_content))
                    {
                        if(client->flags.status.field==0)
                        {
                            hhttp_body_normal_parse_init(&client->body.body_normal,hhttp_client_body_normal_parse_data,0,client);
                        }
                        client->flags.status.field=1;
                        /*
                         * 处理头部字段
                         */
                        hhttp_field_t field;
                        if(!hhttp_field_parse(&field,line_content))
                        {
                            /*
                             * 请求行处理失败
                             */
                            client->flags.error.bits.internal_error=1;
                            ret=false;
                            break;
                        }

                        /*
                         * 根据不同的头部字段做一些处理
                         */
                        if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Content-Length")))
                        {
                            uint64_t content_length=hhttp_string_dec_to_uint64(field.field_values);
                            hhttp_body_normal_parse_init(&client->body.body_normal,hhttp_client_body_normal_parse_data,content_length,client);
                        }
                        else if(hhttp_string_equal(field.field_name,hhttp_string_from_cstring("Transfer-Encoding")))
                        {
                            if(!hhttp_string_is_empty(hhttp_string_strstr(field.field_values,hhttp_string_from_cstring("chunked"))))
                            {
                                client->flags.status.body_chunked=1;
                                hhttp_body_chunked_parse_init(&client->body.body_chunked,hhttp_client_body_chunked_parse_data,client);
                            }
                        }

                        if(client->response.process_field!=NULL)
                        {
                            client->response.process_field(client,field);
                        }
                    }
                    else
                    {
                        /*
                         * 头部结束
                         */
                        client->flags.status.body=1;

                        if((i+1) < datalen)
                        {
                            /*
                             * 主体处理剩余数据
                             */
                            data += (i+1);
                            datalen-= (i+1);
                            hhttp_client_body_input(client,data,datalen);
                            break;
                        }

                    }


                }
                /*
                 * 重新开始计算行
                 */
                client->header_buffer_index=0;
            }
        }
    }


    return ret;
}

bool hhttp_client_input(hhttp_client_t *client,const uint8_t *data,size_t datalen)
{
    bool ret=false;
    if(client==NULL)
    {
        return ret;
    }
    if(hhttp_client_is_error(client) != 0)
    {
        /*
         * 内部错误返回false
         */
        return ret;
    }
    if(hhttp_client_is_end(client))
    {
        /*
         * 已完成数据输入,返回真
         */
        ret=true;
        return ret;
    }
    if(client->flags.status.request==0)
    {
        /*
         * 未发出请求
         */
        client->flags.error.bits.request_error=1;
        return ret;
    }


    ret=true;

    if(client->flags.status.body!=0)
    {
        ret=hhttp_client_body_input(client,data,datalen);
    }
    else
    {
        ret=hhttp_client_header_input(client,data,datalen);
    }

    if(hhttp_client_is_error(client) != 0)
    {
        /*
         * 出错时主动关闭
         */
        if(client->io.close!=NULL)
        {
            client->io.close(&client->io);
        }
    }

    return ret;
}

bool hhttp_client_input_end(hhttp_client_t *client)
{
    bool ret=false;
    if(client==NULL)
    {
        return ret;
    }
    if(hhttp_client_is_error(client) != 0)
    {
        /*
         * 内部错误返回false
         */
        return ret;
    }
    if(hhttp_client_is_end(client))
    {
        /*
         * 已完成数据输入,返回真
         */
        ret=true;
        return ret;
    }
    if(client->flags.status.request==0)
    {
        /*
         * 未发出请求
         */
        client->flags.error.bits.request_error=1;
        return ret;
    }

    if(client->flags.status.body==0)
    {
        /*
         * 不应当结束
         */
        client->flags.error.bits.internal_error=1;
        return ret;
    }

    /*
     * 输入结束
     */
    if(!hhttp_client_input_is_body_chunked(client))
    {
        ret=true;
        hhttp_body_normal_parse_input_end(&client->body.body_normal);
    }
    else
    {
        /*
         * chunked输入不可以在中间中断
         */
        ret=false;
        client->flags.error.bits.internal_error=1;
    }

    return ret;
}

