#include "sys.h"

/*****************************************************
*函数功能  ：对DMA1的通道4进行初始化设置
*函数名    ：DMA1_Channels4_Init
*函数参数  ：void
*函数返回值：u16 ndtr,u8 *data 
*描述      ：
*           DMA将存储器的数据搬运到串口1外设
********************************************************/
void DMA1_Channels4_Init(u16 ndtr,u8 *data)
{
	DMA_InitTypeDef   DMA_InitStruct;    //DMA配置结构体
	
	//时钟使能  DMA1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1时钟使能
	
	//初始化通道，设置为默认配置
	DMA_DeInit(DMA1_Channel4);
		
	/*DMA配置*/
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;           //DMA外设地址：串口1(DMA_CPARx寄存器)
  DMA_InitStruct.DMA_MemoryBaseAddr = (u32)data;                      //DMA存储器0地址：定义一个数组地址(DMA_CMARx寄存器)
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //存储器到外设模式：数据从存储器向外设方向传输(DMA_CCRx寄存器的第4位)
  DMA_InitStruct.DMA_BufferSize = ndtr;                               //数据传输量(DMA_CNDTRx寄存器)
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //外设非增量模式：外设地址指针固定(DMA_CCRx寄存器的第6位)
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //存储器增量模式：每次数据传输后，存储器地址指针递增(DMA_CCRx寄存器的第7位)
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度8bit：每次从外设搬运8字节数据(DMA_CCRx寄存器的第8-9位)
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //存储器数据长度：每次从存储器运输8字节数据(DMA_CCRx寄存器的第10-11位)
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                          //循环模式选择：禁止循环模式(DMA_CCRx寄存器的第5位)
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                  //低优先级：中等优先级(DMA_CCRx寄存器的第12-13位)
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //非存储器到存储器模式(DMA_CCRx寄存器的第14位)
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);//初始化DMA
		
	//串口的DMA发送/接收功能使能
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);        //使能串口的发送功能(USART->CR3寄存器的第7位)  
	
	//数据流使能
	DMA_Cmd(DMA1_Channel4, ENABLE);      //(DMA_SxCR寄存器的第0位)
}
















