#include "lcd.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\lcd.c
*              .\Drivers\BSP\inc\lcd.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：LCD屏(ILI9341)操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* 管理LCD重要参数 */
_lcd_dev lcddev;

/**********************************************************
* @funcName ：LCD_IO_Init
* @brief    ：对LCD对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线： 通用输出   推挽   速度50M
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     控制线： 通用输出   推挽   速度50M
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(数据/命名)---PG0
*           BL（背光）--PB0
*     整理：
*         PB：0
*         PD：0-1   4-5   8-10   14-15  
*         PE：7-15
*         PG：0   12
* @fn       ：
************************************************************/
void LCD_IO_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;          /* GPIOx配置结构体 */

    /* 时钟使能   GPIOB   GPIOD   GPIOE  GPIOG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   /* 使能PB端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   /* 使能PD端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   /* 使能PE端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);   /* 使能PG端口时钟 */

    /* PB0 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;           /* PB0 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;     /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     /* IO口速度为50MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                /* 根据设定参数初始化PB0 */

    /* PD0/1/4/5/8/9/10/14/15 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;     /* PD0/1/4/5/8/9/10/14/15 端口配置 */
    GPIO_Init(GPIOD, &GPIO_InitStruct);                /* 根据设定参数初始化PD0/1/4/5/8/9/10/14/15 */

    /* PE7-15 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  /* PE7-15 端口配置 */
    GPIO_Init(GPIOE, &GPIO_InitStruct);                /* 根据设定参数初始化PE7-15 */

    /* PG0/12 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12; /* PG0/12 端口配置 */
    GPIO_Init(GPIOG, &GPIO_InitStruct);                /* 根据设定参数初始化PG0/12 */

    LCD_CS_H;      /* 片选拉高 */
    LCD_WR_H;      /* 写使能拉高 */
    LCD_RD_H;      /* 读使能拉高 */
    LCD_DC_H;      /* 数据命令选择线拉高 */
    LCD_BL_L;      /* 背光拉低(关闭) */
}



/**********************************************************
* @funcName ：Output_Mode
* @brief    ：数据线切换为输出模式
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线： 推挽输出   速度50M
*     整理：
*         PD：0-1   8-10   14-15  
*         PE：7-15
*     注意：由于数据线的硬件上没有接外部上拉电阻，所以不能采用开漏模式
* @fn       ：
************************************************************/
void Output_Mode(void)
{
    /* PD端口配置寄存器 */
    GPIOD->CRL &= ~(0x0000ff);
    GPIOD->CRH &= ~(0xff000fff);
    GPIOD->CRL |= (0x000033);
    GPIOD->CRH |= (0x33000333);

    /* PE端口配置寄存器 */
    GPIOE->CRL &= ~(0xf0000000);
    GPIOE->CRH &= ~(0xffffffff);
    GPIOE->CRL |= (0x30000000);
    GPIOE->CRH |= (0x33333333);
}



/**********************************************************
* @funcName ：Input_Mode
* @brief    ：数据线切换为输入模式
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线： 上拉输入
*     整理：
*         PD：0-1   8-10   14-15  
*         PE：7-15
*     注意：由于数据线的硬件上没有接外部上拉电阻，所以不能采用开漏模式
* @fn       ：
************************************************************/
void Input_Mode(void)
{
    /* PD端口配置寄存器 */
    GPIOD->CRL &= ~(0x0000ff);
    GPIOD->CRH &= ~(0xff000fff);
    GPIOD->CRL |= (0x000088);
    GPIOD->CRH |= (0x88000888);
     /* F1系列的 调为高电平就是才正式变为上拉模式 */
    GPIOD->ODR |= 0xC703;

    /* PE端口配置寄存器 */
    GPIOE->CRL &= ~(0xf0000000);
    GPIOE->CRH &= ~(0xffffffff);
    GPIOE->CRL |= (0x80000000);
    GPIOE->CRH |= (0x88888888);
     /* F1系列的 调为高电平就是才正式变为上拉模式 */
    GPIOE->ODR |= 0xFF80;
}



/**********************************************************
* @funcName ：LCD_WriteCMD
* @brief    ：向LCD屏发送命令函数
* @param    ：uint8_t cmd(LCD指令/寄存器编号/地址)
* @retval   ：void
* @details  ：
* @fn       ：
*         LCD_CS_H(片选失能)   LCD_CS_L(片选使能)
*         LCD_WR_H(写失能)     LCD_WR_L(写使能)
*         LCD_RD_H(读失能)     LCD_RD_L(读使能)
*         LCD_DC_H(收发数据)   LCD_DC_L(收发命令)
*         LCD_BL_H(开启背光)   LCD_BL_L(关闭背光)
************************************************************/
void LCD_WriteCMD(uint16_t cmd)
{
    LCD_RD_H;           /* 读使能拉高 */
    LCD_DC_L;           /* 数据命令选择线拉低 */
    LCD_CS_L;           /* 片选拉低 */
    LCD_WR_L;           /* 写使能拉低 */

    /* 准备要发送的命令 */
    if(cmd & 0x0001)  LCD_DO_0(1);    else LCD_DO_0(0);
    if(cmd & 0x0002)  LCD_DO_1(1);    else LCD_DO_1(0);
    if(cmd & 0x0004)  LCD_DO_2(1);    else LCD_DO_2(0);
    if(cmd & 0x0008)  LCD_DO_3(1);    else LCD_DO_3(0);
    if(cmd & 0x0010)  LCD_DO_4(1);    else LCD_DO_4(0);
    if(cmd & 0x0020)  LCD_DO_5(1);    else LCD_DO_5(0);
    if(cmd & 0x0040)  LCD_DO_6(1);    else LCD_DO_6(0);
    if(cmd & 0x0080)  LCD_DO_7(1);    else LCD_DO_7(0);
    if(cmd & 0x0100)  LCD_DO_8(1);    else LCD_DO_8(0);
    if(cmd & 0x0200)  LCD_DO_9(1);    else LCD_DO_9(0);
    if(cmd & 0x0400)  LCD_DO_10(1);   else LCD_DO_10(0);
    if(cmd & 0x0800)  LCD_DO_11(1);   else LCD_DO_11(0);
    if(cmd & 0x1000)  LCD_DO_12(1);   else LCD_DO_12(0);
    if(cmd & 0x2000)  LCD_DO_13(1);   else LCD_DO_13(0);
    if(cmd & 0x4000)  LCD_DO_14(1);   else LCD_DO_14(0);
    if(cmd & 0x8000)  LCD_DO_15(1);   else LCD_DO_15(0);

    LCD_WR_H;    /* 写使能拉高 */
    LCD_CS_H;    /* 片选拉高 */
}



/**********************************************************
* @funcName ：LCD_WriteData
* @brief    ：向LCD屏发送数据
* @param    ：uint16_t data(数据)
* @retval   ：void
* @details  ：
* @fn       ：
*         LCD_CS_H(片选失能)   LCD_CS_L(片选使能)
*         LCD_WR_H(写失能)     LCD_WR_L(写使能)
*         LCD_RD_H(读失能)     LCD_RD_L(读使能)
*         LCD_DC_H(收发数据)   LCD_DC_L(收发命令)
*         LCD_BL_H(开启背光)   LCD_BL_L(关闭背光)
************************************************************/
void LCD_WriteData(uint16_t data)
{
    LCD_RD_H;              /* 读使能拉高 */
    LCD_DC_H;              /* 数据命令选择线拉低 */
    LCD_CS_L;              /* 片选拉低 */
    LCD_WR_L;              /* 写使能拉低 */

    /* 准备要发送的数据 */
    if(data & 0x0001)  LCD_DO_0(1);    else LCD_DO_0(0);
    if(data & 0x0002)  LCD_DO_1(1);    else LCD_DO_1(0);
    if(data & 0x0004)  LCD_DO_2(1);    else LCD_DO_2(0);
    if(data & 0x0008)  LCD_DO_3(1);    else LCD_DO_3(0);
    if(data & 0x0010)  LCD_DO_4(1);    else LCD_DO_4(0);
    if(data & 0x0020)  LCD_DO_5(1);    else LCD_DO_5(0);
    if(data & 0x0040)  LCD_DO_6(1);    else LCD_DO_6(0);
    if(data & 0x0080)  LCD_DO_7(1);    else LCD_DO_7(0);
    if(data & 0x0100)  LCD_DO_8(1);    else LCD_DO_8(0);
    if(data & 0x0200)  LCD_DO_9(1);    else LCD_DO_9(0);
    if(data & 0x0400)  LCD_DO_10(1);   else LCD_DO_10(0);
    if(data & 0x0800)  LCD_DO_11(1);   else LCD_DO_11(0);
    if(data & 0x1000)  LCD_DO_12(1);   else LCD_DO_12(0);
    if(data & 0x2000)  LCD_DO_13(1);   else LCD_DO_13(0);
    if(data & 0x4000)  LCD_DO_14(1);   else LCD_DO_14(0);
    if(data & 0x8000)  LCD_DO_15(1);   else LCD_DO_15(0);

    LCD_WR_H;    /* 写使能拉高 */
    LCD_CS_H;    /* 片选拉高 */
}


/**********************************************************
* @funcName ：LCD_ReadData
* @brief    ：接收LCD屏发来的数据
* @param    ：void
* @retval   ：uint16_t
* @details  ：
* @fn       ：
*         LCD_CS_H(片选失能)   LCD_CS_L(片选使能)
*         LCD_WR_H(写失能)     LCD_WR_L(写使能)
*         LCD_RD_H(读失能)     LCD_RD_L(读使能)
*         LCD_DC_H(收发数据)   LCD_DC_L(收发命令)
*         LCD_BL_H(开启背光)   LCD_BL_L(关闭背光)
************************************************************/
uint16_t LCD_ReadData(void)
{
    uint16_t data=0;

    Input_Mode();          /* 切换为输入模式 */
    LCD_WR_H;              /* 写使能拉高 */
    LCD_DC_H;              /* 数据命令选择线拉低 */
    LCD_CS_L;              /* 片选拉低 */
    LCD_RD_L;              /* 读使能拉低 */

    /* 接收数据 */
    data |= (((GPIOD->IDR>>14)&0x1) << 0);
    data |= (((GPIOD->IDR>>15)&0x1) << 1);
    data |= (((GPIOD->IDR>>0)&0x1)  << 2);
    data |= (((GPIOD->IDR>>1)&0x1)  << 3);
    data |= (((GPIOE->IDR>>7)&0x1)  << 4);
    data |= (((GPIOE->IDR>>8)&0x1)  << 5);
    data |= (((GPIOE->IDR>>9)&0x1)  << 6);
    data |= (((GPIOE->IDR>>10)&0x1) << 7);
    data |= (((GPIOE->IDR>>11)&0x1) << 8);
    data |= (((GPIOE->IDR>>12)&0x1) << 9);
    data |= (((GPIOE->IDR>>13)&0x1) << 10);
    data |= (((GPIOE->IDR>>14)&0x1) << 11);
    data |= (((GPIOE->IDR>>15)&0x1) << 12);
    data |= (((GPIOD->IDR>>8)&0x1)  << 13);
    data |= (((GPIOD->IDR>>9)&0x1)  << 14);
    data |= (((GPIOD->IDR>>10)&0x1) << 15);

    LCD_RD_H;    /* 读使能拉高 */
    LCD_CS_H;    /* 片选拉高 */

    Output_Mode();      /* 切换为输出模式 */

    return data;
}



/**********************************************************
* @funcName ：LCD_ILI9341_SetCursor
* @brief    ：设置光标位置(对RGB屏无效)
* @param    ：uint16_t x(x坐标，横坐标)
* @param    ：uint16_t y(y坐标，纵坐标)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_SetCursor(uint16_t x, uint16_t y)
{
    LCD_WriteCMD(lcddev.setxcmd);  /* 发送命令(x坐标) */
    LCD_WriteData(x >> 8);         /* 写入起始x坐标 */
    LCD_WriteData(x & 0XFF);

    LCD_WriteCMD(lcddev.setycmd);  /* 发送命令(y坐标) */
    LCD_WriteData(y >> 8);         /* 写入起始y坐标 */
    LCD_WriteData(y & 0XFF);
}



/**********************************************************
* @funcName ：LCD_Clear
* @brief    ：LCD屏幕填充某一种颜色来清屏函数
* @param    ：uint16_t color(颜色数据)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Clear(uint16_t color)
{
    uint32_t i;

    i = lcddev.width * lcddev.height;

    LCD_ILI9341_SetCursor(0x00, 0x00);      /* 设置光标 */

    /* 发送命令(颜色) */
    LCD_WriteCMD(lcddev.wramcmd);
    /* 开始填色 */
    while(i)
    {
        LCD_WriteData(color);
        i--;
    }
}



/**********************************************************
* @funcName ：LCD_ILI9341_ScanDir
* @brief    ：设置LCD(ILI9341)的自动扫描方向
* @param    ：uint8_t dir(扫描方向)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_ScanDir(uint8_t dir)
{
    uint16_t regval=0;
    uint8_t dirreg=0;
    uint16_t temp;

    /**
    * 横屏时，对1963不改变扫描方向！其他IC改变扫描方向！
    * 竖屏时，1963改变方向，其他IC不改变扫描方向。
    *
    * 注意：我们这里用的屏幕是ILI9341
    **/
    if(lcddev.dir==1)            /* 横屏时处理 */
    {
        switch(dir)              /* 扫描方向转换 */
        {
            case 0:dir=6;break;
            case 1:dir=7;break;
            case 2:dir=4;break;
            case 3:dir=5;break;
            case 4:dir=1;break;
            case 5:dir=0;break;
            case 6:dir=3;break;
            case 7:dir=2;break;
        }
    }

    /* 根据扫描方式 设置 0X36/0X3600 寄存器 bit 5,6,7 位的值 */
    switch (dir)
    {
        /* 从左到右,从上到下 */
        case L2R_U2D:
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        /* 从左到右,从下到上 */
        case L2R_D2U:
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        /* 从右到左,从上到下 */
        case R2L_U2D:
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        /* 从右到左,从下到上 */
        case R2L_D2U:
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        /* 从上到下,从左到右 */
        case U2D_L2R:
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        /* 从上到下,从右到左 */
        case U2D_R2L:
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        /* 从下到上,从左到右 */
        case D2U_L2R:
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        /* 从下到上,从右到左 */
        case D2U_R2L:
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;    /* 对绝大部分驱动IC, 由0X36寄存器控制 */
    regval |= 0X08;   /* ILI9341要设置BGR位 */

    LCD_WriteCMD(dirreg);
    LCD_WriteData(regval);

    /* 对屏幕宽高做调整 */
    if(regval&0X20)
    {
        if (lcddev.width < lcddev.height)   /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }
    else  
    {
        if (lcddev.width > lcddev.height)   /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }

    LCD_WriteCMD(lcddev.setxcmd);              /* 发送命令(x坐标) */
    LCD_WriteData(0);                          /* 写入起始x坐标 */
    LCD_WriteData(0);
    LCD_WriteData((lcddev.width - 1) >> 8);    /* 写入结束x坐标 */
    LCD_WriteData((lcddev.width - 1) & 0XFF);

    LCD_WriteCMD(lcddev.setycmd);              /* 发送命令(y坐标) */
    LCD_WriteData(0);                          /* 写入起始y坐标 */
    LCD_WriteData(0);
    LCD_WriteData((lcddev.height - 1) >> 8);   /* 写入结束y坐标 */
    LCD_WriteData((lcddev.height - 1) & 0XFF);
}



/**********************************************************
* @funcName ：LCD_ILI9341_DisplayDir
* @brief    ：设置LCD横竖屏
* @param    ：uint8_t dir
*   @arg    ：0 --- 竖屏   1 --- 横屏
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_DisplayDir(uint8_t dir)
{
    if(dir==0)
    {
        lcddev.dir = 0;             /* 竖屏 */
        lcddev.width = LCD_WIDTH;   /* 屏幕宽 */
        lcddev.height = LCD_HEIGHT; /* 屏幕高 */
    }
    else
    {
        lcddev.dir = 1;             /* 横屏 */
        lcddev.width = LCD_HEIGHT;  /* 屏幕宽 */
        lcddev.height = LCD_WIDTH;  /* 屏幕高 */
    }

    lcddev.wramcmd=0X2C;    /* 颜色指令 */
    lcddev.setxcmd=0X2A;    /* x坐标指令 */
    lcddev.setycmd=0X2B;    /* y坐标指令 */

    LCD_ILI9341_ScanDir(DFT_SCAN_DIR);  /* 默认扫描方向 */
}


 
/**********************************************************
* @funcName ：LCD_ILI9341_Init
* @brief    ：LCD屏幕初始化函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_Init(void)
{
    /* IO口初始化 */
    LCD_IO_Init();

    delay1_ms(10);
    lcddev.id = LED_ID;    /* 屏幕型号 */

    /* LCD厂家代码 */
    LCD_WriteCMD(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC1);
    LCD_WriteData(0X30);
    LCD_WriteCMD(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0X12);
    LCD_WriteData(0X81);
    LCD_WriteCMD(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x10);
    LCD_WriteData(0x7A);
    LCD_WriteCMD(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);
    LCD_WriteCMD(0xF7);
    LCD_WriteData(0x20);
    LCD_WriteCMD(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0xC0);        /* Power control */
    LCD_WriteData(0x1B);       /* VRH[5:0] */
    LCD_WriteCMD(0xC1);        /* Power control */
    LCD_WriteData(0x01);       /* SAP[2:0];BT[3:0] */
    LCD_WriteCMD(0xC5);        /* VCM control */
    LCD_WriteData(0x30);       /* 3F */
    LCD_WriteData(0x30);       /* 3C */
    LCD_WriteCMD(0xC7);        /* VCM control2 */
    LCD_WriteData(0XB7);
    LCD_WriteCMD(0x36);        /* Memory Access Control */
    LCD_WriteData(0x48);
    LCD_WriteCMD(0x3A);
    LCD_WriteData(0x55);
    LCD_WriteCMD(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1A);
    LCD_WriteCMD(0xB6);        /*  Display Function Control */
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);
    LCD_WriteCMD(0xF2);        /*  3Gamma Function Disable */
    LCD_WriteData(0x00);
    LCD_WriteCMD(0x26);        /* Gamma curve selected */
    LCD_WriteData(0x01);
    LCD_WriteCMD(0xE0);        /* Set Gamma */
    LCD_WriteData(0x0F);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x28);
    LCD_WriteData(0x08);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x08);
    LCD_WriteData(0x54);
    LCD_WriteData(0XA9);
    LCD_WriteData(0x43);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0XE1);        /* Set Gamma */
    LCD_WriteData(0x00);
    LCD_WriteData(0x15);
    LCD_WriteData(0x17);
    LCD_WriteData(0x07);
    LCD_WriteData(0x11);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x56);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x05);
    LCD_WriteData(0x10);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x0F);
    LCD_WriteCMD(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3f);
    LCD_WriteCMD(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xef);
    LCD_WriteCMD(0x11);        /* Exit Sleep */
    delay_ms(120);
    LCD_WriteCMD(0x29);        /* display on */

    /* 用户自己编写 */
    LCD_ILI9341_DisplayDir(0);    /* 设置屏幕为竖屏 */
    LCD_BL_H;                     /* 背光灯打开 */
    LCD_Clear(WHITE);             /* 清屏函数 */
}



/******************************************************************************
*函数功能  ：识别屏幕ID型号
*函数名    ：TFTLCD_Id
*函数参数  ：void
*函数返回值：u16
*函数描述  ：
********************************************************************************/
uint16_t LCD_Recognition_Id(void)
{
    uint16_t id = 0;

    /* 尝试9341 ID的读取 */
    LCD_WriteCMD(0XD3);
    id = LCD_ReadData();  /* dummy read */
    id = LCD_ReadData();  /* 读到0X00 */
    id = LCD_ReadData();  /* 读取93 */
    id <<= 8;
    id |= LCD_ReadData(); /* 读取41 */

    if (id != 0X9341)    /* 不是 9341 , 尝试看看是不是 ST7789 */
    {
        LCD_WriteCMD(0X04);
        id = LCD_ReadData();      /* dummy read */
        id = LCD_ReadData();      /* 读到0X85 */
        id = LCD_ReadData();      /* 读取0X85 */
        id <<= 8;
        id |= LCD_ReadData();     /* 读取0X52 */

        if (id == 0X8552)        /* 将8552的ID转换成7789 */
        {
            id = 0x7789;
        }

        if (id != 0x7789)        /* 也不是ST7789, 尝试是不是 NT35310 */
        {
            LCD_WriteCMD(0XD4);
            id = LCD_ReadData();  /* dummy read */
            id = LCD_ReadData();  /* 读回0X01 */
            id = LCD_ReadData();  /* 读回0X53 */
            id <<= 8;
            id |= LCD_ReadData(); /* 这里读回0X10 */

            if (id != 0X5310)    /* 也不是NT35310,尝试看看是不是NT35510 */
            {
                /* 发送秘钥（厂家提供,照搬即可） */
                LCD_WriteCMD(0xF000);LCD_WriteData(0x0055);
                LCD_WriteCMD(0xF001);LCD_WriteData(0x00AA);
                LCD_WriteCMD(0xF002);LCD_WriteData(0x0052);
                LCD_WriteCMD(0xF003);LCD_WriteData(0x0008);
                LCD_WriteCMD(0xF004);LCD_WriteData(0x0001);

                LCD_WriteCMD(0xC500);     /* 读取ID高8位 */
                id = LCD_ReadData();      /* 读回0X55 */
                id <<= 8;

                LCD_WriteCMD(0xC501);     /* 读取ID低8位 */
                id |= LCD_ReadData();     /* 读回0X10 */
                delay_ms(5);

                if (id != 0X5510)        /* 也不是NT5510,尝试看看是不是SSD1963 */
                {
                    LCD_WriteCMD(0XA1);
                    id = LCD_ReadData();
                    id = LCD_ReadData();  /* 读回0X57 */
                    id <<= 8;
                    id |= LCD_ReadData(); /* 读回0X61 */

                    if (id == 0X5761) id = 0X1963; /* SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963 */
                }
            }
        }
    }

    switch(id)
    {
        case 0X7789: printf("LCD屏型号：ST7789\r\n"); break;
        case 0X9341: printf("LCD屏型号：ILI9341\r\n"); break;
        case 0x5310: printf("LCD屏型号：NT35310\r\n"); break;
        case 0x5510: printf("LCD屏型号：NT35510\r\n"); break;
        case 0X1963: printf("LCD屏型号：SSD1963\r\n"); break;
        default : printf("没有LCD屏或使用了未知型号的LCD屏\r\n"); break;
    }

    return id;
}







