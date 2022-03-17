#include "stm32f4xx.h"   
#include "lcd.h"
#include "fsmc.h"

/*****************************************************
*函数功能  ：批量复用引脚函数
*函数名    ：MyGPIO_AF_Set
*函数参数  ：void
*函数返回值：void
*描述      ：该函数可以批量复用同一类GPIO的引脚
********************************************************/
void MyGPIO_AF_Set(GPIO_TypeDef* GPIOx,u32 BITx,u8 AFx)
{  
	u8 pos;
	u8 i;
	for(i=0;i<16;i++)
	{
		pos = i;
		if((1<<pos)&BITx)  //判断该引脚是否要复用
		{
      //pos>>3是用于判断是高位引脚还是地位引脚
			GPIOx->AFR[pos>>3]&=~(0X0F<<((pos&0X07)*4));
			GPIOx->AFR[pos>>3]|=(u32)AFx<<((pos&0X07)*4);
		}
	}
}
/*************************** FSMC 驱 动 LCD ******************************/
/****************************************************************************
*函数功能  ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
*函数名    ：FSMC_LCD_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*     数据线：//复用 推挽   速度100M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     控制线：//复用  推挽      速度100M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(数据/命名)---PF12	 
*           BL（背光）--PB15   
*     整理：
*          PB：15
*          PD：0-1  4-5  8-10  14-15  
*          PE：7-15
*          PF：12
*          PG：12           
*     需要设置的是FSMC的Bank1的区域4
***************************************************************************/
void FSMC_LCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruc;                  //GPIOx配置结构体 
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStruct;   //FSMC配置结构体
	FSMC_NORSRAMTimingInitTypeDef  ReadTiming;         //片选时序寄存器配置结构体 （读时序控制寄存器）
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming;        //写入时序寄存器配置结构体  (写时序控制寄存器)
  
  //时钟使能  GPIOB  GPIOD  GPIOE  GPIOF  GPIOG  FSMC
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);       //使能FSMC时钟
	
  /*GPIOx端口配置*/
  //PB15
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_15;          //选择PB15
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;        //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;     //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;        //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;    //速度为100MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);                 //根据设定参数初始化PB15
	
	//PD：0-1  4-5  8-10  14-15 
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15);  //选择PD0/1/4/5/8/9/10/14/15
	GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AF;          //复用功能
	GPIO_Init(GPIOD,&GPIO_InitStruc); //根据设定参数初始化PD0/1/4/5/8/9/10/14/15
  MyGPIO_AF_Set(GPIOD,0xc733,12);   //端口复用为FSMC    批量
	
	//PE：7-15
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //选择PE：7-15
	GPIO_Init(GPIOE,&GPIO_InitStruc); //根据设定参数初始化PE：7-15
  MyGPIO_AF_Set(GPIOE,0xff80,12);   //端口复用为FSMC    批量
	
	//PF12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //选择PF12
	GPIO_Init(GPIOF,&GPIO_InitStruc);      //根据设定参数初始化PF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);   //端口复用为FSMC
	
	//PG12
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_12; //选择PG12
	GPIO_Init(GPIOG,&GPIO_InitStruc);      //根据设定参数初始化PG12
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);   //端口复用为FSMC
	
	/********** 关背光 **********/
  LCD_BL_L;
	
  /********* 配置FSMC *********/
/***********************************************************************
*注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
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
*************************************************************************/
	ReadTiming.FSMC_AddressSetupTime = 0X5;	       //地址设置阶段的持续时间：5个HCLK 1/168M=6ns*5=30ns	          (FSMC->BTR寄存器的第0-3位)
  ReadTiming.FSMC_AddressHoldTime = 0x0;	       //地址保持阶段的持续时间:Reserved(模式A未用到)                (FSMC->BTR寄存器的第4-7位)
  ReadTiming.FSMC_DataSetupTime = 8;			       //数据阶段的持续时间:16个HCLK=6*8=48ns                        (FSMC->BTR寄存器的第8-15位)
  ReadTiming.FSMC_BusTurnAroundDuration = 0x0;   //总线周转阶段的持续时间：0×HCLK 周期                        (FSMC->BTR寄存器的第16-19位)
  ReadTiming.FSMC_CLKDivision = 0x0;             //CLK 信号的时钟分频比：保留                                  (FSMC->BTR寄存器的第20-23位)
  ReadTiming.FSMC_DataLatency = 0x0;             //同步突发 NOR Flash 的数据延迟：2个 CLK 时钟周期的数据延迟    (FSMC->BTR寄存器的第24-27位)
  ReadTiming.FSMC_AccessMode = FSMC_AccessMode_A;//访问模式：访问模式 A                                        (FSMC->BTR寄存器的第28-29位) 
    
	//SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
	WriteTiming.FSMC_AddressSetupTime =0x5;	        //地址设置阶段的持续时间：5个HCLK 1/168M=6ns*5=30ns	         (FSMC->BWTR寄存器的第0-3位)
  WriteTiming.FSMC_AddressHoldTime = 0x0;	        //地址保持阶段的持续时间:Reserved(模式A未用到)                (FSMC->BWTR寄存器的第4-7位)	
  WriteTiming.FSMC_DataSetupTime = 8;		          //数据阶段的持续时间:8个HCLK=6*8=48ns                         (FSMC->BWTR寄存器的第8-15位)
  WriteTiming.FSMC_BusTurnAroundDuration = 0x0;   //总线周转阶段的持续时间：0×HCLK 时钟周期                    (FSMC->BWTR寄存器的第16-19位)
  WriteTiming.FSMC_CLKDivision = 0x0;             //CLK 信号的时钟分频比： 保留                                (FSMC->BWTR寄存器的第20-23位)
  WriteTiming.FSMC_DataLatency = 0x0;             //同步突发 NOR Flash 的数据延迟：2 个 CLK 时钟周期的数据延迟  (FSMC->BWTR寄存器的第24-27位)
  WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//访问模式：访问模式 A                                       (FSMC->BWTR寄存器的第28-29位)   
    
	//SRAM/NOR-Flash 片选控制寄存器
	FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM4;                     //选择块与区：bank1(块1)的NE4(区域4)
  FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;             //地址/数据复用使能位：不使能                             (FSMC->BCR寄存器的第1位)
  FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                    //存储器类型：SRAM、 ROM类型                              (FSMC->BCR寄存器的第2-3位)  
  FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;                //存储器数据总线宽度：16bit                               (FSMC->BCR寄存器的第4-5位) 
  FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;            //突发使能位：禁止突发访问模式                            (FSMC->BCR寄存器的第8位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;             //等待信号极性位：NWAIT 高电平有效                        (FSMC->BCR寄存器的第9位)
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;           //异步传输期间的等待信号：运行异步协议时不考虑 NWAIT 信号 (FSMC->BCR寄存器的第15位)
  FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                   //环回突发模式是否支持：未使能直接环回突发                (FSMC->BCR寄存器的第10位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;   //等待时序配置：NWAIT 信号在等待周期之前的一个数据周期有效(FSMC->BCR寄存器的第11位)
  FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;	             //写入使能位：FSMC 在存储区域内使能了写入操作             (FSMC->BCR寄存器的第12位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                 //等待使能位：禁止 NWAIT 信号                             (FSMC->BCR寄存器的第13位)
  FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Enable;                //读和写操作是否采用相同时序：读写使用不相同的时序        (FSMC->BCR寄存器的第14位)
  FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                 //写入突发使能：始终在异步模式下进行写入操作              (FSMC->BCR寄存器的第19位)
	//SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadTiming;
	//SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;                       

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);  //初始化FSMC配置

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1区域4	
}



/***************************FSMC 驱 动 外部SRAN******************************/
/************************************************************************
*函数功能  ：对特定的GPIO口进行FSMC复用，并设置FSMC，用于驱动外部SRAN
*函数名    ：FSMC_SRAM_Init
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*     需要配置的引脚： 
*                   PD：0/1/4/5/8/9/10/11/12/13/14/15 
*                   PE：0/1/7/8/9/10/11/12/13/14/15
*                   PF：0/1/2/3/4/5/12/13/14/15
*                   PG：0/1/2/3/4/5/10
*              需要设置的是FSMC的Bank1的区域3
*************************************************************************/
void FSMC_SRAM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruc;                  //GPIOx配置结构体 
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStruct;   //FSMC配置结构体
	FSMC_NORSRAMTimingInitTypeDef  ReadTiming;         //片选时序寄存器配置结构体 （读时序控制寄存器）
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming;        //写入时序寄存器配置结构体  (写时序控制寄存器)
  
  //时钟使能   GPIOD  GPIOE  GPIOF  GPIOG  FSMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);       //使能FSMC时钟
	
	//特别注意！！！！！
  //PG10,AF12(CS放到最前面,防止复位后CS非法变低,破坏原有数据)
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);   //端口复用为FSMC
	
  /*GPIOx端口配置*/
	//PD：0-1  4-5  8-15 
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //选择PD0/1/4/5/8/9/10/11/12/13/14/15
	GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AF;          //复用功能
	GPIO_Init(GPIOD,&GPIO_InitStruc); //根据设定参数初始化PD0/1/4/5/8/9/10/11/12/13/14/15
  MyGPIO_AF_Set(GPIOD,0xff33,12);   //端口复用为FSMC    批量
	
	//PE：0-1  7-15   
	GPIO_InitStruc.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //选择PE0/1/7/8/9/10/11/12/13/14/15
	GPIO_Init(GPIOE,&GPIO_InitStruc); //根据设定参数初始化PE：0/1/7/8/9/10/11/12/13/14/15
  MyGPIO_AF_Set(GPIOE,0xff83,12);   //端口复用为FSMC    批量
	
	//PF0-5 12-15     
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //选择PF0/1/2/3/4/5/12/13/14/15
	GPIO_Init(GPIOF,&GPIO_InitStruc);      //根据设定参数初始化PF0/1/2/3/4/5/12/13/14/15
	MyGPIO_AF_Set(GPIOF,0xf03f,12);   //端口复用为FSMC
	
	//PG0-5 10   
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10;   //选择PG0/1/2/3/4/5/10
	GPIO_Init(GPIOG,&GPIO_InitStruc);      //根据设定参数初始化PG0/1/2/3/4/5/10
	MyGPIO_AF_Set(GPIOG,0x003f,12);   //端口复用为FSMC
	
	/********** 关背光 **********/
  LCD_BL_L;
	
  /********* 配置FSMC *********/
/***********************************************************************
*注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
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
*************************************************************************/
	//SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）
	ReadTiming.FSMC_AddressSetupTime = 0X5;	       //地址设置阶段的持续时间：5个HCLK 1/168M=6ns*5=30ns	         (FSMC->BTR寄存器的第0-3位)
  ReadTiming.FSMC_AddressHoldTime = 0x0;	       //地址保持阶段的持续时间:Reserved(模式A未用到)                (FSMC->BTR寄存器的第4-7位)
  ReadTiming.FSMC_DataSetupTime = 0x5;			     //数据阶段的持续时间:5个HCLK=6*5=30ns                         (FSMC->BTR寄存器的第8-15位)
  ReadTiming.FSMC_BusTurnAroundDuration = 0xF;   //总线周转阶段的持续时间： 15×HCLK 时钟周期                  (FSMC->BTR寄存器的第16-19位)
  ReadTiming.FSMC_CLKDivision = 0xF;             //CLK 信号的时钟分频比： 16×HCLK 周期                        (FSMC->BTR寄存器的第20-23位)
  ReadTiming.FSMC_DataLatency = 0xF;             //同步突发 NOR Flash 的数据延迟：17 个 CLK 时钟周期的数据延迟 (FSMC->BTR寄存器的第24-27位)
  ReadTiming.FSMC_AccessMode = FSMC_AccessMode_A;//访问模式：访问模式 A                                        (FSMC->BTR寄存器的第28-29位) 
    
	//SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
	WriteTiming.FSMC_AddressSetupTime =0x2;	        //地址设置阶段的持续时间：2个HCLK 1/168M=6ns*2=12ns	           (FSMC->BWTR寄存器的第0-3位)
  WriteTiming.FSMC_AddressHoldTime = 0x0;	        //地址保持阶段的持续时间:Reserved(模式A未用到)                 (FSMC->BWTR寄存器的第4-7位)	
  WriteTiming.FSMC_DataSetupTime = 0x3;		        //数据阶段的持续时间:3个HCLK=6*3=18ns                          (FSMC->BWTR寄存器的第8-15位)
  WriteTiming.FSMC_BusTurnAroundDuration = 0x3;   //总线周转阶段的持续时间：3×HCLK 时钟周期                     (FSMC->BWTR寄存器的第16-19位)
  WriteTiming.FSMC_CLKDivision = 0x8;             //CLK 信号的时钟分频比： 8×HCLK 周期                         (FSMC->BWTR寄存器的第20-23位)
  WriteTiming.FSMC_DataLatency = 0xF;             //同步突发 NOR Flash 的数据延迟：17 个 CLK 时钟周期的数据延迟  (FSMC->BWTR寄存器的第24-27位)
  WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//访问模式：访问模式 A                                         (FSMC->BWTR寄存器的第28-29位)  
    
	//SRAM/NOR-Flash 片选控制寄存器
	FSMC_NORSRAMInitStruct.FSMC_Bank                  = FSMC_Bank1_NORSRAM3;                     //选择块与区：bank1(块1)的NE3(区域4)
  FSMC_NORSRAMInitStruct.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;             //地址/数据复用使能位：不使能                             (FSMC->BCR寄存器的第1位)
  FSMC_NORSRAMInitStruct.FSMC_MemoryType            = FSMC_MemoryType_SRAM;                    //存储器类型：SRAM、 ROM类型                              (FSMC->BCR寄存器的第2-3位)  
  FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;                //存储器数据总线宽度：16bit                               (FSMC->BCR寄存器的第4-5位) 
  FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;            //突发使能位：禁止突发访问模式                            (FSMC->BCR寄存器的第8位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;             //等待信号极性位：NWAIT 高电平有效                        (FSMC->BCR寄存器的第9位)
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;           //异步传输期间的等待信号：运行异步协议时不考虑 NWAIT 信号 (FSMC->BCR寄存器的第15位)
  FSMC_NORSRAMInitStruct.FSMC_WrapMode              = FSMC_WrapMode_Disable;                   //环回突发模式是否支持：未使能直接环回突发                (FSMC->BCR寄存器的第10位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;   //等待时序配置：NWAIT 信号在等待周期之前的一个数据周期有效(FSMC->BCR寄存器的第11位)
  FSMC_NORSRAMInitStruct.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;	             //写入使能位：FSMC 在存储区域内使能了写入操作             (FSMC->BCR寄存器的第12位)
  FSMC_NORSRAMInitStruct.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;                 //等待使能位：禁止 NWAIT 信号                             (FSMC->BCR寄存器的第13位)
  FSMC_NORSRAMInitStruct.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;               //读和写操作是否采用相同时序：读写使用相同的时序          (FSMC->BCR寄存器的第14位)
  FSMC_NORSRAMInitStruct.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;                 //写入突发使能：始终在异步模式下进行写入操作              (FSMC->BCR寄存器的第19位)
	//SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）
  FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadTiming;
	//SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
  FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct     = &WriteTiming;                       

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);  //初始化FSMC配置

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // 使能BANK1区域3	
}











