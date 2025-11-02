#include<my_malloc.h>
#include<stdio.h>


//内存池管理表的每一个位对应内存池的每一个字节
//从内存池中申请的每个内存之间都有一字节间隔，对应的内存池管理表有一个位的间隔

#define MEMORY_LEN 8*1024//最大内存 8字节

u8 MEMORY[MEMORY_LEN];//内存池
u8 MEMORY_LIST[MEMORY_LEN/8];//内存池管理表

void my_malloc_init()//内存管理初始化
{
    u32 i,j;
    for(i=0;i<MEMORY_LEN/8;i++)//清空内存池及管理表
    {
        for(j=0;j<8;j++)MEMORY[i*8+j]=0;
        MEMORY_LIST[i]=0;
    }
}

void*my_malloc(u32 byte)//申请byte字节内存
{
    u32 i=0,j=0;
    while(1)
    {
        if(((MEMORY_LIST[i/8]>>(i%8))&0x01)==0)j++;//寻找连续的空内存
        else j=0;
        i++;
        if(j==byte+1)//找到足够大的连续空内存
        {
            for(j=0;j<byte;j++)MEMORY_LIST[(i-byte+j)/8]|=0x01<<((i-byte+j)%8);//在管理表标记该区域内存已用
            return &MEMORY[i-byte];//返回找到的内存的首地址
        }
        if(i==MEMORY_LEN)return NULL;////找不到足够大的连续空内存，返回NULL
    }
}


void*my_free(void*P)//释放内存
{

    u8 i=0;
    u8*p=P;
    u32 ADD=p-MEMORY;//获取该内存相对内存池首地址位置
    while(1)
    {
        if(((MEMORY_LIST[(ADD+i)/8]>>((ADD+i)%8))&0x01)==1)MEMORY_LIST[(ADD+i)/8]&=~(0x01<<((ADD+i)%8));//在管理表标记该区域内存未用
        else 
        {
            P=NULL;//返回空指针
            return NULL;
        }
        i++;
    }
}


void memory_use(float*per,u32*byte)//内存占用
{
    u32 i;
    float use;
    for(i=0;i<MEMORY_LEN;i++)
    {
        if(((MEMORY_LIST[i/8]>>(i%8))&0x01)==1)use++;//统计被使用的内存
        else if((((MEMORY_LIST[i/8]>>(i%8))&0x01)==0)&&(((MEMORY_LIST[(i+1)/8]>>((i+1)%8))&0x01)==1))use++;//被申请的两个内存之间的间隔也被算入
    }
    *per=use/(MEMORY_LEN)*100;//内存占用率
    *byte=use;//内存占用字节
}
