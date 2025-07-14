#include "sh367309.h"

const uint8_t crc8_table[]=
{                            
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
    0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
    0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
    0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
    0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
    0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
    0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
    0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
    0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
    0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
    0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
    0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
    0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
    0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
    0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
    0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

/**
 * @函数名称: crc8_calculate()
 * @功能描述: CRC8校验码函数
 * @输  　入: p:要计算的数据。length：数据长度
 * @输　  出: 无
 * @返 回 值: 无
 */
static uint8_t crc8_calculate(uint8_t* p, uint8_t length)
{
    uint8_t crc8 = 0;
    for(; length > 0; length--) {    
        crc8 = crc8_table[crc8^*p];    
        p++;    
    }  
    return(crc8);       
}

/**
 * @函数名称: afe_iic_stop_ack()
 * @功能描述: 产生STOP信号
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void afe_iic_stop_ack(void)
{
    AFE_IIC_SDA(0);
    AFE_IIC_US_DELAY();
    AFE_IIC_SCL(1);
    AFE_IIC_US_DELAY();
    AFE_IIC_SDA(1);
    AFE_IIC_US_DELAY();
}

/**
 * @函数名称: afe_iic_start_ack()
 * @功能描述: 产生起始信号信号
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void afe_iic_start_ack(void)
{
    AFE_IIC_SDA(1);
    AFE_IIC_SCL(1);
    AFE_IIC_US_DELAY();
    /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    AFE_IIC_SDA(0);
    AFE_IIC_US_DELAY();
    /* 钳住IIC总线，准备发送或接收数据 */        
    AFE_IIC_SCL(0);
    AFE_IIC_US_DELAY();
}

/**
 * @函数名称: afe_iic_wait_ack()
 * @功能描述: 等待应答信号到来
 * @输入: 无
 * @输出: 无
 * @返回值: 0:接收应答成功。1：接收应答失败
 */
static uint8_t afe_iic_wait_ack(void)
{
    uint8_t temp_time=0;
    uint8_t retval = 0;
    /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    AFE_IIC_SDA(1);
    AFE_IIC_US_DELAY();
    /* SCL=1, 此时从机可以返回ACK */
    AFE_IIC_SCL(1);
    AFE_IIC_US_DELAY();
    while (AFE_IIC_READ_SDA()) {
        temp_time++;
        /* 超时停止 */
        if(temp_time > 250) {
            afe_iic_stop_ack();
            retval = 1;
            break;
        }
    }
    /* SCL=0, 结束ACK检查 */
    AFE_IIC_SCL(0);
    AFE_IIC_US_DELAY();
    return retval;
}

/**
 * @函数名称: afe_iic_ack()
 * @功能描述: 产生ACK应答
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void afe_iic_ack(void)
{
    /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    AFE_IIC_SDA(0);
    AFE_IIC_US_DELAY();
    /* 产生一个时钟 */
    AFE_IIC_SCL(1);
    AFE_IIC_US_DELAY();
    AFE_IIC_SCL(0);
    AFE_IIC_US_DELAY();
    /* 主机释放SDA线 */
    AFE_IIC_SDA(1);
    AFE_IIC_US_DELAY();
}

/**
 * @函数名称: afe_iic_no_ack()
 * @功能描述: 不产生ACK应答
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void afe_iic_no_ack(void)
{
    /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    AFE_IIC_SDA(1);
    AFE_IIC_US_DELAY();
    /* 产生一个时钟 */
    AFE_IIC_SCL(1);
    AFE_IIC_US_DELAY();
    AFE_IIC_SCL(0);
    AFE_IIC_US_DELAY();
}

/**
 * @函数名称: afe_iic_send_byte()
 * @功能描述: IIC发送一个字节
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
static void afe_iic_send_byte(uint8_t data)
{
    uint8_t t = 0;
    for (t = 0; t < 8; t++) {
        /* 高位先发送 */
        AFE_IIC_SDA((data & 0x80) >> 7);
        AFE_IIC_US_DELAY();
        AFE_IIC_SCL(1);
        AFE_IIC_US_DELAY();
        AFE_IIC_SCL(0);
        /* 左移1位,用于下一次发送 */
        data <<= 1;
    }
    /* 发送完成, 主机释放SDA线 */
    AFE_IIC_SDA(1); 
}

/**
 * @函数名称: afe_iic_read_byte()
 * @功能描述: IIC读取一个字节
 * @输入: ack:  ack=1时，发送ack; ack=0时，发送nack
 * @输出: 无
 * @返回值: 无
 */
static uint8_t afe_iic_read_byte(uint8_t ack)
{
    uint8_t i = 0, receive = 0;
    /* 接收1个字节数据 */
    for(i = 0; i < 8; i++) {
        /* 高位先输出,所以先收到的数据位要左移 */
        receive <<= 1;
        AFE_IIC_SCL(1);
        AFE_IIC_US_DELAY();
        if(AFE_IIC_READ_SDA()) {
            receive++;
        }
        AFE_IIC_SCL(0);
        AFE_IIC_US_DELAY();
    }
    if(!ack) {
        afe_iic_no_ack();
    } else {
        afe_iic_ack();
    }
    return receive;
}

/**
 * @函数名称: sh367309_init()
 * @功能描述: 设置IIC口初始化
 * @输入: 无
 * @输出: 无
 * @返回值: 0:初始化成功；1：初始化失败
 */
uint8_t sh367309_init(void)
 {
    uint8_t retval = 0;
    // rcu_periph_clock_enable(RCU_GPIOC);
    // rcu_periph_clock_enable(RCU_GPIOD);
    // gpio_mode_set(AFE_IIC_PORT_SCL, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AFE_IIC_PIN_SCL);
    // gpio_output_options_set(AFE_IIC_PORT_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, AFE_IIC_PIN_SCL);
     
    // gpio_mode_set(AFE_IIC_PORT_SDA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AFE_IIC_PIN_SDA);
    // gpio_output_options_set(AFE_IIC_PORT_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, AFE_IIC_PIN_SDA);
    // afe_iic_stop_ack(); 

    return retval;
 }

/**
 * @函数名称: sh367309_write()
 * @功能描述: 向AFE SH367309寄存器写数据,连续调用该函数，间隔需要35mS
 * @输入: addr：寄存器地址。write_buffer：要写入的数据
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t sh367309_write(uint8_t addr, uint8_t write_buffer)
{
    uint8_t crc = 0;
    uint8_t result = 0;
    uint8_t case_val = 0;
    uint8_t buff[3] = {0};
    VPRO12V8V_CONTROL_ON(); /* TODO：AFE8V电源待修改，此处不能每次写都开启 */
    __disable_irq();/* 临界资源保护 */
    switch(case_val) {
        case 0: {
            buff[case_val] = SH367309_ID;
            afe_iic_start_ack();
            afe_iic_send_byte(SH367309_ID);
            if(afe_iic_wait_ack() == 1) {
                afe_iic_stop_ack();
                result = 1;
                break;
            }
            case_val++;
        }
        case 1: {
            buff[case_val] = addr;
            afe_iic_send_byte(addr);
            if(afe_iic_wait_ack() == 1) {
                afe_iic_stop_ack();
                result = 1;
                break;
            }
            case_val++;
        }
        case 2: {
            buff[case_val] = write_buffer;
            afe_iic_send_byte(write_buffer);
            if(afe_iic_wait_ack() == 1) {
                afe_iic_stop_ack();
                result = 1;
                break;
            }
            crc = crc8_calculate(buff,3);
            afe_iic_send_byte(crc);
            if(afe_iic_wait_ack() == 1) {
                afe_iic_stop_ack();
                result = 1;
                break;
            } else {
                afe_iic_stop_ack();
            }        
        }
        default:break;
    }
    __enable_irq();/* 解除临界资源保护 */
    VPRO12V8V_CONTROL_OFF();/* TODO：AFE8V电源待修改，此处不能每次写都开启 */
    return result;
}

/**
 * @函数名称: sh367309_read()
 * @功能描述: 从AFE SH367309寄存器读取数据
 * @输入: RdAddr：寄存器地址。Length：要读取的数据长度。RdBuf：读取出来的数据
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t sh367309_read(uint8_t addr, uint8_t length, uint8_t* read_buffer)
{
    uint8_t uci = 0;
    uint8_t temp_buff[50] = {0};
    uint8_t crc = 0;
    uint8_t result = 0;
    uint8_t case_val = 0;
    __disable_irq();/* 临界资源保护 */
    if(length > 0) {
        switch(case_val) {
            case 0: {
                temp_buff[case_val] = SH367309_ID;
                afe_iic_start_ack();
                afe_iic_send_byte(SH367309_ID);
                if(afe_iic_wait_ack() == 1) {
                    afe_iic_stop_ack();
                    result = 1;
                    break;
                } else {
                    case_val++;
                }
            }
            case 1: {
                temp_buff[case_val] = addr;
                afe_iic_send_byte(addr);
                if(afe_iic_wait_ack() == 1) {
                    afe_iic_stop_ack();
                    result = 1;
                    break;
                } else {
                    case_val++;
                }
            }
            case 2: {
                temp_buff[case_val] = length;
                afe_iic_send_byte(length);
                if(afe_iic_wait_ack() == 1) {
                    afe_iic_stop_ack();
                    result = 1;
                    break;
                } else {
                    case_val++;
                }
            }
            case 3: {
                temp_buff[case_val] = SH367309_ID+1;
                afe_iic_start_ack();
                afe_iic_send_byte(SH367309_ID+1);
                if(afe_iic_wait_ack() == 1) {
                    afe_iic_stop_ack();
                    result = 1;
                    break;
                } else {
                    case_val++;
                }

                for(uci = case_val;uci < length+case_val;uci++) {
                    temp_buff[uci] = afe_iic_read_byte(1);
                }
                temp_buff[uci] = afe_iic_read_byte(0);
                afe_iic_stop_ack();
                crc = crc8_calculate(temp_buff,uci);
                if(temp_buff[uci] == crc) {
                    for(uci = 0;uci < length;uci++) {
                        if(&read_buffer[uci] != NULL) {
                            read_buffer[uci] = temp_buff[uci+4];
                        } else {
                            afe_iic_stop_ack();
                            result = 1;
                            break;
                        }
                    }
                    break;
                } else {
                    result = 1;
                }
            }
            default: break;
        }
    } else {
        __enable_irq();/* 解除临界资源保护 */
        result = 1;
    }
    __enable_irq();/* 解除临界资源保护 */
    return result;
}

/**
 * @函数名称: sh367309_reset()
 * @功能描述: 向AFE SH367309软件复位
 * @输入: 无
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
void sh367309_reset(void)
{
    uint8_t crc = 0;
    unsigned char buff[4] = {0};
    buff[0] = 0x34;
    buff[1] = 0xEA;
    crc = crc8_calculate(buff, 3);

    afe_iic_start_ack();
    afe_iic_send_byte(SH367309_ID);
    afe_iic_send_byte(0xEA);
    afe_iic_send_byte(0xC0);
    afe_iic_send_byte(crc);
    afe_iic_stop_ack();
}
