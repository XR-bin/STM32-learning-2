#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"

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
	u8 string[256];
  
  LED_Init();
  KEY_Init();
	USART1_Init(9600);
  
	while(1)
	{
		USART1_Receive_Str(string);
    //USART1_Send_Str(string);
    printf("%s\r\n",string);
	}
}


