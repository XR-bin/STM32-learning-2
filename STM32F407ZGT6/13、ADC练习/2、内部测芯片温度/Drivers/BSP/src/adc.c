#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC测芯片内部温度代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/*****************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
******************************************/



/**********************************************************
* @funcName ：ADC1_In16_Init
* @brief    ：ADC1的第16个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            无需引脚，用于测芯片温度
************************************************************/
void ADC1_In16_Init(void)
{
    ADC_CommonInitTypeDef   ADC_CommonInitStruct;   /* ADCx一般配置结构体 */
    ADC_InitTypeDef         ADC_InitStruct;         /* ADCx初始化配置结构体 */

    /* 时钟使能   ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器(ADC->CCR寄存器的第23位)

    /* ADCx寄存器配置 */
    ADC_CommonInitStruct.ADC_Mode             = ADC_Mode_Independent;         /* 独立模式(ADC->CCR寄存器的第0-4位) */
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* 两个采样阶段之间的延迟5个时钟(ADC->CCR寄存器的第8-11位) */
    ADC_CommonInitStruct.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;   /* DMA失能(ADC->CCR寄存器的第14-15位) */
    ADC_CommonInitStruct.ADC_Prescaler        = ADC_Prescaler_Div4;           /* 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR寄存器的第16-17位) */
    ADC_CommonInit(&ADC_CommonInitStruct);                                    /* ADCx一般配置初始化(ADC->CCR) */

    ADC_InitStruct.ADC_Resolution           = ADC_Resolution_12b;             /* 12位模式(ADC->CR1寄存器的第24、25位) */
    ADC_InitStruct.ADC_ScanConvMode         = DISABLE;                        /* 非扫描模式(ADC->CR1寄存器的第8位) */
    ADC_InitStruct.ADC_ContinuousConvMode   = DISABLE;                        /* 关闭连续转换(ADC->CR2寄存器的第1位) */
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  /* 禁止触发检测，使用软件触发(ADC->CR2寄存器的第20-21位/第28-29位) */
    ADC_InitStruct.ADC_DataAlign            = ADC_DataAlign_Right;            /* 右对齐	(ADC->CR2寄存器的第11位) */
    ADC_InitStruct.ADC_NbrOfConversion      = 1;                              /* 1个转换在规则序列中 也就是只转换规则序列1(ADC->SQR1寄存器的第20-23位) */
    ADC_Init(ADC1, &ADC_InitStruct);                                          /* ADCx配置初始化 */

    /* ADC规则通道序列偏序及采样时间配置函数 */
    /* ADC1的第一个转换时通道16，采样时间480个周期 (采样：ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_480Cycles);  

    ADC_Cmd(ADC1, ENABLE);  /* 使能ADCx，并开启AD转换器 */
}



/**********************************************************
* @funcName ：ADC1_In16_Data
* @brief    ：获取ADC1通道16的转换数据
* @param    ：void
* @retval   ：uint16_t
* @details  ：
* @fn       ：
*            检测芯片温度
************************************************************/
uint16_t ADC1_In16_Data(void)
{
    /* 开启规则通道转换开关(ADC1->CR2寄存器的第30位) */
    ADC_SoftwareStartConv(ADC1);
    /* 等待转换完成 */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){};/* 等待转换结束(ADC1->SR寄存器的第1位) */
    /* 读取转换后的数据(ADC1->DR) */
    return ADC_GetConversionValue(ADC1);   
}



/**********************************************************
* @funcName ：Oversampling
* @brief    ：ADC过采样算法，提高分辨率，由12分辨率变为16分辨率
* @param    ：void
* @retval   ：uint16_t  ---  (ADC的16分辨率数据)
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
        sum+=ADC1_In16_Data();
    }

    return (sum>>4);
}



/**********************************************************
* @funcName ：ADC1_Chip_Temperature1
* @brief    ：将ADC值转换为芯片温度值(非过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature1(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = ADC1_In16_Data();
        stm_t[i]=(float)data*(3.3/4096);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



/**********************************************************
* @funcName ：ADC1_Chip_Temperature2
* @brief    ：将ADC值转换为芯片温度值(过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature2(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = Oversampling();
        stm_t[i]=(float)data*(3.3/65536);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



