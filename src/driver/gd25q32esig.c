#include "gd25q32esig.h"


GD25Q_StatusTypeDef gd25qstatus = GD25Q80CSIG_OK;


/*获取芯片状态*/
GD25Q_StatusTypeDef GD25Q_SPIFLASH_Get_Status(void)
{

    return gd25qstatus;
}
 

/*读取设备ID*/
uint16_t gd25a32_readid(void)
{
    uint16_t  temp = 0;     
    /*将CS端拉低为低电平*/     
    FLSH_SPI_CS(0);        
    /*发送指令90h发送读取ID命令*/
    spi_send_byte(0x90);      
    /*发送地址  000000H    */
    spi_send_byte(0x00);             
    spi_send_byte(0x00);             
    spi_send_byte(0x00); 
        
    /*接收数据
    接收制造商ID*/
    temp |= spi_send_byte(0xFF)<<8;  
    /*接收设备ID*/
    temp |= spi_send_byte(0xFF);        
    /*恢复CS端为高电平*/
    FLSH_SPI_CS(1);        
    /*返回ID      */            
    return temp;
}

/*写使能*/
void gd25a32_write_enable(void)   
{
    /*拉低CS端为低电平*/
    FLSH_SPI_CS(0);                          
    /*发送指令06h*/
    spi_send_byte(0x06);                  
    /*拉高CS端为高电平*/
    FLSH_SPI_CS(1);
}  

/*写失能*/
void gd25a32_write_disable(void)   
{
    /*拉低CS端为低电平*/
    FLSH_SPI_CS(0);                          
    /*发送指令06h*/
    spi_send_byte(0x04);                  
    /*拉高CS端为高电平*/
    FLSH_SPI_CS(1);
} 

/*检测线路是否繁忙*/
void gd25a32_wait_busy(void)   
{   
    unsigned char byte = 0;
    do
     { 
        /*拉低CS端为低电平*/
        FLSH_SPI_CS(0); 
        /*发送指令05h*/                           
        spi_send_byte(0x05);                
        /*接收状态寄存器值*/
        byte = spi_send_byte(0Xff);       
        /*恢复CS端为高电平*/
        FLSH_SPI_CS(1);      
     /*判断BUSY位是否为1 如果为1说明在忙，重新读写BUSY位直到为0*/   
     }while( ( byte & 0x01 ) == 1 );  
}


uint8_t spi_flash_gd25q32esig_init(void)
{
    uint8_t i = 0;
    return i;
}

// uint8_t spi_flash_erase_chip(void)
// {
//     uint8_t i = 0;
//     return i;
// }


/**********************************************************
 * 函 数 名 称：GD25Q32_erase_sector
 * 函 数 功 能：擦除一个扇区
 * 传 入 参 数：addr=擦除的扇区号
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：addr=擦除的扇区号，范围=0~15
**********************************************************/
uint8_t spi_flash_erase_sector(uint32_t addr)
{
    uint8_t i = 0;
    /*计算扇区号，一个扇区4KB=4096*/
    addr *= 4096;
    gd25a32_write_enable();  /*写使能 */  

    gd25a32_wait_busy();     /*判断忙，如果忙则一直等待*/

    /*拉低CS端为低电平*/
    FLSH_SPI_CS(0);  
    /*发送指令20h*/                                   
    spi_send_byte(0x20);
    /*发送24位扇区地址的高8位*/                
    spi_send_byte((uint8_t)((addr)>>16));      
    /*发送24位扇区地址的中8位*/    
    spi_send_byte((uint8_t)((addr)>>8));   
    /*发送24位扇区地址的低8位*/    
    spi_send_byte((uint8_t)addr);
    /*恢复CS端为高电平*/  
    FLSH_SPI_CS(1);                  
    /*等待擦除完成*/                                                  
    gd25a32_wait_busy();       
    return i;
}

/*
 * @brief    将任意长度的数据写入 Flash，自动按页分割
 * @param    pbuffer    待写入的数据缓冲区
 * @param    write_addr 目标写地址（字节偏移）
 * @param    len        总写入长度（字节数）
 * @return   SPI_OK (0) 成功，SPI_ERR (1) 失败
 */
uint8_t spi_flash_buffer_write(uint32_t write_addr, uint8_t *pbuffer,  uint32_t len)
{
    printf("write_addr = %ld pbuffer = %d len =%d \r\n",write_addr,pbuffer[0],len);
	uint32_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;
    addr          = write_addr % SPI_FLASH_PAGE_SIZE;
	count         = SPI_FLASH_PAGE_SIZE - addr; //计算当前页剩余多少个字节空间
	num_of_page   = len / SPI_FLASH_PAGE_SIZE;//计算需要写多少页
	num_of_single = len % SPI_FLASH_PAGE_SIZE;//计算不满一页的字节数量
    if (pbuffer == NULL || len == 0) {
        return SPI_ERR;
    }
    printf("addr = %d count = %d num_of_page =%d num_of_single = %d\r\n",addr,count,num_of_page,num_of_single);
    /* 计算首页可写多少字节 */
    if (addr == 0) {
        if(0 == num_of_page)//所写字节数量不满一页,num_byte_to_write < SPI_FLASH_PAGE_SIZE
		spi_flash_gd25q32esig_write(write_addr, pbuffer, len);
		else//所写字节数量超过一页,num_byte_to_write > SPI_FLASH_PAGE_SIZE
		{
			while(num_of_page--)
			{
                spi_flash_gd25q32esig_write(write_addr, pbuffer, SPI_FLASH_PAGE_SIZE);
				write_addr += SPI_FLASH_PAGE_SIZE;
				pbuffer += SPI_FLASH_PAGE_SIZE;
			}
            spi_flash_gd25q32esig_write(write_addr, pbuffer, num_of_single);
		}
    } 
    else {
        if(0 == num_of_page)//所写字节数量不满一页
		{
			if(num_of_single > count)//超过当前页
			{
				temp = num_of_single - count;//计算跨页的字节数量
                spi_flash_gd25q32esig_write(write_addr, pbuffer, count);
				write_addr += count;//修改Flash地址
				pbuffer += count;   //修改指针
                spi_flash_gd25q32esig_write(write_addr, pbuffer, temp);
			}
			else//没有超过当前页
            spi_flash_gd25q32esig_write(write_addr, pbuffer, len);
		}
		else//所写字节数量超过一页
		{
			len -= count;//计算写当前页后的剩余字节总数
			num_of_page = len / SPI_FLASH_PAGE_SIZE;  //剩余字节总数需要多少页
			num_of_single = len % SPI_FLASH_PAGE_SIZE;//剩余字节总数写完整页后的剩余字节数量
            spi_flash_gd25q32esig_write(write_addr, pbuffer, count);
			write_addr += count;//修改Flash地址
			pbuffer += count;   //修改指针
 
			while(num_of_page--)
			{
                spi_flash_gd25q32esig_write(write_addr, pbuffer, SPI_FLASH_PAGE_SIZE);
				write_addr += SPI_FLASH_PAGE_SIZE;//修改Flash地址
				pbuffer += SPI_FLASH_PAGE_SIZE;   //修改指针
			}
 
			if(0 != num_of_single)//最后写剩余的字节
                spi_flash_gd25q32esig_write(write_addr, pbuffer, num_of_single);
		}
	}
    return SPI_OK;
}

uint8_t spi_flash_gd25q32esig_write(uint32_t addr, uint8_t* p_data, uint32_t len)
{
    /* 参数校验 */
    if (p_data == NULL || len == 0) {
        return SPI_ERR;
    }
    // /*擦除扇区数据*/
    //spi_flash_erase_sector(addr/4096);
    /*写使能*/ 
    gd25a32_write_enable();  
    /*忙检测*/  
    gd25a32_wait_busy();    
    /*写入数据
    拉低CS端为低电平*/
    FLSH_SPI_CS(0);         
    /*发送指令02h*/                              
    spi_send_byte(0x02);                 
    /*发送写入的24位地址中的高8位*/   
    spi_send_byte((uint8_t)((addr)>>16));  
    /*发送写入的24位地址中的中8位*/
    spi_send_byte((uint8_t)((addr)>>8));   
    /*发送写入的24位地址中的低8位*/
    spi_send_byte((uint8_t)addr);   
    /*根据写入的字节长度连续写入数据p_data*/
    for(uint32_t i=0;i<len;i++)
    {
        spi_send_byte(p_data[i]);
        if (i == len - 1) {
            // 延迟约10us（根据CPU主频调整，如循环空操作）
            for (volatile uint32_t d = 0; d < 100; d++);
        }  
    }
    /*恢复CS端为高电平*/
    FLSH_SPI_CS(1);
    /*忙检测 */
    gd25a32_wait_busy();     

    //printf("[WRITE] Success: addr=0x%06X, len=%d\r\n", addr, len);
    return SPI_OK;
}

uint8_t spi_flash_gd25q32esig_read(uint32_t addr, uint8_t* p_data, uint32_t len)
{
    uint32_t i;         
    /*拉低CS端为低电平*/
    FLSH_SPI_CS(0);    
    /*发送指令03h */       
    spi_send_byte(0x03);  
    /*发送24位读取数据地址的高8位*/                         
    spi_send_byte((uint8_t)((addr)>>16));     
    /*发送24位读取数据地址的中8位*/      
    spi_send_byte((uint8_t)((addr)>>8));   
    /*发送24位读取数据地址的低8位*/
    spi_send_byte((uint8_t)addr);   
    /*根据读取长度读取出地址保存到p_data中*/
    for(i=0;i<len;i++)
    { 
        p_data[i] = spi_read_write_byte(0XFF);
        // 每读1024字节打印一次进度
    }
    /*恢复CS端为高电平*/
    FLSH_SPI_CS(1);            
    return 0;
}

unsigned char flash_read_status(void)
{
    unsigned char ucChar = 0;
    FLSH_SPI_CS(1);; 
    spi_flash_gd25q32esig_write(0x05,0,1);     /*读取状态*/

    /* ucChar = spi_flash_gd25q32esig_read();*/
    FLSH_SPI_CS(0);
 
    return ucChar;

}


uint8_t erase_and_write_address(uint32_t target_addr, uint8_t *data, uint16_t len) {

    uint32_t sector_addr = target_addr & ~(0xFFF);  
    
    uint8_t sector_buffer[4096];
    if (spi_flash_gd25q32esig_read(sector_addr, sector_buffer, 4096) != 0) {
        return 1;  // 读取失败
    }
    
    uint32_t offset = target_addr - sector_addr;  // 计算在扇区内的偏移
    memcpy(sector_buffer + offset, data, len);
    
    if (spi_flash_erase_sector(sector_addr / 4096) != 0) {  
        return 1;  // 擦除失败
    }
    
    
    if (spi_flash_buffer_write(sector_addr, sector_buffer, 4096) != 0) {
        return 1;  // 写入失败
    }
    
    return 0;  // 成功
}

/**
 * 擦除指定块（64KB）
 * @param block_num：块编号（0 ~ 63，GD25Q32共64个64KB块）
 * @return 0=成功，1=失败
 */
uint8_t spi_flash_erase_block(uint32_t block_num) {
    // 计算块的起始地址（64KB块：0x10000字节/块）
    uint32_t block_addr = block_num * 0x10000;  // 64KB = 65536 = 0x10000

    // 写使能
    gd25a32_write_enable();
    gd25a32_wait_busy();

    // 发送块擦除指令（0xD8）
    FLSH_SPI_CS(0);
    spi_send_byte(0xD8);  // 64KB块擦除指令
    // 发送块地址（24位）
    spi_send_byte((block_addr >> 16) & 0xFF);
    spi_send_byte((block_addr >> 8) & 0xFF);
    spi_send_byte(block_addr & 0xFF);
    FLSH_SPI_CS(1);

    // 等待擦除完成（块擦除耗时较长，约几十毫秒）
    gd25a32_wait_busy();
    return 0;
}
/**
 * 向指定块的任意地址写入数据（自动处理扇区擦除，不影响块外数据）
 * @param block_num：块编号
 * @param offset_in_block：块内偏移地址（0 ~ 0xFFFF，64KB块）
 * @param data：待写入数据
 * @param len：数据长度（需确保不超过块范围）
 * @return 0=成功，1=失败
 */
uint8_t spi_flash_write_block(uint32_t block_num, uint32_t offset_in_block, uint8_t *data, uint16_t len) {
    // 1. 计算块的起始地址
    uint32_t block_start_addr = block_num * 0x10000;  // 64KB块起始地址
    uint32_t target_addr = block_start_addr + offset_in_block;

    // 2. 检查是否超出块范围
    if (offset_in_block + len > 0x10000) {  // 64KB=0x10000字节
        return 1;  // 数据超出块范围
    }

    // 3. 调用已实现的erase_and_write_address函数，自动按扇区处理
    // （该函数会备份扇区数据，擦除后恢复，不影响块内其他扇区）
    return erase_and_write_address(target_addr, data, len);
}
/**
 * 从指定块的任意地址读取数据
 * @param block_num：块编号
 * @param offset_in_block：块内偏移地址（0 ~ 0xFFFF）
 * @param data：接收数据的缓冲区
 * @param len：读取长度
 * @return 0=成功，1=失败
 */
uint8_t spi_flash_read_block(uint32_t block_num, uint32_t offset_in_block, uint8_t *data, uint16_t len) {
    // 计算目标地址
    uint32_t block_start_addr = block_num * 0x10000;
    uint32_t target_addr = block_start_addr + offset_in_block;

    // 检查是否超出块范围
    if (offset_in_block + len > 0x10000) {
        return 1;
    }

    // 调用现有读取函数
    return spi_flash_gd25q32esig_read(target_addr, data, len);
}
/**
 * 擦除整个块并写入数据（适合全块初始化）
 * @param block_num：块编号
 * @param data：待写入数据（长度需 <= 64KB）
 * @param len：数据长度
 * @return 0=成功，1=失败
 */
uint8_t spi_flash_erase_and_write_block(uint32_t block_num, uint8_t *data, uint32_t len) {
    // 1. 擦除整个块
    if (spi_flash_erase_block(block_num) != 0) {
        return 1;
    }

    // 2. 计算块起始地址
    uint32_t block_start_addr = block_num * 0x10000;

    // 3. 写入数据（使用现有spi_flash_buffer_write按页分割写入）
    if (spi_flash_buffer_write(block_start_addr, data, len) != 0) {
        return 1;
    }
    return 0;
}

/**
 * 整片全擦除（擦除整个Flash芯片的所有数据）
 * @return 0=成功，1=失败
 */
uint8_t spi_flash_erase_chip(void) {
    // 1. 写使能（全擦除前必须先使能写入）
    gd25a32_write_enable();
    gd25a32_wait_busy();  // 等待写使能完成

    // 2. 发送全擦除指令（0xC7）
    FLSH_SPI_CS(0);               // 拉低CS，选中芯片
    spi_send_byte(0xC7);          // 发送全擦除指令
    FLSH_SPI_CS(1);               // 拉高CS，执行指令

    // 3. 等待全擦除完成（耗时较长，需持续检测忙状态）
    gd25a32_wait_busy();          // 循环等待，直到BUSY位为0

    return 0;  // 全擦除成功
}