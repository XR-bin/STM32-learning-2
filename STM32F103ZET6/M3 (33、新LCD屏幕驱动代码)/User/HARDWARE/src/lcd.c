#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "fsmc.h"

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	

/***************************************************************************************
*函数功能  ：主控芯片发送命令函数
*函数名    ：LCD_ILI9341_CMD
*函数参数  ：u8 cmd    
*函数返回值：void
*函数描述  ：  
*****************************************************************************************/
void LCD_ILI9341_CMD(u8 cmd)
{
	/********************** 通过FSMC驱动方式 **********************/
  //LCD_CMD1 = cmd;
  LCD->LCD_CMD = cmd;
}

/***************************************************************************************
*函数功能  ：主控芯片发送数据函数
*函数名    ：LCD_ILI9341_Parameter
*函数参数  ：u16  data   
*函数返回值：void
*函数描述  ：	
*****************************************************************************************/
void LCD_ILI9341_Parameter(u16 data)
{
/********************** 通过FSMC驱动方式 **********************/
  //LCD_DAT1 = data;
  LCD->LCD_DAT = data;
}

/***************************************************************************************
*函数功能  ：主控芯片读取数据函数
*函数名    ：LCD_ILI9341_ReadData
*函数参数  ：void   
*函数返回值：u16
*函数描述  ：  
*****************************************************************************************/
u16 LCD_ILI9341_ReadData(void)
{
	return LCD->LCD_DAT;
}

/**************************************************************************
*函数功能  ：LCD屏幕清屏函数
*函数名    ：LCD_Clear
*函数参数  ：u16 colour
*函数返回值：void
*函数描述  ：
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Clear(u16 colour)
{
  u32 i;
  
  i = lcddev.width * lcddev.height;
   
  //发送命令(x坐标)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  
  //发送命令(颜色)
  LCD_ILI9341_CMD(lcddev.wramcmd);
  //开始填色
  while(i)
  {
    LCD->LCD_DAT = colour;
    i--;
  }
}

/**************************************************************************
*函数功能  ：设置LCD的自动扫描方向
*函数名    ：LCD_Scan_Dir
*函数参数  ：u8 dir
*函数返回值：void
*函数描述  ：
*****************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;
	
	if(lcddev.dir==1)//横屏时处理
	{			   
		switch(dir)//方向转换
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
	
	switch(dir)
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://从上到下,从左到右
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	
	dirreg=0X36;
	regval|=0X08;
	LCD->LCD_CMD = dirreg;
	LCD->LCD_DAT = regval;
	
	//对屏幕宽高做调整
	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
	else  
	{
		if(lcddev.width>lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
  LCD_ILI9341_CMD(lcddev.setxcmd); 
	LCD_ILI9341_Parameter(0);LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((lcddev.width-1)>>8);LCD_ILI9341_Parameter((lcddev.width-1)&0XFF);
	LCD_ILI9341_CMD(lcddev.setycmd); 
	LCD_ILI9341_Parameter(0);LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((lcddev.height-1)>>8);LCD_ILI9341_Parameter((lcddev.height-1)&0XFF);
}

/**************************************************************************
*函数功能  ：设置横竖屏
*函数名    ：LCD_Display_Dir
*函数参数  ：u8 dir:0,竖屏；1,横屏
*函数返回值：void
*函数描述  ：
*****************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir = 0;	            //竖屏
		lcddev.width = LCD_WIDTH;   //屏幕宽
		lcddev.height = LCD_HEIGHT; //屏幕高
		
		lcddev.wramcmd=0X2C;    //颜色指令
		lcddev.setxcmd=0X2A;    //x坐标指令
		lcddev.setycmd=0X2B;    //y坐标指令
	}
	else
	{
		lcddev.dir = 1;	            //横屏
		lcddev.width = LCD_HEIGHT;  //屏幕宽
		lcddev.height = LCD_WIDTH;  //屏幕高
		
		lcddev.wramcmd=0X2C;    //颜色指令
		lcddev.setxcmd=0X2A;    //x坐标指令
		lcddev.setycmd=0X2B;    //y坐标指令
	}
	
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}

/***************************************************************************************
*函数功能  ：LCD屏幕初始化函数
*函数名    ：LCD_Init
*函数参数  ：void   
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************************/
void LCD_Init(void)
{
	/******** FSMC驱动方式 ********/
  /*IO口初始化和FSMC初始化*/
  FSMC_LCD_Init();
	
	delay1_ms(10);
	lcddev.id = 9341;
	
  
  /*LCD9341厂家代码*/
	LCD_ILI9341_CMD(0xCF);  
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0xC1); 
	LCD_ILI9341_Parameter(0X30); 
	LCD_ILI9341_CMD(0xED);  
	LCD_ILI9341_Parameter(0x64); 
	LCD_ILI9341_Parameter(0x03); 
	LCD_ILI9341_Parameter(0X12); 
	LCD_ILI9341_Parameter(0X81); 
	LCD_ILI9341_CMD(0xE8);  
	LCD_ILI9341_Parameter(0x85); 
	LCD_ILI9341_Parameter(0x10); 
	LCD_ILI9341_Parameter(0x7A); 
	LCD_ILI9341_CMD(0xCB);  
	LCD_ILI9341_Parameter(0x39); 
	LCD_ILI9341_Parameter(0x2C); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x34); 
	LCD_ILI9341_Parameter(0x02); 
	LCD_ILI9341_CMD(0xF7);  
	LCD_ILI9341_Parameter(0x20); 
	LCD_ILI9341_CMD(0xEA);  
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_CMD(0xC0);    //Power control 
	LCD_ILI9341_Parameter(0x1B);   //VRH[5:0] 
	LCD_ILI9341_CMD(0xC1);    //Power control 
	LCD_ILI9341_Parameter(0x01);   //SAP[2:0];BT[3:0] 
	LCD_ILI9341_CMD(0xC5);    //VCM control 
	LCD_ILI9341_Parameter(0x30); 	 //3F
	LCD_ILI9341_Parameter(0x30); 	 //3C
	LCD_ILI9341_CMD(0xC7);    //VCM control2 
	LCD_ILI9341_Parameter(0XB7); 
	LCD_ILI9341_CMD(0x36);    // Memory Access Control 
	LCD_ILI9341_Parameter(0x48); 
	LCD_ILI9341_CMD(0x3A);   
	LCD_ILI9341_Parameter(0x55); 
	LCD_ILI9341_CMD(0xB1);   
	LCD_ILI9341_Parameter(0x00);   
	LCD_ILI9341_Parameter(0x1A); 
	LCD_ILI9341_CMD(0xB6);    // Display Function Control 
	LCD_ILI9341_Parameter(0x0A); 
	LCD_ILI9341_Parameter(0xA2); 
	LCD_ILI9341_CMD(0xF2);    // 3Gamma Function Disable 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_CMD(0x26);    //Gamma curve selected 
	LCD_ILI9341_Parameter(0x01); 
	LCD_ILI9341_CMD(0xE0);    //Set Gamma 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x2A); 
	LCD_ILI9341_Parameter(0x28); 
	LCD_ILI9341_Parameter(0x08); 
	LCD_ILI9341_Parameter(0x0E); 
	LCD_ILI9341_Parameter(0x08); 
	LCD_ILI9341_Parameter(0x54); 
	LCD_ILI9341_Parameter(0XA9); 
	LCD_ILI9341_Parameter(0x43); 
	LCD_ILI9341_Parameter(0x0A); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x00); 		 
	LCD_ILI9341_CMD(0XE1);    //Set Gamma 
	LCD_ILI9341_Parameter(0x00); 
	LCD_ILI9341_Parameter(0x15); 
	LCD_ILI9341_Parameter(0x17); 
	LCD_ILI9341_Parameter(0x07); 
	LCD_ILI9341_Parameter(0x11); 
	LCD_ILI9341_Parameter(0x06); 
	LCD_ILI9341_Parameter(0x2B); 
	LCD_ILI9341_Parameter(0x56); 
	LCD_ILI9341_Parameter(0x3C); 
	LCD_ILI9341_Parameter(0x05); 
	LCD_ILI9341_Parameter(0x10); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_Parameter(0x3F); 
	LCD_ILI9341_Parameter(0x3F); 
	LCD_ILI9341_Parameter(0x0F); 
	LCD_ILI9341_CMD(0x2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3f);
	LCD_ILI9341_CMD(0x2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xef);	 
	LCD_ILI9341_CMD(0x11); //Exit Sleep
	delay_ms(120);
	LCD_ILI9341_CMD(0x29); //display on	

  
  /*用户自己编写*/
	//设置屏幕为竖屏
	LCD_Display_Dir(0);  
	//背光灯打开
	LCD_BL_H;
	//清屏函数
	LCD_Clear(WHITE);
}

/**************************************************************************
*函数功能  ：LCD屏幕将某个矩形区域某种颜色
*函数名    ：LCD_Clear_XY
*函数参数  ：u16 x,u16 y,u16 w,u16 h,u16 colour  
*函数返回值：void
*函数描述  ：  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Clear_XY(u16 x,u16 y,u16 width,u16 height,u16 color)
{
  u32 i;
  i = width * height;
  //发送命令(x坐标)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(x>>8);
  LCD_ILI9341_Parameter(x);
  //末尾x坐标
  LCD_ILI9341_Parameter((x+width) >> 8);
  LCD_ILI9341_Parameter(x+width);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(y>>8);
  LCD_ILI9341_Parameter(y);
  //末尾y坐标
  LCD_ILI9341_Parameter((y+height) >> 8);
  LCD_ILI9341_Parameter(y+height);
  
  //发送命令(颜色)
  LCD_ILI9341_CMD(lcddev.wramcmd);
  //开始填色
  while(i)
  {
    LCD->LCD_DAT = color;
    i--;
  }
}

/**************************************************************************
*函数功能  ：在指定区域内填充指定颜色块
*函数名    ：LCD_Color_Fill
*函数参数  ：u16 x1,u16 y1,u16 x2,u16 y2,u16 *color 
*函数返回值：void
*函数描述  ：  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Color_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 *color)
{  
	u16 height,width;
	u16 i,j,y=0;
	width = x2 - x1 + 1; 			//得到填充的宽度
	height = y2 - y1 + 1;			//高度
 	for(i=0;i<height;i++)
	{
		//发送命令(x坐标)
		LCD_ILI9341_CMD(lcddev.setxcmd);
		LCD_ILI9341_Parameter(x1>>8);
		LCD_ILI9341_Parameter(x1&0xff);
		//发送命令(y坐标)
		y=y1+i;
		LCD_ILI9341_CMD(lcddev.setycmd);
		LCD_ILI9341_Parameter(y>>8);
		LCD_ILI9341_Parameter(y&0xff);
		
		//发送命令(颜色)
		LCD_ILI9341_CMD(lcddev.wramcmd);
		for(j=0;j<width;j++) LCD->LCD_DAT=color[i*width+j];
	}		  
} 

/**************************************************************************
*函数功能  ：LCD屏幕画点函数
*函数名    ：LCD_Point
*函数参数  ：u16 x,u16 y,u16 colour 
*函数返回值：void
*函数描述  ：
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Point(u16 x,u16 y,u16 color)
{
  //发送命令(x坐标)
  LCD_ILI9341_CMD(lcddev.setxcmd);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(x >> 8);
  LCD_ILI9341_Parameter(x);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(lcddev.setycmd);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(y >> 8);
  LCD_ILI9341_Parameter(y);
  
  //发送命令(颜色)
  LCD->LCD_CMD = lcddev.wramcmd;
  //开始填色
  LCD->LCD_DAT=color;
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(空心)
*函数名    ：LCD_Circle
*函数参数  ：u16 x,u16 y,u16 r,u16 colour 
*函数返回值：void
*函数描述  ：			
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void LCD_Circle(u16 x,u16 y,u16 r,u16 color)
{
  u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				LCD_Point(i,j,color);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(实心圆)
*函数名    ：LCD_Circle2
*函数参数  ：u16 x,u16 y,u16 r,u16 colour  颜色
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void LCD_Circle2(u16 x,u16 y,u16 r,u16 color)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				LCD_Point(i,j,color);
			}
		}
	}
}




























