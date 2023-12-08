#ifndef HCPPOBJECTPOOL_H
#define HCPPOBJECTPOOL_H
#ifdef __cplusplus
#include "HCPPObject.h"
#include <string>

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

#endif // __cplusplus
#endif // CPPOBJECTPOOL_H
