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
#include "touch.h"
#include "adc.h"
#include "time.h"
#include "pwm.h"
#include "capture.h"
#include "dma.h"
#include "rtc.h"

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
	RTC_DATES time = {19,12,10,6,12,20,20,0};
	RTC_TimeTypeDef Rtc_Time;
  RTC_DateTypeDef Rtc_Date;
	
  SysTick_Init();
  USART1_Init(115200);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  MYRTC_Init(time);
  while(1)
  {
		//这个RTC_Format_BIN有点特别和设置时间的库函数的RTC_Format_BIN的意思不一样
		//这里的RTC_Format_BIN是指把二进制数据转为十进制数据
		RTC_GetTime(RTC_Format_BIN,&Rtc_Time);         
		RTC_GetDate(RTC_Format_BIN, &Rtc_Date);
    printf("20%d年%d月%d日 星期%d\r\n",Rtc_Date.RTC_Year,Rtc_Date.RTC_Month,Rtc_Date.RTC_Date,Rtc_Date.RTC_WeekDay);
    printf("%d : %d : %d\r\n",Rtc_Time.RTC_Hours,Rtc_Time.RTC_Minutes,Rtc_Time.RTC_Seconds);
    delay_ms(1000);
  }
}


