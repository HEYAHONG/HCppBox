/***************************************************************
 * Name:      hsetenv.h
 * Purpose:   声明hsetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-08
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSETENV_H__
#define __HSETENV_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 设置环境变量
 *
 * \param envname const char* 环境变量名称（不得含有=号）
 * \param envval const char*  环境变量值
 * \param overwrite int       是否覆盖，当为0时，若环境变量存在则直接返回成功而不操作
 * \return int                成功返回0,失败返回-1
 *
 */
int hsetenv(const char *envname, const char *envval, int overwrite);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSETENV_H__
