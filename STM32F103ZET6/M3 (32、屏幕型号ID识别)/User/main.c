#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "TFTlcd.h"
#include "usart.h"
#include "stdio.h"




int main(void)
{
	u16 id=0;

	SysTick_Init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	USART1_Init(115200);
	TFTLCD_Init();			//LCD初始化	
	delay_us(5);
	id = TFTLCD_Id();
	
	printf("id = %d , %x",id,id);
	
	

	while(1);
}


