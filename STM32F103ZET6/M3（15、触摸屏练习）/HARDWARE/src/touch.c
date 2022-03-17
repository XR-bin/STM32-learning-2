#include "sys.h"
#include "touch.h"
#include "delay.h"

//200ns延时函数
static void delay_200ns(void)
{
  u8 i = 20;
  while(i) i--;
}

/*****************************************************
*函数功能  ：对触摸屏芯片对应的GPIO口进行初始化设置
*函数名    ：Touch_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            CS     PF11  通用输出
*            SCK    PB1   通用输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   通用输出
********************************************************/
void Touch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
	
	//时钟使能   GPIOB   GPIOF
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置
	/*输出的引脚*/
  //PB1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				     //PB1 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //根据设定参数初始化PB1
	//PF11、PF9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;				     //PF9、PF11端口配置
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //根据设定参数初始化PF9、PF11
	/*输入的引脚*/
	//PB2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;				     //PB2端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;        //上下拉输出模式 
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //根据设定参数初始化PB1
	//PF10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;				     //PF10端口配置
	GPIO_Init(GPIOF, &GPIO_InitStruct);					       //根据设定参数初始化PF10
	
	//初始化状态空闲
  T_CS_H;
  T_SCK_L;
  T_MOSI_L;
}



/*****************************************************
*函数功能  ：通过触摸屏芯片获取触摸的坐标数据
*函数名    ：Touch_Data
*函数参数  ：u16
*函数返回值：u8 command 
*描述      ：
*       片选线        T_CS_H       T_CS_L  
*       时钟线        T_SCK_H      T_SCK_L  
*       输出数据线    T_MOSI_H     T_MOSI_L
*       输入数据线    T_MISO 
*       笔接触线      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u8 i;
  u16 data = 0;
  
  //数据线拉低
  T_MOSI_L;
  //片选线拉低
  T_CS_L;
  //循环8次发送指令数据
  for(i=0;i<8;i++)
  {
    //时钟线拉低
    T_SCK_L;
    //判断指令位电平
    if(command & 0x80)T_MOSI_H;
    else T_MOSI_L;
    //延时200ns，用于数据建立
    delay_200ns();
    //时钟线拉高
    T_SCK_H;
    //延时200ns，用于数据读取
    delay_200ns();
    //指令数据左移
    command <<= 1;
  }
  //时钟线拉低
  T_SCK_L;
  //数据线拉低
  T_MOSI_L;
  //延时120us，用于等待ADC把数据转换好
  delay_us(120);
  //空出一个空脉冲
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //循环12次读取数据
  for(i=0;i<12;i++)
  {
    //指令数据左移
    data <<= 1;
    //时钟线拉低
    T_SCK_L;
    //延时200ns，用于数据建立
    delay_200ns();
    //时钟线拉高
    T_SCK_H;
    //读取数据位电平
    if(T_MISO) data |= 0x1;
    //延时200ns，用于读取数据
    delay_200ns();
  }
  //片选拉高
  T_CS_H;
  //时钟线拉低
  T_SCK_L;
  
  return data;
}



