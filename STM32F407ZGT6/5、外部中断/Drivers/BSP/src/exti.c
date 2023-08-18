#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：外部中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI0_Init
* @brief    ：外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY_UP--------PA0 高电平按下，低电平抬起
* @fn       ：
************************************************************/
void EXTI0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;   /* GPIOx配置结构体 */
    EXTI_InitTypeDef EXTI_InitStruct;   /* 外部中断配置结构体 */
    NVIC_InitTypeDef NVIC_InitStruct;   /* NVIC(中断)配置结构体 */

    /* 时钟使能     SYSCFG  GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    /* GPIOx配置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;       /* PA0 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;     /* 通用输入 */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL; /* 无上下拉 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; /* 速度为50MHz */
    GPIO_Init(GPIOA,&GPIO_InitStruct);             /* 根据设定参数初始化PA0 */

    /* 映射管脚 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); /* PA0 连接到中断线0(SYSCFG->EXTICR1寄存器) */

    /*中断配置*/
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;            /* 外部中断0 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* 使能外部中断通道 */
    NVIC_Init(&NVIC_InitStruct);                             /* 配置 */

    /* 外部中断线初始化 */
    EXTI_InitStruct.EXTI_Line    = EXTI_Line0;          /* 选择外部中断线0 */
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; /* 中断事件 */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; /* 上升沿触发    (EXTI_RTSR寄存器的第0位) */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              /* 使能0线的中断 (EXTI->IMR寄存器的第0位) */
    EXTI_Init(&EXTI_InitStruct);                        /* 初始化外部中断0 */
}



/**********************************************************
* @funcName ：EXTI0_IRQHandler
* @brief    ：外部中断0服务函数
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY_UP--------PA0 高电平按下，低电平抬起
* @fn       ：
************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
    {
        delay_ms(20);
        if((GPIOA->IDR & (1<<0)) == 0)
        {
            GPIOF->ODR ^= (1<<9);     /* LED0翻转 */
            EXTI->PR |= (1<<0);       /* 写1清除中断标志 */
        }
    }
}


