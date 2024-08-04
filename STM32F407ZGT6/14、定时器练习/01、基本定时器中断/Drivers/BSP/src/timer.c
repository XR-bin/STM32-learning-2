#include "timer.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\timer.c
*              .\Drivers\BSP\inc\timer.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：基本定时器中断练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：TIM6_Init
* @brief    ：对定时器6进行初始化设置
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
    NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体

    /* 时钟使能  TIM6 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  

    /* TIMx配置 */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                /* 自动重装载值(TIMx->ARR寄存器) */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                /* 定时器分频(TIMx->PSC寄存器) */
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; /* 向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;       /* 时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个) */
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);                /* 初始化TIM6 */

    /* TIMx中断选择并使能 */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);  /* 允许定时器6更新中断 */

    /* TIM6中断配置-----NVIC 配置 */
    NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;        /* TIM6中断通道 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;         /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            /* IRQ通道使能 */
    NVIC_Init(&NVIC_InitStruct);                            /* 根据指定的参数初始化VIC寄存器 */

    TIM_Cmd(TIM6,ENABLE);  /* 使能定时器6 */
}



/**********************************************************
* @funcName ：TIM6_DAC_IRQn
* @brief    ：定时器6的中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_DAC_IRQHandler(void)
{
    /* 判断是否定时器6中断 */
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
    {
        /* 清中断标志位 */
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        /* 紧急事件 */
        GPIOF->ODR ^= (1<<10);     /* LED1闪烁 */
    }
}



