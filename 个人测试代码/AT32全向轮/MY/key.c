#include<key.h>


/********************************************************************************************************************************说明********************************************************************************************************************************

连续触发模式：按键按下一直返回1，反之返回0
按下触发模式：按键按下后返回一次1，之后一直返回0，直到按键弹起再按下
弹起触发模式：按键按下后再弹起返回一次1，其他情况一直返回0
长按触发模式：按键按下后在一定时间内（即扫描次数<T）弹起，则返回一次1，超出一定时间未弹起则返回一次2（如果SERIES==1，则在按键抬起之前一直返回2），其他情况返回0
多按触发模式：按键按下后在一定时间内（即扫描次数<T）弹起再按下，按下次数会累加起来，当超出一定时间未弹起再按下或者累加次数==MAX，则返回一次之前的累加次数，其他情况返回0
快速复按模式：按键按下后弹起在一定时间内（即扫描次数<T）再按下，按下次数会累加起来，期间按键按下的时侯会一直返回累加起来的值（如果WAIT==1，在期间即使按键不按下也会一直返回累加起来的值），当超出一定时间弹起未再按下或累加次数==MAX弹起，累加次数清零，其他情况返回0

********************************************************************************************************************************************************************************************************************************************************************/



/*按键初始化*/
//key：按键结构体
//KEY_MODE：按键模式
void key_init(KEY*key,long long KEY_MODE)
{
    key->KEY_MODE=KEY_MODE;//获取按键模式
}






/*按键扫描*/
//key：按键结构体
//event：触发事件（传入的参数为逻辑1就表示按键按下）
u8 key_scan(KEY*key,key_bool event)
{
    u8 temp;
    switch(key->KEY_MODE&0x0F)
    {
        /*连续触发模式*///该模式下 KEY_FLAG：31~0bit:保留 ； 触发返回1,反之返回0
        case KEY_MODE_SERIES     :    if(event)return 1;//按键按下返回1
                                      else return 0;//反之返回0
                                  
        
        /*按下触发模式*///该模式下 KEY_FLAG：31~1bit:保留，0bit:记录按键是否已经触发 ； 触发返回1,反之返回0
        case KEY_MODE_DOWN       :    if(event && key->KEY_FLAG.trigger==1)//按键按下且未触发
                                      {
                                          key->KEY_FLAG.trigger=0;//标记按键已经触发
                                          return 1;//返回1
                                      }
                                      else if(!event)key->KEY_FLAG.trigger=1;//按键弹起，标记按键未触发
                                      return 0;
                                  
        /*弹起触发模式*///该模式下 KEY_FLAG：31~1bit:保留，0bit:记录按键是否已经按下 ； 触发返回1,反之返回0
        case KEY_MODE_UP          :   if(event)key->KEY_FLAG.trigger=1;//按键按下且标记未标记为按下，记录按键已经按下
                                      else if(!event && key->KEY_FLAG.trigger==1)//按键弹起
                                      {
                                          key->KEY_FLAG.trigger=0;//标记按键弹起
                                          return 1;//返回1
                                      }
                                      return 0;
                                      
        /*长按触发模式*///该模式下 KEY_FLAG：31~8bit:记录按下时间，7~1:保留，0bit:记录按键是否已经触发 ； 短按触发返回1,长按触发返回2,反之返回0
        case KEY_MODE_LONG(0,0)  :    if(event)//按键按下
                                      {
                                          if(key->KEY_FLAG.time<(key->KEY_MODE>>8))key->KEY_FLAG.time++;//按下时长未达到指定时间，继续累加时间
                                          else if(((key->KEY_MODE>>4)&0x0F) || (!((key->KEY_MODE>>4)&0x0F) && key->KEY_FLAG.trigger))//反之，处于连按模式或者非连按模式且未触发
                                          {
                                              key->KEY_FLAG.trigger=0;//标记为已触发
                                              return 2;//返回2
                                          }
                                      }
                                      else //按键未按下
                                      {
                                          if((key->KEY_FLAG.time)>0 && (key->KEY_FLAG.time)<(key->KEY_MODE>>8))//记录的按下时间大于0，且未达到最大
                                          {
                                              key->KEY_FLAG.trigger=1;//标记为未触发
                                              key->KEY_FLAG.time=0;//计时清零
                                              return 1;//返回1
                                          }
                                          key->KEY_FLAG.trigger=1;//标记为未触发
                                          key->KEY_FLAG.time=0;//计时清零
                                      }
                                      return 0;
                                  
        /*多按触发模式*///该模式下 KEY_FLAG：31~8bit:记录按下时间，7~1:记录多按的次数，0bit:记录按键是否已经触发 ； 返回连续触发次数
        case KEY_MODE_MANY(0,0)    :  if(event && (key->KEY_FLAG.trigger))//按键按下且未触发
                                      {
                                          key->KEY_FLAG.time=0;//清除累加时间
                                          key->KEY_FLAG.trigger=0;//标记为已触发
                                          key->KEY_FLAG.trigger_num++;//按下次数+1
                                      }
                                      else if(!event)key->KEY_FLAG.trigger=1;//按键未按下，标记为未触发
                                      
                                      if((key->KEY_FLAG.time>=((key->KEY_MODE>>8)&0xFFF)) || (key->KEY_FLAG.trigger_num==(key->KEY_MODE>>32)))//累加时间达到指定时间，或者达到最大按下次数
                                      {
                                          temp=key->KEY_FLAG.trigger_num;//得到按下次数
                                          key->KEY_FLAG.time=0;//清除累加时间
                                          key->KEY_FLAG.trigger_num=0;//清除按下次数
                                          key->KEY_FLAG.trigger=0;//标记已触发
                                          return temp;//返回按下次数
                                      }
                                      key->KEY_FLAG.time++;//不断累加时间
                                      return 0;
                                  
        /*快速复按模式*///该模式下 KEY_FLAG：31~8bit:记录弹起时间，7~1:记录复按的次数，0bit:记录按键是否已经触发 ； 返回复按次数
        case KEY_MODE_REPEAT(0,0,0):  if(event)//按键按下
                                      {
                                          if(key->KEY_FLAG.trigger)key->KEY_FLAG.trigger_num++;//按键未触发，复按次数+1
                                          key->KEY_FLAG.time=0;//清除弹起时间
                                          key->KEY_FLAG.trigger=0;//标记已触发
                                          return key->KEY_FLAG.trigger_num;//返回复按次数
                                      }
                                      else//按键未按下
                                      {
                                          key->KEY_FLAG.trigger=1;//标记未触发
                                          
                                          if((key->KEY_FLAG.time<((key->KEY_MODE>>8)&0xFFF)) && (key->KEY_FLAG.trigger_num<(key->KEY_MODE>>32)))key->KEY_FLAG.time++;//累加时间未达到最大且复按次数未达到最大，不断累加时间
                                          else key->KEY_FLAG.trigger_num=0;//达到最大时间，清除按下次数
                                          
                                          if((key->KEY_MODE>>4)&0x0F)return key->KEY_FLAG.trigger_num;//选择等待时间，则返回复按次数
                                          else return 0;//否则直接返回0
                                      }
                                  
    }
    return 0;
}


