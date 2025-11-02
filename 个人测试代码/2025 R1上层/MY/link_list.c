#include<sys.h>
#include<stdarg.h>
#include<link_list.h>
#include<stdlib.h>
#include <FreeRTOS.h>


//*************************************************************双向静态链表*****************************************************************//

//连接链表节点
//link_list_mode   0：环形；1：线形
//len              输入节点数
//node,...             链表节点
void link(u8 link_list_mode,u32 len,linklist*node,...)
{
    int i;
    linklist*NODE1;//节点1
    linklist*NODE2;//节点2
    va_list p;
    va_start(p,node);
    NODE1=node;//将第一个节点参数赋予节点1
    for(i=0;i<len-1;i++)
    {
        NODE2=va_arg(p,linklist*);//将下一个节点参数赋予节点2
        NODE1->next=NODE2;//将节点1的下一个节点指向节点2
        NODE2->last=NODE1;//将节点2的下一个节点指向节点1
        NODE1=NODE2;//将节点2参数赋予节点1
    }//如此反复，将所有节点连接成链表
    va_end(p);
    if(link_list_mode==0)//如果是环形链表，将表头和表尾连接成环
    {
        NODE2=node;    
        NODE1->next=NODE2;
        NODE2->last=NODE1;
    }
    
}

//读链表
//Link_List_node  链表节点
//shift           偏移量（以输入的节点为标准进行移位，例如：1就移向下一个节点，-1就移向上一个节点）
//返回读取的数据
int linklist_read(linklist*Link_List_node,int shift)
{
    linklist*Link_List=Link_List_node;//从输入的节点开始
    if(shift>0)//偏移量大于0
    {
        while(shift--)Link_List=Link_List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift;//将偏移量转为正数
        while(shift--)Link_List=Link_List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
    }
    return Link_List->data;//返回目标节点数据
}


//写链表
//Link_List_node  链表节点
//shift           偏移量（以输入的节点为标准进行移位，例如：1就移向下一个节点，-1就移向上一个节点）
//data            要写的数据
void linklist_write(linklist*Link_List_node,int shift,int data)
{
    linklist*Link_List=Link_List_node;//从输入的节点开始
    if(shift>0)//偏移量大于0
    {
        while(shift--)Link_List=Link_List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift;//将偏移量转为正数
        while(shift--)Link_List=Link_List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
    }
    Link_List->data=data;//在目标节点写入数据
}


//增删链表节点（在shift_node1与shift_node2之间插入节点或删除shift_node1与shift_node2之间（包括shift_node1与shift_node2）的节点）
//list_head_node 链表表头（可以是任意一个节点）
//shift_node1    偏移到的节点1
//shift_node2    偏移到的节点2
//list_node      要插入的节点（如果是传入Delete，则为删除节点）
//注意：
//     必须shift_node1<=shift_node2
//     非环形链表不要头插和尾插或头删和尾删
//     插入节点时shift_node1与shift_node2不能相等，shift_node1与shift_node2要相邻否则将会是list_node将shift_node1与shift_node2的节点代替
u8 linklist_add_delete(linklist*list_head_node,int shift_node1,int shift_node2,linklist*list_node)
{
    linklist*Link_List1=list_head_node;//从输入的节点开始
    linklist*Link_List2=list_head_node;//从输入的节点开始
    if(shift_node1>shift_node2)return 0;//如果shift_node1>shift_node2则返回0
    if(list_node==Delete)//删除环节
    {
        //节点1
        if(shift_node1>0)//偏移量大于0
        {
            while(shift_node1--)Link_List1=Link_List1->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        else if(shift_node1<0)//偏移量小于0
        {
            shift_node1=-shift_node1;//将偏移量转为正数
            while(shift_node1--)Link_List1=Link_List1->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        //节点2
        if(shift_node2>0)//偏移量大于0
        {
            while(shift_node2--)Link_List2=Link_List2->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        else if(shift_node2<0)//偏移量小于0
        {
            shift_node2=-shift_node2;//将偏移量转为正数
            while(shift_node2--)Link_List2=Link_List2->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        //将节点1的上一个节点和节点2下一个相接，实现将节点1和节点2之间（包括节点1和节点2）删除
        Link_List1->last->next=Link_List2->next;
        Link_List2->next->last=Link_List1->last;
    }
    else//插入环节
    {
        if(shift_node2-shift_node1==0)return 0;//如果shift_node1=shift_node2则返回0
        //节点1
        if(shift_node1>0)//偏移量大于0
        {
            while(shift_node1--)Link_List1=Link_List1->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        else if(shift_node1<0)//偏移量小于0
        {
            shift_node1=-shift_node1;//将偏移量转为正数
            while(shift_node1--)Link_List1=Link_List1->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        //节点2
        if(shift_node2>0)//偏移量大于0
        {
            while(shift_node2--)Link_List2=Link_List2->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        else if(shift_node2<0)//偏移量小于0
        {
            shift_node2=-shift_node2;//将偏移量转为正数
            while(shift_node2--)Link_List2=Link_List2->last;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
        }
        //将要插入的节点与节点1的尾部相接，节点2的头部相接，实现将目标节点插入节点1和节点2之间
        Link_List1->next=list_node;
        Link_List2->last=list_node;
        list_node->next=Link_List2;
        list_node->last=Link_List1;
    }
    return 1;
}


//统计链表长度（链表节点个数）
//注意：只能用于环形链表
u32 linklist_len(linklist*list_head_node)
{
    linklist*list_node=list_head_node;//从头节点开始
    u32 len=0;
    while(1)//统计节点地址不同的个数，从而得到链表长度
    {
        list_node=list_node->next;
        len++;
        if(list_node==list_head_node)break;//遇到地址等于表头地址，说明已经将所有节点统计了
    }
    return len;//返回统计个数（即为长度）
}


//******************************************************************************************************************************************//






//*************************************************************双向动态链表******************************************************************//


//链表初始化
//list：           创建的链表
//link_list_mode： 链表模式；‘0’环形，‘1’线形，其他无效
//len：            链表长度（节点数（除去表头表尾））
u8 link_list_init(LinkList*list,char link_list_mode,u32 len)
{
    int i;
    linklist*NODE1=&list->LIST_HEAD;//节点1，开始指向表头
    linklist*NODE2;//节点2
    if(link_list_mode=='0')//环形，表头表尾连接
    {
        list->LIST_END.next=&(list->LIST_HEAD);
        list->LIST_HEAD.last=&(list->LIST_END);
    }
    else if(link_list_mode=='1')//线形，表头表尾指空
    {
        list->LIST_END.next=NULL;
        list->LIST_HEAD.last=NULL;
    }
    else return 0;//其他，无效，返回0
    list->LIST_HEAD.data=len;//将链表长度放在表头
    list->LIST_END.data=link_list_mode;//将链表形态放在表尾
    for(i=0;i<len;i++)//创建链表节点
    {
        NODE2=(linklist*)pvPortMalloc(sizeof(linklist));
        NODE1->next=NODE2;
        NODE2->last=NODE1;
        NODE1=NODE2;
    }
    NODE1->next=&list->LIST_END;//最后连接表尾
    list->LIST_END.last=NODE1;
    return 1;//创建成功，返回1
}


//读链表
//list：  链表地址
//shift： 链表相对第一个节点偏移量
//返回读取链表数据
//注意：链表为线形链表时，shift>链表长度-1时，取链表最后一个节点，shift<0时，取链表第一个节点
int link_list_read(LinkList*list,int shift)
{
    int len=0;//统计偏移次数，用于判断是否遇到表头表尾
    linklist*List=&list->LIST_HEAD;//节点，指向表头
    List=List->next;//向下偏移一次，从第一个节点开始
    if(list->LIST_END.data=='1')//如果是线形链表
    {
        if(shift>list->LIST_HEAD.data-1)shift=list->LIST_HEAD.data-1;//偏移量超过链表长度-1时，取链表长度-1
        if(shift<0)shift=0;//偏移量小于0时，取链表首位
    }
    if(shift>0)//偏移量大于0
    {
        while(shift--)
        {
            List=List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->next->next;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift-1;//将偏移量转为正数并-1
        List=List->last->last->last;//偏移到最后一个节点，从第最后一个节点开始
        while(shift--)
        {
            List=List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->last->last;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    return List->data;//返回目标节点数据
}


//写链表
//list：  链表地址
//shift： 链表相对第一个节点偏移量
//data：  要写入的数据
//注意：链表为线形链表时，shift>链表长度-1时，取链表最后一个节点，shift<0时，取链表第一个节点
void link_list_write(LinkList*list,int shift,int data)
{
    int len=0;//统计偏移次数，用于判断是否遇到表头表尾
    linklist*List=&list->LIST_HEAD;//节点，指向表头
    List=List->next;//向下偏移一次，从第一个节点开始
    if(list->LIST_END.data=='1')//如果是线形链表
    {
        if(shift>list->LIST_HEAD.data-1)shift=list->LIST_HEAD.data-1;//偏移量超过链表长度-1时，取链表长度-1
        if(shift<0)shift=0;//偏移量小于0时，取链表首位
    }
    if(shift>0)//偏移量大于0
    {
        while(shift--)
        {
            List=List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->next->next;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift-1;//将偏移量转为正数并-1
        List=List->last->last->last;//偏移到最后一个节点，从第最后一个节点开始
        while(shift--)
        {
            List=List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->last->last;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    List->data=data;//在目标节点写入数据
}


//增加链表节点
//list：  链表地址
//shift： 链表相对第一个节点偏移量
//data：  写入新节点的数据
//注意：链表为线形链表时，shift>链表长度时，取链表最后一个节点，shift<0时，取链表第一个节点
void link_list_add(LinkList*list,int shift,int data)
{
    int len=0;//统计偏移次数，用于判断是否遇到表头表尾
    linklist*List=&list->LIST_HEAD;//节点，指向表头
    linklist*NODE=(linklist*)pvPortMalloc(sizeof(linklist));//创建一个新节点
    NODE->data=data;//向新节点写入数据
    List=List->next;//向下偏移一次，从第一个节点开始
    if(list->LIST_END.data=='1')//如果是线形链表
    {
        if(shift>list->LIST_HEAD.data)shift=list->LIST_HEAD.data;//偏移量超过链表长度时，取链表长度
        if(shift<0)shift=0;//偏移量小于0时，取链表首位
    }
    if(shift>0)//偏移量大于0
    {
        while(shift--)
        {
            List=List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data+1)//当偏移次数等于链表长度，说明遇到表尾
            {
                List=List->next;//跳过表尾
                len=0;//清零偏移次数
            }
        }
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift-1;//将偏移量转为正数并-1
        List=List->last->last->last;//偏移到最后一个节点，从第最后一个节点开始
        while(shift--)
        {
            List=List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data+1)//当偏移次数等于链表长度+1，说明遇到表头
            {
                List=List->last;//跳过表头
                len=0;//清零偏移次数
            }
        }
    }
    //将新节点插入目标位置
    NODE->last=List->last;
    List->last->next=NODE;
    NODE->next=List;
    List->last=NODE;
    //链表长度+1，记录在表头
    list->LIST_HEAD.data++;
}



//删除链表节点
//list：  链表地址
//shift： 链表相对第一个节点偏移量
//注意：链表为线形链表时，shift>链表长度-1时，取链表最后一个节点，shift<0时，取链表第一个节点
void link_list_delete(LinkList*list,int shift)
{
    int len=0;//统计偏移次数，用于判断是否遇到表头表尾
    linklist*List=&list->LIST_HEAD;//节点，指向表头
    List=List->next;//向下偏移一次，从第一个节点开始
    if(list->LIST_END.data=='1')//如果是线形链表
    {
        if(shift>list->LIST_HEAD.data-1)shift=list->LIST_HEAD.data-1;//偏移量超过链表长度-1时，取链表长度-1
        if(shift<0)shift=0;//偏移量小于0时，取链表首位
    }
    if(shift>0)//偏移量大于0
    {
        while(shift--)
        {
            List=List->next;//向下一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->next->next;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    else if(shift<0)//偏移量小于0
    {
        shift=-shift-1;//将偏移量转为正数并-1
        List=List->last->last->last;//偏移到最后一个节点，从第最后一个节点开始
        while(shift--)
        {
            List=List->last;//向上一个节点偏移，同时偏移量-1；直到偏移量为0
            len++;//统计偏移次数
            if(len==list->LIST_HEAD.data)//当偏移次数等于链表长度，说明遇到表头表尾
            {
                List=List->last->last;//跳过表头表尾
                len=0;//清零偏移次数
            }
        }
    }
    //清除目标节点
    List->next->last=List->last;
    List->last->next=List->next;
    //链表长度-1，记录在表头
    list->LIST_HEAD.data--;
}


//******************************************************************************************************************************************//
