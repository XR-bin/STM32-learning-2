#ifndef _LCD_H
#define _LCD_H

  #include "stm32f10x.h"
	
	//LCD重要参数集
	typedef struct  
	{										    
		u16 width;			//LCD 宽度
		u16 height;			//LCD 高度
		u16 id;				  //LCD ID
		u8  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
		u16	wramcmd;		//开始写gram指令
		u16 setxcmd;		//设置x坐标指令
		u16 setycmd;		//设置y坐标指令 
	}_lcd_dev; 
  
    /*******控制线********/
  //片选
  #define LCD_CS_H    GPIO_SetBits(GPIOG,GPIO_Pin_12)
  #define LCD_CS_L   	GPIO_ResetBits(GPIOG,GPIO_Pin_12) 
  //写使能
  #define LCD_WR_H   	GPIO_SetBits(GPIOD,GPIO_Pin_5)
  #define LCD_WR_L   	GPIO_ResetBits(GPIOD,GPIO_Pin_5)
  //读使能
  #define LCD_RD_H   	GPIO_SetBits(GPIOD,GPIO_Pin_4) 
  #define LCD_RD_L   	GPIO_ResetBits(GPIOD,GPIO_Pin_4) 
  //数据命令选择线
  #define LCD_DC_H   	GPIO_SetBits(GPIOG,GPIO_Pin_0) 
  #define LCD_DC_L   	GPIO_ResetBits(GPIOG,GPIO_Pin_0) 
  //背光
  #define LCD_BL_H    GPIO_SetBits(GPIOB,GPIO_Pin_0)
  #define LCD_BL_L    GPIO_ResetBits(GPIOB,GPIO_Pin_0)

  #define LCD_DO_0(a)   (a) ? (GPIOD->ODR |= (1<<14)) : (GPIOD->ODR &= ~(1<<14))
  #define LCD_DO_1(a)   (a) ? (GPIOD->ODR |= (1<<15)) : (GPIOD->ODR &= ~(1<<15))
  #define LCD_DO_2(a)   (a) ? (GPIOD->ODR |= (1<<0))  : (GPIOD->ODR &= ~(1<<0))
  #define LCD_DO_3(a)   (a) ? (GPIOD->ODR |= (1<<1))  : (GPIOD->ODR &= ~(1<<1))
  #define LCD_DO_4(a)   (a) ? (GPIOE->ODR |= (1<<7))  : (GPIOE->ODR &= ~(1<<7))
  #define LCD_DO_5(a)   (a) ? (GPIOE->ODR |= (1<<8))  : (GPIOE->ODR &= ~(1<<8))
  #define LCD_DO_6(a)   (a) ? (GPIOE->ODR |= (1<<9))  : (GPIOE->ODR &= ~(1<<9))
  #define LCD_DO_7(a)   (a) ? (GPIOE->ODR |= (1<<10)) : (GPIOE->ODR &= ~(1<<10))
  #define LCD_DO_8(a)   (a) ? (GPIOE->ODR |= (1<<11)) : (GPIOE->ODR &= ~(1<<11))
  #define LCD_DO_9(a)   (a) ? (GPIOE->ODR |= (1<<12)) : (GPIOE->ODR &= ~(1<<12))
  #define LCD_DO_10(a)  (a) ? (GPIOE->ODR |= (1<<13)) : (GPIOE->ODR &= ~(1<<13))
  #define LCD_DO_11(a)  (a) ? (GPIOE->ODR |= (1<<14)) : (GPIOE->ODR &= ~(1<<14))
  #define LCD_DO_12(a)  (a) ? (GPIOE->ODR |= (1<<15)) : (GPIOE->ODR &= ~(1<<15))
  #define LCD_DO_13(a)  (a) ? (GPIOD->ODR |= (1<<8))  : (GPIOD->ODR &= ~(1<<8))
  #define LCD_DO_14(a)  (a) ? (GPIOD->ODR |= (1<<9))  : (GPIOD->ODR &= ~(1<<9))
  #define LCD_DO_15(a)  (a) ? (GPIOD->ODR |= (1<<10)) : (GPIOD->ODR &= ~(1<<10))
  
  //屏幕大小
  #define LCD_WIDTH     240    //宽
  #define LCD_HEIGHT    320    //高
  //显示方向
  #define LCD_Crosswise  0xA8  //横向
  #define LCD_Lengthways 0x08  //纵向
	
	//扫描方向定义
	#define L2R_U2D  0 //从左到右,从上到下
	#define L2R_D2U  1 //从左到右,从下到上
	#define R2L_U2D  2 //从右到左,从上到下
	#define R2L_D2U  3 //从右到左,从下到上

	#define U2D_L2R  4 //从上到下,从左到右
	#define U2D_R2L  5 //从上到下,从右到左
	#define D2U_L2R  6 //从下到上,从左到右
	#define D2U_R2L  7 //从下到上,从右到左	 

	#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向
  
  //颜色
  //GUI颜色
  #define WHITE        0xFFFF
  #define BLACK        0x0000	  
  #define BLUE         0x001F  
  #define BRED         0XF81F
  #define GRED 			   0XFFE0
  #define GBLUE			   0X07FF
  #define RED          0xF800
  #define MAGENTA      0xF81F
  #define GREEN        0x07E0
  #define CYAN         0x7FFF
  #define YELLOW       0xFFE0
  #define BROWN 			 0XBC40 //棕色
  #define BRRED 			 0XFC07 //棕红色
  #define GRAY  			 0X8430 //灰色
  //PANEL的颜色
  #define DARKBLUE     0X01CF	//深蓝色
  #define LIGHTBLUE    0X7D7C	//浅蓝色  
  #define GRAYBLUE     0X5458 //灰蓝色
  #define LIGHTGREEN   0X841F //浅绿色
  #define LIGHTGRAY    0XEF5B //浅灰色(PANNEL)
  #define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
  #define LGRAYBLUE    0XA651 //浅灰蓝色(中间层颜色)
  #define LBBLUE       0X2B12 //浅棕蓝色(选择条目的反色)
	
	void LCD_IO_Init(void);
  void LCD_ILI9341_CMD(u8 cmd);
  void LCD_ILI9341_Parameter(u16 data);
	u16 LCD_ILI9341_ReadData(void);
  void LCD_Clear(u16 colour);
	void LCD_Scan_Dir(u8 dir);
	void LCD_Display_Dir(u8 dir);
  void LCD_Init(void);
  void LCD_Clear_XY(u16 x,u16 y,u16 width,u16 height,u16 color);
	void LCD_Color_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 *color);
  void LCD_Point(u16 x,u16 y,u16 color);
  void LCD_Circle(u16 x,u16 y,u16 r,u16 color);
  void LCD_Circle2(u16 x,u16 y,u16 r,u16 color);
	
#endif	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
