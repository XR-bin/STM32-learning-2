#include "stm32f4xx.h"   

/*****************************************************
*函数功能  ：对通用定时器12进行初始化设置并输出PWM
*函数名    ：TIM12_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LCD屏亮度
*            PWM输出口 ： PB15   TIM12_CH2  输出通道2
********************************************************/
void TIM12_PWM_Init(u16 arr,u16 psc,u16 ccr2)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //通道配置结构体
	
  //时钟使能   GPIOB   TIM12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); //使能TIM12时钟
	
	/*GPIO口配置*/
	//PB15
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_15;              //选择PB15
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉下拉
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//速度100MHz
	GPIO_Init(GPIOB,&GPIO_InitStruc);                   //根据设定参数初始化PB15
	
	//TIM12对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);  //PB15 复用为TIM12
	
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(通用定时器12只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStruct);           //初始化TIM12
	
	//PWM模式配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //配置为PWM模式1(TIMx_CCMR1寄存器的输出模式的第4-6位)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //通道选择输出模式(TIMx->CCER寄存器的第0位)
	TIM_OCInitStruct.TIM_Pulse = ccr2;	                       //设置比较寄存器(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置有效电平为低电平(TIMx->CCER寄存器的第1位)
	TIM_OC2Init(TIM12, &TIM_OCInitStruct);                     //通道2初始化配置
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);         //通道2比较寄存器影子寄存器使能
 
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM12, ENABLE);			                  // 使能TIM12重载寄存器ARR
	
	/*使能定时器12函数 */
	TIM_Cmd(TIM12, ENABLE);   //使能定时器12
}

/*****************************************************
*函数功能  ：对通用定时器10进行初始化设置并输出PWM
*函数名    ：TIM10_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LED亮度
*            PWM输出口 ： PF6   TIM10_CH1  输出通道1
********************************************************/
void TIM10_PWM_Init(u16 arr,u16 psc,u16 ccr1)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //通道配置结构体
	
  //时钟使能   GPIOF   TIM10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE); //使能TIM12时钟
	
	/*GPIO口配置*/
	//PF6
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_6;               //选择PF6
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉下拉
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//速度100MHz
	GPIO_Init(GPIOF,&GPIO_InitStruc);                   //根据设定参数初始化PF6
	
	//TIM12对应引脚复用映射
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);  //PF6 复用为TIM10
	
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(通用定时器10只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);           //初始化TIM10
	
	//PWM模式配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //配置为PWM模式1(TIMx_CCMR1寄存器的输出模式的第4-6位)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //通道选择输出模式(TIMx->CCER寄存器的第0位)
	TIM_OCInitStruct.TIM_Pulse = ccr1;	                       //设置比较寄存器(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置有效电平为低电平(TIMx->CCER寄存器的第1位)
	TIM_OC1Init(TIM10, &TIM_OCInitStruct);                     //通道1初始化配置
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);         //通道1比较寄存器影子寄存器使能
 
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM10, ENABLE);			                  // 使能TIM10重载寄存器ARR
	
	/*使能定时器12函数 */
	TIM_Cmd(TIM10, ENABLE);   //使能定时器10
}







