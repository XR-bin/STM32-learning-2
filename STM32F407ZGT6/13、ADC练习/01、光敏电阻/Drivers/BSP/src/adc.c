#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC配置代码
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
* @funcName ：ADC3_In5_Init
* @brief    ：ADC3的第5个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
*             PF7----------ADC3_IN5   模拟模式
* @fn       ：
************************************************************/
void ADC3_In5_Init(void)
{
    GPIO_InitTypeDef      GPIO_InitStruct;       /* GPIOx配置结构体 */
    ADC_CommonInitTypeDef ADC_CommonInitStruct;  /* ADCx一般配置结构体 */
    ADC_InitTypeDef       ADC_InitStruct;        /* ADCx初始化配置结构体 */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); /* 使能GPIOF时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  /* 使能ADC3时钟 */

    /* PF7 */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;        /* PF7 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;      /* 模拟输入 */
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ; /* 不带上下拉 */
    GPIO_Init(GPIOF, &GPIO_InitStruct);            /* GPIO初始 */
    /* ADC配置 */
    ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;         /* 独立模式(ADC->CCR寄存器的第0-4位) */
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* 两个采样阶段之间的延迟5个时钟(ADC->CCR寄存器的第8-11位) */
    ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;   /* DMA失能(ADC->CCR寄存器的第14-15位) */
    ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div4;           /* 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR寄存器的第16-17位) */
    ADC_CommonInit(&ADC_CommonInitStruct);                                    /* ADCx一般配置初始化(ADC->CCR) */

    ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;             /* 12位模式(ADC->CR1寄存器的第24、25位) */
    ADC_InitStruct.ADC_ScanConvMode         = DISABLE;                        /* 非扫描模式(ADC->CR1寄存器的第8位) */
    ADC_InitStruct.ADC_ContinuousConvMode   = DISABLE;                        /* 关闭连续转换(ADC->CR2寄存器的第1位) */
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  /* 禁止触发检测，使用软件触发(ADC->CR2寄存器的第20-21位/第28-29位) */
    ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;            /* 右对齐 (ADC->CR2寄存器的第11位) */
    ADC_InitStruct.ADC_NbrOfConversion      = 1;                              /* 1个转换在规则序列中 也就是只转换规则序列1(ADC->SQR1寄存器的第20-23位) */
    ADC_Init(ADC3, &ADC_InitStruct);                                          /* ADCx配置初始化 */

    /* ADC规则通道序列偏序及采样时间配置函数 */
    /* ADC3的第一个转换时通道5，采样时间480个周期 (采样：ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1,ADC_SampleTime_480Cycles); 

    ADC_Cmd(ADC3, ENABLE);    /* 开启AD转换器 */
}



/**********************************************************
* @funcName ：ADC3_In5_Data
* @brief    ：获取ADC3通道5的转换数据
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t ADC3_In5_Data(void)
{
    ADC_SoftwareStartConv(ADC3);                     /* 启动规则转换通道 */
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));  /* 等待转换结束 */
    return ADC_GetConversionValue(ADC3);             /* 返回最近一次ADC3规则组的转换结果 */
}



/**********************************************************
* @funcName ：Oversampling
* @brief    ：ADC过采样算法，提高分辨率，由12分辨率变为16分辨率
* @param    ：void
* @retval   ：uint32_t --- ADC的16分辨率数据
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t Oversampling(void)
{
    int i;
    uint32_t sum=0;

    for(i=0; i<256; i++)
    {
        sum+=ADC3_In5_Data();
    }

    return (sum>>4);
}









