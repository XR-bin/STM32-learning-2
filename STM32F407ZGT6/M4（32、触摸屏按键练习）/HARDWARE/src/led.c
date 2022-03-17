#include "stm32f4xx.h"  
#include "led.h"
#include "beep.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：LED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            LED1     PF6
*            LED2     PF9
*            LED3     PF10
*            LED4     PC0
*            高电平灭，低电平亮-----输出模式
********************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruc;      //GPIOx结构体配置
  
  //时钟使能  GPIOF  GPIOC
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
  /*GPIOx端口配置*/
  //PF6、9、10
  GPIO_InitStruc.GPIO_Pin   = (GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10);   //选择PF6、9、10
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;                             //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;                          //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;                             //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;                          //速度为50MHz
  GPIO_Init(GPIOF,&GPIO_InitStruc);                                      //根据设定参数初始化PF6、9、10
  
  //PC0
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PC0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;    //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;    //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz; //速度为50MHz
  GPIO_Init(GPIOC,&GPIO_InitStruc);             //根据设定参数初始化PC0
  
  /*设置输出数据寄存器*/
  //先将LED都关闭
  GPIO_SetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));      //高电平灭
  GPIO_SetBits(GPIOC,GPIO_Pin_0);                                   //高电平灭
}

/************************************************************
*函数功能  ：LED灯全亮
*函数名    ：LED_All_ON
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_All_ON(void)
{
  GPIO_ResetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));
  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}

/************************************************************
*函数功能  ：所有LED全灭
*函数名    ：LED_All_OFF
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_All_OFF(void)
{
  GPIO_SetBits(GPIOF,(GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10));
  GPIO_SetBits(GPIOC,GPIO_Pin_0);
}

/************************************************************
*函数功能  ：选择灯灭
*函数名    ：LED_All_ON
*函数参数  ：u8
*函数返回值：void
*描述      ：
***************************************************************/
void LED_X_ON(u8 n)
{
  switch(n)
  {
    case 0: LED0_ON;break;
    case 1: LED1_ON;break;
    case 2: LED2_ON;break;
    case 3: LED4_ON;break;
  }
}

/************************************************************
*函数功能  ：选择灯亮
*函数名    ：LED_All_OFF
*函数参数  ：u8
*函数返回值：void
*描述      ：
***************************************************************/
void LED_X_OFF(u8 n)
{
  switch(n)
  {
    case 0: LED0_OFF;break;
    case 1: LED1_OFF;break;
    case 2: LED2_OFF;break;
    case 3: LED4_OFF;break;
  }
}

/************************************************************
*函数功能  ：流水灯
*函数名    ：LED_Flash1
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash1(void)
{
  static int i = 0;
  LED_X_ON(i);
  delay_ms(300);
  LED_X_OFF(i);
  i++;
  if(i>3)
  {
    i=0;
  }
}

/************************************************************
*函数功能  ：流水灯+蜂鸣器
*函数名    ：LED_Flash2
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash2(int n)
{
  static int i = 0;
  static int cont = 0;
  static int c = 1;
  LED_X_ON(i);
  cont++;
  if(cont >= 1000000/2*n)
  {
    BEEP_OFF;
  }
  if(cont >= 1000000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    BEEP_ON;
    c=!c;
    cont = 0;
  }
}

/************************************************************
*函数功能  ：流水灯
*函数名    ：LED_Flash3
*函数参数  ：int n
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash3(int n)
{
  static int i = 0;
  static int cont = 0;
  
  LED_X_ON(i);
  cont++;
  
  if(cont >= 30000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    cont = 0;
  }
}

