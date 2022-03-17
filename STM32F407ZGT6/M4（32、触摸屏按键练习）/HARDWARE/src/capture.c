#include "stm32f4xx.h"  
#include "stdio.h"

/*****************************************************
*函数功能  ：对通用定时器5进行初始化设置并捕获脉冲信号
*函数名    ：TIM5_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过测量脉冲宽度来计算按下时间
*            脉冲输入口 ： PA0   TIM5_CH1  输入通道1
********************************************************/
void TIM5_Capture_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	TIM_ICInitTypeDef  TIM_ICInitStruct;                 //捕获通道配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //中断配置结构体
	
  //时钟使能   GPIOA   TIM5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  //使能TIM5时钟
	
	/*GPIO口配置*/
	//PA0
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0;               //选择PA0
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉下拉
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//速度100MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //根据设定参数初始化PA0
	
	//TIM5对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);  //PA0 复用为TIM5
	
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);            //初始化TIM5
	
	//捕获通道配置
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;                //选择输入端通道
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //上升沿捕获(TIMx->CCER寄存器的第1位)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上(TIMx->CCMR1寄存器的输入模式的第0-1位)
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //配置输入分频：不分频(TIMx->CCMR1寄存器的输入模式的第2-3位)
  TIM_ICInitStruct.TIM_ICFilter = 0x00;                        //配置输入滤波器：不滤波(TIMx->CCMR1寄存器的输入模式的第4-7位)
  TIM_ICInit(TIM5, &TIM_ICInitStruct);                         //捕获通道配置初始化
	
	//定时器中断形式选择并使能
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);     //允许更新中断
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);        //允许CC1IE捕获中断	
	
	//TIM5中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;        //TIM5中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                      //根据指定的参数初始化VIC寄存器
 
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM5, ENABLE);			                  // 使能TIM5重载寄存器ARR
	
	/*使能定时器5函数 */
	TIM_Cmd(TIM5, ENABLE);   //使能定时器5
}

/*************************************************************************
*函数功能  ：对通用定时器9进行初始化设置并捕获脉冲信号
*函数名    ：TIM9_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc
*描述      ：通过测量脉冲宽度来计算按下时间
*            脉冲输入口 ： PA2   TIM9_CH1  输入通道1  映射通道IC1、IC2
*****************************************************************************/
void TIM9_Capture_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	TIM_ICInitTypeDef  TIM_ICInitStruct;                 //捕获通道配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //中断配置结构体
	
  //时钟使能   GPIOA   TIM9
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  //使能TIM9时钟
	
	/*GPIO口配置*/
	//PA2
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_2;               //选择PA2
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉下拉
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//速度100MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //根据设定参数初始化PA2
	
	//TIM9对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);  //PA2 复用为TIM9
	
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(定时器9只能递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);            //初始化TIM9
	
	//捕获通道配置
	//通道1映射到T1
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;                //选择输入端通道  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //上升沿捕获(TIMx->CCER寄存器的第1位)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上(TIMx->CCMR1寄存器的输入模式的第0-1位)
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //配置输入分频：不分频(TIMx->CCMR1寄存器的输入模式的第2-3位)
  TIM_ICInitStruct.TIM_ICFilter = 0x00;                        //配置输入滤波器：不滤波(TIMx->CCMR1寄存器的输入模式的第4-7位)
  TIM_ICInit(TIM9, &TIM_ICInitStruct);                         //捕获通道配置初始化
 
	//通道1映射到T2
	//因为配置一样，所以一些变量无需改变
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;                  //选择输入端通道  
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;	     //下降沿捕获(TIMx->CCER寄存器的第1位)
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI; //映射到通道1上(TI1)上(这个必须结合手册才能理解)(TIMx->CCMR1寄存器的输入模式的第0-1位)
  TIM_ICInit(TIM9, &TIM_ICInitStruct);                           //捕获通道配置初始化
	
	//触发选择：TI1 边沿检测器(TIMx->SMCR寄存器的第4-6)
	TIM_SelectInputTrigger(TIM9,TIM_TS_TI1FP1);
	//从模式选择(TIMx->SMCR寄存器的第0-2)
	TIM_SelectSlaveMode(TIM9,TIM_SlaveMode_Reset);                 //从模式选择：复位模式(TIMx->SMCR寄存器的第0-2位)
	
	//定时器中断形式选择并使能
	TIM_ITConfig(TIM9,TIM_IT_CC1,ENABLE);        //允许CC1IE捕获中断	
	TIM_ITConfig(TIM9,TIM_IT_CC2,ENABLE);        //允许CC2IE捕获中断	
	
	//TIM5中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;        //TIM5中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;         //抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		           //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			           //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                               //根据指定的参数初始化VIC寄存器
 
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM9, ENABLE);			                  // 使能TIM9重载寄存器ARR
	
	/*使能定时器9函数 */
	TIM_Cmd(TIM9, ENABLE);   //使能定时器9
}




