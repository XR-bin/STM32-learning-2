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
    ADC_InitTypeDef   ADC_InitStruct;         /* ADCx一般配置结构体 */

    /* 时钟使能  ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

    ADC_DeInit(ADC1);                    /* ADC1接口复位,将外设ADC1的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位) */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);    /* 对总线进行6分频12MHZ */

    /* ADCx初始化设置 */
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;       /* 独立模式(ADC_CR1寄存器的第16-19位) */
    ADC_InitStruct.ADC_ScanConvMode       = DISABLE;                    /* 非扫描(ADC_CR1寄存器的第8位) */
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                    /* 单次转换模式(ADC_CR2寄存器的第1位) */
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;  /* 触发方式：软件触发(ADC_CR2寄存器的第17-19位) */
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;        /* 数据对齐：右对齐(ADC_CR2寄存器的第11位) */
    ADC_InitStruct.ADC_NbrOfChannel       = 1;                          /* 有几个转换(ADC_SQR1寄存器的第20-23位) */
    ADC_Init(ADC1, &ADC_InitStruct);                                    /* ADC1初始化 */

    /* 开启内部温度传感器(ADC_CR2寄存器的第23位) */
    ADC_TempSensorVrefintCmd(ENABLE); 

    /* ADC规则通道序列偏序及采样时间配置函数 */
    /* ADC1的第一个转换的是通道16，采样时间239.5周期    (采样：ADCx->SMPRx) */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5); 

    /* ADC1使能函数 */
    ADC_Cmd(ADC1, ENABLE);

    /* 校准部分(这是必须的 )*/ 
    /* 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 */
    ADC_ResetCalibration(ADC1);                 /* 使能复位校准 */
    while(ADC_GetResetCalibrationStatus(ADC1)); /* 等待校准结束 */
    /* 该位由软件设置以开始校准，并在校准结束时由硬件清除 */
    ADC_StartCalibration(ADC1);                 /* 开启ADC校准 */
    while(ADC_GetCalibrationStatus(ADC1));      /* 等待校准结束 */
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
    uint16_t data;

    /* 开启规则通道转换开关(ADC_CR2寄存器的22位) */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    /* 等待转换完成(ADC_SR寄存器的第1位) */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    /* 读取转换后的数据(ADC_DR寄存器) */
    data = ADC_GetConversionValue(ADC1);

    return data;
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

