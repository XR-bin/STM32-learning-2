#include "stm32f4xx.h" 
#include "key.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}



/*****************************************************
*函数功能  ：对KEY对应的GPIO口进行初始化设置
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           KEY1  PA0    高电平按下
*           KEY2  PE2    低电平按下
*           KEY3  PE3    低电平按下
*           KEY4  PE4    低电平按下
*                  输入模式
*                  无上下拉
********************************************************/
void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruc;
  
  //时钟使能  GPIOA  GPIOE 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//PA0
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_0;       //PA0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_IN;     //通用输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //无上下拉
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;  //速度为50MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);             //根据设定参数初始化PA0
	//PE2/3/4
	//因为PE2/3/4与PA0配置一样，所以一些变量无需改变
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;       //PE2/3/4
	GPIO_Init(GPIOE,&GPIO_InitStruc);              //根据设定参数初始化PA0
}

/*****************************************************
*函数功能  ：按键扫描函数
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay&&(KEY1||(!KEY2)||(!KEY3)||(!KEY4)))
  {
    delay_ms(10);
    if(KEY1||(!KEY2)||(!KEY3)||(!KEY4))
    {
      key_flay = 0;
      if(KEY1) temp = 1;
      else if(!(KEY2)) temp = 2;
      else if(!(KEY3)) temp = 3;
      else if(!(KEY4)) temp = 4;
    }
  }
  
  if((!KEY1)&& KEY2 && KEY3 && KEY4)
  {
    key_flay = 1;
  }
  
  return temp;
}


/*****************************************************
*函数功能  ：两个按键同时按
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
u8 KEY_Scan2(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  static u32 cont = 0 ;
  if(key_flay &&(KEY1||(!KEY2)))
  {
    cont++;
    if(KEY1 && (!KEY2) && (cont==250000))
    {
      delay_ms(10);
      if(KEY1 && !(KEY2))
      {
        key_flay = 0;
        temp=12;
      }
    }
  }
  if( cont > 250000)
  {
    cont = 255000;
  }
  if((!KEY1) && KEY2)
  {
    key_flay = 1;
    cont = 0;
  }
  
  return temp;
}




