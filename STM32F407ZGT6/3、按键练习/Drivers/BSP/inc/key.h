#ifndef __KEY_H
#define __KEY_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    #define KEY0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)         /* 低电平按下 */
    #define KEY1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)         /* 低电平按下 */
    #define KEY2    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)         /* 低电平按下 */
    #define KEY_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)         /* 高电平按下 */



    /****************    函数外部声明   *****************/
    void KEY_Init(void);         /* 按键初始化 */
    uint8_t KEY_Scan(void);      /* 按键扫描函数 */

#endif




