#include "pwm.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\pwm.c
*              .\Drivers\BSP\inc\pwm.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：pwm练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：TIM14_PWM_Init
* @brief    ：对通用定时器14进行初始化设置并输出PWM
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @param    ：uint16_t ccr (比较值/占空比)
* @retval   ：void
* @details  ：
*            PWM输出口  PF9   TIM14_CH1  输出通道1
* @fn       ：
*            有效电平为高电平
*            定时器向上计数
*            先输出有效电平后输出无效电平
************************************************************/
void TIM14_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    GPIO_InitTypeDef        GPIO_InitStruct;             /* GPIOx配置结构体 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      /* TIMx配置结构体 */
    TIM_OCInitTypeDef       TIM_OCInitStruct;            /* 通道配置结构体 */

    /* 使能时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);    /* TIM14时钟使能 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   /* 使能GPIOF时钟 */

    /* 复用 */
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);  /* PF9复用为定时器14 */

    /* 引脚配置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;        /* PF9 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;      /* 复用功能 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 速度100MHz */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* 推挽复用输出 */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* 上拉 */
    GPIO_Init(GPIOF,&GPIO_InitStruct);              /* 初始化PF9 */

    /* 定时器配置 */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                /* 定时器分频(TIMx->PSC寄存器)*/
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; /* 向上计数模式(TIMx->CR1寄存器的第4位)(通用定时器12只有递增计数) */
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                /* 自动重装载值(TIMx->ARR寄存器) */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;       /* 时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位) */
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);               /* 初始化定时器14 */

    /* 初始化TIM14 Channel1 PWM模式 */
    TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;        /* 配置为PWM模式1(TIMx_CCMR1寄存器的输出模式的第4-6位) */
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /* 通道选择输出模式(TIMx->CCER寄存器的第0位) */
    TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;     /* 设置有效电平为低电平(TIMx->CCER寄存器的第1位) */
    TIM_OC1Init(TIM14, &TIM_OCInitStruct);                     /* 根据T指定的参数初始化外设TIM14OC1 */

    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  /* 使能TIM14在CCR1上的预装载寄存器 */
    TIM_ARRPreloadConfig(TIM14,ENABLE);                 /* ARR使能 */
    TIM_Cmd(TIM14, ENABLE);                             /* 使能TIM14 */
}



