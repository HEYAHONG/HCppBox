/***************************************************************
 * Name:      hgetchar.h
 * Purpose:   声明hgetchar接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGETCHAR_H__
#define __HGETCHAR_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 从标准输入中读取字符
 *
 * \return int 字符，出错时返回EOF（-1）
 *
 */
int hgetchar(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HGETCHAR_H__
