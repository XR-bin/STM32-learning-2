#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：软件中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI5_Swier_Init
* @brief    ：软件外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            无需端口
* @fn       ：
************************************************************/
void EXTI5_Swier_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(中断)配置结构体 */

    /* 外部中断配置 */
    /* 软件中断事件寄存器 */
    EXTI->SWIER &= ~(1<<5);

    /* 中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;          /* 外部中断5 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* 使能外部中断通道 */
    NVIC_Init(&NVIC_InitStruct);                             /* 配置外部中断 */

    /* 中断屏蔽寄存器 */
    EXTI->IMR |= (1<<5);
}



/**********************************************************
* @funcName ：EXTI9_5_IRQHandler
* @brief    ：外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
    {
        GPIOB->ODR ^= (1<<5);            /* LED0翻转 */
        EXTI->PR |= (1<<5);              /* 写1清除中断标志 */
    }
}







