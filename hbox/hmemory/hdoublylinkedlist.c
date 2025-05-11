/***************************************************************
 * Name:      hdoublylinkedlist.c
 * Purpose:   实现hdoublylinkedlist接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdoublylinkedlist.h"


void hdoublylinkedlist_init(hdoublylinkedlist_head_t *list_head)
{
    if(list_head!=NULL)
    {
        //所有节点均指向自己
        list_head->next=list_head;
        list_head->prev=list_head;
    }
}

bool hdoublylinkedlist_is_empty(hdoublylinkedlist_head_t* item)
{
    if(item==NULL)
    {
        return true;
    }
    if((item->prev==NULL || item->prev==item)&&(item->next==NULL || item->next==item))
    {
        return true;
    }
    return false;
}

void hdoublylinkedlist_enum(hdoublylinkedlist_head_t *list_head,hdoublylinkedlist_on_enum_callback_t cb,void *usr)
{
    if(list_head==NULL || cb == NULL)
    {
        return;
    }
    HDOUBLYLINKEDLIST_FOREACH(list_head,item)
    {
        cb(item,usr);
    }
}

void hdoublylinkedlist_add_front(hdoublylinkedlist_head_t *item,hdoublylinkedlist_head_t *new_item)
{
    if(item!=NULL &&new_item!=NULL)
    {
        new_item->next=item;
        new_item->prev=item->prev;

        if(new_item->prev!=NULL)
        {
            new_item->prev->next=new_item;
        }

        if(new_item->next!=NULL)
        {
            new_item->next->prev=new_item;
        }
    }
}

void hdoublylinkedlist_add_back(hdoublylinkedlist_head_t *item,hdoublylinkedlist_head_t *new_item)
{
    if(item!=NULL &&new_item!=NULL)
    {
        new_item->next=item->next;
        new_item->prev=item;

        if(new_item->prev!=NULL)
        {
            new_item->prev->next=new_item;
        }

        if(new_item->next!=NULL)
        {
            new_item->next->prev=new_item;
        }
    }
}

void hdoublylinkedlist_remove(hdoublylinkedlist_head_t *item)
{
    if(item!=NULL)
    {
        if(item->prev!=NULL)
        {
            item->prev->next=item->next;
        }
        if(item->next!=NULL)
        {
            item->next->prev=item->prev;
        }


        //节点均指向自己
        item->next=item;
        item->prev=item;
    }
}


