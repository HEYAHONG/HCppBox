#ifndef CPPOBJECTPOOL_H
#define CPPOBJECTPOOL_H
#ifdef __cplusplus
#include "CPPObject.h"
#include <string>

/** \brief 将CPPObject对象指针放入对象池
 *
 * \param id std::string id标识，必须具有唯一性（由用户自行确保）
 * \param obj CPPObject::CPPObject* CPPObject对象指针
 *
 */
void CPPObjectPool_Set(std::string id,CPPObject::CPPObject *obj);

/** \brief 从对象池获取CPPObject对象指针
 *
 * \param id std::string id标识，必须具有唯一性（由用户自行确保）
 * \return CPPObject::CPPObject* CPPObject对象指针
 *
 */
CPPObject::CPPObject *CPPObjectPool_Get(std::string id);

#endif // __cplusplus
#endif // CPPOBJECTPOOL_H
