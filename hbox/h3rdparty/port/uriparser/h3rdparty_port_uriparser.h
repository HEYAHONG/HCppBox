/***************************************************************
 * Name:      h3rdparty_port_uriparser.h
 * Purpose:   声明h3rdparty接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __H3RDPARTY_PORT_URIPARSER_H_INCLUDE__
#define __H3RDPARTY_PORT_URIPARSER_H_INCLUDE__

/*
 * 引入uriparser库
 */
#ifndef H3RDPARTY_USING_SYSTEM_URIPARSER
/*
 * 隐藏uriparser符号
 */
#define  URI_STATIC_BUILD  1
#ifndef  H3RDPARTY_URIPARSER_URI_HEADER
#define  H3RDPARTY_URIPARSER_URI_HEADER   "h3rdparty/3rdparty/uriparser/include/uriparser/Uri.h"
#endif
#else
#ifndef  H3RDPARTY_URIPARSER_URI_HEADER
#define  H3RDPARTY_URIPARSER_URI_HEADER   "uriparser/Uri.h"
#endif
#endif



#endif // __H3RDPARTY_PORT_URIPARSER_H_INCLUDE__
