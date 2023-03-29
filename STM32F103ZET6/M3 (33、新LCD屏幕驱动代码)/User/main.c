#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"

int main(void)
{	
	SysTick_Init();
	LED_Init();
	LCD_Init();
	LED0_OFF;
	LED1_OFF;

	delay_ms(5000);
	
	LCD_Clear_XY(50,40,50,50,RED);

	while(1)
	{
		LED1_OFF;
		delay_ms(500);
		LED1_ON;
		delay_ms(500);
	}
}







