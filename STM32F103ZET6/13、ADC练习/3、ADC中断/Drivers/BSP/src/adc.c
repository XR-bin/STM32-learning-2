#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC中断练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
****************************************/



/**********************************************************
* @funcName ：ADC3_In6_Init
* @brief    ：ADC3的第6个通道初始化配置(带ADC中断)
* @param    ：void
* @retval   ：void
* @details  ：
*             PF8----------ADC3_IN6   模拟模式
* @fn       ：
************************************************************/
void ADC3_In6_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;      /* GPIOx配置结构体 */
    ADC_InitTypeDef   ADC_InitStruct;       /* ADCx一般配置结构体 */
    NVIC_InitTypeDef  NVIC_InitStruct;      /* NVIC(中断)配置结构体 */

    /* 时钟使能  GPIOF  ADC3 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;        /* PF8端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;     /* 模拟输入引脚 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  /* IO口速度为50MHz */
    GPIO_Init(GPIOF, &GPIO_InitStruct);             /* 根据设定参数初始化PF8 */

    ADC_DeInit(ADC3);                    /* ADC3接口复位,将外设ADC3的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);    /* 对总线进行6分频12MHZ */

    /* ADCx初始化设置 */
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;       /* 独立模式(ADC_CR1寄存器的第16-19位) */
    ADC_InitStruct.ADC_ScanConvMode       = DISABLE;                    /* 非扫描(ADC_CR1寄存器的第8位) */
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                    /* 单次转换模式(ADC_CR2寄存器的第1位) */
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  /* 触发方式：软件触发(ADC_CR2寄存器的第17-19位) */
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;        /* 数据对齐：右对齐(ADC_CR2寄存器的第11位) */
    ADC_InitStruct.ADC_NbrOfChannel       = 1;                          /* 有几个转换(ADC_SQR1寄存器的第20-23位) */
    ADC_Init(ADC3, &ADC_InitStruct);                                    /* ADC3初始化 */

    /* ADC规则通道序列偏序及采样时间配置函数 */
    /* ADC3的第一个转换的是通道6，采样时间239.5周期    (采样：ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5); 

    /* NVIC配置 */
    /* 选择中断方式并使能该方式中断 */
    ADC_ITConfig(ADC3,ADC_IT_EOC, ENABLE);

    /* ADC中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = ADC3_IRQn;             /* 选择ADC3中断 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* IRQ通道使能 */
    NVIC_Init(&NVIC_InitStruct);                             /* ADC3中断初始化 */

    /* ADC3使能函数 */
    ADC_Cmd(ADC3, ENABLE);

    /* 校准部分(这是必须的) */ 
    /* 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 */
    ADC_ResetCalibration(ADC3);                 /* 使能复位校准 */
    while(ADC_GetResetCalibrationStatus(ADC3)); /* 等待校准结束 */
    /* 该位由软件设置以开始校准，并在校准结束时由硬件清除 */
    ADC_StartCalibration(ADC3);                 /* 开启ADC校准 */
    while(ADC_GetCalibrationStatus(ADC3));      /* 等待校准结束 */

    /* 开启规则通道转换开关(ADC_CR2寄存器的22位) */
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}



/**********************************************************
* @funcName ：ADC3_IRQHandler
* @brief    ：ADC3中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void ADC3_IRQHandler(void)
{
    uint16_t data;

    if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)
    { 
        /* 读取转换后的数据 */
        data = ADC_GetConversionValue(ADC3);
        printf("data:%d\r\n",data);
    }

    /* 清除标志 */
    ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
}





