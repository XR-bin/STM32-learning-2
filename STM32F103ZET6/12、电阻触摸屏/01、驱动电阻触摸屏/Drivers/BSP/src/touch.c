#include "touch.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-03
* @brief     ：电阻触摸屏驱动配置和操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：Touch_IO_Init
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            CS     PF11  推挽输出
*            SCK    PB1   推挽输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   推挽输出
* @fn       ：
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;       /* GPIOx配置结构体 */

    /* 时钟使能   GPIOB   GPIOF */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    /* 输出的引脚 */
    /* PB1 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;          /* PB1 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;    /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    /* IO口速度为50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);               /* 根据设定参数初始化PB1 */
    /* PF9、PF11 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;/* PF9、PF11端口配置 */
    GPIO_Init(GPIOF, &GPIO_InitStruct);               /* 根据设定参数初始化PF9、PF11 */

    /* 输入的引脚 */
    /* PB2 */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;           /* PB2端口配置 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;       /* 上下拉输出模式 */
    GPIO_Init(GPIOB, &GPIO_InitStruct);               /* 根据设定参数初始化PB1 */
    /* PF10 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;           /* PF10端口配置 */
    GPIO_Init(GPIOF, &GPIO_InitStruct);               /* 根据设定参数初始化PF10 */

    /* 初始化状态空闲 */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ：Touch_Data
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：uint8_t command (指令)
* @retval   ：uint16_t
* @details  ：
*            片选线        T_CS_H       T_CS_L  
*            时钟线        T_SCK_H      T_SCK_L  
*            输出数据线    T_MOSI_H     T_MOSI_L
*            输入数据线    T_MISO
*            笔接触线      T_PEN
* @fn       ：
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;            /* 数据线拉低 */
    T_CS_L;              /* 片选使能 */

    for(i=0;i<8;i++)
    {
        T_SCK_L;             /* 时钟线拉低 */

        /* 判断指令位电平 */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;  /* 指令数据左移 */

        T_SCK_H;          /* 时钟线拉高 */
        delay_us(1);      /* 延时，用于从机读取数据 */
    }
    T_SCK_L;            /* 时钟线拉低 */
    T_MOSI_L;           /* 数据线拉低 */
    delay_us(6);        /* 延时6us，用于等待ADC把数据转换好 */

    /* 空出一个空脉冲 */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;              /* 指令数据左移 */
        T_SCK_L;                 /* 时钟线拉低 */
        delay_us(1);             /* 延时，用于数据建立 */
        T_SCK_H;                 /* 时钟线拉高 */
        if(T_MISO) data |= 0x1;  /* 读取数据位电平 */
        delay_us(1);             /* 延时，用于数据建立 */
    }

    T_CS_H;    /* 片选拉高 */
    T_SCK_L;   /* 时钟线拉低 */

    return data;
}





