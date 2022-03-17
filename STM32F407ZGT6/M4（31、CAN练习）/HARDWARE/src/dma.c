#include "stm32f4xx.h"      

/*****************************************************
*函数功能  ：对DMA2的数据流7通道4进行初始化设置
*函数名    ：DMA2_Stream7ch4_Init
*函数参数  ：void
*函数返回值：u16 ndtr,u16 psc  
*描述      ：
*           DMA将存储器的数据搬运到串口1外设
********************************************************/
void DMA2_Stream7ch4_Init(u16 ndtr,u8 *data)
{
	DMA_InitTypeDef   DMA_InitStruct;    //DMA配置结构体
	
	//时钟使能  DMA2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能
	
	//初始化数据流通道，设置为默认配置
	DMA_DeInit(DMA2_Stream7);
	//判断DMAx的数据流使能是否被关闭(必须关闭才能配置)
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}//等待DMA可配置 
		
	 /*DMA配置*/
  DMA_InitStruct.DMA_Channel = DMA_Channel_4;                         //通道选择：通道4(DMA_SxCR寄存器的第25-27位)
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;           //DMA外设地址：串口1(DMA->SxPAR寄存器)
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)data;                     //DMA存储器0地址：定义一个数组地址(DMA->SxM0AR寄存器)
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;                //存储器到外设模式：数据从存储器向外设方向传输(DMA_SxCR寄存器的第6位)
  DMA_InitStruct.DMA_BufferSize = ndtr;                               //数据传输量(DMA_SxCR寄存器)
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //外设非增量模式：外设地址指针固定(DMA_SxCR寄存器的第9位)
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //存储器增量模式：每次数据传输后，存储器地址指针递增(DMA_SxCR寄存器的第10位)
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度8bit：每次从外设搬运8字节数据(DMA_SxCR寄存器的第11-12位)
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //存储器数据长度：每次从存储器运输8字节数据(DMA_SxCR寄存器的第13-14位)
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                          //循环模式选择：禁止循环模式(DMA_SxCR寄存器的第8位)
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                  //低优先级：中等优先级(DMA_SxCR寄存器的第16-17位)
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;                 //直接模式选择：禁止直接模式(DMA_SxFCR寄存器的第2位)
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;          //FIFO 阈值选择： FIFO 完整容量(DMA_SxFCR寄存器的第0-1位)
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;            //存储器突发传输配置：存储器突发单次传输(DMA_SxCR寄存器的第23-24位)
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    //外设突发传输配置：外设突发单次传输(DMA_SxCR寄存器的第21-22位)
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);//初始化DMA Stream
		
	//串口的DMA发送/接收功能使能
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);        //使能串口的发送功能(USART->CR3寄存器的第7位)  
	
	//数据流使能
	DMA_Cmd(DMA2_Stream7, ENABLE);      //(DMA_SxCR寄存器的第0位)
}

















