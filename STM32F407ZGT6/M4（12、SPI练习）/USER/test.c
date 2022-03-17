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
	u8 str[] = "momo没哈哈";
  u8 str2[30]; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	SysTick_Init();
	USART1_Init(9600);
	
	W25Q64_Init();
  W25Q64_Sector_Erase(0x5);
  W25Q64_Page_Write(0x1,sizeof(str),str);
  W25Q64_Read_Bytes(0x1,sizeof(str),str2);
  
	while(1)
	{
		printf("str2 : %s\r\n",str2);
    delay1_ms(500);
	}
}


