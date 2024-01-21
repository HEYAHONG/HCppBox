/***************************************************************
 * Name:      hcpprt.h
 * Purpose:   hcpprt接口,实现常用的C++移植。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef HCPPRT_H
#define HCPPRT_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief  hcpprt初始化，若环境不支持进行一些初始化操作,可调用此函数初始化。
 *
 *
 */
void hcpprt_init();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HCPPRT_H
