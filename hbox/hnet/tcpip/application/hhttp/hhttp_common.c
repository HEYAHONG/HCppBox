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
    int ret=0;
    if(str.str!=NULL && str.str_len!=0)
    {
        for(size_t i=0; i<str.str_len; i++)
        {
            if(str.str[i] >= '0' && str.str[i] <= '9')
            {
                ret*=10;
                ret+=(str.str[i]-'0');
            }
        }
    }
    return ret;
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


