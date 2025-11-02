#include<input.h>
#include<usart.h>
#include<sys.h>
#include<string.h>
#include<math.h>
#include<stdarg.h>
int QWQ=0;

//接收并比较串口发送数据函数
//show>0,则显示从串口接收到的内容，反之则不显示
//str：定下做比较的数据
//show=on_choose:选择与定下相同的数据接收，并显示
//show=off_choose;选择与定下相同的数据接收，不显示
int Gets(int show,char*str)
{
    int len,t,num=0,i,Num=0;
    if(USART_RX_STA&0x8000)//接收到数据
        {  
            len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
            for(i=0;i<strlen(str);i++)//比较数据
            {
                if(*(str+i)==USART_RX_BUF[i]) Num++;
            }
            if(strcmp(str,(char*)USART_RX_BUF)==0||(Num==len&&len==strlen(str))) num=1;//比较数据
            if(show>0&&(show!=on_choose||(show==on_choose&&num==1)))
            {
                for(t=0;t<len;t++)
                {
                    usart_data_transmit(USART1, USART_RX_BUF[t]);//向串口1发送数据
                    while((USART1->sts&0X40)==0);//等待发送结束
                }
            }
            if((show!=on_choose&&show!=off_choose)||(num==1)) USART_RX_STA=0;//清空数据
            if(num==1) return 1;//如果接收到的数据和定下的数据相同，则返回1
            else return 0;//如果接收到的数据和定下的数据不同，则返回0
        }
        else
        return -1;//如果没接收到数据，则返回-1
}


//清除未接收的数据函数
//当数据经历y次循环后未被接收或清除，则清除该数据
//x>0,则显示未接收的数据再清除，反之直接清除未接收的数据
int Gets_clear(int x,int y)
{
    int len,t;
    if(QWQ==y&&USART_RX_STA&0x8000)
    {
        if(x>0)
        {
        len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        for(t=0;t<len;t++)//向串口1发送数据
            {
                usart_data_transmit(USART1, USART_RX_BUF[t]);//向串口1发送数据
                while((USART1->sts&0X40)==0);//等待发送结束
            }
        }
        USART_RX_STA=0;//清空数据
        QWQ=0;
        return 1;//成功清空数据则返回1
    }
    if(USART_RX_STA&0x8000)//统计循环的的次数
    {
        QWQ++;
    }
    return 0;//没清空数据则返回0
}


//接收整数函数
//如果x>0,则显示接收内容,反之则不显示
//x=on_choose:只选择接收整数，并显示
//x=off_choose;只选择接收整数，不显示
int Gets_num(int x)
{
    int i,j,t,len,num=1,number=0;
    if(USART_RX_STA&0x8000)//接收到数据
    {
         len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        if(USART_RX_BUF[0]!='-')//如果第一个字符不为'-';
        {
             for(i=0;i<len;i++)//检测接收的是否为全部为数字,是则num=1,不是则num=0
            {
                if(USART_RX_BUF[i]<'0'||USART_RX_BUF[i]>'9') num=0;
            }
        }
        else //如果第一个字符为'-';
        {
            num=-1;
            for(i=1;i<len;i++)//检测剩下接收的是否为全部为数字,是则num=-1,不是则num=0
            {
                if(USART_RX_BUF[i]<'0'||USART_RX_BUF[i]>'9') num=0;
            }
        }
        if((x>0&&x!=on_choose)||(x==on_choose&&num==1))
        {
            for(t=0;t<len;t++)
            {
                usart_data_transmit(USART1, USART_RX_BUF[t]);//向串口1发送数据
                while((USART1->sts&0X40)==0);//等待发送结束
            }
        }
        if((x!=on_choose&&x!=off_choose)||num==1||num==-1)  USART_RX_STA=0;//清空数据
        if(num==1)
        {
            for(j=0;j<len;j++)//计算得到从串口接收的正整数
            {
                number+=(USART_RX_BUF[len-1-j]-48)*pow(10,j);
            }
        }
        if(num==-1)
        {
            for(j=1;j<len;j++)//计算得到从串口接收的负整数
            {
                number+=(USART_RX_BUF[len-j]-48)*pow(10,j-1);
            }
        }
    }
    if(num==1) return number;//如果从串口接收是整数,则返回这个数字
    else if(num==-1) return number*(-1);
    else return 0;//否则返回0
}


//接收浮点数函数
//如果x>0,则显示接收内容,反之则不显示
//x=on_choose:只选择接收整数，并显示
//x=off_choose;只选择接收整数，不显示
double Gets_snum(int x)
{
    int i,j,k,t,len,num=1;
    double number=0;
    if(USART_RX_STA&0x8000)//接收到数据
    {
         len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        if(USART_RX_BUF[0]!='-')//如果第一个字符不为'-';
        {
             for(i=0;i<len;i++)//检测接收的是否为全部为数字,是则num=1,不是则num=0
            {
                if((USART_RX_BUF[i]<'0'||USART_RX_BUF[i]>'9')&&USART_RX_BUF[i]!='.') num=0;
            }
        }
        else //如果第一个字符为'-';
        {
            num=-1;
            for(i=1;i<len;i++)//检测剩下接收的是否为全部为数字,是则num=-1,不是则num=0
            {
                if((USART_RX_BUF[i]<'0'||USART_RX_BUF[i]>'9')&&USART_RX_BUF[i]!='.') num=0;
            }
        }
        if((x>0&&x!=on_choose)||(x==on_choose&&num==1))
        {
            for(t=0;t<len;t++)
            {
                usart_data_transmit(USART1, USART_RX_BUF[t]);//向串口1发送数据
                while((USART1->sts&0X40)==0);//等待发送结束
            }
        }
        if((x!=on_choose&&x!=off_choose)||num==1||num==-1)  USART_RX_STA=0;//清空数据
        if(num==1)
        {
            for(j=0;USART_RX_BUF[j]!='.';j++)//计算得到从串口接收的正整数
            {
                number=number*10+USART_RX_BUF[j]-48;
                if(j==len-1) break;//如果没有小数部分，则结束循环
            }
            for(k=j+1;k<len;k++)//计算得到从串口接收的正小数
            {
                number+=(USART_RX_BUF[k]-48)/pow(10,k-j);
            }
        }
        if(num==-1)
        {
            for(j=1;USART_RX_BUF[j]!='.';j++)//计算得到从串口接收的负整数
            {
                number=number*10+USART_RX_BUF[j]-48;
                if(j==len-1) break;
            }
            for(k=j+1;k<len;k++)//计算得到从串口接收的负小数
            {
                number+=(USART_RX_BUF[k]-48)/pow(10,k-j);
            }
        }
    }
    if(num==1) return number;//如果从串口接收是浮点数,则返回这个数字
    else if(num==-1) return number*(-1);
    else return 0;//否则返回0
}


//生成缓冲区函数
void Getchar()
{
    while(1)
    {
        if(USART_RX_STA&0x8000) break;//接收到数据后跳出缓冲区
    }
}


//检测串口是否接收到数据
int Get_find()
{
    if(USART_RX_STA&0x8000) return 1;//接收到数据，则返回1
    else return 0;//否则返回0  
}


//接收并比较串口发送的数据是否符合类型，并且返回格式化整数或字符（支持 %d %c）
//不支持中文
//接收数据符合格式，则将格式化参量通过指针返回到对应的变量，同时函数返回1
//接收数据不符合格式，函数直接返回0
int Scanf(char*str,const int*p,...)
{
    int len,t,i,j=0,k=0,l=0,ret=0,num=1;
    int word[100];
    va_list q;
    int*tmp;
    
    len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
    for(t=0;t<len;t++)
    {
        usart_data_transmit(USART1, USART_RX_BUF[t]);//向串口1发送数据
        while((USART1->sts&0X40)==0);//等待发送结束
    }
    
    for(k=0;k<strlen(str);k++)
    {
        if(*(str+k)=='%'&&(*(str+k+1)=='c'||*(str+k+1)=='d')) ret++;//统计格式化参数个数
    }
    
    for(i=0;i<strlen(str);i++)
    {
        if(*(str+i)=='%')
        {
            if(*(str+i+1)=='d')//格式化参量为 %d
            {
                num=1;//默认输入的整数为正数
                i++;
                if(USART_RX_BUF[j]=='-')//如果输入的整数为负数
                {
                    j++;
                    num=-1;
                }
                if(USART_RX_BUF[j]>='0'&&USART_RX_BUF[j]<='9')//接收的数据为数字，即符合格式
                {
                    word[l]=0;//归零
                    while(USART_RX_BUF[j]>='0'&&USART_RX_BUF[j]<='9')
                    {
                        word[l]=word[l]*10+USART_RX_BUF[j]-48;//将接收到的字符串的数字部分转化为整数
                        USART_RX_BUF[j]=0;//清除接收缓存
                        j++;
                    }
                    word[l]=word[l]*num;
                }
                else break;//接收的数据不为数字，即不符合格式
                l++;
            }
           else if(*(str+i+1)=='c')//格式化参量为 %c
            {
                i++;
                word[l]=USART_RX_BUF[j];//得到一个字符
                j++;
                l++;
            }
            else//不是格式化参量
            {
                if(*(str+i)==USART_RX_BUF[j]) //符合格式，则正常运行
                {
                    j++;
                }
                else break; //不符合格式，则跳出循环
            }
        }
        else//不是格式化参量
        {
            if(*(str+i)==USART_RX_BUF[j]) //符合格式，则正常运行
            {
                j++;
            }
            else break;//不符合格式，则跳出循环
        }
    }
    USART_RX_STA=0;//清除接收数据
    if(i==strlen(str)&&j==len)//接收的内容符合格式，则将格式化参量通过指针返回到对应的变量，同时函数返回1
    {
        va_start(q,p);
        *(int*)p=word[0];
        for(t=1;t<ret;t++)
        {
            tmp=va_arg(q,int*);
            *tmp=word[t];
        }
        va_end(q);
        return 1;
    }
    else return 0;//接收的内容不符合格式，函数直接返回0
}
