/***************************************************************
 * Name:      hunsetenv.h
 * Purpose:   声明hunsetenv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-08
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUNSETENV_H__
#define __HUNSETENV_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 删除环境变量
 *
 * \param envname const char* 环境变量名称（不得含有=号）
 * \return int                成功返回0,失败返回-1
 *
 */
int hunsetenv(const char *envname);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUNSETENV_H__
