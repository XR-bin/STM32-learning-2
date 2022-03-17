#ifndef _LED_H
#define _LED_H

	#include "stm32f4xx.h"  

	//开灯宏定义
	#define LED0_ON GPIO_ResetBits(GPIOF,GPIO_Pin_6); 
	#define LED1_ON GPIO_ResetBits(GPIOF,GPIO_Pin_9);
	#define LED2_ON GPIO_ResetBits(GPIOF,GPIO_Pin_10);
	#define LED4_ON GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	//关灯宏定义
	#define LED0_OFF GPIO_SetBits(GPIOF,GPIO_Pin_6);
	#define LED1_OFF GPIO_SetBits(GPIOF,GPIO_Pin_9);
	#define LED2_OFF GPIO_SetBits(GPIOF,GPIO_Pin_10);
	#define LED4_OFF GPIO_SetBits(GPIOC,GPIO_Pin_0);

	void LED_Init(void);
	void LED_All_ON(void);
	void LED_All_OFF(void);
	void LED_X_OFF(u8 n);
	void LED_Flash1(void);
	void LED_Flash2(int n);
	void LED_Flash3(int n);

#endif







