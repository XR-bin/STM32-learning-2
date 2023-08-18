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
* @funcName ：ADC3_In6_Init
* @brief    ：ADC3的第6个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
*             PF8----------ADC3_IN6   模拟模式
* @fn       ：
************************************************************/
void ADC3_In6_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx配置结构体 */
    ADC_InitTypeDef   ADC_InitStruct;      /* ADCx一般配置结构体 */

    /* 时钟使能  GPIOF  ADC3 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;         /* PF8端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;      /* 模拟输入引脚 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   /* IO口速度为50MHz */
    GPIO_Init(GPIOF, &GPIO_InitStruct);              /* 根据设定参数初始化PF8 */

    ADC_DeInit(ADC3);                                /* ADC3接口复位,将外设ADC3的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                /* 对总线进行6分频12MHZ */

    /*ADCx初始化设置*/
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

    /* ADC3使能函数 */
    ADC_Cmd(ADC3, ENABLE);

    /* 校准部分(这是必须的) */ 
    /* 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 */
    ADC_ResetCalibration(ADC3);                 /* 使能复位校准 */
    while(ADC_GetResetCalibrationStatus(ADC3)); /* 等待校准结束 */
    /* 该位由软件设置以开始校准，并在校准结束时由硬件清除 */
    ADC_StartCalibration(ADC3);                 /* 开启AD校准 */
    while(ADC_GetCalibrationStatus(ADC3));      /* 等待校准结束 */
}



/**********************************************************
* @funcName ：ADC3_In6_Data
* @brief    ：获取ADC3通道6的转换数据
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t ADC3_In6_Data(void)
{
    uint16_t data;

    /* 开启规则通道转换开关(ADC_CR2寄存器的22位) */
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    /* 等待转换完成(ADC_SR寄存器的第1位) */
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
    /* 读取转换后的数据(ADC_DR寄存器) */
    data = ADC_GetConversionValue(ADC3);

    return data;
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
        sum+=ADC3_In6_Data();
    }

    return (sum>>4);
}



