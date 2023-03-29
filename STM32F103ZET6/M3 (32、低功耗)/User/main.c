#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "stdio.h"
#include "lcd.h"
#include "LowPower.h"

void delay_mm(u16 t)
{
	int i = t*2000;
	
	while(i--);
}

int main(void)
{	
	u32 num=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(115200);
	SysTick_Init();
	KEY_Init();
	LED_Init();
	LCD_Init();
	LowPower_Init();
	LED0_OFF;
	LED1_OFF;

	delay_mm(5000);
	
	LCD_Clear_XY(50,40,50,50,RED);

	while(1)
	{
		printf("num: %d\r\n", num);
		num++;
		LED1_OFF;
		delay_mm(5000);
		LED1_ON;
		delay_mm(5000);
		
		if(num == 6) Sys_Stop();   //进入停止模式
	}
}







