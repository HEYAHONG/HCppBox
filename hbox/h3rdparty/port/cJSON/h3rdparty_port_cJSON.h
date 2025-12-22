/***************************************************************
 * Name:      h3rdparty_port_cJSON.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-09-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_CJSON_H_INCLUDE__
#define __H3RDPARTY_PORT_CJSON_H_INCLUDE__

/*
 * cJSON库
 */
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
#define  H3RDPARTY_CJSON_CJSON_HEADER       "h3rdparty/3rdparty/cJSON/cJSON.h"
#define  H3RDPARTY_CJSON_CJSONUTILS_HEADER  "h3rdparty/3rdparty/cJSON/cJSON_Utils.h"
#else
#define  H3RDPARTY_CJSON_CJSON_HEADER        "cJSON.h"
#define  H3RDPARTY_CJSON_CJSONUTILS_HEADER   "cJSON_Utils.h"
#endif


/*
 * 引入hcjson头文件(符合hbox命名风格)
 */
#include "hcjson.h"

#endif // __H3RDPARTY_PORT_CJSON_H_INCLUDE__
