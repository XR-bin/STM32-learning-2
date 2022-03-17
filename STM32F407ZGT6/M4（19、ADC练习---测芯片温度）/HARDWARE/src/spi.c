#include "stm32f4xx.h"  
#include "stdio.h"

/******************************************************************
*函数功能  ：对SPI1通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
*函数名    ：SPI1_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           PB3   SPI1_SCK   时钟线  推挽 
*           PB4   SPI1_MISO  主输入  无上下拉
*           PB5   SPI1_MOSI  主输出  推挽
*******************************************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx配置结构体
	SPI_InitTypeDef   SPI_InitStruct;     //SPIx配置结构体
	
	//时钟使能  GPIOB  SPI1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/*GPIOx配置部分*/
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);//选择PB3/4/5	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                     //复用功能
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;                   //推挽输出
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;               //速度为100MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                           //根据设定参数初始化PB3/4/5

	//SPI1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
	
	/*配置SPI寄存器*/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工(SPI->CR1的第15、10位)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		                   //设置SPI工作模式:设置为主SPI(SPI->CR1的第2位)
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;		               //设置SPI的数据大小:SPI发送接收8位帧结构(SPI->CR1的第11位)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;		                       //串行同步时钟的空闲状态为低电平(SPI->CR1的第1位)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;	                       //从第一个时钟边沿开始采样数据(SPI->CR1的第0位)
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;		                       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制(SPI->CR1的第9位)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	 //定义波特率预分频的值:波特率预分频值为4(SPI->CR1的第3-5位)
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	                 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始(SPI->CR1的第7位)
	SPI_Init(SPI1, &SPI_InitStruct);                                 //初始化SPI1
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI1
}



/******************************************************************
*函数功能  ：SPI1接收/发送一个字节数据（8位）
*函数名    ：SPI1_RS_Byte
*函数参数  ：u8 data
*函数返回值：u8
*描述      ：
*           PB3   SPI1_SCK   时钟线
*           PB4   SPI1_MISO  主输入
*           PB5   SPI1_MOSI  主输出
*******************************************************************/
u8 SPI1_RS_Byte(u8 data)
{
  //判断发送缓存区是否为空   SET=1   RESET=0
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
	SPI_I2S_SendData(SPI1, data);
  //判断接收缓存区是否为空   SET=1   RESET=0
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
  data = SPI_I2S_ReceiveData(SPI1);
  
  return data;
}



