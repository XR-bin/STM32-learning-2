#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\w25q128.c
*              .\Drivers\BSP\inc\w25q128.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：w25q128读写操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***
*W25Q128存储芯片
*容量大小             ：16Mbyte == 128Mbit
*共有块区             ：256块
*每块区共有扇区       ：16扇区
*每扇区共有页         ：16页
*每页字节             ：256字节
*地址可写最大十六进制数：0x9fffff
*        9f:块区   f：扇区    f：页    ff：字节
*/



/**********************************************************
* @funcName ：NorFlash_Init
* @brief    ：NorFlash初始化
* @param    ：void
* @retval   ：void
* @details  ：
*             SPI_CS     PB14
* @fn       ：
************************************************************/
void NorFlash_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx配置结构体 */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /* 使能GPIOB时钟 */

    /* GPIOB14 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14;       /* PB14 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;     /* 输出 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; /* 100MHz */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;      /* 上拉 */
    GPIO_Init(GPIOB, &GPIO_InitStruct);             /* 初始化 */

    GPIO_SetBits(GPIOB, GPIO_Pin_14);               /* 拉低电平 */

    SPI1_Init();                                    /* SPI初始化 */
}



/**********************************************************
* @funcName ：NorFlash_Read_ID
* @brief    ：读取norflash芯片的ID
* @param    ：void
* @retval   ：uint16_t -- 返回芯片ID
* @details  ：
* @fn       ：
************************************************************/
uint16_t NorFlash_Read_ID(void)
{
    uint16_t id;

    SPI_CS_L;                               /* 片选使能 */
    SPI1_RS_Byte(FLASH_ManufactDeviceID);   /* 发送读 ID 命令 */
    SPI1_RS_Byte(0);                        /* 写入一个字节 */
    SPI1_RS_Byte(0);
    SPI1_RS_Byte(0);
    id =  SPI1_RS_Byte(0xFF) << 8;          /* 读取高8位字节 */
    id |= SPI1_RS_Byte(0xFF);               /* 读取低8位字节 */
    SPI_CS_H;                               /* 片选失能 */

    /* 芯片ID列表参考norfalsh.h里的宏定义 */
    printf("芯片ID : %x\r\n", id);

    return id;
}



