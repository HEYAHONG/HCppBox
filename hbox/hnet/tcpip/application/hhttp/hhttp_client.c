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

