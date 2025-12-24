/***************************************************************
 * Name:      hpb_msg_main_entry.c
 * Purpose:   实现hpb_msg_main_entry接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hpb_msg_main_entry.h"
#include H3RDPARTY_NANOPB_PB_HEADER
#include H3RDPARTY_NANOPB_PB_ENCODE_HEADER
#include H3RDPARTY_NANOPB_PB_DECODE_HEADER
#include "pbinc/main_entry/main_entry.pb.h"
#include "pbinc/main_entry/main_entry.pb.c"


static bool hnanopb_msg_main_entry_encode_argv(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    const char **argv=(const char **)*arg;
    if(stream!=NULL && argv!=NULL && field->tag == MainEntry_argv_tag)
    {
        for(size_t i=0; argv[i]!=NULL; i++)
        {
            if (!pb_encode_tag_for_field(stream, field))
            {
                return false;
            }
            if(!pb_encode_string(stream,(const pb_byte_t *)argv[i],hstrlen(argv[i])))
            {
                return false;
            }
        }
    }
    return true;
};

size_t hnanopb_msg_main_entry_encode(void *buffer,size_t buffer_length,int argc,const char *argv[])
{
    size_t ret=0;
    if(buffer==NULL || buffer_length==0)
    {
        return ret;
    }
    MainEntry message=MainEntry_init_zero;
    message.argc=argc;
    message.argv.funcs.encode= hnanopb_msg_main_entry_encode_argv;
    message.argv.arg=(void *)argv;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_length);
    if(pb_encode(&stream, MainEntry_fields, &message))
    {
        ret=stream.bytes_written;
    }
    return ret;
}

static bool hnanobp_msg_main_entry_argv_read(void *ctx,void *buff,size_t buff_length)
{
    pb_istream_t *stream=(pb_istream_t *)ctx;
    if(stream==NULL || buff==NULL || buff_length == 0)
    {
        return false;
    }
    if(buff_length < stream->bytes_left)
    {
        return false;
    }
    pb_read(stream,(pb_byte_t *)buff,stream->bytes_left);
    return true;
}

typedef struct
{
    hnanobp_msg_main_entry_on_argv_read_t argv_cb;
    void *usr;
}   hnanopb_msg_main_entry_decode_argv_arg_t;

static bool hnanopb_msg_main_entry_decode_argv (pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    hnanopb_msg_main_entry_decode_argv_arg_t *argv_arg=(hnanopb_msg_main_entry_decode_argv_arg_t *)*arg;
    if(stream!=NULL && field->tag == MainEntry_argv_tag)
    {
        if(argv_arg!=NULL && argv_arg->argv_cb!=NULL)
        {
            return argv_arg->argv_cb(stream,hnanobp_msg_main_entry_argv_read,stream->bytes_left,argv_arg->usr);
        }
    }
    return true;
};


bool hnanobp_msg_main_entry_decode(const void *buffer,size_t buffer_length,hnanobp_msg_main_entry_on_argc_read_t  argc_cb,hnanobp_msg_main_entry_on_argv_read_t argv_cb,void *usr)
{
    bool ret=false;
    if(buffer==NULL || buffer_length==0)
    {
        return ret;
    }
    MainEntry message=MainEntry_init_zero;
    message.argc=0;
    message.argv.funcs.decode=hnanopb_msg_main_entry_decode_argv;
    hnanopb_msg_main_entry_decode_argv_arg_t argv_arg= {0};
    argv_arg.argv_cb=argv_cb;
    argv_arg.usr=usr;
    message.argv.arg=(void *)&argv_arg;
    pb_istream_t stream = pb_istream_from_buffer(buffer, buffer_length);
    if(pb_decode(&stream, MainEntry_fields, &message))
    {
        ret=true;
        if(ret && argc_cb!=NULL)
        {
            ret=argc_cb(message.argc,usr);
        }
    }

    return ret;
}
