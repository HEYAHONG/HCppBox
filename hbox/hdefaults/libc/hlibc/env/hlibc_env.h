/***************************************************************
 * Name:      hlibc_env.h
 * Purpose:   声明hlibc_env接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLIBC_ENV_H__
#define __HLIBC_ENV_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 环境变量初始化
 *
 *
 */
void hlibc_env_init(void);

/** \brief 获取环境变量
 *
 * \param name const char* 环境变量名称
 * \return char* 环境变量值，失败返回NULL
 *
 */
char * hlibc_env_getenv(const char *name);

/** \brief 删除环境变量
 *
 * \param envname const char* 环境变量名称（不得含有=号）
 * \return int                成功返回0,失败返回-1
 *
 */
int hlibc_env_unsetenv(const char *envname);


/** \brief 设置环境变量
 *
 * \param envname const char* 环境变量名称（不得含有=号）
 * \param envval const char*  环境变量值
 * \param overwrite int       是否覆盖，当为0时，若环境变量存在则直接返回成功而不操作
 * \return int                成功返回0,失败返回-1
 *
 */
int hlibc_env_setenv(const char *envname, const char *envval, int overwrite);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLIBC_ENV_H__
