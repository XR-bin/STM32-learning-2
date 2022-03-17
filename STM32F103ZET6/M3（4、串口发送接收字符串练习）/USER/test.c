#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"

int main(void)
{			
	u8 string[256];
	
	SysTick_Init();	  	//—” ±≥ı ºªØ
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




