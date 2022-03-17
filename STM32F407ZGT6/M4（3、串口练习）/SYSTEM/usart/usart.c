#include "stm32f4xx.h" 

/******************************************************************
*函数功能  ：对串口1对应的GPIO口进行初始化和对串口1的寄存器进行设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
*           PA9   TX    输出
*           PA10  RX    输入
*******************************************************************/
void USART1_Init(u32 bps)
{
	GPIO_InitTypeDef   GPIO_InitStruc;    //GPIOx配置结构体
	USART_InitTypeDef  USART_InitStruct;  //USART配置结构体
	
  //时钟使能   GPIOA   USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	
	/*GPIO口配置*/
	//PA9、PA10
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //选择PA9、PA10
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;     	//速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //根据设定参数初始化PA9，PA10
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //PA9 复用为USART1的输出(TX)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //PA10复用为USART1的输入(RX)
	
	/*串口初始化*/
	//USART1
	USART_InitStruct.USART_BaudRate = bps;                    //波特率设置(USART->BRR寄存器)
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式(USART->CR1寄存器的第12位)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;       //一个停止位(USART->CR2寄存器的第12、13位)
	USART_InitStruct.USART_Parity = USART_Parity_No;          //无奇偶校验位(USART->CR1寄存器的第10位)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制(USART->CR3寄存器的第9位)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式(USART->CR1寄存器的第2、3位)
  USART_Init(USART1, &USART_InitStruct);                        //初始化串口1
	
	//串口使能
	USART_Cmd(USART1, ENABLE);  //使能串口1
}


/******************************************************************
*函数功能  ：串口1发送一个字节数据（8位）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//等待发送结束(USART1->SR的第6位)
	USART_SendData(USART1, data);                           //发送一个字节的数据(USART1->DR)
}

/******************************************************************
*函数功能  ：串口1接收一个字节数据（8位）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8 
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET)    //等待发送结束(USART1->SR的第5位)

  data = USART_ReceiveData(USART1);                             //读取一个字节的数据(USART1->DR)
  
  return data;
}















