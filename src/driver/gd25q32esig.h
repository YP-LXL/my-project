#ifndef GD25Q32ESIG_H
#define GD25Q32ESIG_H

#include "common.h"
#include "gd32f4x.h"

#define SPI_OK   0
#define SPI_ERR  1
#define SPI_FLASH_PAGE_SIZE    256
typedef enum
{
	GD25Q80CSIG_OK								= 0x00,
	GD25Q80CSIG_ERROR							= 0x01
}GD25Q_StatusTypeDef;

uint8_t spi_flash_gd25q32esig_init(void);
uint8_t spi_flash_erase_chip(void);
uint8_t spi_flash_erase_sector(uint32_t block);
uint8_t spi_flash_gd25q32esig_read(uint32_t addr, uint8_t* p_data, uint32_t len);
uint8_t spi_flash_gd25q32esig_write(uint32_t addr, uint8_t* p_data, uint32_t len);
uint8_t spi_flash_buffer_write(uint32_t write_addr, uint8_t *pbuffer,  uint32_t len);
uint8_t erase_and_write_address(uint32_t target_addr, uint8_t *data, uint16_t len);
uint16_t gd25a32_readid(void);
void gd25a32_write_enable(void);
void gd25a32_wait_busy(void);   
void gd25a32_erase_sector(uint32_t addr);  
/* 块操作 */
uint8_t spi_flash_erase_and_write_block(uint32_t block_num, uint8_t *data, uint32_t len);
uint8_t spi_flash_read_block(uint32_t block_num, uint32_t offset_in_block, uint8_t *data, uint16_t len);
uint8_t spi_flash_write_block(uint32_t block_num, uint32_t offset_in_block, uint8_t *data, uint16_t len);
uint8_t spi_flash_erase_block(uint32_t block_num);

/* 整片操作 */
uint8_t spi_flash_erase_chip(void);
#endif
