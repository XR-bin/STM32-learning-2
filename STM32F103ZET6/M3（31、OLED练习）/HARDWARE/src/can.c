#include "sys.h"  
#include "stdio.h"

/*****************************************************
*函数功能  ：对CAM相应的GPIO口和寄存器进行配置
*函数名    ：CAN1_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
********************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef         CAN_InitStruct;           //CAN主控与状态配置结构体
	CAN_FilterInitTypeDef   CAN_FilterInitStruct;     //CAN筛选器配置结构体
	
	//时钟时能  CAN1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  //使能CAN1时钟	
	
	/*GPIOx配置*/
	//回环模式直接用就行，不用GPIO
	
	/*CAN寄存器配置*/
	//主控寄存器
	CAN_InitStruct.CAN_TTCM = DISABLE;	                 //禁止时间触发通信模式     (CAN->MCR寄存器的第7位)   
	CAN_InitStruct.CAN_ABOM = DISABLE;	                 //启动自动的总线关闭管理   (CAN->MCR寄存器的第6位)	  
	CAN_InitStruct.CAN_AWUM = DISABLE;                   //启动自动唤醒模式         (CAN->MCR寄存器的第5位)	
	CAN_InitStruct.CAN_NART = DISABLE;	                 //启动报文自动传送         (CAN->MCR寄存器的第4位)	
	CAN_InitStruct.CAN_RFLM = DISABLE;	                 //报文不锁定,新的覆盖旧的  (CAN->MCR寄存器的第3位)
	CAN_InitStruct.CAN_TXFP = DISABLE;	                 //优先级由报文标识符决定   (CAN->MCR寄存器的第2位)
	CAN_InitStruct.CAN_Mode = CAN_Mode_Silent_LoopBack;	 //静默与回环模式           (CAN_BTR寄存器的第30-31位)
	CAN_InitStruct.CAN_SJW  = CAN_SJW_2tq;	             //允许两个时间片的误差     (CAN_BTR寄存器的第24-25位)
	//500K 这一参数是通过计算得到，参考参考手册622
	CAN_InitStruct.CAN_BS1  = CAN_BS1_11tq;              //Tbs1范围1 ~ 16           (CAN_BTR寄存器的第20-22位)
	CAN_InitStruct.CAN_BS2  = CAN_BS2_7tq;               //Tbs2范围1 ~ 8            (CAN_BTR寄存器的第16-19位)
	CAN_InitStruct.CAN_Prescaler = 0x03;                 //分频系数(Fdiv)为brp+1	    (CAN_BTR寄存器的第0-9位)
	CAN_Init(CAN1, &CAN_InitStruct);                     // 初始化CAN1主控与状态寄存器  
	
	//筛选器主寄存器
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;	                     //我们选择过滤器0
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdList;   //设置0号邮箱的筛选器的模式为标识符列表模式(CAN->FM1R寄存器)
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;   //0号邮箱的筛选器尺度配置为单32位尺度 (CAN->FS1R寄存器)
	
	/*************************************************************
	*注意1：
	*     CAN_FiRx寄存器分为两个筛选寄存器分别是FR1和FR2
	*     主筛选器(FR1)与从筛选器(FR2)是配合使用的,
	*     FR1是用来设置CANx邮箱的ID，FR2是用来设置CANx邮箱的掩码
	*     FR2在某位上是1的话，ID(发送过来的邮箱号)对应位上的数值
	*     就必须和FR1该位上的一样，保持一致，反之则是“不关心”。
	*
	*注意2：
	*     左移21位是因为与CAN 接收 FIFO 邮箱标识符寄存器 的位对齐，
	*     (接收 FIFO 邮箱标识符寄存器的21位开始才是标识符ID)
	***************************************************************/
	CAN_FilterInitStruct.CAN_FilterIdHigh         = (123<<5);        //高16位：邮箱ID(标准标识符):123
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;          //低16位                    主筛选器(FR1)    
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;          //高16位：邮箱ID掩码:0
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;          //低16位                    从筛选器(FR2)     
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//筛选器0关联到FIFO0  (CAN->FFA1R寄存器)
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;          //激活筛选器0         (CAN->FA1R寄存器)
	CAN_FilterInit(&CAN_FilterInitStruct);                           //初始化筛CAN1选器寄存器
}

/******************************************
*函数功能  ：CAN1总线发送一个字节数据
*函数名    ：CAN1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*函数描述  ：
*********************************************/
void CAN1_Send_Byte(u8 data)
{
	u8 mbox;
  CanTxMsg TxMessage;   //发送邮箱配置结构体
	
	//准备发送
  TxMessage.StdId = 123;	 //标准标识符(邮箱号ID) ：123                (CAN_TIxR寄存器的21-31位)
  TxMessage.ExtId = 0;	   //设置扩展标示符 ：0                        (CAN_TIxR寄存器的3-20位)
  TxMessage.IDE = 0;		   //使用扩展标识符                            (CAN_TIxR寄存器的2位)
  TxMessage.RTR = 0;		   //消息类型为数据帧                          (CAN_TIxR寄存器的1位)
  TxMessage.DLC = 1;		   //一次发送数据长度(范围：1~8个字节)：1个字节(CAN_TDTxR寄存器的0-3位)
  TxMessage.Data[0]= data; //要发送的数据                              (CAN_TDLxR寄存器)    
  mbox= CAN_Transmit(CAN1, &TxMessage);     //开始发送 ，返回值是当前所用的空邮箱(每个CAN都有3个邮箱：0邮箱、1邮箱、2邮箱)   
	
	//等待发送结束
	//CAN_TxStatus_Ok      :请求完成、发送成功、邮箱x空
	//CAN_TxStatus_Pending :无空邮箱
	//CAN_TxStatus_Failed  :请求完成、邮箱x空
  while(!(CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Ok));	
}

/******************************************
*函数功能  ：CAN1总线接收一个字节数据
*函数名    ：CAN1_Receive_Byte
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*********************************************/
void CAN1_Receive_Byte(void)
{
	CanRxMsg RxMessage;
	u8 data;

	//有数据的邮箱个数不为0就连续接收数据(CAN1->RF0R寄存器的第0-2位)
	while(CAN_MessagePending(CAN1,CAN_FIFO0) != 0)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
		data = RxMessage.Data[0];                //(CAN_RDLxR寄存器）
		
		printf("接收到的内容是:%d\r\n",data);
	}
}
















