/***************************************************************
 * Name:      hputchar.h
 * Purpose:   声明hputchar接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HPUTCHAR_H__
#define __HPUTCHAR_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 将字符写入标准输出中
 *
 * \param ch int 字符
 * \return int 出错时返回EOF（-1）
 *
 */
int hputchar(int ch);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HPUTCHAR_H__
