#include "stm32f4xx.h"   
#include "stdio.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

/******************************************
*函数名    ：ADC1_Init_In5
*函数功能  ：ADC1的第五个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA5----------ADC12_IN5   模拟模式
*********************************************/
void ADC1_Init_In5(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx结构体配置
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCx一般配置结构体
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx初始化配置结构体
  
  //时钟使能  GPIOA  ADC1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
  /*GPIOx端口配置*/
  //PA5
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_5;         //选择PA5
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //模拟输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //根据设定参数初始化PA5
	
	/*ADCx寄存器配置*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC->CCR寄存器的第0-4位)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟(ADC->CCR寄存器的第8-11位)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMA失能(ADC->CCR寄存器的第14-15位)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR寄存器的第16-17位)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCx一般配置初始化(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12位模式(ADC->CR1寄存器的第24、25位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //非扫描模式(ADC->CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换(ADC->CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发(ADC->CR2寄存器的第20-21位/第28-29位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐	(ADC->CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1(ADC->SQR1寄存器的第20-23位)
  ADC_Init(ADC1, &ADC_InitStruct);                                         //ADCx配置初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC1的第一个转换时通道5，采样时间480个周期        (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC1, ENABLE);//使能ADCx，并开启AD转换器	
}

/******************************************
*函数名    ：ADC1_In5_Data
*函数功能  ：获取ADC1通道5的转换数据
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换可调电阻数据
*********************************************/
u16 ADC1_In5_Data(void)
{
  u16 data;
  
  //开启规则通道转换开关(ADC1->CR2寄存器的第30位)
	ADC_SoftwareStartConv(ADC1);    
  //等待转换完成
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//等待转换结束(ADC1->SR寄存器的第1位)
  //读取转换后的数据(ADC1->DR)
  data = ADC_GetConversionValue(ADC1);   
  
  return data;
}

/******************************************
*函数名    ：ADC2_Init_In4
*函数功能  ：ADC2的第4个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA4----------ADC2_IN4   模拟模式
*********************************************/
void ADC2_Init_In4(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx结构体配置
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCx一般配置结构体
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx初始化配置结构体
  
  //时钟使能  GPIOA  ADC2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); 
	
  /*GPIOx端口配置*/
  //PA4
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_4;         //选择PA4
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //模拟输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //根据设定参数初始化PA4
	
	/*ADCx寄存器配置*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC->CCR寄存器的第0-4位)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟(ADC->CCR寄存器的第8-11位)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMA失能(ADC->CCR寄存器的第14-15位)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR寄存器的第16-17位)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCx一般配置初始化(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12位模式(ADC->CR1寄存器的第24、25位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //非扫描模式(ADC->CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换(ADC->CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发(ADC->CR2寄存器的第20-21位/第28-29位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐	(ADC->CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1(ADC->SQR1寄存器的第20-23位)
  ADC_Init(ADC2, &ADC_InitStruct);                                         //ADCx配置初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC2的第一个转换时通道4，采样时间480个周期        (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC2, ENABLE);//使能ADCx，并开启AD转换器	
}

/******************************************
*函数名    ：ADC2_In4_Data
*函数功能  ：获取ADC2通道4的转换数据
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换光敏电阻数据
*********************************************/
u16 ADC2_In4_Data(void)
{
	u16 data;
  
  //开启规则通道转换开关(ADC2->CR2寄存器的第30位)
	ADC_SoftwareStartConv(ADC2);    
  //等待转换完成
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC )){};//等待转换结束(ADC2->SR寄存器的第1位)
  //读取转换后的数据(ADC1->DR)
  data = ADC_GetConversionValue(ADC2);   
  
  return data;
}











