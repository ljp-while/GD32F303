#ifndef  __IIC_H
#define  __IIC_H

#include "main.h"

#define IIC0_PORT 0
#define IIC1_PORT 1


#define IIC0_SCL_PORT  GPIOB
#define IIC0_SDA_PORT  GPIOA
#define IIC0_SCL_PIN   GPIO_PIN_0
#define IIC0_SDA_PIN   GPIO_PIN_7
#define IIC0_SCL_RCU   RCU_GPIOB
#define IIC0_SDA_RCU   RCU_GPIOA


#define IIC1_SCL_PORT  GPIOA
#define IIC1_SDA_PORT  GPIOA
#define IIC1_SCL_PIN   GPIO_PIN_4
#define IIC1_SDA_PIN   GPIO_PIN_3
#define IIC1_SCL_RCU   RCU_GPIOA
#define IIC1_SDA_RCU   RCU_GPIOA

#define IIC_SCL_HIGH(x)  {GPIO_BOP(x.sclPort) = x.scl;}
#define IIC_SCL_LOW(x)   {GPIO_BC(x.sclPort) = x.scl;}
#define IIC_SDA_HIGH(x)  {GPIO_BOP(x.sdaPort) = x.sda;}
#define IIC_SDA_LOW(x)   {GPIO_BC(x.sdaPort) = x.sda;}
#define IIC_SDA_OUT(x) 	 gpio_init(x.sdaPort, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, x.sda)
#define IIC_SDA_IN(x) 	 gpio_init(x.sdaPort, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, x.sda)
#define IIC_SDA_STATE(x) gpio_input_bit_get(x.sdaPort, x.sda)

#define I2C_TIM 5
#define SENSOR_ADDR 0x68



typedef struct{
	uint32_t sclPort; 
	uint32_t sdaPort; 
	uint16_t scl;
	uint16_t sda;
}iicSoftware;

extern iicSoftware iic0;
extern iicSoftware iic1;

void iic_init(uint8_t num);
void iic_delay_us(uint8_t us);
void sccb_bus_start(iicSoftware x);
void sccb_bus_stop(iicSoftware x);
void sccb_bus_send_noack(iicSoftware x);
void sccb_bus_send_ack(iicSoftware x);
uint8_t sccb_bus_write_byte(iicSoftware x,uint8_t data);
uint8_t sccb_bus_read_byte(iicSoftware x);
uint8_t wrSensorReg8_8(iicSoftware x, int regID, int regDat);
uint8_t rdSensorReg8_8(iicSoftware x, uint8_t regID, uint8_t* regDat);
uint8_t wrSensorReg16_8(iicSoftware x, int regID, int regDat);
uint8_t rdSensorReg16_8(iicSoftware x, uint16_t regID, uint8_t* regDat);
uint8_t rdSensorReg16_16(iicSoftware x, uint16_t regID, uint16_t* regDat);
#endif/*__IIC_H*/


