#ifndef __LED_H
#define __LED_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* 开灯 */
    #define LED0_ON   GPIO_ResetBits(GPIOF,GPIO_Pin_9)
    #define LED1_ON   GPIO_ResetBits(GPIOF,GPIO_Pin_10)
    /* 关灯 */
    #define LED0_OFF  GPIO_SetBits(GPIOF,GPIO_Pin_9)
    #define LED1_OFF  GPIO_SetBits(GPIOF,GPIO_Pin_10)



    /****************    函数外部声明   *****************/
    void LED_Init(void);         /* LED初始化 */

#endif








