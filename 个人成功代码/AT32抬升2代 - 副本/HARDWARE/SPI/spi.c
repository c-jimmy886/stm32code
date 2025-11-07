#include "spi.h"

void SPI1_Init(void)
{
	gpio_init_type gpio_initstructure;
	spi_init_type spi_init_struct;
 
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);						//开启spi cs gpio时钟
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);						//开启spi clk gpio时钟
	crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK , TRUE);						//开启spi时钟
	
	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);						//开启复用时钟
	gpio_pin_remap_config(SWJTAG_MUX_010,TRUE);									//关闭掉jtag,保留swd
	gpio_pin_remap_config(SPI1_MUX_01,TRUE);									//spi io复用到pa15,pb3，pb4,pb5
	
	/* software cs, pb12 as a general io to control flash cs */			//spi cs 脚配置，软件模式，配置为上拉推挽输出
	gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;		//推挽模式	
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;			//输出
	gpio_initstructure.gpio_pins           = GPIO_PINS_15;				//cs pin
	gpio_init(GPIOA, &gpio_initstructure);
 
	/* sck */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;				//复用推挽
	gpio_initstructure.gpio_pins           = GPIO_PINS_3;				//clk pin
	gpio_init(GPIOB, &gpio_initstructure);
 
	/* miso */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_INPUT;			//输入模式，配置为复用推挽也行，但是设置为输入比较规范些。
	gpio_initstructure.gpio_pins           = GPIO_PINS_4;				//iso pin
	gpio_init(GPIOB, &gpio_initstructure);
 
	/* mosi */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;				//复用推挽
	gpio_initstructure.gpio_pins           = GPIO_PINS_5;				//osi pin
	gpio_init(GPIOB, &gpio_initstructure);
 
//	FLASH_CS_HIGH();
  gpio_bits_set(GPIOA, GPIO_PINS_15);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;		//全双工模式
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;				//主模式
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;				//时钟8分频，120M/8=15M
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;			//大小端，高位在前
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;						//数据为8bit模式
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;			//配置 SPI 时钟空闲时为高电平
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;				//配置 SPI 在第二个时钟边沿采样
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;			//软件cs模式
	spi_init(SPI1, &spi_init_struct);									//spi结构体参数设置为以上参数
	spi_enable(SPI1, TRUE);												//使能spi
}
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_MCLK_DIV_2~SPI_MCLK_DIV_1024  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(spi_mclk_freq_div_type SPI_BaudRatePrescaler)
{
	spi_init_type spi_init_struct;
	spi_init_struct.mclk_freq_division = SPI_BaudRatePrescaler;
	spi_init(SPI1, &spi_init_struct);
	spi_enable(SPI1, TRUE);		 //使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(u8 data)
{
  uint8_t brxbuff;
  spi_i2s_data_transmit(SPI1, data);
  while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
  brxbuff = spi_i2s_data_receive(SPI1);
  while(spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) != RESET);
  return brxbuff;
}










void SPI4_Init(void)
{
	gpio_init_type gpio_initstructure;
	spi_init_type spi_init_struct;
 
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);						//开启spi gpio时钟
	crm_periph_clock_enable(CRM_SPI4_PERIPH_CLOCK , TRUE);						//开启spi时钟
	
	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);						//开启复用时钟
	gpio_pin_remap_config(SWJTAG_MUX_010,TRUE);									//关闭掉jtag,保留swd
	gpio_pin_remap_config(SPI4_GMUX_0001,TRUE);									//spi io复用到pe12 pe11 pe13 pe14
	
	/* software cs, pb12 as a general io to control flash cs */			//spi cs 脚配置，软件模式，配置为上拉推挽输出
	gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;		//推挽模式	
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;			//输出
	gpio_initstructure.gpio_pins           = GPIO_PINS_12;				//cs pin
	gpio_init(GPIOE, &gpio_initstructure);
 
	/* sck */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;				//复用推挽
	gpio_initstructure.gpio_pins           = GPIO_PINS_11;				//clk pin
	gpio_init(GPIOE, &gpio_initstructure);
 
	/* miso */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_INPUT;			//输入模式，配置为复用推挽也行，但是设置为输入比较规范些。
	gpio_initstructure.gpio_pins           = GPIO_PINS_13;				//iso pin
	gpio_init(GPIOE, &gpio_initstructure);
 
	/* mosi */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;				//上拉
	gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;				//复用推挽
	gpio_initstructure.gpio_pins           = GPIO_PINS_14;				//osi pin
	gpio_init(GPIOE, &gpio_initstructure);
 
//	FLASH_CS_HIGH();
  gpio_bits_set(GPIOE, GPIO_PINS_12);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;		//全双工模式
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;				//主模式
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;				//时钟8分频，120M/8=15M
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;			//大小端，高位在前
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;						//数据为8bit模式
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;			//配置 SPI 时钟空闲时为高电平
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;				//配置 SPI 在第二个时钟边沿采样
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;			//软件cs模式
	spi_init(SPI4, &spi_init_struct);									//spi结构体参数设置为以上参数
	spi_enable(SPI4, TRUE);												//使能spi
}
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_MCLK_DIV_2~SPI_MCLK_DIV_1024  
//fAPB2时钟一般为84Mhz：
void SPI4_SetSpeed(spi_mclk_freq_div_type SPI_BaudRatePrescaler)
{
	spi_init_type spi_init_struct;
	spi_init_struct.mclk_freq_division = SPI_BaudRatePrescaler;
	spi_init(SPI4, &spi_init_struct);
	spi_enable(SPI4, TRUE);		 //使能SPI4
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI4_ReadWriteByte(uint8_t data)
{
  uint8_t brxbuff;	
  spi_i2s_data_transmit(SPI4, data);
  while(spi_i2s_flag_get(SPI4, SPI_I2S_RDBF_FLAG) == RESET);
  brxbuff = spi_i2s_data_receive(SPI4);
  while(spi_i2s_flag_get(SPI4, SPI_I2S_BF_FLAG) != RESET);
  return brxbuff;
}




















