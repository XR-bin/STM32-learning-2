#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"

int main(void)
{				  
	SysTick_Init();	  	//—” ±≥ı ºªØ
  LED_Init();
	while(1)
	{
		LED0_ON;
		LED1_ON;
		delay_ms(100);
		LED0_OFF;
		LED1_OFF;
		delay_ms(100);
	}	 
}




