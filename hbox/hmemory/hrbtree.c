/***************************************************************
 * Name:      hrbtree.c
 * Purpose:   实现hrbtree接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-29
 * Generated: 由 DeepSeek V4.1 Flash 1017 生成
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrbtree.h"

void hrbtree_init(hrbtree_t *tree)
{
    if(tree!=NULL)
    {
        //守卫节点指向自身,空树的根节点为守卫节点
        tree->nil.parent=&tree->nil;
        tree->nil.left=&tree->nil;
        tree->nil.right=&tree->nil;
        tree->nil.value=0;
        tree->nil.flags=0;
        tree->nil.color=HRBTREE_FLAGS_COLOR_BLACK;
        tree->root=&tree->nil;
        tree->node_count=0;
    }
}

void hrbtree_node_init(hrbtree_node_t *node,uintptr_t value)
{
    if(node!=NULL)
    {
        //新节点尚未附加到红黑树,附加时其指针将指向树中的守卫节点
        node->parent=NULL;
        node->left=NULL;
        node->right=NULL;
        node->value=value;
        node->flags=0;
        node->color=HRBTREE_FLAGS_COLOR_RED;
    }
}

bool hrbtree_is_empty(const hrbtree_t *tree)
{
    return (hrbtree_node_count(tree)==0);
}

size_t hrbtree_node_count(const hrbtree_t *tree)
{
    if(tree==NULL)
    {
        return 0;
    }
    return tree->node_count;
}

/** \brief 红黑树左旋
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 旋转节点
 *
 */
static inline void hrbtree_left_rotate(hrbtree_t *tree,hrbtree_node_t *node)
{
    hrbtree_node_t *right=node->right;
    node->right=right->left;
    if(right->left!=&tree->nil)
    {
        right->left->parent=node;
    }
    right->parent=node->parent;
    if(node->parent==&tree->nil)
    {
        tree->root=right;
    }
    else if(node==node->parent->left)
    {
        node->parent->left=right;
    }
    else
    {
        node->parent->right=right;
    }
    right->left=node;
    node->parent=right;
}

/** \brief 红黑树右旋
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 旋转节点
 *
 */
static inline void hrbtree_right_rotate(hrbtree_t *tree,hrbtree_node_t *node)
{
    hrbtree_node_t *left=node->left;
    node->left=left->right;
    if(left->right!=&tree->nil)
    {
        left->right->parent=node;
    }
    left->parent=node->parent;
    if(node->parent==&tree->nil)
    {
        tree->root=left;
    }
    else if(node==node->parent->right)
    {
        node->parent->right=left;
    }
    else
    {
        node->parent->left=left;
    }
    left->right=node;
    node->parent=left;
}

/** \brief 红黑树插入后修正
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 新插入的节点
 *
 */
static inline void hrbtree_insert_fixup(hrbtree_t *tree,hrbtree_node_t *node)
{
    while(node->parent->color==HRBTREE_FLAGS_COLOR_RED)
    {
        if(node->parent==node->parent->parent->left)
        {
            hrbtree_node_t *uncle=node->parent->parent->right;
            if(uncle->color==HRBTREE_FLAGS_COLOR_RED)
            {
                //父节点与叔节点均为红色,变色并向上继续处理
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                uncle->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->parent->color=HRBTREE_FLAGS_COLOR_RED;
                node=node->parent->parent;
            }
            else
            {
                if(node==node->parent->right)
                {
                    //内侧节点,先左旋转化为外侧节点
                    node=node->parent;
                    hrbtree_left_rotate(tree,node);
                }
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->parent->color=HRBTREE_FLAGS_COLOR_RED;
                hrbtree_right_rotate(tree,node->parent->parent);
            }
        }
        else
        {
            hrbtree_node_t *uncle=node->parent->parent->left;
            if(uncle->color==HRBTREE_FLAGS_COLOR_RED)
            {
                //父节点与叔节点均为红色,变色并向上继续处理
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                uncle->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->parent->color=HRBTREE_FLAGS_COLOR_RED;
                node=node->parent->parent;
            }
            else
            {
                if(node==node->parent->left)
                {
                    //内侧节点,先右旋转化为外侧节点
                    node=node->parent;
                    hrbtree_right_rotate(tree,node);
                }
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->parent->color=HRBTREE_FLAGS_COLOR_RED;
                hrbtree_left_rotate(tree,node->parent->parent);
            }
        }
    }
    //根节点始终为黑色
    tree->root->color=HRBTREE_FLAGS_COLOR_BLACK;
}

bool hrbtree_attach(hrbtree_t *tree,hrbtree_node_t *node)
{
    if(tree==NULL || node==NULL || node==&tree->nil)
    {
        return false;
    }
    hrbtree_node_t *parent=&tree->nil;
    hrbtree_node_t *current=tree->root;
    while(current!=&tree->nil)
    {
        parent=current;
        if(node->value < current->value)
        {
            current=current->left;
        }
        else
        {
            //相同值的节点附加到右侧
            current=current->right;
        }
    }

    node->parent=parent;
    node->left=&tree->nil;
    node->right=&tree->nil;
    node->color=HRBTREE_FLAGS_COLOR_RED;

    if(parent==&tree->nil)
    {
        tree->root=node;
    }
    else if(node->value < parent->value)
    {
        parent->left=node;
    }
    else
    {
        parent->right=node;
    }

    hrbtree_insert_fixup(tree,node);
    tree->node_count++;
    return true;
}

/** \brief 用替换节点替换子树
 *
 * \param tree hrbtree_t* 红黑树
 * \param old_node hrbtree_node_t* 原节点
 * \param new_node hrbtree_node_t* 替换节点
 *
 */
static inline void hrbtree_transplant(hrbtree_t *tree,hrbtree_node_t *old_node,hrbtree_node_t *new_node)
{
    if(old_node->parent==&tree->nil)
    {
        tree->root=new_node;
    }
    else if(old_node==old_node->parent->left)
    {
        old_node->parent->left=new_node;
    }
    else
    {
        old_node->parent->right=new_node;
    }
    new_node->parent=old_node->parent;
}

/** \brief 获取指定子树的最小节点
 *
 * \param tree const hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 子树根节点
 * \return hrbtree_node_t* 最小节点
 *
 */
static inline hrbtree_node_t *hrbtree_minimum_node(const hrbtree_t *tree,hrbtree_node_t *node)
{
    while(node->left!=&tree->nil)
    {
        node=node->left;
    }
    return node;
}

/** \brief 获取指定子树的最大节点
 *
 * \param tree const hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 子树根节点
 * \return hrbtree_node_t* 最大节点
 *
 */
static inline hrbtree_node_t *hrbtree_maximum_node(const hrbtree_t *tree,hrbtree_node_t *node)
{
    while(node->right!=&tree->nil)
    {
        node=node->right;
    }
    return node;
}

/** \brief 红黑树删除后修正
 *
 * \param tree hrbtree_t* 红黑树
 * \param node hrbtree_node_t* 替换被删除节点的节点
 *
 */
static inline void hrbtree_delete_fixup(hrbtree_t *tree,hrbtree_node_t *node)
{
    while(node!=tree->root && node->color==HRBTREE_FLAGS_COLOR_BLACK)
    {
        if(node==node->parent->left)
        {
            hrbtree_node_t *brother=node->parent->right;
            if(brother==&tree->nil)
            {
                //守卫节点保持黑色,不参与变色
                node=node->parent;
                continue;
            }
            if(brother->color==HRBTREE_FLAGS_COLOR_RED)
            {
                brother->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->color=HRBTREE_FLAGS_COLOR_RED;
                hrbtree_left_rotate(tree,node->parent);
                brother=node->parent->right;
            }
            if(brother->left->color==HRBTREE_FLAGS_COLOR_BLACK && brother->right->color==HRBTREE_FLAGS_COLOR_BLACK)
            {
                brother->color=HRBTREE_FLAGS_COLOR_RED;
                node=node->parent;
            }
            else
            {
                if(brother->right->color==HRBTREE_FLAGS_COLOR_BLACK)
                {
                    brother->left->color=HRBTREE_FLAGS_COLOR_BLACK;
                    brother->color=HRBTREE_FLAGS_COLOR_RED;
                    hrbtree_right_rotate(tree,brother);
                    brother=node->parent->right;
                }
                brother->color=node->parent->color;
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                brother->right->color=HRBTREE_FLAGS_COLOR_BLACK;
                hrbtree_left_rotate(tree,node->parent);
                node=tree->root;
            }
        }
        else
        {
            hrbtree_node_t *brother=node->parent->left;
            if(brother==&tree->nil)
            {
                //守卫节点保持黑色,不参与变色
                node=node->parent;
                continue;
            }
            if(brother->color==HRBTREE_FLAGS_COLOR_RED)
            {
                brother->color=HRBTREE_FLAGS_COLOR_BLACK;
                node->parent->color=HRBTREE_FLAGS_COLOR_RED;
                hrbtree_right_rotate(tree,node->parent);
                brother=node->parent->left;
            }
            if(brother->left->color==HRBTREE_FLAGS_COLOR_BLACK && brother->right->color==HRBTREE_FLAGS_COLOR_BLACK)
            {
                brother->color=HRBTREE_FLAGS_COLOR_RED;
                node=node->parent;
            }
            else
            {
                if(brother->left->color==HRBTREE_FLAGS_COLOR_BLACK)
                {
                    brother->right->color=HRBTREE_FLAGS_COLOR_BLACK;
                    brother->color=HRBTREE_FLAGS_COLOR_RED;
                    hrbtree_left_rotate(tree,brother);
                    brother=node->parent->left;
                }
                brother->color=node->parent->color;
                node->parent->color=HRBTREE_FLAGS_COLOR_BLACK;
                brother->left->color=HRBTREE_FLAGS_COLOR_BLACK;
                hrbtree_right_rotate(tree,node->parent);
                node=tree->root;
            }
        }
    }
    node->color=HRBTREE_FLAGS_COLOR_BLACK;
}

bool hrbtree_detach(hrbtree_t *tree,hrbtree_node_t *node)
{
    if(tree==NULL || node==NULL || node==&tree->nil)
    {
        return false;
    }
    hrbtree_node_t *fixup_node=&tree->nil;
    hrbtree_node_t *removed_node=node;
    uint8_t removed_color=removed_node->color;
    if(node->left==&tree->nil)
    {
        fixup_node=node->right;
        hrbtree_transplant(tree,node,node->right);
    }
    else if(node->right==&tree->nil)
    {
        fixup_node=node->left;
        hrbtree_transplant(tree,node,node->left);
    }
    else
    {
        //被删除的节点有两个子节点,使用后继节点替换
        removed_node=hrbtree_minimum_node(tree,node->right);
        removed_color=removed_node->color;
        fixup_node=removed_node->right;
        if(removed_node->parent==node)
        {
            fixup_node->parent=removed_node;
        }
        else
        {
            hrbtree_transplant(tree,removed_node,removed_node->right);
            removed_node->right=node->right;
            removed_node->right->parent=removed_node;
        }
        hrbtree_transplant(tree,node,removed_node);
        removed_node->left=node->left;
        removed_node->left->parent=removed_node;
        removed_node->color=node->color;
    }

    if(removed_color==HRBTREE_FLAGS_COLOR_BLACK)
    {
        hrbtree_delete_fixup(tree,fixup_node);
    }

    tree->node_count--;

    //已分离的节点指针指向守卫节点
    node->parent=&tree->nil;
    node->left=&tree->nil;
    node->right=&tree->nil;
    node->color=HRBTREE_FLAGS_COLOR_RED;
    return true;
}

size_t hrbtree_find(const hrbtree_t *tree,uintptr_t value,hrbtree_on_find_callback_t cb,void *usr)
{
    if(tree==NULL)
    {
        return 0;
    }
    hrbtree_node_t *node=tree->root;
    hrbtree_node_t *first=NULL;
    while(node!=&tree->nil)
    {
        if(value <= node->value)
        {
            //记录可能是首个匹配的节点,继续在左子树中查找
            first=node;
            node=node->left;
        }
        else
        {
            node=node->right;
        }
    }
    if(first==NULL || first->value!=value)
    {
        return 0;
    }

    size_t count=0;
    for(node=first;node!=NULL && node->value==value;node=hrbtree_next(tree,node))
    {
        if(cb!=NULL)
        {
            cb(node,usr);
        }
        count++;
    }
    return count;
}

hrbtree_node_t *hrbtree_first(const hrbtree_t *tree)
{
    if(tree==NULL || tree->root==&tree->nil)
    {
        return NULL;
    }
    return hrbtree_minimum_node(tree,tree->root);
}

hrbtree_node_t *hrbtree_last(const hrbtree_t *tree)
{
    if(tree==NULL || tree->root==&tree->nil)
    {
        return NULL;
    }
    return hrbtree_maximum_node(tree,tree->root);
}

hrbtree_node_t *hrbtree_next(const hrbtree_t *tree,const hrbtree_node_t *node)
{
    if(tree==NULL || node==NULL || node==&tree->nil)
    {
        return NULL;
    }
    if(node->right!=&tree->nil)
    {
        //右子树的最小节点为下一个节点
        return hrbtree_minimum_node(tree,node->right);
    }
    const hrbtree_node_t *parent=node->parent;
    while(parent!=&tree->nil && node==parent->right)
    {
        node=parent;
        parent=parent->parent;
    }
    if(parent==&tree->nil)
    {
        return NULL;
    }
    return (hrbtree_node_t *)parent;
}

hrbtree_node_t *hrbtree_prev(const hrbtree_t *tree,const hrbtree_node_t *node)
{
    if(tree==NULL || node==NULL || node==&tree->nil)
    {
        return NULL;
    }
    if(node->left!=&tree->nil)
    {
        //左子树的最大节点为上一个节点
        return hrbtree_maximum_node(tree,node->left);
    }
    const hrbtree_node_t *parent=node->parent;
    while(parent!=&tree->nil && node==parent->left)
    {
        node=parent;
        parent=parent->parent;
    }
    if(parent==&tree->nil)
    {
        return NULL;
    }
    return (hrbtree_node_t *)parent;
}

void hrbtree_enum(const hrbtree_t *tree,hrbtree_on_enum_callback_t cb,void *usr)
{
    if(tree==NULL || cb==NULL)
    {
        return;
    }
    HRBTREE_FOREACH(tree,item)
    {
        cb(item,usr);
    }
}
