#include "fsmc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\fsmc.c
*              .\Drivers\BSP\inc\fsmc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：FSMC配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：FSMC_LCD_Init
* @brief    ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线：
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     控制线：
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(数据/命名)---PG0
*           BL（背光）--PB0
*     整理：
*         推挽复用
*               PD：0-1   4-5   8-10   14-15  
*               PE：7-15
*               PG：0   12
*         推挽输出
*               PB：0
*
*     需要设置的是FSMC的Bank1的区域4
* @fn       ：
************************************************************/
void FSMC_LCD_Init(void)
{
    GPIO_InitTypeDef               GPIO_InitStruct;        /* GPIOx配置结构体 */
    FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStruct; /* FSMC配置结构体 */
    FSMC_NORSRAMTimingInitTypeDef  ReadWriteTiming;        /* 片选时序寄存器配置结构体(读时序控制寄存器) */
    FSMC_NORSRAMTimingInitTypeDef  WriteTiming;            /* 写入时序寄存器配置结构体(写时序控制寄存器) */

    /* 时钟使能   GPIOB  GPIOD  GPIOE  GPIOG  FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  /* 使能PB端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  /* 使能PD端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  /* 使能PE端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);  /* 使能PG端口时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);      /* FSMC时钟使能 */

    /***************GPIOx寄存器配置***************/
    /* PB0 */
    /* 背光灯，配置通用输出 */
    /* 端口配置寄存器 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;       /* PB0 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP; /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; /* IO口速度为50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);            /* 根据设定参数初始化PB0 */
    /* PD0/1/4/5/8/9/10/14/15 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;    /* PD0/1/4/5/8/9/10/14/15 端口配置 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   /* 推挽输出 */
    GPIO_Init(GPIOD, &GPIO_InitStruct);            /* 根据设定参数初始化PD0/1/4/5/8/9/10/14/15 */
    /* PE7-15 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  /* PE7-15 端口配置 */
    GPIO_Init(GPIOE, &GPIO_InitStruct);            /* 根据设定参数初始化PE7-15 */
    /* PG0/12 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;  /* PG0/12 端口配置 */
    GPIO_Init(GPIOG, &GPIO_InitStruct);                 /* 根据设定参数初始化PG0/12 */

    /********** 关背光 **********/
    GPIOB->ODR &= ~(1<<0); 

    /***************FSMC寄存器配置***************/
    /***
    * 注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
    *
    *      FSMC_BCRx 和 FSMC_BTRx，组合成 BTCR[8]寄存器组，他们的对应关系如下：
    *      BTCR[0]对应 FSMC_BCR1，BTCR[1]对应 FSMC_BTR1
    *      BTCR[2]对应 FSMC_BCR2，BTCR[3]对应 FSMC_BTR2
    *      BTCR[4]对应 FSMC_BCR3，BTCR[5]对应 FSMC_BTR3
    *      BTCR[6]对应 FSMC_BCR4，BTCR[7]对应 FSMC_BTR4
    *
    *      FSMC_BWTRx 则组合成 BWTR[7]，他们的对应关系如下：
    *      BWTR[0]对应 FSMC_BWTR1，BWTR[2]对应 FSMC_BWTR2，
    *      BWTR[4]对应 FSMC_BWTR3，BWTR[6]对应 FSMC_BWTR4，
    *      BWTR[1]、BWTR[3]和 BWTR[5]保留，没有用到。
    */
    /* SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）*/
    ReadWriteTiming.FSMC_AddressSetupTime      = 0;      /* 地址设置阶段的持续时间1×HCLK 时钟周期 (FSMC->BTR寄存器的第0-3位) */
    ReadWriteTiming.FSMC_AddressHoldTime       = 0;      /* 地址保持阶段的持续时间1×HCLK 时钟周期 (FSMC->BTR寄存器的第4-7位) */
    ReadWriteTiming.FSMC_DataSetupTime         = 0xf;    /* 数据阶段的持续时间16×HCLK时钟周期 (FSMC->BTR寄存器的第8-15位) */
    ReadWriteTiming.FSMC_BusTurnAroundDuration = 0;      /* 总线恢复时间=1个HCLK时钟周期 (FSMC->BTR寄存器的第16-19位) */
    ReadWriteTiming.FSMC_CLKDivision           = 0;      /* CLK 信号的时钟分频比：保留 (FSMC->BTR寄存器的第20-23位) */
    ReadWriteTiming.FSMC_DataLatency           = 0;      /* 同步突发 NOR Flash 的数据延迟：2个 CLK 时钟周期的数据延迟 (FSMC->BTR寄存器的第24-27位) */
    ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; /* 访问模式：访问模式 A (FSMC->BTR寄存器的第28-29位) */

    /* SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器) */
    WriteTiming.FSMC_AddressSetupTime      = 0;     /* 地址设置阶段的持续时间1×HCLK 时钟周期 (FSMC->BWTR寄存器的第0-3位) */
    WriteTiming.FSMC_AddressHoldTime       = 0;     /* 地址保持阶段的持续时间1×HCLK 时钟周期 (FSMC->BWTR寄存器的第4-7位) */
    WriteTiming.FSMC_DataSetupTime         = 1;     /* 数据阶段的持续时间2×HCLK时钟周期 (FSMC->BWTR寄存器的第8-15位) */
    WriteTiming.FSMC_BusTurnAroundDuration = 0;     /* 总线周转阶段的持续时间：0×HCLK 时钟周期 (FSMC->BWTR寄存器的第16-19位) */
    WriteTiming.FSMC_CLKDivision           = 0;     /* CLK 信号的时钟分频比： 保留 (FSMC->BWTR寄存器的第20-23位) */
    WriteTiming.FSMC_DataLatency           = 0;     /* 同步突发 NOR Flash 的数据延迟：2 个 CLK 时钟周期的数据延迟 (FSMC->BWTR寄存器的第24-27位) */
    WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;/* 访问模式：访问模式 A (FSMC->BWTR寄存器的第28-29位) */

    /* SRAM/NOR-Flash 片选控制寄存器 */
    FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM4;                    /* 选择块与区：bank1(块1)的NE4(区域4)也就对应BTCR[6],[7]。 */
    FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;            /* 地址/数据复用使能位：不使能 (FSMC->BCR寄存器的第1位) */
    FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                   /* 存储器类型：SRAM、 ROM类型 (FSMC->BCR寄存器的第2-3位) */
    FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;               /* 存储器数据总线宽度：16bit  (FSMC->BCR寄存器的第4-5位) */
    FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;           /* 禁用成组访问模式；这是复位后的默认状态。 (FSMC->BCR寄存器的第8位) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;            /* 等待信号极性位：NWAIT 低电平有效 (FSMC->BCR寄存器的第9位) */
    FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;          /* 异步传输期间的等待信号：运行异步协议时不考虑 NWAIT 信号 (FSMC->BCR寄存器的第15位) */
    FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                  /* 不允许直接的非对齐成组操作；这是复位后的默认状态。 (FSMC->BCR寄存器的第10位) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;  /* 等待时序配置：NWAIT 信号在等待周期之前的一个数据周期有效 (FSMC->BCR寄存器的第11位) */
    FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;             /* 写入使能位：FSMC 在存储区域内使能了写入操作 (FSMC->BCR寄存器的第12位) */
    FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                /* 等待使能位：禁止 NWAIT 信号 (FSMC->BCR寄存器的第13位) */
    FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;               /* 读和写操作是否采用相同时序：读写使用不相同的时序 (FSMC->BCR寄存器的第14位) */
    FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                /* 写入突发使能：始终在异步模式下进行写入操作 (FSMC->BCR寄存器的第19位) */
    /* SRAM/NOR-Flash 片选时序寄存器 (读时序控制寄存器) */
    FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadWriteTiming; /* 读时序 */
    /* SRAM/NOR-Flash 写入时序寄存器 (写时序控制寄存器) */
    FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;     /* 写时序 */

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);                            /* 初始化FSMC配置 */

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);                         /* 使能BANK1区域4 */
}








