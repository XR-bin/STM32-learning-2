#ifndef __W25Q128_H
#define __W25Q128_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* 片选 */
    #define SPI_CS_H  GPIO_SetBits(GPIOB,GPIO_Pin_12)     /* 片选失能 */
    #define SPI_CS_L  GPIO_ResetBits(GPIOB,GPIO_Pin_12)   /* 片选使能 */

    /* 指令表 */
    #define FLASH_WriteEnable           0x06        /* 写使能 */
    #define FLASH_WriteDisable          0x04        /* 写失能 */
    #define FLASH_ReadStatusReg1        0x05        /* 读状态寄存器1 */
    #define FLASH_ReadStatusReg2        0x35        /* 读状态寄存器2 */
    #define FLASH_ReadStatusReg3        0x15        /* 读状态寄存器3 */
    #define FLASH_WriteStatusReg1       0x01        /* 写状态寄存器1 */
    #define FLASH_WriteStatusReg2       0x31        /* 写状态寄存器2 */
    #define FLASH_WriteStatusReg3       0x11        /* 写状态寄存器3 */
    #define FLASH_ReadData              0x03        /* 读数据指令 */
    #define FLASH_FastReadData          0x0B 
    #define FLASH_FastReadDual          0x3B 
    #define FLASH_FastReadQuad          0xEB  
    #define FLASH_PageProgram           0x02        /* 写页指令 */
    #define FLASH_PageProgramQuad       0x32 
    #define FLASH_BlockErase            0xD8 
    #define FLASH_SectorErase           0x20        /* 扇区擦除 */
    #define FLASH_ChipErase             0xC7        /* 芯片擦除 */
    #define FLASH_PowerDown             0xB9 
    #define FLASH_ReleasePowerDown      0xAB 
    #define FLASH_DeviceID              0xAB 
    #define FLASH_ManufactDeviceID      0x90 
    #define FLASH_JedecDeviceID         0x9F 
    #define FLASH_Enable4ByteAddr       0xB7
    #define FLASH_Exit4ByteAddr         0xE9
    #define FLASH_SetReadParam          0xC0 
    #define FLASH_EnterQPIMode          0x38
    #define FLASH_ExitQPIMode           0xFF



    /****************    函数外部声明   *****************/
    void W25Q128_Init(void);                             /* W25Q128初始化 */
    uint8_t W25Q128_Read_SR(uint8_t serial);             /* 读取W25Q128状态寄存器值 */
    void W25Q128_Write_SR(uint8_t serial, uint8_t data); /* 修改W25Q128状态寄存器值 */
    static void W25Q128_Wait_Busy(void);                 /* 等待W25Q128空闲 */
    void W25Q128_Write_Enable(void);                     /* W25Q128写使能 */
    static void W25Q128_Send_Address(uint32_t address);  /* 向W25QXX发送地址 */

    void W25Q128_Page_Write(uint32_t address, uint32_t datalen, uint8_t *arr);           /* 写数据到W25Q128(页操纵，不可跨页) */
    static void W25Q128_Write_NoCheck(uint32_t address, uint16_t datalen, uint8_t *arr); /* 写数据到W25Q128(无检验写操作) */

    /* W25Q128读写正式函数 */
    void W25Q128_Read_Bytes(uint32_t address, uint32_t datalen, uint8_t *arr); /* 从W25Q128读取数据 */
    void W25Q128_Write(uint32_t address, uint16_t datalen, uint8_t *arr);      /* 写数据到W25Q128(带擦操作) */

    void W25Q128_Sector_Erase(uint32_t address);                    /* 扇区擦除 */
    void W25Q128_Block_Erase(uint32_t address);                     /* 块区擦除 */
    void W25Q128_Chip_Erase(void);                                  /* 芯片擦除 */
    void W25Q128_Blocks_Erase(uint32_t address,uint8_t blockNum);   /* 擦除多个块 */

#endif



