#ifndef __RC_H__
#define __RC_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif

/** \brief 通过名称获取资源大小
 *
 * \param name const char* 名称，详见编译时的列表
 * \return size_t 资源大小，失败返回0
 *
 */
size_t RCGetSize(const char * name);

/** \brief 通过名称获取资源指针
 *
 * \param name const char* 详见编译时的列表
 * \return const unsigned char* 资源指针，失败返回NULL
 *
 */
const unsigned char * RCGetHandle(const char * name);

/** \brief 资源枚举回调
 *
 * \param Name const unsigned char 资源名称指针
 * \param NameLength size_t 名称长度
 * \param Resource const unsigned char 资源指针
 * \param ResourceLength size_t
 *
 */
typedef void (*RCEnumCallback_t)(const unsigned char *Name,size_t NameLength,const unsigned char *Resource,size_t ResourceLength);

/** \brief 资源枚举
 *
 * \param cb RCEnumCallback_t 资源枚举回调
 *
 */
void RCEnum(RCEnumCallback_t cb);

#ifdef __cplusplus
};
#endif
#endif // __RC_H__
