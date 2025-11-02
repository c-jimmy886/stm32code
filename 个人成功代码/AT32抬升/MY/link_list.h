#ifndef __LINK_LIST_H
#define __LINK_LIST_H	 
#include "sys.h"

#define Delete 0
#define linklist struct Link_list
#define LinkList struct Link_List


struct Link_list//链表结构体
{
    int data;
    struct Link_list*last;
    struct Link_list*next;
};

struct Link_List//创建链表结构体（动态）
{
    linklist LIST_HEAD;//表头
    linklist LIST_END;//表尾
};
//静态***********************************************************************************************************
void link(u8 link_list_mode,u32 len,linklist*node,...);//连接链表节点
int linklist_read(linklist*Link_List_node,int shift);//读链表
void linklist_write(linklist*Link_List_node,int shift,int data);//写链表
u8 linklist_add_delete(linklist*list_head_node,int shift_node1,int shift_node2,linklist*list_node);//增删链表节点
u32 linklist_len(linklist*list_head_node);//统计链表长度

//动态************************************************************************************************************
u8 link_list_init(LinkList*list,char link_list_mode,u32 len);//链表初始化
int link_list_read(LinkList*list,int shift);//读链表
void link_list_write(LinkList*list,int shift,int data);//写链表
void link_list_add(LinkList*list,int shift,int data);//插入节点
void link_list_delete(LinkList*list,int shift);//删除节点

#endif
