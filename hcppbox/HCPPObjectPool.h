/***************************************************************
 * Name:      HCPPObjectPool.h
 * Purpose:   HCPPObjectPool,提供一个全局的对象池，提供字符串与HCPPObject*指针之间的转换。\
                                     其相对于一般的对象池的特点为当HCPPObject*指针所指对象被delete后，将自动从对象池移除其指针。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPOBJECTPOOL_H__
#define __HCPPOBJECTPOOL_H__
#ifdef __cplusplus
#include "HCPPObject.h"
#include <string>
#include <vector>
#include <functional>

/** \brief 将HCPPObject对象指针放入对象池
 *
 * \param id std::string id标识，必须具有唯一性（由用户自行确保）
 * \param obj HCPPObject* HCPPObject对象指针
 *
 */
void HCPPObjectPool_Set(std::string id,HCPPObject *obj);

/** \brief 从对象池获取HCPPObject对象指针
 *
 * \param id std::string id标识，必须具有唯一性（由用户自行确保）
 * \return HCPPObject* HCPPObject对象指针
 *
 */
HCPPObject *HCPPObjectPool_Get(std::string id);

/** \brief id子字符串列表转化为id字符串,将忽略空id子字符串。
 *
 * \param id_List std::vector<std::string> id子字符串列表
 * \return std::string id字符串(用.连接)
 *
 */
std::string HCPPObjectPool_Id_Helper(std::vector<std::string> id_list);

/** \brief id字符串转化为id子字符串列表
 *
 * \param id std::string id字符串
 * \return std::vector<std::string> id子字符串列表(用.分割)
 *
 */
std::vector<std::string> HCPPObjectPool_Id_Helper(std::string id);

/** \brief 对象池枚举
 *
 * \param on_enum std::function<void(std::string,HCPPObject *)>,枚举回调，第一个参数为id字符串，第二个参数为HCPPObject对象指针
 *
 */
void HCPPObjectPool_Enum(std::function<void(std::string,HCPPObject *)> on_enum);

#endif // __cplusplus
#endif // CPPOBJECTPOOL_H
