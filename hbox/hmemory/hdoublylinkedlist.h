/***************************************************************
 * Name:      hdoublylinkedlist.h
 * Purpose:   声明hdoublylinkedlist接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDOUBLYLINKEDLIST_H__
#define __HDOUBLYLINKEDLIST_H__
/*
 * 导入hobject文件（主要使用GET_STRUCT_PTR_BY_MEMBER_PTR宏定义）
 */
#include "hobject.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hdoublylinkedlist_head hdoublylinkedlist_head_t;
struct hdoublylinkedlist_head
{
    hdoublylinkedlist_head_t *next;                          /**< 下一个节点  */
    hdoublylinkedlist_head_t *prev;                          /**< 上一个节点*/
};

/** \brief 双向链表初始化
 *
 * \param list_head hdoublylinkedlist_head_t* 双向链表头
 *
 */
void hdoublylinkedlist_init(hdoublylinkedlist_head_t *list_head);

/** \brief 链表是否为空
 *
 * \param item hdoublylinkedlist_head_t* 链表项
 * \return bool 是否为空项,链表项为空时返回true,链表项不为空时一般不可进行内存释放
 *
 */
bool hdoublylinkedlist_is_empty(hdoublylinkedlist_head_t* item);

/** \brief 循环遍历双向链表(过程中不可移除链表中的项)
 *
 * \param list_head         双向链表头
 * \param item              遍历所用的变量名称
 *
 */
#define HDOUBLYLINKEDLIST_FOREACH(list_head,item)      \
    for(hdoublylinkedlist_head_t * item=list_head;item!=NULL; (item->next!=list_head)?(item=item->next):(item=NULL))

/** \brief 双向链表枚举回调
 *
 * \param item hdoublylinkedlist_head_t*    双向链表头
 * \param usr void*                         用户参数
 *
 */
typedef void (*hdoublylinkedlist_on_enum_callback_t)(hdoublylinkedlist_head_t *item,void *usr);

/** \brief 双向链表枚举
 *
 * \param list_head hdoublylinkedlist_head_t*     双向链表头
 * \param cb hdoublylinkedlist_on_enum_callback_t 双向链表枚举回调
 * \param usr void*                               用户参数
 *
 */
void hdoublylinkedlist_enum(hdoublylinkedlist_head_t *list_head,hdoublylinkedlist_on_enum_callback_t cb,void *usr);


/** \brief 在当前项前面添加项
 *
 * \param item hdoublylinkedlist_head_t*      原链表中的项（一般使用双向链表头）
 * \param new_item hdoublylinkedlist_head_t*  新项
 *
 */
void hdoublylinkedlist_add_front(hdoublylinkedlist_head_t *item,hdoublylinkedlist_head_t *new_item);


/** \brief 在当前项后面添加项
 *
 * \param item hdoublylinkedlist_head_t*      原链表中的项（一般使用双向链表头）
 * \param new_item hdoublylinkedlist_head_t*  新项
 *
 */
void hdoublylinkedlist_add_back(hdoublylinkedlist_head_t *item,hdoublylinkedlist_head_t *new_item);


/** \brief 将当前项从链表中移除
 * 注意：遍历过程中不可移除项。本操作也不会释放内存。
 *
 * \param item hdoublylinkedlist_head_t* 原链表中的项
 *
 */
void hdoublylinkedlist_remove(hdoublylinkedlist_head_t *item);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDOUBLYLINKEDLIST_H__
