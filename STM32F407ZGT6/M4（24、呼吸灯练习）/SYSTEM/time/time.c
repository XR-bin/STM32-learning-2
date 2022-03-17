#include "stm32f4xx.h"  

/*****************************************************
*函数功能  ：对定时器6进行初始化设置
*函数名    ：TIM6_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);            //初始化TIM6
	//TIMx中断选择并使能
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	
	//TIM6中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;    //TIM6中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                      //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM6, ENABLE);			            // 使能TIM6重载寄存器ARR
	
	TIM_Cmd(TIM6,ENABLE); //使能定时器6
}

/******************************************************
*函数功能  ：对基本定时器7进行初始化设置
*函数名    ：TIM7_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);            //初始化TIM7
	//TIMx中断选择并使能
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器7更新中断
	
	//TIM6中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;        //TIM7中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                      //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM7, ENABLE);			            // 使能TIM7重载寄存器ARR
	
	TIM_Cmd(TIM7,ENABLE); //使能定时器7
}







