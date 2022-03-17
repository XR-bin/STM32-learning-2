#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"

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
	
  LED_Init();
	
  while(1)
  {
    LED_Flash1();
  }
}


