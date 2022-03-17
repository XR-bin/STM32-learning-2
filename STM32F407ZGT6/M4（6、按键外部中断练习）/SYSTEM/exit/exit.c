#include "stm32f4xx.h" 

/******************************************************************
*函数功能  ：外部中断EXIT0触发初始化
*函数名    ：EXTI0_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             PA0按键       上升沿触发
*******************************************************************/
void EXTI0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;    //GPIOx配置结构体
	EXTI_InitTypeDef EXTI_InitStruct;   //外部中断配置结构体
	NVIC_InitTypeDef NVIC_InitStruct;   //NVIC(中断)配置结构体
	
	//时钟使能     SYSCFG  GPIOA 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	/*GPIOx配置*/
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PA0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;     //通用输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //无上下拉
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;  //速度为50MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);             //根据设定参数初始化PA0
	//映射管脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 连接到中断线0(SYSCFG->EXTICR1寄存器)

	/*中断配置*/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;            //外部中断0
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;       //子优先级2
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
  NVIC_Init(&NVIC_InitStruct);                             //配置
	
	/*外部中断线初始化*/
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;             //选择外部中断线0        
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;    //中断事件
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发    (EXTI_RTSR寄存器的第0位)
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //使能0线的中断 (EXTI->IMR寄存器的第0位)
  EXTI_Init(&EXTI_InitStruct);                        //初始化外部中断0
}












