/***************************************************************
 * Name:      hgetenv.h
 * Purpose:   声明hgetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-04
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGETENV_H__
#define __HGETENV_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 获取环境变量
 *
 * \param name const char* 环境变量名称
 * \return char* 环境变量值，失败返回NULL
 *
 */
char * hgetenv(const char *name);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HGETENV_H__
