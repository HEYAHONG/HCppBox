/***************************************************************
 * Name:      hhttp_common.c
 * Purpose:   实现hhttp_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hhttp_common.h"
#include "hcrypto.h"

hhttp_string_t hhttp_string_from_cstring(const char *c_str)
{
    hhttp_string_t ret;
    ret.str=c_str;
    if(c_str==NULL)
    {
        ret.str="";
    }
    ret.str_len=hstrlen(ret.str);
    return ret;
}

hhttp_string_t hhttp_string_from_buffer(const uint8_t *buffer,size_t buffer_len)
{
    hhttp_string_t ret;
    ret.str=(const char *)buffer;
    ret.str_len=buffer_len;
    if(ret.str == NULL)
    {
        ret.str_len = 0;
    }
    return ret;
}

bool hhttp_string_is_empty(hhttp_string_t src)
{
    bool ret=false;
    if(src.str == NULL || src.str_len == 0)
    {
        ret=true;
    }
    return ret;
}



bool hhttp_string_equal(hhttp_string_t src,hhttp_string_t dst)
{
    bool ret=false;
    if(src.str != NULL && dst.str != NULL)
    {
        if(src.str_len==dst.str_len)
        {
            ret=true;
            for(size_t i=0; i<src.str_len; i++)
            {
                if(src.str[i]!=dst.str[i])
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    return ret;
}

hhttp_string_t hhttp_string_strstr(hhttp_string_t big_str,hhttp_string_t small_str)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    if(hhttp_string_is_empty(big_str) || hhttp_string_is_empty(small_str))
    {
        return ret;
    }
    if(big_str.str_len < small_str.str_len)
    {
        return ret;
    }
    for(size_t i=0; i<=(big_str.str_len-small_str.str_len); i++)
    {
        if(big_str.str[i]==small_str.str[0])
        {
            hhttp_string_t temp;
            temp.str=&big_str.str[i];
            temp.str_len=small_str.str_len;
            if(hhttp_string_equal(temp,small_str))
            {
                ret.str=&big_str.str[i];
                ret.str_len=big_str.str_len-i;
                break;
            }
        }
    }
    return ret;
}

uint64_t hhttp_string_dec_to_uint64(hhttp_string_t src)
{
    uint64_t ret=0;
    if(!hhttp_string_is_empty(src))
    {
        for(size_t i=0; i<src.str_len; i++)
        {
            if(src.str[i] >= '0' && src.str[i] <= '9')
            {
                ret*=10;
                ret+= (src.str[i]-'0');
            }
            else
            {
                break;
            }
        }
    }
    return ret;
}

static inline uint64_t hhttp_pow_uint64(uint64_t base,uint64_t pow)
{
    uint64_t ret=1;
    for(size_t i=0; i<pow; i++)
    {
        ret*=base;
    }
    return ret;
}

hhttp_string_t hhttp_string_dec_from_uint64(uint8_t *buffer,size_t buffer_len,uint64_t number)
{
    hhttp_string_t ret=hhttp_string_from_buffer(buffer,buffer_len);
    if(buffer!=NULL && buffer_len!=0)
    {
        {
            size_t max_number_bytes=1;      //至少需要一位数字
            for(max_number_bytes=1; max_number_bytes <= ret.str_len; max_number_bytes++)
            {
                if((number/(hhttp_pow_uint64(10,max_number_bytes)))==0)
                {
                    break;
                }
            }
            ret.str_len=max_number_bytes;
        }
        for(size_t i=0; i<ret.str_len; i++)
        {
            buffer[ret.str_len-1-i] = ((number%10)+'0');
            number /= 10;
        }
    }
    return ret;
}

uint64_t hhttp_string_hex_to_uint64(hhttp_string_t src)
{
    uint64_t ret=0;
    if(!hhttp_string_is_empty(src))
    {
        for(size_t i=0; i<src.str_len; i++)
        {
            if((src.str[i] >= '0' && src.str[i] <= '9') || (src.str[i] >= 'A' && src.str[i] <= 'F') || (src.str[i] >= 'a' && src.str[i] <= 'f') )
            {
                ret*=16;
                if(src.str[i] >= '0' && src.str[i] <= '9')
                {
                    ret+= (src.str[i]-'0');
                }
                else if(src.str[i] >= 'A' && src.str[i] <= 'F')
                {
                    ret+= (src.str[i]-'A'+10);
                }
                else if(src.str[i] >= 'a' && src.str[i] <= 'f')
                {
                    ret+= (src.str[i]-'a'+10);
                }
            }
            else
            {
                break;
            }
        }
    }
    return ret;
}

hhttp_string_t hhttp_string_hex_from_uint64(uint8_t *buffer,size_t buffer_len,uint64_t number)
{
    hhttp_string_t ret=hhttp_string_from_buffer(buffer,buffer_len);
    if(buffer!=NULL && buffer_len!=0)
    {
        {
            size_t max_number_bytes=1;      //至少需要一位数字
            for(max_number_bytes=1; max_number_bytes <= ret.str_len; max_number_bytes++)
            {
                if((number/(hhttp_pow_uint64(16,max_number_bytes)))==0)
                {
                    break;
                }
            }
            ret.str_len=max_number_bytes;
        }
        for(size_t i=0; i<ret.str_len; i++)
        {
            uint8_t byte_number=(number%16);
            if(byte_number < 10)
            {
                buffer[ret.str_len-1-i]=('0'+byte_number);
            }
            else
            {
                buffer[ret.str_len-1-i]=('A'+byte_number-10);
            }
            number /= 16;
        }
    }
    return ret;
}

bool hhttp_string_check_line(hhttp_string_t line)
{
    return !hhttp_string_is_empty(hhttp_string_strstr(line,hhttp_string_from_cstring(HHTTP_LINE_END_STRING)));
}

hhttp_string_t hhttp_string_line_get_content(hhttp_string_t line)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    if(!hhttp_string_check_line(line))
    {
        return ret;
    }

    hhttp_string_t line_end=hhttp_string_strstr(line,hhttp_string_from_cstring(HHTTP_LINE_END_STRING));
    /*
     * 修正行结束长度
     */
    line_end.str_len=hhttp_string_from_cstring(HHTTP_LINE_END_STRING).str_len;
    ret.str=line.str;
    ret.str_len=line_end.str_len+(((uintptr_t)line_end.str)-((uintptr_t)line.str));

    return ret;
}

bool hhttp_string_line_content_is_empty(hhttp_string_t line_content)
{
    bool ret=false;
    if(hhttp_string_is_empty(line_content))
    {
        return ret;
    }

    ret = hhttp_string_equal(line_content,hhttp_string_from_cstring(HHTTP_LINE_END_STRING));

    return ret;
}

HDEFAULTS_RO_ATTRIBUTE
static const struct
{
    hhttp_method_t method;
    const char *   method_string;
} hhttp_method_std_string_table[]=
{
    {HHTTP_METHOD_GET,HHTTP_METHOD_GET_STRING},
    {HHTTP_METHOD_HEAD,HHTTP_METHOD_HEAD_STRING},
    {HHTTP_METHOD_POST,HHTTP_METHOD_POST_STRING},
    {HHTTP_METHOD_PUT,HHTTP_METHOD_PUT_STRING},
    {HHTTP_METHOD_DELETE,HHTTP_METHOD_DELETE_STRING},
    {HHTTP_METHOD_CONNECT,HHTTP_METHOD_CONNECT_STRING},
    {HHTTP_METHOD_OPTIONS,HHTTP_METHOD_OPTIONS_STRING},
    {HHTTP_METHOD_TRACE,HHTTP_METHOD_TRACE_STRING}
};

hhttp_string_t hhttp_method_std_to_string(hhttp_method_t method)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    for(size_t i=0; i<sizeof(hhttp_method_std_string_table)/sizeof(hhttp_method_std_string_table[0]); i++)
    {
        if(hhttp_method_std_string_table[i].method==method)
        {
            ret=hhttp_string_from_cstring(hhttp_method_std_string_table[i].method_string);
            break;
        }
    }
    return ret;
}

hhttp_method_t hhttp_method_from_string(hhttp_string_t str)
{
    hhttp_method_t ret=HHTTP_METHOD_UNKNOWN;
    for(size_t i=0; i<sizeof(hhttp_method_std_string_table)/sizeof(hhttp_method_std_string_table[0]); i++)
    {
        if(hhttp_string_equal(str,hhttp_string_from_cstring(hhttp_method_std_string_table[i].method_string)))
        {
            ret=hhttp_method_std_string_table[i].method;
            break;
        }
    }
    return ret;
}

HDEFAULTS_RO_ATTRIBUTE
static const struct
{
    hhttp_version_t version;
    const char *   version_string;
} hhttp_version_std_string_table[]=
{
    {HHTTP_VERSION_1_0,HHTTP_VERSION_1_0_STRING},
    {HHTTP_VERSION_1_1,HHTTP_VERSION_1_1_STRING}
};

hhttp_string_t hhttp_version_std_to_string(hhttp_version_t version)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    for(size_t i=0; i<sizeof(hhttp_version_std_string_table)/sizeof(hhttp_version_std_string_table[0]); i++)
    {
        if(hhttp_version_std_string_table[i].version==version)
        {
            ret=hhttp_string_from_cstring(hhttp_version_std_string_table[i].version_string);
            break;
        }
    }
    return ret;
}


hhttp_version_t hhttp_version_from_string(hhttp_string_t str)
{
    hhttp_version_t ret=HHTTP_VERSION_UNKNOWN;
    for(size_t i=0; i<sizeof(hhttp_version_std_string_table)/sizeof(hhttp_version_std_string_table[0]); i++)
    {
        if(hhttp_string_equal(str,hhttp_string_from_cstring(hhttp_version_std_string_table[i].version_string)))
        {
            ret=hhttp_version_std_string_table[i].version;
            break;
        }
    }
    return ret;
}

bool hhttp_request_line_parse(hhttp_request_line_t *request_line,hhttp_string_t line)
{
    bool ret=false;
    if(request_line == NULL || !hhttp_string_check_line(line))
    {
        return ret;
    }
    hhttp_string_t line_content=hhttp_string_line_get_content(line);

    /*
     * 去除末尾的\r\n
     */
    line_content.str_len -= 2;

    {
        hhttp_string_t first_space_string=hhttp_string_strstr(line_content,hhttp_string_from_cstring(" "));
        if(hhttp_string_is_empty(first_space_string))
        {
            return ret;
        }
        hhttp_string_t method_string;
        method_string.str=line_content.str;
        method_string.str_len=(((uintptr_t)first_space_string.str)-((uintptr_t)method_string.str));
        request_line->method=hhttp_method_from_string(method_string);

        line_content.str = &line_content.str[method_string.str_len+1];
        line_content.str_len -= (method_string.str_len+1);
    }

    {
        hhttp_string_t second_space_string=hhttp_string_strstr(line_content,hhttp_string_from_cstring(" "));
        if(hhttp_string_is_empty(second_space_string))
        {
            return ret;
        }
        hhttp_string_t target_string;
        target_string.str=line_content.str;
        target_string.str_len=(((uintptr_t)second_space_string.str)-((uintptr_t)line_content.str));
        request_line->target=target_string;
        hhttp_string_t version_string;
        version_string.str=&line_content.str[target_string.str_len+1];
        version_string.str_len=(line_content.str_len-(target_string.str_len+1));
        request_line->version=hhttp_version_from_string(version_string);
    }

    if(request_line->method != HHTTP_METHOD_UNKNOWN && !hhttp_string_is_empty(request_line->target) && request_line->version != HHTTP_VERSION_UNKNOWN)
    {
        ret=true;
    }

    return ret;
}

bool hhttp_request_line_pack(hhttp_request_line_t *request_line,hhttp_request_line_pack_write_callback_t cb,void *usr)
{
    bool ret=false;
    if(request_line == NULL )
    {
        return ret;
    }
    if(request_line->method == HHTTP_METHOD_UNKNOWN || hhttp_string_is_empty(request_line->target) || request_line->version == HHTTP_VERSION_UNKNOWN)
    {
        return ret;
    }
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t space_string=hhttp_string_from_cstring(" ");
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);


    {
        /*
         * 方法
         */
        hhttp_string_t method_string=hhttp_method_std_to_string(request_line->method);
        if(hhttp_string_is_empty(method_string))
        {
            ret=false;
        }
        cb(usr,offset,method_string);
        offset += method_string.str_len;
    }

    /*
     * 空格
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    /*
     * 目标
     */
    cb(usr,offset,request_line->target);
    offset += request_line->target.str_len;

    /*
     * 空格
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    {
        /*
         * 版本
         */
        hhttp_string_t version_string=hhttp_version_std_to_string(request_line->version);
        if(hhttp_string_is_empty(version_string))
        {
            ret=false;
        }
        cb(usr,offset,version_string);
        offset += version_string.str_len;
    }

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;
}

HDEFAULTS_RO_ATTRIBUTE
static const  struct
{
    hhttp_status_code_t status_code;
    const char *        status_code_c_string;
}   hhttp_status_code_std_string_table[]=
{
    /*
     * 信息 1XX
     */
    {HHTTP_STATUS_CODE_100,HHTTP_STATUS_CODE_100_STRING},   /**< 继续 */
    {HHTTP_STATUS_CODE_101,HHTTP_STATUS_CODE_101_STRING},   /**< 切换协议 */

    /*
     * 成功
     */
    {HHTTP_STATUS_CODE_200,HHTTP_STATUS_CODE_200_STRING},   /**< 成功 */
    {HHTTP_STATUS_CODE_201,HHTTP_STATUS_CODE_201_STRING},   /**< 已创建*/
    {HHTTP_STATUS_CODE_202,HHTTP_STATUS_CODE_202_STRING},   /**< 已接受 */
    {HHTTP_STATUS_CODE_203,HHTTP_STATUS_CODE_203_STRING},   /**< 没有认证信息 */
    {HHTTP_STATUS_CODE_204,HHTTP_STATUS_CODE_204_STRING},   /**< 没有内容 */
    {HHTTP_STATUS_CODE_205,HHTTP_STATUS_CODE_205_STRING},   /**< 重置链接 */
    {HHTTP_STATUS_CODE_206,HHTTP_STATUS_CODE_206_STRING},   /**< 部分内容 */

    /*
     * 重定向
     */
    {HHTTP_STATUS_CODE_300,HHTTP_STATUS_CODE_300_STRING},   /**< 多个选择 */
    {HHTTP_STATUS_CODE_301,HHTTP_STATUS_CODE_301_STRING},   /**< 永久移动 */
    {HHTTP_STATUS_CODE_302,HHTTP_STATUS_CODE_302_STRING},   /**< 已找到 */
    {HHTTP_STATUS_CODE_303,HHTTP_STATUS_CODE_303_STRING},   /**< 见其它 */
    {HHTTP_STATUS_CODE_304,HHTTP_STATUS_CODE_304_STRING},   /**< 未修改 */
    {HHTTP_STATUS_CODE_305,HHTTP_STATUS_CODE_305_STRING},   /**< 使用代理 */
    {HHTTP_STATUS_CODE_306,HHTTP_STATUS_CODE_306_STRING},   /**< (未使用) */
    {HHTTP_STATUS_CODE_307,HHTTP_STATUS_CODE_307_STRING},   /**< 临时重定向 */
    {HHTTP_STATUS_CODE_308,HHTTP_STATUS_CODE_308_STRING},   /**< 永久重定向 */

    /*
     * 客户端错误
     */
    {HHTTP_STATUS_CODE_400,HHTTP_STATUS_CODE_400_STRING},   /**< 请求错误 */
    {HHTTP_STATUS_CODE_401,HHTTP_STATUS_CODE_401_STRING},   /**< 未认证 */
    {HHTTP_STATUS_CODE_402,HHTTP_STATUS_CODE_402_STRING},   /**< 需要支付 */
    {HHTTP_STATUS_CODE_403,HHTTP_STATUS_CODE_403_STRING},   /**< 禁止访问 */
    {HHTTP_STATUS_CODE_404,HHTTP_STATUS_CODE_404_STRING},   /**< 未找到 */
    {HHTTP_STATUS_CODE_405,HHTTP_STATUS_CODE_405_STRING},   /**< 不允许的方法 */
    {HHTTP_STATUS_CODE_406,HHTTP_STATUS_CODE_406_STRING},   /**< 不可接受 */
    {HHTTP_STATUS_CODE_407,HHTTP_STATUS_CODE_407_STRING},   /**< 需要代理认证 */
    {HHTTP_STATUS_CODE_408,HHTTP_STATUS_CODE_408_STRING},   /**< 请求超时 */
    {HHTTP_STATUS_CODE_409,HHTTP_STATUS_CODE_409_STRING},   /**< 冲突 */
    {HHTTP_STATUS_CODE_410,HHTTP_STATUS_CODE_410_STRING},   /**< 已消失 */
    {HHTTP_STATUS_CODE_411,HHTTP_STATUS_CODE_411_STRING},   /**< 需要长度 */
    {HHTTP_STATUS_CODE_412,HHTTP_STATUS_CODE_412_STRING},   /**< 条件失败 */
    {HHTTP_STATUS_CODE_413,HHTTP_STATUS_CODE_413_STRING},   /**< 内容太长 */
    {HHTTP_STATUS_CODE_414,HHTTP_STATUS_CODE_414_STRING},   /**< URI太长 */
    {HHTTP_STATUS_CODE_415,HHTTP_STATUS_CODE_415_STRING},   /**< 不支持的媒体类型 */
    {HHTTP_STATUS_CODE_416,HHTTP_STATUS_CODE_416_STRING},   /**< 范围不合适 */
    {HHTTP_STATUS_CODE_417,HHTTP_STATUS_CODE_417_STRING},   /**< 预期结果失败 */
    {HHTTP_STATUS_CODE_418,HHTTP_STATUS_CODE_418_STRING},   /**< (未使用) */
    {HHTTP_STATUS_CODE_421,HHTTP_STATUS_CODE_421_STRING},   /**< 错误方向的请求 */
    {HHTTP_STATUS_CODE_422,HHTTP_STATUS_CODE_422_STRING},   /**< 无法处理的内容 */
    {HHTTP_STATUS_CODE_426,HHTTP_STATUS_CODE_426_STRING},   /**< 需要升级协议 */

    /*
     * 服务器错误
     */
    {HHTTP_STATUS_CODE_500,HHTTP_STATUS_CODE_500_STRING},   /**< 内部服务器错误 */
    {HHTTP_STATUS_CODE_501,HHTTP_STATUS_CODE_501_STRING},   /**< 未实现 */
    {HHTTP_STATUS_CODE_502,HHTTP_STATUS_CODE_502_STRING},   /**< 网关错误 */
    {HHTTP_STATUS_CODE_503,HHTTP_STATUS_CODE_503_STRING},   /**< 服务不可用 */
    {HHTTP_STATUS_CODE_504,HHTTP_STATUS_CODE_504_STRING},   /**< 网关超时 */
    {HHTTP_STATUS_CODE_505,HHTTP_STATUS_CODE_505_STRING},   /**< HTTP版本不支持 */

};

hhttp_string_t hhttp_status_code_std_to_string(hhttp_status_code_t status_code)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    for(size_t i=0; i<sizeof(hhttp_status_code_std_string_table)/sizeof(hhttp_status_code_std_string_table[0]); i++)
    {
        if(hhttp_status_code_std_string_table[i].status_code == status_code)
        {
            ret=hhttp_string_from_cstring(hhttp_status_code_std_string_table[i].status_code_c_string);
            break;
        }
    }
    return ret;
}

int hhttp_status_code_from_string(hhttp_string_t str)
{
    return hhttp_string_dec_to_uint64(str);
}

HDEFAULTS_RO_ATTRIBUTE
static const struct
{
    hhttp_status_code_t status_code;
    const char *        status_code_c_string;
}   hhttp_status_code_std_reason_string_table[]=
{
    /*
     * 信息 1XX
     */
    {HHTTP_STATUS_CODE_100,HHTTP_STATUS_CODE_100_REASON_STRING},    /**< 继续 */
    {HHTTP_STATUS_CODE_101,HHTTP_STATUS_CODE_101_REASON_STRING},    /**< 切换协议 */

    /*
     * 成功
     */
    {HHTTP_STATUS_CODE_200,HHTTP_STATUS_CODE_200_REASON_STRING},    /**< 成功 */
    {HHTTP_STATUS_CODE_201,HHTTP_STATUS_CODE_201_REASON_STRING},    /**< 已创建*/
    {HHTTP_STATUS_CODE_202,HHTTP_STATUS_CODE_202_REASON_STRING},    /**< 已接受 */
    {HHTTP_STATUS_CODE_203,HHTTP_STATUS_CODE_203_REASON_STRING},    /**< 没有认证信息 */
    {HHTTP_STATUS_CODE_204,HHTTP_STATUS_CODE_204_REASON_STRING},    /**< 没有内容 */
    {HHTTP_STATUS_CODE_205,HHTTP_STATUS_CODE_205_REASON_STRING},    /**< 重置链接 */
    {HHTTP_STATUS_CODE_206,HHTTP_STATUS_CODE_206_REASON_STRING},    /**< 部分内容 */

    /*
     * 重定向
     */
    {HHTTP_STATUS_CODE_300,HHTTP_STATUS_CODE_300_REASON_STRING},    /**< 多个选择 */
    {HHTTP_STATUS_CODE_301,HHTTP_STATUS_CODE_301_REASON_STRING},    /**< 永久移动 */
    {HHTTP_STATUS_CODE_302,HHTTP_STATUS_CODE_302_REASON_STRING},    /**< 已找到 */
    {HHTTP_STATUS_CODE_303,HHTTP_STATUS_CODE_303_REASON_STRING},    /**< 见其它 */
    {HHTTP_STATUS_CODE_304,HHTTP_STATUS_CODE_304_REASON_STRING},    /**< 未修改 */
    {HHTTP_STATUS_CODE_305,HHTTP_STATUS_CODE_305_REASON_STRING},    /**< 使用代理 */
    {HHTTP_STATUS_CODE_306,HHTTP_STATUS_CODE_306_REASON_STRING},    /**< (未使用) */
    {HHTTP_STATUS_CODE_307,HHTTP_STATUS_CODE_307_REASON_STRING},    /**< 临时重定向 */
    {HHTTP_STATUS_CODE_308,HHTTP_STATUS_CODE_308_REASON_STRING},    /**< 永久重定向 */

    /*
     * 客户端错误
     */
    {HHTTP_STATUS_CODE_400,HHTTP_STATUS_CODE_400_REASON_STRING},    /**< 请求错误 */
    {HHTTP_STATUS_CODE_401,HHTTP_STATUS_CODE_401_REASON_STRING},    /**< 未认证 */
    {HHTTP_STATUS_CODE_402,HHTTP_STATUS_CODE_402_REASON_STRING},    /**< 需要支付 */
    {HHTTP_STATUS_CODE_403,HHTTP_STATUS_CODE_403_REASON_STRING},    /**< 禁止访问 */
    {HHTTP_STATUS_CODE_404,HHTTP_STATUS_CODE_404_REASON_STRING},    /**< 未找到 */
    {HHTTP_STATUS_CODE_405,HHTTP_STATUS_CODE_405_REASON_STRING},    /**< 不允许的方法 */
    {HHTTP_STATUS_CODE_406,HHTTP_STATUS_CODE_406_REASON_STRING},    /**< 不可接受 */
    {HHTTP_STATUS_CODE_407,HHTTP_STATUS_CODE_407_REASON_STRING},    /**< 需要代理认证 */
    {HHTTP_STATUS_CODE_408,HHTTP_STATUS_CODE_408_REASON_STRING},    /**< 请求超时 */
    {HHTTP_STATUS_CODE_409,HHTTP_STATUS_CODE_409_REASON_STRING},    /**< 冲突 */
    {HHTTP_STATUS_CODE_410,HHTTP_STATUS_CODE_410_REASON_STRING},    /**< 已消失 */
    {HHTTP_STATUS_CODE_411,HHTTP_STATUS_CODE_411_REASON_STRING},    /**< 需要长度 */
    {HHTTP_STATUS_CODE_412,HHTTP_STATUS_CODE_412_REASON_STRING},    /**< 条件失败 */
    {HHTTP_STATUS_CODE_413,HHTTP_STATUS_CODE_413_REASON_STRING},    /**< 内容太长 */
    {HHTTP_STATUS_CODE_414,HHTTP_STATUS_CODE_414_REASON_STRING},    /**< URI太长 */
    {HHTTP_STATUS_CODE_415,HHTTP_STATUS_CODE_415_REASON_STRING},    /**< 不支持的媒体类型 */
    {HHTTP_STATUS_CODE_416,HHTTP_STATUS_CODE_416_REASON_STRING},    /**< 范围不合适 */
    {HHTTP_STATUS_CODE_417,HHTTP_STATUS_CODE_417_REASON_STRING},    /**< 预期结果失败 */
    {HHTTP_STATUS_CODE_418,HHTTP_STATUS_CODE_418_REASON_STRING},    /**< (未使用) */
    {HHTTP_STATUS_CODE_421,HHTTP_STATUS_CODE_421_REASON_STRING},    /**< 错误方向的请求 */
    {HHTTP_STATUS_CODE_422,HHTTP_STATUS_CODE_422_REASON_STRING},    /**< 无法处理的内容 */
    {HHTTP_STATUS_CODE_426,HHTTP_STATUS_CODE_426_REASON_STRING},    /**< 需要升级协议 */

    /*
     * 服务器错误
     */
    {HHTTP_STATUS_CODE_500,HHTTP_STATUS_CODE_500_REASON_STRING},    /**< 内部服务器错误 */
    {HHTTP_STATUS_CODE_501,HHTTP_STATUS_CODE_501_REASON_STRING},    /**< 未实现 */
    {HHTTP_STATUS_CODE_502,HHTTP_STATUS_CODE_502_REASON_STRING},    /**< 网关错误 */
    {HHTTP_STATUS_CODE_503,HHTTP_STATUS_CODE_503_REASON_STRING},    /**< 服务不可用 */
    {HHTTP_STATUS_CODE_504,HHTTP_STATUS_CODE_504_REASON_STRING},    /**< 网关超时 */
    {HHTTP_STATUS_CODE_505,HHTTP_STATUS_CODE_505_REASON_STRING},    /**< HTTP版本不支持 */

};

hhttp_string_t hhttp_status_code_std_to_reason_string(hhttp_status_code_t status_code)
{
    hhttp_string_t ret=hhttp_string_from_cstring("");
    for(size_t i=0; i<sizeof(hhttp_status_code_std_reason_string_table)/sizeof(hhttp_status_code_std_reason_string_table[0]); i++)
    {
        if(hhttp_status_code_std_reason_string_table[i].status_code == status_code)
        {
            ret=hhttp_string_from_cstring(hhttp_status_code_std_reason_string_table[i].status_code_c_string);
            break;
        }
    }
    return ret;
}

bool hhttp_status_line_parse(hhttp_status_line_t *status_line,hhttp_string_t line)
{
    bool ret=false;
    if(status_line == NULL || !hhttp_string_check_line(line))
    {
        return ret;
    }
    hhttp_string_t line_content=hhttp_string_line_get_content(line);

    /*
     * 去除末尾的\r\n
     */
    line_content.str_len -= 2;

    {
        hhttp_string_t first_space_string=hhttp_string_strstr(line_content,hhttp_string_from_cstring(" "));
        if(hhttp_string_is_empty(first_space_string))
        {
            return ret;
        }
        hhttp_string_t version_string;
        version_string.str=line_content.str;
        version_string.str_len=(((uintptr_t)first_space_string.str)-((uintptr_t)version_string.str));
        status_line->version=hhttp_version_from_string(version_string);

        line_content.str = &line_content.str[version_string.str_len+1];
        line_content.str_len -= (version_string.str_len+1);
    }

    {
        hhttp_string_t second_space_string=hhttp_string_strstr(line_content,hhttp_string_from_cstring(" "));
        if(hhttp_string_is_empty(second_space_string))
        {
            return ret;
        }
        hhttp_string_t status_string;
        status_string.str=line_content.str;
        status_string.str_len=(((uintptr_t)second_space_string.str)-((uintptr_t)line_content.str));
        status_line->status=hhttp_status_code_from_string(status_string);
        hhttp_string_t reason_string;
        reason_string.str=&line_content.str[status_string.str_len+1];
        reason_string.str_len=(line_content.str_len-(status_string.str_len+1));
        status_line->reason=reason_string;
    }

    if(status_line->status != 0 && status_line->version != HHTTP_VERSION_UNKNOWN)
    {
        ret=true;
    }

    return ret;

}


bool hhttp_status_line_pack(hhttp_status_line_t *status_line,hhttp_status_line_pack_write_callback_t cb,void *usr)
{
    bool ret=false;
    if(status_line == NULL )
    {
        return ret;
    }
    if(status_line->version == HHTTP_VERSION_UNKNOWN || status_line->status == 0)
    {
        return ret;
    }
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t space_string=hhttp_string_from_cstring(" ");
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);


    {
        /*
         * 版本
         */
        hhttp_string_t version_string=hhttp_version_std_to_string(status_line->version);
        if(hhttp_string_is_empty(version_string))
        {
            ret=false;
        }
        cb(usr,offset,version_string);
        offset += version_string.str_len;
    }

    /*
     * 空格
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    /*
     * 状态
     */
    bool is_status_code_std=true;
    {
        uint8_t buffer[8]= {0};
        hhttp_string_t status_string=hhttp_status_code_std_to_string(status_line->status);
        if(hhttp_string_is_empty(status_string))
        {
            /*
             * 非标状态码
             */
            status_string=hhttp_string_dec_from_uint64(buffer,sizeof(buffer)-1,(uint64_t)status_line->status);
            is_status_code_std=false;
        }
        cb(usr,offset,status_string);
        offset += status_string.str_len;
    }

    /*
     * 空格
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    {
        /*
         * 原因
         */
        hhttp_string_t reason_string=hhttp_status_code_std_to_reason_string(status_line->status);
        if(!is_status_code_std)
        {
            reason_string=status_line->reason;
        }
        cb(usr,offset,reason_string);
        offset += reason_string.str_len;
    }

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;

}

static bool hhttp_field_strip_ows(hhttp_string_t *str)
{
    bool ret=false;
    if(str==NULL || hhttp_string_is_empty(*str))
    {
        return ret;
    }
    while(str->str[str->str_len-1] == ' ' || str->str[str->str_len-1] == '\t')
    {
        str->str_len--;
    }
    while(str->str[0] == ' ' || str->str[0] == '\t')
    {
        str->str++;
        str->str_len--;
    }
    ret=true;
    return ret;
}

bool hhttp_field_parse(hhttp_field_t *field_line,hhttp_string_t line)
{
    bool ret=false;
    if(field_line == NULL || !hhttp_string_check_line(line))
    {
        return ret;
    }

    /*
     * 注意：不支持已被弃用的折叠行写法
     */
    hhttp_string_t line_content=hhttp_string_line_get_content(line);

    /*
     * 去除末尾的\r\n
     */
    line_content.str_len -= 2;

    hhttp_string_t colon_string=hhttp_string_strstr(line_content,hhttp_string_from_cstring(":"));
    if(hhttp_string_is_empty(colon_string))
    {
        return ret;
    }

    field_line->field_name.str=&line_content.str[0];
    field_line->field_name.str_len= (((uintptr_t)colon_string.str)-((uintptr_t)line_content.str));
    field_line->field_values.str=&line_content.str[field_line->field_name.str_len+1];
    field_line->field_values.str_len=line_content.str_len-(field_line->field_name.str_len+1);

    /*
     * 去除首尾的可选空白
     */
    if(!hhttp_field_strip_ows(&field_line->field_values))
    {
        return ret;
    }

    ret=true;
    return ret;
}

bool hhttp_field_line_pack(hhttp_field_t *field_line,hhttp_field_line_pack_write_callback_t cb,void *usr)
{
    bool ret=false;
    if(field_line == NULL )
    {
        return ret;
    }
    if(hhttp_string_is_empty(field_line->field_name)|| hhttp_string_is_empty(field_line->field_values))
    {
        return ret;
    }
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t space_string=hhttp_string_from_cstring(" ");
    hhttp_string_t colon_string=hhttp_string_from_cstring(":");
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);

    /*
     * 名称
     */
    cb(usr,offset,field_line->field_name);
    offset += field_line->field_name.str_len;

    /*
     * 冒号
     */
    cb(usr,offset,colon_string);
    offset += colon_string.str_len;

    /*
     * 空格（可选）
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    /*
     * 值
     */
    cb(usr,offset,field_line->field_values);
    offset += field_line->field_values.str_len;

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;
}

bool hhttp_field_basic_authorization_pack(hhttp_field_basic_authorization_t *basic_authorization,hhttp_field_line_pack_write_callback_t cb,void *usr)
{
    bool ret=false;
    if(basic_authorization == NULL )
    {
        return ret;
    }
    if(hhttp_string_is_empty(basic_authorization->userinfo))
    {
        return ret;
    }
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t space_string=hhttp_string_from_cstring(" ");
    hhttp_string_t colon_string=hhttp_string_from_cstring(":");
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);

    /*
     * 名称
     */
    {
        hhttp_string_t name_string=hhttp_string_from_cstring("Authorization");
        cb(usr,offset,name_string);
        offset += name_string.str_len;
    }

    /*
     * 冒号
     */
    cb(usr,offset,colon_string);
    offset += colon_string.str_len;

    /*
     * 空格（可选）
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    /*
     * Basic
     */
    {
        hhttp_string_t name_string=hhttp_string_from_cstring("Basic");
        cb(usr,offset,name_string);
        offset += name_string.str_len;
    }

    /*
     * 空格
     */
    cb(usr,offset,space_string);
    offset += space_string.str_len;

    /*
     * 值
     */
    {

        for(size_t i=0; i<basic_authorization->userinfo.str_len; i+=3)
        {
            size_t len=3;
            if(len+i > basic_authorization->userinfo.str_len)
            {
                len=basic_authorization->userinfo.str_len-i;
            }
            char buffer[8]= {0};
            hbase64_encode_with_null_terminator(buffer,sizeof(buffer)-1,(uint8_t *)&basic_authorization->userinfo.str[i],len);
            hhttp_string_t part_base64_string=hhttp_string_from_cstring(buffer);
            cb(usr,offset,part_base64_string);
            offset += part_base64_string.str_len;
        }
    }

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;
}

bool hhttp_body_is_begin(hhttp_string_t line)
{
    return hhttp_string_line_content_is_empty(hhttp_string_line_get_content(line));
}

bool hhttp_body_normal_parse_init(hhttp_body_normal_parse_t *body_normal,hhttp_body_normal_parse_data_t data,size_t max_length,void *usr)
{
    bool ret=false;
    if(body_normal==NULL || data == NULL)
    {
        return ret;
    }
    ret=true;
    body_normal->data=data;
    body_normal->offset=0;
    body_normal->max_length=max_length;
    body_normal->usr=usr;
    memset(&body_normal->flags,0,sizeof(body_normal->flags));
    return ret;
}

bool hhttp_body_normal_parse_is_end(hhttp_body_normal_parse_t *body_normal)
{
    bool ret=false;
    if(body_normal == NULL)
    {
        return ret;
    }

    if((body_normal->max_length != (HHTTP_BODY_NORMAL_MAX_LENGTH_UNDEFINED)) && (body_normal->offset >= body_normal->max_length))
    {
        ret=true;
    }

    return ret;
}

bool hhttp_body_normal_parse_input(hhttp_body_normal_parse_t *body_normal,const uint8_t *data,size_t *data_length)
{
    bool ret=false;
    if(body_normal == NULL || body_normal->data == NULL)
    {
        return ret;
    }
    if(data == NULL || data_length == NULL)
    {
        return ret;
    }
    hhttp_string_t data_string=hhttp_string_from_buffer(data,(*data_length));
    if((body_normal->offset+data_string.str_len > body_normal->max_length) && (body_normal->max_length != HHTTP_BODY_NORMAL_MAX_LENGTH_UNDEFINED))
    {
        data_string.str_len = body_normal->max_length-body_normal->offset;
    }

    body_normal->data(body_normal,body_normal->offset,data_string);

    (*data_length)=data_string.str_len;

    body_normal->offset += data_string.str_len;

    ret=true;

    if(hhttp_body_normal_parse_is_end(body_normal) && body_normal->flags.data_end_call==0)
    {
        /*
         * 触发一个空的调用
         */
        uint8_t buffer[4]= {0};
        size_t  buffer_length=0;
        body_normal->data(body_normal,body_normal->offset,hhttp_string_from_buffer(buffer,buffer_length));
        body_normal->flags.data_end_call=1;
    }

    return ret;
}

bool hhttp_body_normal_parse_input_end(hhttp_body_normal_parse_t *body_normal)
{
    bool ret=false;
    if(body_normal == NULL || body_normal->data == NULL)
    {
        return ret;
    }

    body_normal->max_length=body_normal->offset;

    ret=true;

    {
        /*
         * 触发一个空的调用
         */
        uint8_t buffer[4]= {0};
        size_t  buffer_length=0;
        ret=hhttp_body_normal_parse_input(body_normal,buffer,&buffer_length);
    }


    return ret;
}

bool hhttp_body_chunked_parse_init(hhttp_body_chunked_parse_t *body_chunked,hhttp_body_chunked_parse_data_t data,void *usr)
{
    bool ret=false;
    if(body_chunked==NULL || data == NULL)
    {
        return ret;
    }
    memset(body_chunked,0,sizeof(*body_chunked));
    body_chunked->data=data;
    body_chunked->usr=usr;
    return ret;
}

bool hhttp_body_chunked_parse_is_end(hhttp_body_chunked_parse_t *body_chunked)
{
    bool ret=false;
    if(body_chunked==NULL)
    {
        return ret;
    }

    ret=body_chunked->flags.data_end!=0;

    return ret;
}

bool hhttp_body_chunked_parse_input(hhttp_body_chunked_parse_t *body_chunked,const uint8_t *data,size_t *data_length)
{
    bool ret=false;
    if(body_chunked == NULL || body_chunked->data == NULL)
    {
        return ret;
    }
    if(data == NULL || data_length == NULL)
    {
        return ret;
    }
    if(hhttp_body_chunked_parse_is_end(body_chunked))
    {
        /*
         * 数据已结束，不再处理输入数据
         */
        (*data_length)=0;
    }

    if(body_chunked->flags.l_d == 0)
    {
        /*
         * 处理长度
         */
        size_t i=0;
        for(i=0; i<(*data_length); i++)
        {
            if(body_chunked->single_offset >= sizeof(body_chunked->length_string_buffer)-1)
            {
                (*data_length)=i;
                return ret;
            }
            body_chunked->length_string_buffer[body_chunked->single_offset]=data[i];
            body_chunked->single_offset++;
            if(data[i]=='\n')
            {
                hhttp_string_t line=hhttp_string_from_buffer(body_chunked->length_string_buffer,body_chunked->single_offset);
                if(!hhttp_string_is_empty(line))
                {
                    /*
                     * 切换至数据处理
                     */
                    body_chunked->flags.l_d = 1;
                    body_chunked->single_length=hhttp_string_hex_to_uint64(line);
                    body_chunked->single_offset=0;
                    i++;
                    break;
                }
            }
        }
        (*data_length)=i;
    }
    else
    {
        /*
         * 处理数据
         */
        if(body_chunked->single_offset < body_chunked->single_length)
        {
            size_t single_data_length=(*data_length);
            if(single_data_length != 0)
            {
                if(body_chunked->single_offset+single_data_length <= body_chunked->single_length)
                {
                    hhttp_string_t data_string=hhttp_string_from_buffer(data,single_data_length);
                    body_chunked->data(body_chunked,body_chunked->offset,data_string);
                    body_chunked->offset += data_string.str_len;
                    body_chunked->single_offset += data_string.str_len;
                }
                else
                {
                    single_data_length=body_chunked->single_length - body_chunked->single_offset;
                    hhttp_string_t data_string=hhttp_string_from_buffer(data,single_data_length);
                    body_chunked->data(body_chunked,body_chunked->offset,data_string);
                    body_chunked->offset += data_string.str_len;
                    body_chunked->single_offset += data_string.str_len;
                    (*data_length)=single_data_length;
                }
            }
        }
        else
        {
            size_t single_data_length=(*data_length);
            while(single_data_length!=0)
            {
                /*
                 * 检测行结束
                 */
                if((body_chunked->single_offset - body_chunked->single_length) == 0)
                {
                    /*
                     * \r字符
                     */
                    if(data[body_chunked->single_offset - body_chunked->single_length]!='\r')
                    {
                        (*data_length)=(*data_length)-single_data_length;
                        /*
                         * 已结束
                         */
                        body_chunked->flags.data_end=1;
                        return ret;
                    }

                    single_data_length--;
                    body_chunked->single_offset++;
                }

                /*
                 * 检测行结束
                 */
                if((body_chunked->single_offset - body_chunked->single_length) == 1)
                {
                    /*
                     * \n字符
                     */
                    if(data[body_chunked->single_offset - body_chunked->single_length]!='\n')
                    {
                        (*data_length)=(*data_length)-single_data_length;
                        /*
                         * 已结束
                         */
                        body_chunked->flags.data_end=1;
                        return ret;
                    }

                    single_data_length--;
                    body_chunked->single_offset++;
                }



                if(body_chunked->single_offset >= body_chunked->single_length+2)
                {
                    /*
                     * 切换到长度解析
                     */
                    body_chunked->flags.l_d = 0;
                    if(body_chunked->single_length == 0)
                    {
                        /*
                         * 检测到结束
                         */
                        body_chunked->flags.data_end=1;
                    }
                    body_chunked->single_length=0;
                    body_chunked->single_offset=0;

                    (*data_length)=(*data_length)-single_data_length;

                    break;
                }
            }
        }
    }

    ret = true;

    if(hhttp_body_chunked_parse_is_end(body_chunked) && body_chunked->flags.data_end_call == 0)
    {
        /*
         * 触发一个空的调用
         */
        body_chunked->flags.data_end_call=1;
        body_chunked->data(body_chunked,body_chunked->offset,hhttp_string_from_cstring(""));
    }

    return ret;
}

bool hhttp_body_begin(hhttp_body_write_callback_t cb,void *usr)
{
    bool ret=false;
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;
}

bool hhttp_body_chunked_pack(hhttp_body_write_callback_t cb,void *usr,hhttp_string_t data)
{
    bool ret=false;
    if(hhttp_string_is_empty(data))
    {
        data=hhttp_string_from_cstring("");
    }
    if(cb == NULL)
    {
        return ret;
    }
    ret=true;
    size_t offset=0;
    hhttp_string_t line_end_string=hhttp_string_from_cstring(HHTTP_LINE_END_STRING);

    {
        /*
         * 输出长度
         */
        uint8_t buffer[36]= {0};
        hhttp_string_t length_string=hhttp_string_hex_from_uint64(buffer,sizeof(buffer)-1,data.str_len);
        cb(usr,offset,length_string);
        offset += length_string.str_len;
    }

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;

    /*
     * 数据
     */
    cb(usr,offset,data);
    offset += data.str_len;

    /*
     * 行结束
     */
    cb(usr,offset,line_end_string);
    offset += line_end_string.str_len;


    return ret;
}

bool hhttp_body_chunked_end(hhttp_body_write_callback_t cb,void *usr)
{
    return hhttp_body_chunked_pack(cb,usr,hhttp_string_from_cstring(""));
}

