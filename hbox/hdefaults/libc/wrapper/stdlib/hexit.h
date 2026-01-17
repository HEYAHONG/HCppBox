/***************************************************************
 * Name:      hexit.h
 * Purpose:   声明hexit接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HEXIT_H__
#define __HEXIT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 在主线程退出程序（将执行清理代码）
 *
 * \param exit_code int 退出代码
 *
 */
void hexit(int exit_code);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HEXIT_H__
