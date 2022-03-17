#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "exti.h"

int main(void)
{			
	SysTick_Init();	  	//延时初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  LED_Init();
  EXTI4_Init();
  
	while(1)
	{
    
  }
}




