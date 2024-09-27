/***************************************************************
 * Name:      h3rdparty.h
 * Purpose:   引入第三方头文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_H__
#define __H3RDPARTY_H__

/*
 *  此模块引入第三方库
 */

/*
 * 引入cJSON库
 */
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
#include "3rdparty/cJSON/cJSON.h"
/*
 * 如需cJSON_Utils请单独添加cJSON_Utils.h与cJSON_Utils.c
 */
//#include "3rdparty/cJSON/cJSON_Utils.h"
#else
#include "cJSON.h"
#include "cJSON_Utils.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 初始化第三方库
 *
 *
 */
void h3rdparty_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __H3RDPARTY_H__


