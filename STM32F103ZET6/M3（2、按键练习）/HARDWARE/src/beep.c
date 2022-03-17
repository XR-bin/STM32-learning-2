#include "sys.h"

/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：BEEP_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            BEEP     PB8
*            高电平响，低电平不响-----输出模式
********************************************************/
void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;                  //GPIOx配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
	//PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				     //PB8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					     //根据设定参数初始化PB8
	
	//端口输出数据寄存器配置   //beep的初始状态
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						             //PB8 输出低
}















