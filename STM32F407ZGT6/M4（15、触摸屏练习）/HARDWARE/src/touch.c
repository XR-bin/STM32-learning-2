#include "stm32f4xx.h"  
#include "touch.h"
#include "delay.h"
#include "stdio.h"

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
*            CS     PC13 通用输出
*            SCK    PB0  通用输出
*            PEN    PB1  通用输入
*            MISO   PB2  通用输入
*            MOSI   PF11 通用输出
********************************************************/
void Touch_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruc;        //GPIOx配置结构体
  
  //时钟使能  GPIOB  GPIOC  GPIOF
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
  /*GPIOx端口配置*/
	/*输出*/
  //PB0
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;         //选择PB0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;  //速度为100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);               //根据设定参数初始化PB0
	//PC13
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_13;        //选择PC13
	GPIO_Init(GPIOC,&GPIO_InitStruc);               //根据设定参数初始化PC13
	//PF11
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_11;        //选择PF11
	GPIO_Init(GPIOF,&GPIO_InitStruc);               //根据设定参数初始化PF11
	/*输入*/
	//PB1、2
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2;         //选择PB1、2
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;                  //通用输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;              //无上下拉
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;             //速度为100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                          //根据设定参数初始化PB1、2
	
	//初始状态空闲
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
  u16 data = 0;
  u8 i;
  
  //拉低输出数据线
  T_MOSI_L;
  //拉低片选
  T_CS_L;
  //循环8次发送指令
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
  //拉低数据输出线
  T_MOSI_L;
  //拉低时钟线
  T_SCK_L;
  //延时120us，用于等待ADC转换数据
  delay_us(120);
  //空出一个空脉冲
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //循环12次接收数据
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
    if(T_MISO) 
    {
      data |= 0x1;
    }
    //延时200ns，用于数据读取
    delay_200ns();
  }
  //片选拉高
  T_CS_H;
  //时钟线拉低
  T_SCK_L;
  
  return data;
}















