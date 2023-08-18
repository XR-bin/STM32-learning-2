#include "dma.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\dma.c
*              .\Drivers\BSP\inc\dma.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：DMA练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：DMA1_Usart1_Init
* @brief    ：对DMA1的通道4进行初始化设置(连接USART1的发送通道)
* @param    ：uint8_t* data (DMA存储器地址)
* @retval   ：void
* @details  ：
* @fn       ：
*            DMA1将存储器的数据搬运到串口1外设
************************************************************/
void DMA1_Usart1_Init(uint8_t* data)
{
    DMA_InitTypeDef   DMA_InitStruct;      /* DMA配置结构体 */

    /* 时钟使能  DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   /* DMA1时钟使能 */
    delay_ms(5);                                        /* 等待DMA时钟稳定 */

    /* 初始化通道，设置为默认配置 */
    DMA_DeInit(DMA1_Channel4);

    /* DMA配置 */
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;             /* DMA外设地址：串口1(DMA_CPARx寄存器) */
    DMA_InitStruct.DMA_MemoryBaseAddr     = (u32)data;                    /* DMA存储器0地址：定义一个数组地址(DMA_CMARx寄存器) */
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;        /* 存储器到外设模式：数据从存储器向外设方向传输(DMA_CCRx寄存器的第4位) */
    DMA_InitStruct.DMA_BufferSize         = 0;                            /* 数据传输量(DMA_CNDTRx寄存器) 先不设置 */
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;    /* 外设非增量模式：外设地址指针固定(DMA_CCRx寄存器的第6位) */
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;         /* 存储器增量模式：每次数据传输后，存储器地址指针递增(DMA_CCRx寄存器的第7位) */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /* 外设数据长度8bit：每次从外设搬运8字节数据(DMA_CCRx寄存器的第8-9位) */
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;      /* 存储器数据长度：每次从存储器运输8字节数据(DMA_CCRx寄存器的第10-11位) */
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;              /* 循环模式选择：禁止循环模式(DMA_CCRx寄存器的第5位) */
    DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;          /* 优先级：中等优先级(DMA_CCRx寄存器的第12-13位) */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                             /* 非存储器到存储器模式(DMA_CCRx寄存器的第14位) */
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);                             /* 初始化DMA */

    /* 串口的DMA发送/接收功能使能 */
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                        /* 使能串口的发送功能(USART->CR3寄存器的第7位) */
}



/**********************************************************
* @funcName ：DMA1_Enable
* @brief    ：开启DMA1数据传输
* @param    ：uint16_t cndtr (DMA传输数据量)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void DMA1_Enable(uint16_t cndtr)
{
    DMA_Cmd(DMA1_Channel4, DISABLE);         /* 关闭DMA传输 (DMA_CCR寄存器的第0位) */

    while (DMA1_Channel4->CCR & (1 << 0));   /* 确保DMA可以被设置 */
    DMA1_Channel4->CNDTR = cndtr;            /* DMA传输数据量 */

    DMA_Cmd(DMA1_Channel4, ENABLE);          /* 开启DMA传输 (DMA_CCR寄存器的第0位) */
}










