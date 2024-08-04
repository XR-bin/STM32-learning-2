#include "beep.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\beep.c
*              .\Drivers\BSP\inc\beep.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：BEEP蜂鸣器驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：BEEP_Init
* @brief    ：对BEEP对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            BEEP     PF8
*            高电平响，低电平不响-----输出模式
* @fn       ：
************************************************************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;                    /* GPIOx配置结构体 */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); /* 使能GPIOF时钟 */

    /* 初始化蜂鸣器对应引脚GPIOF8 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;              /* PF8 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;           /* 普通输出模式 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;           /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;       /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;          /* 下拉 */
    GPIO_Init(GPIOF, &GPIO_InitStruct);                   /* 初始化GPIO */

    GPIO_ResetBits(GPIOF,GPIO_Pin_8);                     /* 蜂鸣器对应引脚GPIOF8拉低*/
}


