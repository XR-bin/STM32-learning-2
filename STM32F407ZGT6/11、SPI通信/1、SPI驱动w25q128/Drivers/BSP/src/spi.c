#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\spi.c
*              .\Drivers\BSP\inc\spi.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：SPI配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：SPI1_Init
* @brief    ：对SPI1通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
* @param    ：void
* @retval   ：void
* @details  ：
*            PB3   SPI1_SCK   时钟线  复用推挽
*            PB4   SPI1_MISO  主输入  复用推挽
*            PB5   SPI1_MOSI  主输出  复用推挽
*            极性：SCK为低电平为空闲状态
*            相位：数据采样从第一个时钟边沿开始
* @fn       ：
************************************************************/
void SPI1_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx配置结构体 */
    SPI_InitTypeDef    SPI_InitStruct;     /* SPI配置结构体 */

    /* 时钟使能   GPIOB   SPI1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    /* 使能GPIOB时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);     /* 使能SPI1时钟 */

    /* GPIOFB3,4,5初始化设置 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;   /* PB3~5 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;                       /* 复用功能 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;                      /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;                  /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;                       /* 上拉 */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                              /* 初始化GPIOx */
    /* IO复用 */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);            /* PB3复用为 SPI1 */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);            /* PB4复用为 SPI1 */
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);            /* PB5复用为 SPI1 */

    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  /* 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工(SPI->CR1的第15、10位) */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                  /* 设置SPI工作模式:设置为主SPI(SPI->CR1的第2位) */
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                  /* 设置SPI的数据大小:SPI发送接收8位帧结构(SPI->CR1的第11位) */
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;                    /* 串行同步时钟的空闲状态为高电平(SPI->CR1的第1位) */
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;                   /* 串行同步时钟的第二个跳变沿（上升或下降）数据被采样 (SPI->CR1的第0位) */
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                     /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制(SPI->CR1的第9位) */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;          /* 定义波特率预分频的值:波特率预分频值为4(SPI->CR1的第3-5位) */
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                 /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始(SPI->CR1的第7位) */
    SPI_InitStruct.SPI_CRCPolynomial      = 7;                               /* CRC值计算的多项式 */
    SPI_Init(SPI1, &SPI_InitStruct);  /* 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器 */

    SPI_Cmd(SPI1, ENABLE);            /* 使能SPI外设 */

    SPI1_RS_Byte(0xff);               /* 启动传输 */
}



/**********************************************************
* @funcName ：SPI1_RS_Byte
* @brief    ：SPI1接收/发送一个字节数据(8位)
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
uint8_t SPI1_RS_Byte(uint8_t data)
{
    /* 判断发送缓存区是否为空 */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    /* 判断接收缓存区是否为空 */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    data = SPI_I2S_ReceiveData(SPI1);

    return data;
}

