#ifndef __STMFLASH_H
#define __STMFLASH_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* FLASH起始地址 */
    #define STM32_FLASH_SIZE        0x80000         /* STM32 FLASH 总大小 */
    #define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH 起始地址 */

    /* STM32F103 扇区大小 */
    #if STM32_FLASH_SIZE < 256 * 1024
        #define STM32_SECTOR_SIZE   1024           /* 容量小于256K的 F103, 扇区大小为1K字节 */
    #else
        #define STM32_SECTOR_SIZE   2048           /* 容量大于等于于256K的 F103, 扇区大小为2K字节 */
    #endif



    /****************    函数外部声明   *****************/
    uint16_t STMFLASH_Read_HalfWord(uint32_t faddr);                               /* 从内部flash读取一个uint16_t数据 */
    void STMFLASH_Read(uint32_t rAddr, uint16_t *pBuf, uint16_t length);           /* 从内部flash读取多个uint16_t数据 */
    void STMFLASH_Write_NoCheck(uint32_t wAddr, uint16_t *pBuf, uint16_t length);  /* 向内部flash写入多个uint16_t数据(不带检测) */
    void STMFLASH_Write(uint32_t wAddr, uint16_t *pBuf, uint16_t length);          /* 向内部flash写入多个uint16_t数据(有带检测) */

#endif





