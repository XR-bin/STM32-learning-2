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
* @funcName ：SPI2_Init
* @brief    ：对SPI2通信要用的GPIO口进行初始化和对SPI2的寄存器进行设置
* @param    ：void
* @retval   ：void
* @details  ：
*            PB13   SPI2_SCK   时钟线  复用推挽
*            PB14   SPI2_MISO  主输入  复用推挽
*            PB15   SPI2_MOSI  主输出  复用推挽
*            极性：SCK为低电平为空闲状态
*            相位：数据采样从第一个时钟边沿开始
* @fn       ：
************************************************************/
void SPI2_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx配置结构体 */
    SPI_InitTypeDef   SPI_InitStruct;      /* SPIx配置结构体 */

    /* 时钟使能   GPIOB   SPI2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   /* GPIOB时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);    /* SPI2时钟 */

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    /* PB13、PB14、PB15 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;     /* PB13、PB14、PB15 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;                         /* 复用推挽输 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                        /* IO口速度为50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                                   /* 根据设定参数初始化PB13、PB14、PB15 */

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);              /* 端口输出寄存器输出高(上拉电阻) */

    /* SPI2寄存器初始化设置 */
    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;   /* SPI设置为双线双向全双工,且NSS上的电平为高电平(SPI_CR1寄存器的第10、15位) */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   /* 设置为主SPI(SPI_CR1寄存器的第2、8位) */
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   /* 使用8位数据帧格式进行发送/接收(SPI_CR1寄存器的第11位) */
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      /* 空闲状态时， SCK保持低电平(SPI_CR1寄存器的第1位) */
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    /* 数据采样从第一个时钟边沿开始(SPI_CR1寄存器的第0位) */
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      /* 启用软件从设备管理，NSS引脚上的电平由SSI位的值决定(SPI_CR1寄存器的第9位) */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;           /* 波特率预分频值为2  波特率：fPCLK/2  ==  36M/2  ==  18M (SPI_CR1寄存器的第3-5位) */
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  /* 数据传输从MSB位开始(对齐标准:左对齐)(SPI_CR1寄存器的第7位) */
    SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 /* CRC值计算的多项式:0x7是复位值(SPI_CRCPR寄存器) */
    SPI_Init(SPI2, &SPI_InitStruct);                                          /* 初始化SPI2 */

    SPI_Cmd(SPI2, ENABLE);     /* 使能SPI外设(SPI_CR1寄存器的第6位) */
}



/**********************************************************
* @funcName ：SPI2_RS_Byte
* @brief    ：SPI2接收/发送一个字节数据(8位)
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
uint8_t SPI2_RS_Byte(uint8_t data)
{
    /* 判断发送缓存区是否为空(SPI->SR寄存器的第1位) */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);
    /* 判断接收缓存区是否为空(SPI->SR寄存器的第0位) */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    data = SPI_I2S_ReceiveData(SPI2);

    return data;
}






