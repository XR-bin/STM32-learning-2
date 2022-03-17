#ifndef _KEY_H
#define _KEY_H

  #include "stm32f4xx.h"  

  #define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
  #define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
	#define KEY3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
	#define KEY4 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
  
  void KEY_Init(void);
  u8 KEY_Scan(void);
  u8 KEY_Scan2(void);
  
#endif











