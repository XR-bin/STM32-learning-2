#include "stm32f10x.h"
#include "TFTLCD.h"
#include "delay.h"
#include "fsmc.h"


//我们知道ARGB指的是一种色彩模式，里面A代表Alpha，R表示red，G表示green，B表示blue。
typedef struct _HEADCOLOR
{
   unsigned char scan;      //扫描方向
   unsigned char gray;      //灰度值
   unsigned short w;        //图片宽
   unsigned short h;        //图片高
   unsigned char is565;     //每个像素占四位，即R=5，G=6，B=5，没有透明度，那么一个像素点占5+6+5=16位
   unsigned char rgb;       //RGB
}HEADCOLOR;

/****************************************************************************
*函数功能  ：对LCD对应的GPIO口进行初始化设置
*函数名    ：TFTLCD_IO_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*     数据线：//通用输出 推挽      速度50M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     控制线：//通用输出  推挽      速度50M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(数据/命名)---PG0	 
*           BL（背光）--PB0   
*     整理：
*         PB：0
*         PD：0-1  4-5  8-10  14-15  
*         PE：7-15
*         PG：0  12          
***************************************************************************/
void TFTLCD_IO_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;    //GPIOx配置结构体
	
	//时钟使能   GPIOB   GPIOD   GPIOE  GPIOG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PG端口时钟
	
	//PB0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;				     //PB0 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //根据设定参数初始化PB0
	
	//PD0/1/4/5/8/9/10/14/15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;  //PD0/1/4/5/8/9/10/14/15 端口配置
	GPIO_Init(GPIOD, &GPIO_InitStruct);					     //根据设定参数初始化PD0/1/4/5/8/9/10/14/15
	
	//PE7-15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //PE7-15 端口配置
	GPIO_Init(GPIOE, &GPIO_InitStruct);	             //根据设定参数初始化PE7-15
	
	//PG0/12 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;  //PG0/12 端口配置
	GPIO_Init(GPIOG, &GPIO_InitStruct);	              //根据设定参数初始化PG0/12 
	
	//片选拉高
  TFTLCD_CS_H;
  //写使能拉高
  TFTLCD_WR_H;
  //读使能拉高
  TFTLCD_RD_H;
  //数据命令选择线拉高
  TFTLCD_DC_H;
  //背光拉低(关闭)
  TFTLCD_BL_L;
}	

/***************************************************************************************
*函数功能  ：主控芯片发送命令函数
*函数名    ：TFTLCD_ILI9341_CMD
*函数参数  ：u8 cmd    
*函数返回值：void
*函数描述  ：  
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*****************************************************************************************/
void TFTLCD_ILI9341_CMD(u8 cmd)
{
//********************** 8080通信方式驱动 **********************//
	/*
  //读使能拉高
  TFTLCD_RD_H;
  //数据命令选择线拉低
  TFTLCD_DC_L;
  //片选拉低
  TFTLCD_CS_L;
  //写使能拉低
  TFTLCD_WR_L;
  //准备要发送的命令
  if(cmd & 0x01) TFTLCD_DO_0(1);    else TFTLCD_DO_0(0);
	if(cmd & 0x02) TFTLCD_DO_1(1);    else TFTLCD_DO_1(0);
	if(cmd & 0x04) TFTLCD_DO_2(1);    else TFTLCD_DO_2(0);
	if(cmd & 0x08) TFTLCD_DO_3(1);    else TFTLCD_DO_3(0);
	if(cmd & 0x10) TFTLCD_DO_4(1);    else TFTLCD_DO_4(0);
	if(cmd & 0x20) TFTLCD_DO_5(1);    else TFTLCD_DO_5(0);
	if(cmd & 0x40) TFTLCD_DO_6(1);    else TFTLCD_DO_6(0);
	if(cmd & 0x80) TFTLCD_DO_7(1);    else TFTLCD_DO_7(0);
  //写使能拉高
  TFTLCD_WR_H;
  //片选拉高
  TFTLCD_CS_H;
	*/
	/********************** 通过FSMC驱动方式 **********************/
  //TFTLCD_CMD1 = cmd;
  TFTLCD->TFTLCD_CMD = cmd;
}

/***************************************************************************************
*函数功能  ：主控芯片发送数据函数
*函数名    ：TFTLCD_ILI9341_Parameter
*函数参数  ：u16  data   
*函数返回值：void
*函数描述  ：	
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*****************************************************************************************/
void TFTLCD_ILI9341_Parameter(u16 data)
{
//********************** 8080通信方式驱动 **********************//
  /*
  //读使能拉高
  TFTLCD_RD_H;
  //数据命令选择线拉低
  TFTLCD_DC_H;
  //片选拉低
  TFTLCD_CS_L;
  //写使能拉低
  TFTLCD_WR_L;
  //准备要发送的命令
  if(data & 0x0001)  TFTLCD_DO_0(1);    else TFTLCD_DO_0(0);
	if(data & 0x0002)  TFTLCD_DO_1(1);    else TFTLCD_DO_1(0);
	if(data & 0x0004)  TFTLCD_DO_2(1);    else TFTLCD_DO_2(0);
	if(data & 0x0008)  TFTLCD_DO_3(1);    else TFTLCD_DO_3(0);
	if(data & 0x0010)  TFTLCD_DO_4(1);    else TFTLCD_DO_4(0);
	if(data & 0x0020)  TFTLCD_DO_5(1);    else TFTLCD_DO_5(0);
	if(data & 0x0040)  TFTLCD_DO_6(1);    else TFTLCD_DO_6(0);
	if(data & 0x0080)  TFTLCD_DO_7(1);    else TFTLCD_DO_7(0);
	if(data & 0x0100)  TFTLCD_DO_8(1);    else TFTLCD_DO_8(0);
	if(data & 0x0200)  TFTLCD_DO_9(1);    else TFTLCD_DO_9(0);
	if(data & 0x0400)  TFTLCD_DO_10(1);   else TFTLCD_DO_10(0);
	if(data & 0x0800)  TFTLCD_DO_11(1);   else TFTLCD_DO_11(0);
	if(data & 0x1000)  TFTLCD_DO_12(1);   else TFTLCD_DO_12(0);
	if(data & 0x2000)  TFTLCD_DO_13(1);   else TFTLCD_DO_13(0);
	if(data & 0x4000)  TFTLCD_DO_14(1);   else TFTLCD_DO_14(0);
	if(data & 0x8000)  TFTLCD_DO_15(1);   else TFTLCD_DO_15(0);
  //写使能拉高
  TFTLCD_WR_H;
  //片选拉高
  TFTLCD_CS_H;
	*/
/********************** 通过FSMC驱动方式 **********************/
  //TFTLCD_DAT1 = data;
  TFTLCD->TFTLCD_DAT = data;
}

/**************************************************************************
*函数功能  ：TFTLCD屏幕清屏函数
*函数名    ：TFTLCD_Clear
*函数参数  ：u16 colour
*函数返回值：void
*函数描述  ：
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void TFTLCD_Clear(u16 colour)
{
  u32 i;
  
  i = TFTLCD_Long * TFTLCD_Wide;
  
  //发送命令(x坐标)
  TFTLCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  TFTLCD_ILI9341_Parameter(0);
  TFTLCD_ILI9341_Parameter(0);
  //末尾x坐标
  TFTLCD_ILI9341_Parameter((TFTLCD_Long-1) >> 8);
  TFTLCD_ILI9341_Parameter(TFTLCD_Long-1);
  
  //发送命令(y坐标)
  TFTLCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  TFTLCD_ILI9341_Parameter(0);
  TFTLCD_ILI9341_Parameter(0);
  //末尾y坐标
  TFTLCD_ILI9341_Parameter((TFTLCD_Wide-1) >> 8);
  TFTLCD_ILI9341_Parameter(TFTLCD_Wide-1);
  
  //发送命令(颜色)
  TFTLCD_ILI9341_CMD(0x2c);
  //开始填色
  while(i)
  {
    TFTLCD_ILI9341_Parameter(colour);
    i--;
  }
}

/***************************************************************************************
*函数功能  ：LCD屏幕初始化函数
*函数名    ：TFTLCD_Init
*函数参数  ：void   
*函数返回值：void
*函数描述  ：
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************************/
void TFTLCD_Init(void)
{
	/******** 8080通信驱动方式 ********/
  /*IO口初始化*/
	//TFTLCD_IO_Init();
	/******** FSMC驱动方式 ********/
  /*IO口初始化和FSMC初始化*/
  FSMC_TFTLCD_Init();
  
  /*TFTLCD厂家代码*/
	TFTLCD_ILI9341_CMD(0xCF);  
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0xC1); 
	TFTLCD_ILI9341_Parameter(0X30); 
	TFTLCD_ILI9341_CMD(0xED);  
	TFTLCD_ILI9341_Parameter(0x64); 
	TFTLCD_ILI9341_Parameter(0x03); 
	TFTLCD_ILI9341_Parameter(0X12); 
	TFTLCD_ILI9341_Parameter(0X81); 
	TFTLCD_ILI9341_CMD(0xE8);  
	TFTLCD_ILI9341_Parameter(0x85); 
	TFTLCD_ILI9341_Parameter(0x10); 
	TFTLCD_ILI9341_Parameter(0x7A); 
	TFTLCD_ILI9341_CMD(0xCB);  
	TFTLCD_ILI9341_Parameter(0x39); 
	TFTLCD_ILI9341_Parameter(0x2C); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x34); 
	TFTLCD_ILI9341_Parameter(0x02); 
	TFTLCD_ILI9341_CMD(0xF7);  
	TFTLCD_ILI9341_Parameter(0x20); 
	TFTLCD_ILI9341_CMD(0xEA);  
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_CMD(0xC0);    //Power control 
	TFTLCD_ILI9341_Parameter(0x1B);   //VRH[5:0] 
	TFTLCD_ILI9341_CMD(0xC1);    //Power control 
	TFTLCD_ILI9341_Parameter(0x01);   //SAP[2:0];BT[3:0] 
	TFTLCD_ILI9341_CMD(0xC5);    //VCM control 
	TFTLCD_ILI9341_Parameter(0x30); 	 //3F
	TFTLCD_ILI9341_Parameter(0x30); 	 //3C
	TFTLCD_ILI9341_CMD(0xC7);    //VCM control2 
	TFTLCD_ILI9341_Parameter(0XB7); 
	TFTLCD_ILI9341_CMD(0x36);    // Memory Access Control 
	TFTLCD_ILI9341_Parameter(0x48); 
	TFTLCD_ILI9341_CMD(0x3A);   
	TFTLCD_ILI9341_Parameter(0x55); 
	TFTLCD_ILI9341_CMD(0xB1);   
	TFTLCD_ILI9341_Parameter(0x00);   
	TFTLCD_ILI9341_Parameter(0x1A); 
	TFTLCD_ILI9341_CMD(0xB6);    // Display Function Control 
	TFTLCD_ILI9341_Parameter(0x0A); 
	TFTLCD_ILI9341_Parameter(0xA2); 
	TFTLCD_ILI9341_CMD(0xF2);    // 3Gamma Function Disable 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_CMD(0x26);    //Gamma curve selected 
	TFTLCD_ILI9341_Parameter(0x01); 
	TFTLCD_ILI9341_CMD(0xE0);    //Set Gamma 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x2A); 
	TFTLCD_ILI9341_Parameter(0x28); 
	TFTLCD_ILI9341_Parameter(0x08); 
	TFTLCD_ILI9341_Parameter(0x0E); 
	TFTLCD_ILI9341_Parameter(0x08); 
	TFTLCD_ILI9341_Parameter(0x54); 
	TFTLCD_ILI9341_Parameter(0XA9); 
	TFTLCD_ILI9341_Parameter(0x43); 
	TFTLCD_ILI9341_Parameter(0x0A); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x00); 		 
	TFTLCD_ILI9341_CMD(0XE1);    //Set Gamma 
	TFTLCD_ILI9341_Parameter(0x00); 
	TFTLCD_ILI9341_Parameter(0x15); 
	TFTLCD_ILI9341_Parameter(0x17); 
	TFTLCD_ILI9341_Parameter(0x07); 
	TFTLCD_ILI9341_Parameter(0x11); 
	TFTLCD_ILI9341_Parameter(0x06); 
	TFTLCD_ILI9341_Parameter(0x2B); 
	TFTLCD_ILI9341_Parameter(0x56); 
	TFTLCD_ILI9341_Parameter(0x3C); 
	TFTLCD_ILI9341_Parameter(0x05); 
	TFTLCD_ILI9341_Parameter(0x10); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_Parameter(0x3F); 
	TFTLCD_ILI9341_Parameter(0x3F); 
	TFTLCD_ILI9341_Parameter(0x0F); 
	TFTLCD_ILI9341_CMD(0x2B); 
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x01);
	TFTLCD_ILI9341_Parameter(0x3f);
	TFTLCD_ILI9341_CMD(0x2A); 
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0x00);
	TFTLCD_ILI9341_Parameter(0xef);	
	
	TFTLCD_ILI9341_CMD(0x36);//控制横屏还是竖屏，xy(0,0)中心点是左上、右上、左下、右下
	TFTLCD_ILI9341_Parameter(TFTLCD_Lengthways); //纵向显示，(0,0)在左上角
//  TFTLCD_ILI9341_Parameter(TFTLCD_Crosswise);
	
	TFTLCD_ILI9341_CMD(0x11); //Exit Sleep
	delay_ms(120);
	TFTLCD_ILI9341_CMD(0x29); //display on	

  
  /*用户自己编写*/
	//背光灯打开
	TFTLCD_BL_H;
	//清屏函数
	TFTLCD_Clear(WHITE);
}

/**************************************************************************
*函数功能  ：LCD屏幕将某个矩形区域某种颜色
*函数名    ：TFTLCD_Clear_XY
*函数参数  ：u16 x,u16 y,u16 w,u16 h,u16 colour  
*函数返回值：void
*函数描述  ：
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void TFTLCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour)
{
  u32 i;
  i = Long * Wide;
  //发送命令(x坐标)
  TFTLCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  TFTLCD_ILI9341_Parameter(x>>8);
  TFTLCD_ILI9341_Parameter(x);
  //末尾x坐标
  TFTLCD_ILI9341_Parameter((x+Long) >> 8);
  TFTLCD_ILI9341_Parameter(x+Long);
  
  //发送命令(y坐标)
  TFTLCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  TFTLCD_ILI9341_Parameter(y>>8);
  TFTLCD_ILI9341_Parameter(y);
  //末尾y坐标
  TFTLCD_ILI9341_Parameter((y+Wide) >> 8);
  TFTLCD_ILI9341_Parameter(y+Wide);
  
  //发送命令(颜色)
  TFTLCD_ILI9341_CMD(0x2c);
  //开始填色
  while(i)
  {
    TFTLCD->TFTLCD_DAT = colour;
    i--;
  }
}

/**************************************************************************
*函数功能  ：LCD屏幕画点函数
*函数名    ：TFTLCD_Point
*函数参数  ：u16 x,u16 y,u16 colour 
*函数返回值：void
*函数描述  ：
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void TFTLCD_Point(u16 x,u16 y,u16 colour)
{
  //发送命令(x坐标)
  TFTLCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  TFTLCD_ILI9341_Parameter(x >> 8);
  TFTLCD_ILI9341_Parameter(x);
  //末尾x坐标
  TFTLCD_ILI9341_Parameter(x >> 8);
  TFTLCD_ILI9341_Parameter(x);
  
  //发送命令(y坐标)
  TFTLCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  TFTLCD_ILI9341_Parameter(y >> 8);
  TFTLCD_ILI9341_Parameter(y);
  //末尾y坐标
  TFTLCD_ILI9341_Parameter(y >> 8);
  TFTLCD_ILI9341_Parameter(y);
  
  //发送命令(颜色)
	TFTLCD->TFTLCD_CMD = 0x2c;
  //开始填色
	TFTLCD->TFTLCD_DAT = colour;
}

u16 LCD_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=TFTLCD->TFTLCD_DAT;	
	return ram;	 
}	

void opt_delay(u8 i)
{
	while(i--);
}

/**************************************************************************
*函数功能  ：读取个某点的颜色值	 
*函数名    ：TFTLCD_ReadPoint
*函数参数  ：u16 x,u16 y
*函数返回值：u16   此点的颜色
*函数描述  ：
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
u16 TFTLCD_ReadPoint(u16 x,u16 y)
{
	u16 r=0,g=0,b=0;
	if(x>=TFTLCD_Long||y>=TFTLCD_Wide)return 0;	//超过了范围,直接返回
	
	//发送命令(x坐标)
  TFTLCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  TFTLCD_ILI9341_Parameter(x >> 8);TFTLCD_ILI9341_Parameter(x);
  TFTLCD_ILI9341_Parameter(x >> 8);TFTLCD_ILI9341_Parameter(x);
  
  //发送命令(y坐标)
  TFTLCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  TFTLCD_ILI9341_Parameter(y >> 8);TFTLCD_ILI9341_Parameter(y);
  TFTLCD_ILI9341_Parameter(y >> 8);TFTLCD_ILI9341_Parameter(y);
	
	TFTLCD->TFTLCD_CMD = 0X2E;
	
	//读数据
	r=TFTLCD->TFTLCD_DAT;
	opt_delay(2);	
	r=TFTLCD->TFTLCD_DAT;  		  						//实际坐标颜色
	opt_delay(2);
	b=TFTLCD->TFTLCD_DAT;  		  						//实际坐标颜色
	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510需要公式转换一下
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(空心)
*函数名    ：TFTLCD_Circle
*函数参数  ：u16 x,u16 y,u16 r,u16 colour 
*函数返回值：void
*函数描述  ：			
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void TFTLCD_Circle(u16 x,u16 y,u16 r,u16 colour)
{
  u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				TFTLCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(实心圆)
*函数名    ：TFTLCD_Circle2
*函数参数  ：u16 x,u16 y,u16 r,u16 colour  颜色
*函数返回值：void
*函数描述  ：
*         片选             TFTLCD_CS_H   TFTLCD_CS_L   
*         写使能           TFTLCD_WR_H   TFTLCD_WR_L   
*         读使能           TFTLCD_RD_H   TFTLCD_RD_L 
*         数据命令选择线   TFTLCD_DC_H   TFTLCD_DC_L
*         背光             TFTLCD_BL_H   TFTLCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void TFTLCD_Circle2(u16 x,u16 y,u16 r,u16 colour)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				TFTLCD_Point(i,j,colour);
			}
		}
	}
}


/******************************************************************************
*函数功能  ：识别屏幕ID型号
*函数名    ：TFTLCD_Id
*函数参数  ：void
*函数返回值：u16
*函数描述  ：
********************************************************************************/
u16 TFTLCD_Id(void)
{
	u16 id = 0;

	//读ID（9320/9325/9328/4531/4535等IC）
	TFTLCD->TFTLCD_CMD = 0x0000;
	delay_us(5);
	id = TFTLCD->TFTLCD_DAT;
	
	//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
	if(id<0XFF || id==0XFFFF || id==0X9300)
	{
		//尝试9341 ID的读取		
		TFTLCD->TFTLCD_CMD = 0XD3;				   
		id = TFTLCD->TFTLCD_DAT;	  //dummy read 	
 		id = TFTLCD->TFTLCD_DAT;	  //读到0X00
  	id = TFTLCD->TFTLCD_DAT;   	//读取93								   
 		id <<= 8;
		id |= TFTLCD->TFTLCD_DAT;  	//读取41 
		
		//非9341,尝试是不是6804
		if(id != 0X9341)
		{
			TFTLCD->TFTLCD_CMD = 0XBF;
			id = TFTLCD->TFTLCD_DAT; 	//dummy read 	 
	 		id = TFTLCD->TFTLCD_DAT;  //读回0X01			   
	 		id = TFTLCD->TFTLCD_DAT; 	//读回0XD0 			  	
	  	id = TFTLCD->TFTLCD_DAT;	//这里读回0X68 
			id <<= 8;
	  	id |= TFTLCD->TFTLCD_DAT;	//这里读回0X04	  
			
			//也不是6804,尝试看看是不是NT35310
			if(id != 0X6804)
			{
				TFTLCD->TFTLCD_CMD = 0XD4;				   
				id = TFTLCD->TFTLCD_DAT;//dummy read  
				id = TFTLCD->TFTLCD_DAT;//读回0X01	 
				id = TFTLCD->TFTLCD_DAT;//读回0X53	
				id <<= 8;	 
				id |= TFTLCD->TFTLCD_DAT;	//这里读回0X10	 
				
				//也不是NT35310,尝试看看是不是NT35510
				if(id != 0X5310)	
				{
					TFTLCD->TFTLCD_CMD = 0XDA00;	
					id = TFTLCD->TFTLCD_DAT;		//读回0X00	 
					TFTLCD->TFTLCD_CMD = 0XDB00;	
					id = TFTLCD->TFTLCD_DAT;		//读回0X80
					id <<= 8;	
					TFTLCD->TFTLCD_CMD = 0XDC00;	
					id |= TFTLCD->TFTLCD_DAT;		//读回0X00	
					//NT35510读回的ID是8000H,为方便区分,我们强制设置为5510
					if(id == 0x8000) id = 0x5510;
					
					//也不是NT5510,尝试看看是不是SSD1963
					if(id!=0X5510)
					{
						TFTLCD->TFTLCD_CMD = 0XA1;
						id = TFTLCD->TFTLCD_DAT;
						id = TFTLCD->TFTLCD_DAT;	//读回0X57
						id<<=8;	 
						id |= TFTLCD->TFTLCD_DAT;	//读回0X61
            //SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963						
						if(id==0X5761)id=0X1963;
					}
				}
			}
		}
	}
	
	return id;
}






