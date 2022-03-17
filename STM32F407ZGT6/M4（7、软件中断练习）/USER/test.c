#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "exit.h"

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

int main(void)
{ 
	u8 key_flay;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  LED_Init();
	USART1_Init(9600);
	EXTI5_Swier_Init();
  KEY_Init();
  
	while(1)
	{
		key_flay = KEY_Scan();
    if(key_flay == 4)
    {
      EXTI_GenerateSWInterrupt(EXTI_Line5);  //(EXTI->SWIER寄存器)
    }
	}
}


