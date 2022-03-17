#include "sys.h"

/*************************************************************************
*函数功能  ：对通用定时器4进行初始化设置并捕获脉冲信号
*函数名    ：TIM4_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc
*描述      ：通过测量另一个定时器的占空比
*            脉冲输入口 ： PB6   TIM4_CH1  输入通道1  映射通道IC1、IC2
*****************************************************************************/
void TIM4_Capture_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef   GPIO_InitStruct;               //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct; //TIMx配置结构体
	TIM_ICInitTypeDef  TIM_ICInitStruct;              //捕获通道配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;               //中断配置结构体
	
	/*时钟使能*/   //GPIOB  TIM4
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//复用重映射寄存器(这个操作必须有，否则无法使用捕获功能)
	//AFIO时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//没有重映像,使PB6变成TIM4捕获通道
  GPIO_PinRemapConfig(GPIO_Remap_TIM4,DISABLE);
	
	/*GPIOx配置*/
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6;               //PB6 端口配置
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;		      //复用推挽输出
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                   //根据设定参数初始化PB6
	
	/*TIMx初始化设置*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //重载值(TIMx_ARR寄存器)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //分频(TIMx_PSC寄存器)
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有)
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增)
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);                 //初始化TIM4
	
	/*捕获通道配置*/
	//通道1映射到T1
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;                //选择输入端通道  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //上升沿捕获(TIMx->CCER寄存器的第1位)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上(TIMx->CCMR1寄存器的输入模式的第0-1位)
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //配置输入分频：不分频(TIMx->CCMR1寄存器的输入模式的第2-3位)
  TIM_ICInitStruct.TIM_ICFilter = 0x00;                        //配置输入滤波器：不滤波(TIMx->CCMR1寄存器的输入模式的第4-7位)
  TIM_ICInit(TIM4, &TIM_ICInitStruct);                         //捕获通道配置初始化
 
	//通道1映射到T2
	//因为配置一样，所以一些变量无需改变
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;                  //选择输入端通道  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;	     //下降沿捕获(TIMx->CCER寄存器的第1位)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; //映射到通道1上(TI1)上(这个必须结合手册才能理解)(TIMx->CCMR1寄存器的输入模式的第0-1位)
  TIM_ICInit(TIM4, &TIM_ICInitStruct);                           //捕获通道配置初始化
	
	//触发选择：TI1 边沿检测器(TIMx->SMCR寄存器的第4-6)
	TIM_SelectInputTrigger(TIM4,TIM_TS_TI1FP1);
	//从模式选择(TIMx->SMCR寄存器的第0-2)
	TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_Reset);                 //从模式选择：复位模式(TIMx->SMCR寄存器的第0-2位)
	
	//定时器中断形式选择并使能
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);        //允许CC1IE捕获中断	
	TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);        //允许CC2IE捕获中断	
	
	/*NVIC配置*/
  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;             //选择TIM4中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct);                             //TIM4中断初始化
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM4, ENABLE);			                  // 使能TIM4重载寄存器ARR
	
	/*使能定时器4函数 */
	TIM_Cmd(TIM4, ENABLE);   //使能定时器9
}















