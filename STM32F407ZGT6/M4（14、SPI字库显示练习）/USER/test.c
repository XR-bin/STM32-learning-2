#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "exit.h"
#include "iic.h"
#include "at24cxx.h"
#include "delay.h"
#include "lcd.h"
#include "w25q64.h"

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
*         2、AT24C02的0~16用于存储LCD与触摸屏之间的校准系数
***************************************************************/

int main(void)
{ 
	u8 data;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	SysTick_Init();
	USART1_Init(115200);
	W25Q64_Init();
  LCD_Init();
  
	while(1)
	{
		data = 'a';
    while(data != 123)
    {
      LCD_Show_HZ16(100,100,&data,RED,WHITE);
      LCD_Show_HZ32(100,200,&data,RED,WHITE);
      data++;
      delay_ms(300);
    }
    data = 'A';
    while(data != 91)
    {
      LCD_Show_HZ16(100,100,&data,RED,WHITE);
      LCD_Show_HZ32(100,200,&data,RED,WHITE);
      data++;
      delay_ms(300);
    }
	}
}


