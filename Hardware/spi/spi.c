#include "spi.h"


void spi_init_config(void)
{
	spi_parameter_struct  spi_init_struct;
	
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_SPI1);
    /* SPI1 GPIO config:SCK/PB13, MOSI/PB15 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15 | GPIO_PIN_13);
    /* config PB12 as SPI1_NSS */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

    /*  MISO/PB14 */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);


    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);


//    spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);
//    spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_RBNE);

//    SET_SPI0_NSS_LOW

    /* SPI enable */
    spi_enable(SPI1);
	
	CS_LOW;
	
}


uint8_t spi1_write_read_byte(uint8_t ndata,uint32_t TimeOut)
{
	uint32_t timeout_t=0;

	timeout_t = TimeOut;

	/* loop while spi tx data register in not emplty */
	while( RESET == spi_i2s_flag_get(SPI1,SPI_FLAG_TBE) )
	{
		if(TimeOut > 0) TimeOut--; 
		else 				break;			
	}
	/* send byte through the SPI1 peripheral */
	spi_i2s_data_transmit(SPI1,ndata);

	while( RESET == spi_i2s_flag_get(SPI1,SPI_FLAG_RBNE))
	{
		if(timeout_t > 0) timeout_t--; 
		else 					break;		
	}		
	return spi_i2s_data_receive(SPI1);
}


