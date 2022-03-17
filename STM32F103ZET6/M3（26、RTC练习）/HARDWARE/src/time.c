#include "sys.h"

/*****************************************************
*函数功能  ：对定时器6进行初始化设置
*函数名    ：TIM6_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx配置结构体
  NVIC_InitTypeDef NVIC_InitStruct;                //中断配置结构体
  
  //TIMx时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	/*TIMx初始化设置*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //重载值(TIMx_ARR寄存器)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //分频(TIMx_PSC寄存器)
  //TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有)
  //TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增)
  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);                 //初始化TIM6
	
	/*NVIC配置*/
  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;             //选择TIM6中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct);                             //TIM6中断初始化
	
	//允许TIMx更新中断
  TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM6, ENABLE);			// 使能TIM6重载寄存器ARR
	
  //使能TIMx
  TIM_Cmd(TIM6, ENABLE);
}

/*****************************************************
*函数功能  ：对定时器7进行初始化设置
*函数名    ：TIM7_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx配置结构体
  NVIC_InitTypeDef NVIC_InitStruct;                //中断配置结构体
  
  //TIMx时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/*TIMx初始化设置*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //重载值(TIMx_ARR寄存器)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //分频(TIMx_PSC寄存器)
  //TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有)
  //TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增)
  TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);                 //初始化TIM7
	
	/*NVIC配置*/
  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;             //选择TIM7中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct);                             //TIM7中断初始化
	
	//允许TIMx更新中断
  TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM7, ENABLE);			// 使能TIM7重载寄存器ARR
	
  //使能TIMx
  TIM_Cmd(TIM7, ENABLE);
}








