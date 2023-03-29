#include "LowPower.h"
#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "stdio.h"

/******************************************************************
*函数功能  ：低功耗引脚相关配置
*函数名    ：LowPower_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*             PA0作为外部中断0引脚
*******************************************************************/
void LowPower_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  	//引脚配置	  
	NVIC_InitTypeDef NVIC_InitStructure;    //中断优先级
	EXTI_InitTypeDef EXTI_InitStructure;    //中断
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA和复用功能时钟
	
	//引脚配置
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	   //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	       //初始化IO
	//使用外部中断方式
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//中断线0连接GPIOA.0
	
	//中断配置
	EXTI_InitStructure.EXTI_Line    = EXTI_Line0;	         //设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt; //设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	                       // 初始化外部中断
	
	//中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;          //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;          //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void EXTI0_IRQHandler(void)
{ 		    		    				     		    	
	EXTI->IMR &= ~(1<<0);   //关闭外部中断0，避免重复触发
	if(Check_WKUP())//关机?
	{	
		Sys_Standby();  
	}
	EXTI->IMR |= (1<<0);
	EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位
} 

u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	while(1)
	{
		if(KEY_UP)
		{
			t++;			//已经按下了 
			delay_ms(30);
			if(t>=100)		//按下超过3秒钟
			{
				return 1; 	//按下3s以上了
			}
		}else 
		{ 
			SystemInit();
			return 0; //按下不足3秒
		}
	}
} 

/******************************************************************
*函数功能  ：进入待机模式
*函数名    ：Sys_Standby
*函数参数  ：void
*函数返回值：void
*描述      ：
*             PA0作为外部中断0引脚
*******************************************************************/
void Sys_Standby(void)
{  
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 		 
}

void Sys_Stop(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);	  //进入停止模式 		 
}

