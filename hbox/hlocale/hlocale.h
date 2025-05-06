/***************************************************************
 * Name:      hlocale.h
 * Purpose:   声明hlocale接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLOCALE_H__
#define __HLOCALE_H__
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 获取locale设置
 *          Windows系统下通过GetACP获取代码页。
 *          Unix系统/类Unix系统通过环境变量获取。
 *          其它条件下，参考Unix系统/类Unix系统，环境变量需要用户自行实现。
 *
 * \return const char* locale字符串
 *
 */
const char *hlocale_locale_get(void);


/** \brief 字符集是否是UTF-8
 *
 * \return bool 是否为UTF-8
 *
 */
bool hlocale_charset_is_utf8(void);

/** \brief 字符集是否是GB2312
 * 注意：虽然不严谨,GBK也视作GB2312
 *
 * \return bool 是否为GB2312
 *
 */
bool hlocale_charset_is_gb2312(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLOCALE_H__
