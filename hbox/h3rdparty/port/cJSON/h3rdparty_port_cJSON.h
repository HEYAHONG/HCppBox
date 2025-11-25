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
 * 引入cJSON库
 */
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
/*
 * 隐藏cJSON符号
 */
#define  CJSON_HIDE_SYMBOLS 1
#include "h3rdparty/3rdparty/cJSON/cJSON.h"
#include "h3rdparty/3rdparty/cJSON/cJSON_Utils.h"
#else
#include "cJSON.h"
#include "cJSON_Utils.h"
#endif



#endif // __H3RDPARTY_PORT_CJSON_H_INCLUDE__
