#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\norflash.c
*              .\Drivers\BSP\inc\norflash.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：识别norflash芯片ID代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：NorFlash_Init
* @brief    ：NorFlash芯片相关引脚初始化
* @param    ：void
* @retval   ：void
* @details  ：
*             SPI_CS     PB12
* @fn       ：
************************************************************/
void NorFlash_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx配置结构体 */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    /* 时钟使能   GPIOB */

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    /* PB12 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;        /* PB12 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;   /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   /* IO口速度为50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);              /* 根据设定参数初始化PB12 */
    /* 端口输出数据寄存器(片选拉高) */
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                 /* 端口输出寄存器输出高(上拉电阻) */

    SPI2_Init();         /* SPI初始化 */
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
    SPI2_RS_Byte(FLASH_ManufactDeviceID);   /* 发送读 ID 命令 */
    SPI2_RS_Byte(0);                        /* 写入一个字节 */
    SPI2_RS_Byte(0);
    SPI2_RS_Byte(0);
    id =  SPI2_RS_Byte(0xFF) << 8;          /* 读取高8位字节 */
    id |= SPI2_RS_Byte(0xFF);               /* 读取低8位字节 */
    SPI_CS_H;                               /* 片选失能 */

    /* 芯片ID列表参考norfalsh.h里的宏定义 */
    printf("芯片ID : %x\r\n", id);

    return id;
}































