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
* @funcName ：EXTI4_Init
* @brief    ：外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void EXTI4_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;        /* GPIOx配置结构体 */
    EXTI_InitTypeDef  EXTI_InitStruct;        /* 外部中断线配置结构体 */
    NVIC_InitTypeDef  NVIC_InitStruct;        /* 中断配置结构体 */

    /* 时钟使能  AFIO  GPIOE */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   /* 使能复用功能时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  /* GPIOE */

    /* GPIOx配置 */
    /* PE */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;       /* PE4端口配置 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    /* 上拉输入 */
    GPIO_Init(GPIOE, &GPIO_InitStruct);           /* 根据设定参数初始化GPIE PE4 */

    /* 外部中断配置 */
    /* 外部中断配置寄存器清零 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    /* 开始配置外部中断线 */
    EXTI_InitStruct.EXTI_Line    = EXTI_Line4;             /* 选择外部中断线4 */
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;    /* 中断事件 */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;   /* 下降沿触发    (EXTI_RTSR寄存器的第0位) */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;                 /* 使能4线的中断 (EXTI->IMR寄存器的第0位) */
    EXTI_Init(&EXTI_InitStruct);                           /* 初始化外部中断4 */

    /* 中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;          /* 外部中断4 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级1 */ 
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* 使能外部中断通道 */
    NVIC_Init(&NVIC_InitStruct);                           /* 配置中断优先级 */
}



/**********************************************************
* @funcName ：EXTI4_IRQHandler
* @brief    ：外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
    {
        delay_ms(20);
        if((GPIOE->IDR & (1<<4)) == 0)
        {
            GPIOB->ODR ^= (1<<5);     /* LED0翻转 */
            EXTI->PR |= (1<<4);       /* 写1清除中断标志 */
        }
    }
}












