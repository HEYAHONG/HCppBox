/***************************************************************
 * Name:      hlibfdt.h
 * Purpose:   声明hlibfdt接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLIBFDT_H_INCLUDE__
#define __HLIBFDT_H_INCLUDE__
#include "h3rdparty_port_libfdt.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 检查是否为合法的fdt
 *
 * \param fdt void* fdt指针
 * \return bool 是否为合法的fdt
 *
 */
bool hlibfdt_check(const void *fdt);

/** \brief 遍历节点回调
 *
 * \param fdt const void* fdt指针
 * \param offset int 节点偏移
 * \param name const char* 节点名称
 * \param depth int 深度，从1开始
 * \param usr void * 用户参数
 *
 */
typedef void (*hlibfdt_traverse_on_node_t)(const void *fdt,int offset,const char *name,int depth,void *usr);


/** \brief 遍历节点
 *
 * \param fdt const void* fdt指针
 * \param on_node hlibfdt_traverse_on_node_t 遍历节点回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hlibfdt_traverse_node(const void *fdt,hlibfdt_traverse_on_node_t on_node,void *usr);


/** \brief 遍历节点属性回调
 *
 * \param fdt const void* fdt指针
 * \param offset int 节点属性偏移
 * \param name const char* 属性名称
 * \param value onst uint8_t * 属性值
 * \param value_length size_t 属性值长度
 * \param usr void * 用户参数
 *
 */
typedef void (*hlibfdt_traverse_on_node_property_t)(const void *fdt,int offset,const char *name,const uint8_t *value,size_t value_length,void *usr);

/** \brief 遍历节点属性
 *
 * \param fdt const void* fdt指针
 * \param node_offset int 节点偏移
 * \param on_property hlibfdt_traverse_on_node_property_t 遍历节点属性回调
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
bool hlibfdt_traverse_node_property(const void *fdt,int node_offset,hlibfdt_traverse_on_node_property_t on_property,void *usr);


/** \brief 获取节点的phandle
 *
 * \param fdt const void* fdt指针
 * \param node_offset int 节点偏移
 * \return uint32_t phandle值，为0或者-1(0xFFFFFFFF)表示出错
 *
 */
uint32_t hlibfdt_get_phandle(const void *fdt, int node_offset);


/** \brief 通过phandle获取节点偏移
 *
 * \param fdt const void* fdt指针
 * \param phandle uint32_t phandle值
 * \return int 节点偏移，大于等于0时为成功
 *
 */
int fdt_node_offset_by_phandle(const void *fdt, uint32_t phandle);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLIBFDT_H_INCLUDE__
