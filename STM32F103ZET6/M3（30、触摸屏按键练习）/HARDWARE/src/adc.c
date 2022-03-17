#include "sys.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

/******************************************
*函数名    ：ADC3_Init_In6
*函数功能  ：ADC1的第五个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PF8----------ADC3_IN6   模拟模式
*********************************************/
void ADC3_Init_In6(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
	ADC_InitTypeDef   ADC_InitStruct;      //ADCx一般配置结构体
	
	//时钟使能  GPIOF  ADC3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;          //PF8端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //模拟输入引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					    //根据设定参数初始化PF8
	
	ADC_DeInit(ADC3);                    //ADC3接口复位,将外设ADC3的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //对总线进行6分频12MHZ

	/*ADCx初始化设置*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC_CR1寄存器的第16-19位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //非扫描(ADC_CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //单次转换模式(ADC_CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //触发方式：软件触发(ADC_CR2寄存器的第17-19位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //数据对齐：右对齐(ADC_CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //有几个转换(ADC_SQR1寄存器的第20-23位)
  ADC_Init(ADC3, &ADC_InitStruct);                                   //ADC1初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC3的第一个转换的是通道6，采样时间71.5周期    (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_71Cycles5); 
	
	//ADC3使能函数
  ADC_Cmd(ADC3, ENABLE);
	
	/*校准部分(这是必须的)*/ 
  //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。
  ADC_ResetCalibration(ADC3);                 //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC3)); //等待校准结束
  //该位由软件设置以开始校准，并在校准结束时由硬件清除
  ADC_StartCalibration(ADC3);                 //开启ADC校准
  while(ADC_GetCalibrationStatus(ADC3));      //等待校准结束
}

/******************************************
*函数名    ：ADC3_In6_Data
*函数功能  ：获取ADC3通道6的转换数据
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换光敏电阻数据
*********************************************/
u16 ADC3_In6_Data(void)
{
	u16 data;
	
	//开启规则通道转换开关(ADC_CR2寄存器的22位)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		
	//等待转换完成(ADC_SR寄存器的第1位)
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
	//读取转换后的数据(ADC_DR寄存器)
	data = ADC_GetConversionValue(ADC3);
	
	return data;
}

/******************************************
*函数功能  ：ADC1的第十六个通道初始化配置
*函数名    ：ADC1_Init_In16
*函数参数  ：void
*函数返回值：void 
*函数描述  ：无需引脚，用于测芯片温度
*********************************************/
void ADC1_Init_In16(void)
{
	ADC_InitTypeDef   ADC_InitStruct;      //ADCx一般配置结构体
	
	//时钟使能  ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADC_DeInit(ADC1);                    //ADC1接口复位,将外设ADC1的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //对总线进行6分频12MHZ

	/*ADCx初始化设置*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC_CR1寄存器的第16-19位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //非扫描(ADC_CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //单次转换模式(ADC_CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //触发方式：软件触发(ADC_CR2寄存器的第17-19位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //数据对齐：右对齐(ADC_CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //有几个转换(ADC_SQR1寄存器的第20-23位)
  ADC_Init(ADC1, &ADC_InitStruct);                                   //ADC1初始化
	
	//开启内部温度传感器(ADC_CR2寄存器的第23位)
	ADC_TempSensorVrefintCmd(ENABLE); 
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC1的第一个转换的是通道16，采样时间71.5周期    (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_71Cycles5); 
	
	//ADC1使能函数
  ADC_Cmd(ADC1, ENABLE);
	
	/*校准部分(这是必须的)*/ 
  //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。
  ADC_ResetCalibration(ADC1);                 //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC1)); //等待校准结束
  //该位由软件设置以开始校准，并在校准结束时由硬件清除
  ADC_StartCalibration(ADC1);                 //开启ADC校准
  while(ADC_GetCalibrationStatus(ADC1));      //等待校准结束
}

/******************************************
*函数功能  ：获取ADC1通道16的转换数据
*函数名    ：ADC1_In16_Data
*函数参数  ：void
*函数返回值：u16
*函数描述  ：检测芯片温度
*********************************************/
u16 ADC1_In16_Data(void)
{
	u16 data;
	
	//开启规则通道转换开关(ADC_CR2寄存器的22位)
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
	//等待转换完成(ADC_SR寄存器的第1位)
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//读取转换后的数据(ADC_DR寄存器)
	data = ADC_GetConversionValue(ADC1);
	
	return data;
}

/******************************************
*函数功能  ：对ADC1通道16转换的数据进行处理
*函数名    ：ADC1_Chip_Temperature
*函数参数  ：void
*函数返回值：float
*函数描述  ：用于检测芯片温度
*********************************************/
double ADC1_Chip_Temperature(void)
{
  u8 i;
  u32 data;
  float sum = 0;
  double stm_t[6];
  
  for(i=0;i<5;i++)
  {
    data = ADC1_In16_Data();
    stm_t[i]=(float)data*(3.3/4096);		//电压值 
	  stm_t[i]=(1.43-stm_t[i])/0.0043+25;	//转换为温度值 	
    sum += stm_t[i];
  }
  
  stm_t[5] = sum/5;
  
  return stm_t[5];
}

/******************************************
*函数名    ：ADC3_Init2_In6
*函数功能  ：ADC1的第五个通道初始化配置(有用到中断)
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PF8----------ADC3_IN6   模拟模式
*********************************************/
void ADC3_Init2_In6(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
	ADC_InitTypeDef   ADC_InitStruct;      //ADCx一般配置结构体
	NVIC_InitTypeDef  NVIC_InitStruct;     //NVIC(中断)配置结构体
	
	//时钟使能  GPIOF  ADC3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;          //PF8端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //模拟输入引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStruct);					    //根据设定参数初始化PF8
	
	ADC_DeInit(ADC3);                    //ADC3接口复位,将外设ADC3的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //对总线进行6分频12MHZ

	/*ADCx初始化设置*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC_CR1寄存器的第16-19位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //非扫描(ADC_CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //单次转换模式(ADC_CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //触发方式：软件触发(ADC_CR2寄存器的第17-19位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //数据对齐：右对齐(ADC_CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //有几个转换(ADC_SQR1寄存器的第20-23位)
  ADC_Init(ADC3, &ADC_InitStruct);                                   //ADC1初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC3的第一个转换的是通道6，采样时间71.5周期    (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_71Cycles5); 
	
	/*NVIC配置*/
	//选择中断方式并使能该方式中断
	ADC_ITConfig(ADC3,ADC_IT_EOC, ENABLE);
	
	//ADC中断配置
  NVIC_InitStruct.NVIC_IRQChannel = ADC3_IRQn;             //选择串口1中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct);                             //ADC3中断初始化
	
	//ADC3使能函数
  ADC_Cmd(ADC3, ENABLE);
	
	/*校准部分(这是必须的)*/ 
  //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。
  ADC_ResetCalibration(ADC3);                 //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC3)); //等待校准结束
  //该位由软件设置以开始校准，并在校准结束时由硬件清除
  ADC_StartCalibration(ADC3);                 //开启ADC校准
  while(ADC_GetCalibrationStatus(ADC3));      //等待校准结束
	
	//开启规则通道转换开关(ADC_CR2寄存器的22位)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}








