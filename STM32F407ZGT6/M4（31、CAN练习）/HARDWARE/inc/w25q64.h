#ifndef _W25Q64_H
#define _W25Q64_H

  #include "stm32f4xx.h"  
  
	//Ƭѡ
  #define SPI_CS_H  GPIO_SetBits(GPIOB,GPIO_Pin_14)
  #define SPI_CS_L  GPIO_ResetBits(GPIOB,GPIO_Pin_14)
  
  void W25Q64_Init(void);
  void W25Q64_Write_Enable(void);
  u8 W25Q64_Read_Status(void);
  void W25Q64_Read_Bytes(u32 address,u32 num_byte,u8 *str);
  void W25Q64_Page_Write(u32 address,u16 num_byte,u8 *str);
  void W25Q64_Sector_Erase(u32 address);
  void W25Q64_Block_Erase(u32 address);
  void W25Q64_Chip_Erase(void);
  void W25Q64_Blocks_Erase(u32 address,u8 num_blcok);
#endif











