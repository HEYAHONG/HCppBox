/***************************************************************
 * Name:      hrbtree.h
 * Purpose:   声明hrbtree接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-29
 * Generated: 由 DeepSeek V4.1 Flash 1017 生成
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HRBTREE_H__
#define __HRBTREE_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    HRBTREE_FLAGS_COLOR_RED=0,                                    /**< 红色节点 */
    HRBTREE_FLAGS_COLOR_BLACK=1                                   /**< 黑色节点 */
} hrbtree_color_t;

struct hrbtree_node;
typedef struct hrbtree_node hrbtree_node_t;
struct hrbtree_node
{
    hrbtree_node_t *parent;                                 /**< 父节点 */
    hrbtree_node_t *left;                                   /**< 左子节点 */
    hrbtree_node_t *right;                                  /**< 右子节点 */
    uintptr_t       value;                                  /**< 节点值,用于比较节点大小,由用户在初始化时写入 */
    union
    {
        uint32_t flags;                                     /**< 32位标志,低1位为节点颜色,其余31位由用户使用 */
        struct
        {
            uint32_t color:1;                               /**< 节点颜色,占用flags低1位 */
            uint32_t user_flags:31;                         /**< 用户标志,占用flags其余31位 */
        };
    };
};

struct hrbtree;
typedef struct hrbtree hrbtree_t;
struct hrbtree
{
    hrbtree_node_t *root;                                   /**< 根节点,空树时指向守卫节点 */
    hrbtree_node_t  nil;                                    /**< 守卫节点,代替NULL,减少算法中的判空操作 */
    size_t          node_count;                             /**< 当前节点总数 */
};

/** \brief 红黑树初始化
 *
 * \param tree hrbtree_t* 红黑树
 *
 */
void hrbtree_init(hrbtree_t *tree);

/** \brief 红黑树节点初始化
 *
 * \param node hrbtree_node_t* 红黑树节点
 * \param value uintptr_t 节点值,用于比较节点大小
 *
 */
void hrbtree_node_init(hrbtree_node_t *node,uintptr_t value);

/** \brief 红黑树是否为空
 *
 * \param tree const hrbtree_t* 红黑树
 * \return bool 是否为空
 *
 */
bool hrbtree_is_empty(const hrbtree_t *tree);

/** \brief 获取红黑树中的节点总数
 *
 * \param tree const hrbtree_t* 红黑树
 * \return size_t 当前节点总数,失败返回0
 *
 */
size_t hrbtree_node_count(const hrbtree_t *tree);

/** \brief 将节点附加(插入)到红黑树
 * 注意：节点由用户提供,本组件不进行任何内存分配。当树中已有相同值的节点时,新节点将插入到相同值节点的右侧。
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 待附加的节点
 * \return bool 是否成功附加
 *
 */
bool hrbtree_attach(hrbtree_t *tree,hrbtree_node_t *node);

/** \brief 将节点从红黑树分离(移除)
 * 注意：本组件不进行任何内存释放。
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 待分离的节点
 * \return bool 是否成功分离
 *
 */
bool hrbtree_detach(hrbtree_t *tree,hrbtree_node_t *node);

/** \brief 红黑树查找回调
 *
 * \param node hrbtree_node_t* 查找到的节点
 * \param usr void*            用户参数
 *
 */
typedef void (*hrbtree_on_find_callback_t)(hrbtree_node_t *node,void *usr);

/** \brief 查找指定值的所有节点(C++风格命名,相同值的节点按升序依次回调)
 * 注意：回调过程中不可分离节点。
 *
 * \param tree const hrbtree_t* 红黑树
 * \param value uintptr_t 节点值
 * \param cb hrbtree_on_find_callback_t 查找回调,可为NULL,为NULL时仅计数
 * \param usr void* 用户参数
 * \return size_t 查找到的节点个数
 *
 */
size_t hrbtree_find(const hrbtree_t *tree,uintptr_t value,hrbtree_on_find_callback_t cb,void *usr);

/** \brief 获取红黑树中的最小节点
 *
 * \param tree const hrbtree_t* 红黑树
 * \return hrbtree_node_t* 最小节点,空树返回NULL
 *
 */
hrbtree_node_t *hrbtree_first(const hrbtree_t *tree);

/** \brief 获取红黑树中的最大节点
 *
 * \param tree const hrbtree_t* 红黑树
 * \return hrbtree_node_t* 最大节点,空树返回NULL
 *
 */
hrbtree_node_t *hrbtree_last(const hrbtree_t *tree);

/** \brief 获取红黑树中指定节点的下一个节点(按节点值升序)
 *
 * \param tree const hrbtree_t* 红黑树
 * \param node const hrbtree_node_t* 当前节点
 * \return hrbtree_node_t* 下一个节点,不存在返回NULL
 *
 */
hrbtree_node_t *hrbtree_next(const hrbtree_t *tree,const hrbtree_node_t *node);

/** \brief 获取红黑树中指定节点的上一个节点(按节点值升序)
 *
 * \param tree const hrbtree_t* 红黑树
 * \param node const hrbtree_node_t* 当前节点
 * \return hrbtree_node_t* 上一个节点,不存在返回NULL
 *
 */
hrbtree_node_t *hrbtree_prev(const hrbtree_t *tree,const hrbtree_node_t *node);

/** \brief 循环遍历红黑树(按节点值升序,过程中不可分离链表中的项)
 *
 * \param tree              红黑树
 * \param item              遍历所用的变量名称
 *
 */
#define HRBTREE_FOREACH(tree,item)      \
    for(hrbtree_node_t * item=hrbtree_first(tree);item!=NULL;item=hrbtree_next(tree,item))

/** \brief 红黑树枚举回调
 *
 * \param node hrbtree_node_t* 红黑树节点
 * \param usr void*            用户参数
 *
 */
typedef void (*hrbtree_on_enum_callback_t)(hrbtree_node_t *node,void *usr);

/** \brief 红黑树枚举(按节点值升序)
 *
 * \param tree const hrbtree_t*             红黑树
 * \param cb hrbtree_on_enum_callback_t    红黑树枚举回调
 * \param usr void*                         用户参数
 *
 */
void hrbtree_enum(const hrbtree_t *tree,hrbtree_on_enum_callback_t cb,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HRBTREE_H__
