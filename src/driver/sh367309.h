#ifndef SH367309_H
#define SH367309_H

#include "common.h"

/* IIC1用于AFE */
#define AFE_IIC_SCL(x) do{x?gpio_bit_set(IIC1_PORT_SCL,IIC1_PIN_SCL):gpio_bit_reset(IIC1_PORT_SCL,IIC1_PIN_SCL);}while(0)
#define AFE_IIC_SDA(x) do{x?gpio_bit_set(IIC1_PORT_SDA,IIC1_PIN_SDA):gpio_bit_reset(IIC1_PORT_SDA,IIC1_PIN_SDA);}while(0)

#define AFE_IIC_DELAY_US(x) gd32f4x_delay_us(x)
#define AFE_IIC_US_DELAY() AFE_IIC_DELAY_US(iic1_delay_rate) 

#define AFE_IIC_READ_SDA() gpio_input_bit_get(IIC1_PORT_SDA,IIC1_PIN_SDA)

#define SH367309_ID 0x34

extern const uint8_t crc8_table[];

uint8_t sh367309_init(void);
uint8_t sh367309_write(uint8_t addr,uint8_t write_buffer);
uint8_t sh367309_read(uint8_t addr, uint8_t length, uint8_t* read_buffer);
void sh367309_reset(void);
#endif
