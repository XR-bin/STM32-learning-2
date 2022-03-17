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
	u8 key_flay;
	
	SysTick_Init();	  	//延时初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  LED_Init();
  KEY_Init();
  EXTI5_Swier_Init();
  
	while(1)
	{
    key_flay = KEY_Scan();
    if(key_flay == 2)
    {
      EXTI->SWIER |= (1<<5);
    }
  }
}




