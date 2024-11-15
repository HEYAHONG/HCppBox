/***************************************************************
 * Name:      HCPPInit.h
 * Purpose:   HCPPInit对外接口。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPINIT_H__
#define __HCPPINIT_H__
/*
此文件的目的主要是减少耦合，如根据需要直接增加/减少源代码文件来增加/减少组件而不影响编译。
理论上可通过全局变量的构造函数实现此目的，当增加源代码时，全局构造函数可调用注册函数将将该源代码中的组件注册在程序中。
在rt-thread中，通过连接脚本的段(section)实现类似功能。
*/
#ifdef __cplusplus
#include  <string>
#include  <functional>
#include <map>
#include <queue>
#include <mutex>
#endif
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 执行初始化。  注意：此函数必须在适当位置（如main函数中当各组件都初始化完成后，再执行动态的组件加载）被调用，否则此组件不起作用。
 *
 *
 */
void HCPPInit(void);

/** \brief 注册初始化函数，通常在不确定是否所有组件都初始化完成时调用(如全局变量的构造函数中)。
 *
 * \param tag const char * 标签，初始化的顺序由标签决定
 * \param init_fn void (*)(void)) 初始化函数
 *
 */
void HCPPInitRegister(const char * tag,void (*init_fn)(void));


#ifdef __cplusplus
}
#endif // __cplusplus


#ifdef __cplusplus
/** \brief 注册初始化函数，通常在不确定是否所有组件都初始化完成时调用(如全局变量的构造函数中)。
 *
 * \param tag std::string 标签，初始化的顺序由标签决定
 * \param init_fn std::function<void (void)> 初始化函数
 *
 */
void HCPPInitRegister(std::string tag,std::function<void (void)> init_fn);
#endif


#endif // __HCPPINIT_H__
