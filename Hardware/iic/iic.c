#include "iic.h"

iicSoftware iic0;
iicSoftware iic1;

void iic_init(uint8_t num)
{

	switch(num)
	{
		case IIC0_PORT:
			rcu_periph_clock_enable(IIC0_SCL_RCU);
			rcu_periph_clock_enable(IIC0_SDA_RCU);
			iic0.sclPort = IIC0_SCL_PORT;
			iic0.sdaPort = IIC0_SDA_PORT;
			iic0.scl = IIC0_SCL_PIN;
			iic0.sda = IIC0_SDA_PIN;	
			gpio_init(iic0.sclPort, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, iic0.scl);
			gpio_init(iic0.sdaPort, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, iic0.sda);
			break;
		case IIC1_PORT:
			rcu_periph_clock_enable(IIC1_SCL_RCU);
			rcu_periph_clock_enable(IIC1_SDA_RCU);
			iic1.sclPort = IIC1_SCL_PORT;
			iic1.sdaPort = IIC1_SDA_PORT;
			iic1.scl = IIC1_SCL_PIN;
			iic1.sda = IIC1_SDA_PIN;	
			gpio_init(iic1.sclPort, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, iic1.scl);
			gpio_init(iic1.sdaPort, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, iic1.sda);
			break;
		default:
			break;
	}
}


void sccb_bus_start(iicSoftware x)
{
    IIC_SDA_HIGH(x);             
    iic_delay_us(I2C_TIM);
    IIC_SCL_HIGH(x);	           
    iic_delay_us(I2C_TIM);
    IIC_SDA_LOW(x);
    iic_delay_us(I2C_TIM);
    IIC_SCL_LOW(x);	           
    iic_delay_us(I2C_TIM);
}


void sccb_bus_stop(iicSoftware x)
{
    IIC_SDA_LOW(x);
    iic_delay_us(I2C_TIM);
    IIC_SCL_HIGH(x);	
    iic_delay_us(I2C_TIM);  
    IIC_SDA_HIGH(x);	
    iic_delay_us(I2C_TIM);  
}


void sccb_bus_send_noack(iicSoftware x)
{	
	IIC_SDA_HIGH(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SCL_HIGH(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SCL_LOW(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SDA_HIGH(x);	
	iic_delay_us(I2C_TIM);
}

void sccb_bus_send_ack(iicSoftware x)
{	
	IIC_SDA_LOW(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SCL_LOW(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SCL_HIGH(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SCL_LOW(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SDA_HIGH(x);	
	iic_delay_us(I2C_TIM);
}

uint8_t sccb_bus_write_byte(iicSoftware x,uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++) 
	{
		if((data<<i) & 0x80)
		{
			IIC_SDA_HIGH(x);
		}
		else 
		{
			IIC_SDA_LOW(x);
		}
		iic_delay_us(I2C_TIM);
		IIC_SCL_HIGH(x);	
		iic_delay_us(I2C_TIM);
		IIC_SCL_LOW(x);	

	}
	IIC_SDA_IN(x);
	iic_delay_us(I2C_TIM);
	IIC_SCL_HIGH(x);	
	iic_delay_us(I2C_TIM);
	if(IIC_SDA_STATE(x))
	{
		tem = 0;               
	}
	else 
	{
		tem = 1;                
	}

	IIC_SCL_LOW(x);	
	iic_delay_us(I2C_TIM);	
	IIC_SDA_OUT(x);
	return tem;  
}

uint8_t sccb_bus_read_byte(iicSoftware x)
{	
	uint32_t i;
	uint8_t read = 0;
	
	IIC_SDA_IN(x);
	iic_delay_us(I2C_TIM);
	for(i = 8; i > 0; i--) 
	{		     
		iic_delay_us(I2C_TIM);
		IIC_SCL_HIGH(x);
		iic_delay_us(I2C_TIM);
		read = read << 1;
		if(IIC_SDA_STATE(x))
		{
			read += 1; 
		}
		IIC_SCL_LOW(x);
		iic_delay_us(I2C_TIM);
	}	
    IIC_SDA_OUT(x);
	return read;
}

uint8_t wrSensorReg8_8(iicSoftware x, int regID, int regDat)
{
	iic_delay_us(5);
	sccb_bus_start(x);                          
	if(sccb_bus_write_byte(x,SENSOR_ADDR) == 0)         
	{
		sccb_bus_stop(x);                        
		return 1;
	}
	iic_delay_us(5);
	if(sccb_bus_write_byte(x,regID) == 0)
	{
		sccb_bus_stop(x);                              
		return 2;                                       
	}
	iic_delay_us(5);
	if(sccb_bus_write_byte(x,regDat)==0)                    
	{
		sccb_bus_stop(x);                                 
		return 3;
	}
	sccb_bus_stop(x);                                    
	return 0;
}


uint8_t rdSensorReg8_8(iicSoftware x, uint8_t regID, uint8_t* regDat)
{
	iic_delay_us(10);
	
	sccb_bus_start(x);
	if(sccb_bus_write_byte(x,SENSOR_ADDR) == 0)                 
	{
		sccb_bus_stop(x);                                
		//goto start;
		return 1;                                        
	}
	iic_delay_us(10);
	if(sccb_bus_write_byte(x,regID)==0)//ID
	{
		sccb_bus_stop(x);                                  
		//goto start;
		return 2;                                       
	}
	sccb_bus_stop(x);                                   
	iic_delay_us(10);	
	sccb_bus_start(x);
	if(sccb_bus_write_byte(x,SENSOR_ADDR|0x01)==0)                    
	{
		sccb_bus_stop(x);                                   
		//goto start;
		return 3;                                          
	}
	iic_delay_us(10);
	*regDat = sccb_bus_read_byte(x);                    
	sccb_bus_send_noack(x);                                
	sccb_bus_stop(x);                                      
	return 0;                
}

uint8_t wrSensorReg16_8(iicSoftware x, int regID, int regDat)
{
	sccb_bus_start(x);
	if(0==sccb_bus_write_byte(x,SENSOR_ADDR))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(5);
  if(0==sccb_bus_write_byte(x,regID>>8))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(5);
  if(0==sccb_bus_write_byte(x,regID))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(5);
  if(0==sccb_bus_write_byte(x,regDat))
	{
		sccb_bus_stop(x);
		return(0);
	}
  sccb_bus_stop(x);
	
  return(1);
}


uint8_t rdSensorReg16_8(iicSoftware x, uint16_t regID, uint8_t* regDat)
{
	sccb_bus_start(x);                  
	if(0==sccb_bus_write_byte(x,SENSOR_ADDR))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	if(0==sccb_bus_write_byte(x,regID>>8))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	if(0==sccb_bus_write_byte(x,regID))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	sccb_bus_stop(x);
	
	iic_delay_us(20);
	sccb_bus_start(x);                 
	if(0==sccb_bus_write_byte(x,SENSOR_ADDR|0x01))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
  *regDat=sccb_bus_read_byte(x);
  sccb_bus_send_noack(x);
  sccb_bus_stop(x);
  return(1);
}


uint8_t rdSensorReg16_16(iicSoftware x, uint16_t regID, uint16_t* regDat)
{
	uint8_t high = 0;
	uint8_t low = 0;
	sccb_bus_start(x);                  
	if(0==sccb_bus_write_byte(x,SENSOR_ADDR))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	if(0==sccb_bus_write_byte(x,regID>>8))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	if(0==sccb_bus_write_byte(x,regID))
	{
		sccb_bus_stop(x);
		return(0);
	}
	iic_delay_us(20);
	sccb_bus_stop(x);
	
	iic_delay_us(20);
	sccb_bus_start(x);                 
	if(0==sccb_bus_write_byte(x,SENSOR_ADDR|0x01))
	{
		sccb_bus_stop(x);
		return(0);
	}
  iic_delay_us(20);
  high=sccb_bus_read_byte(x);
  sccb_bus_send_ack(x);
  low=sccb_bus_read_byte(x);
  sccb_bus_send_noack(x);
  sccb_bus_stop(x);
  *regDat = (high << 8 ) | low;
  return(1);
}

void iic_delay_us(uint8_t us)
{
	delay_us(us);
}


