#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"

int main(void)
{			
	SysTick_Init();	  	//延时初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  LED_Init();
  KEY_Init();
	USART1_Init(9600);
  
	while(1)
	{
		LED0_ON;
		delay_ms(500);
		LED0_OFF;
		delay_ms(500);
	}
}




