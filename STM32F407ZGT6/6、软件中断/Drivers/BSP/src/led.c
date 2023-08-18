#include "led.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\led.c
*              .\Drivers\BSP\inc\led.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：LED灯驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：LED_Init
* @brief    ：对LED对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            LED0     PF9
*            LED1     PF10
*            高电平灭，低电平亮-----输出模式
* @fn       ：
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;                    /* GPIOx配置结构体 */

    /* 端口时钟使能   GPIOF */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   /* 使能GPIOF时钟 */

    /* GPIOF9,F10初始化设置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;  /* LED0和LED1对应IO口 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;             /* 普通输出模式 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;             /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;         /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;              /* 上拉 */
    GPIO_Init(GPIOF, &GPIO_InitStruct);                     /* 初始化GPIO */

    GPIO_SetBits(GPIOF, GPIO_Pin_9|GPIO_Pin_10);            /* GPIOF9,F10设置高，灯灭 */
} 








