#include "sys.h"

/*****************************************************
*函数功能  ：外部中断初始化
*函数名    ：EXTI4_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            按键0--------PE4 低电平按下，高电平抬起
********************************************************/
void EXTI4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  //GPIOx配置结构体
	NVIC_InitTypeDef  NVIC_InitStruct;  //中断配置结构体
	EXTI_InitTypeDef  EXTI_InitStruct;  //外部中断线配置结构体
	
	//时钟使能     AFIO  GPIOE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   	//使能复用功能时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   //GPIOE
	
	//GPIOx配置
  //PE
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;	      //PE4端口配置
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ; 	//上拉输入
  GPIO_Init(GPIOE, &GPIO_InitStruct);					  //根据设定参数初始化GPIE PE4 
	
	/*中断配置*/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;			        //外部中断4
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;				//子优先级0 
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
  NVIC_Init(&NVIC_InitStruct);  	                          //配置
	
	/*外部中断配置*/
	//外部中断配置寄存器清零
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	//开始配置外部中断线
  EXTI_InitStruct.EXTI_Line=EXTI_Line4;                     //选择外部中断线4
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	        //中断事件
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;      //下降沿触发    (EXTI_RTSR寄存器的第0位)
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;                    //使能4线的中断 (EXTI->IMR寄存器的第0位)
  EXTI_Init(&EXTI_InitStruct);	  	                        //初始化外部中断4
}

/******************************************************************
*函数功能  ：软件外部中断发初始化
*函数名    ：EXTI5_Swier_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             无需端口
*******************************************************************/
void EXTI5_Swier_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;   //NVIC(中断)配置结构体
	
	//软件中断寄存器
	EXTI->SWIER &= ~(1<<5);
	
	/*中断配置*/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;          //外部中断5
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;       //子优先级2
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
  NVIC_Init(&NVIC_InitStruct);                             //配置
	
	//中断屏蔽器
  EXTI->IMR |= (1<<5);
}



