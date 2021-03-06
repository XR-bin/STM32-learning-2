#ifndef _TOUCH_H
#define _TOUCH_H

  #include "sys.h"

  //片选线
  #define T_CS_H    GPIO_SetBits(GPIOF,GPIO_Pin_11)
  #define T_CS_L    GPIO_ResetBits(GPIOF,GPIO_Pin_11)
  //时钟线
  #define T_SCK_H   GPIO_SetBits(GPIOB,GPIO_Pin_1)
  #define T_SCK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_1);
  //输出线
  #define T_MOSI_H  GPIO_SetBits(GPIOF,GPIO_Pin_9)
  #define T_MOSI_L  GPIO_ResetBits(GPIOF,GPIO_Pin_9);
  //输入线
  #define T_MISO    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
  //笔接触线
  #define T_PEN     GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)
  
  //指令
  #define TOUCH_X   0xD0    //获取x坐标数据
  #define TOUCH_Y   0x90    //获取y坐标数据
  
  void Touch_Init(void);
  u16 Touch_Data(u8 command);

#endif















