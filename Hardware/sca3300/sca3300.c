#include "sca3300.h"




union float_data floatConvert;
float Ax = 0.0, Ay = 0.0, Az = 0.0;

#define pai 3.1415926
static void sca3300_write_register(uint32_t cmd);
static uint8_t sca3300_read_register(uint32_t cmd, uint8_t *buff);
static uint8_t CalculateCRC(uint32_t Data);
static uint8_t CRC8(uint8_t bitValue, uint8_t crc);
//static uint8_t crc8(uint8_t bitValue, uint8_t crc);

void sca3300_init(void)
{
	uint8_t buff[4];
	uint32_t state = 0;
	spi_init_config();
	
	sca3300_write_register(SW_RESET);                         // 
	delay_ms(200);
	
    sca3300_write_register(CHANGE_TO_MODE4);                         // 选择倾角模式

    delay_ms(200);
    
    sca3300_read_register(READ_STATUS_SUMMARY, buff);                     // 清除状态

    sca3300_read_register(READ_STATUS_SUMMARY, buff);

    sca3300_read_register(READ_STATUS_SUMMARY, buff);

//	sca3300_read_register(READ_STATUS_SUMMARY, buff);
 
//    sca3300_write_register(ENABLE_ANGLE_OUTPUTS);                    // 使能角度输出
//	sca3300_read_register(READ_ACC_X, buff);
//	sca3300_read_register(READ_ACC_Y, buff);
//	
//	sca3300_read_register(READ_ACC_Z, buff);
//	sca3300_read_register(READ_ACC_X, buff);
}

void sca3300_get_data(void)
{
	uint8_t buff[4], num = 0;
	int value1 = 0,value2 = 0,value3 = 0;
	float accX = 0.0f,accY = 0.0f, accZ = 0.0f;
	float value[3];
	Ax = 0.0;
	Ay = 0.0;
	Az = 0.0;
	for(num = 0;num < 10; num++)
	{
		if(sca3300_read_register(READ_ACC_X, buff) == TRUE)
		{
			value1 = (buff[1] << 8) + buff[2];
			if(value1 >= 50000)
			{
				value1 = value1 - 65535;
			}
		}
		if(sca3300_read_register(READ_ACC_X, buff) == TRUE)
		{
			value1 = (buff[1] << 8) + buff[2];
			if(value1 >= 50000)
			{
				value1 = value1 - 65535;
			}
		}
		if(sca3300_read_register(READ_ACC_Y, buff) == TRUE)
		{
			value2 = (buff[1] << 8) + buff[2];
			if(value2 >= 50000)
			{
				value2 = value2 - 65535;
			}
		}
		if(sca3300_read_register(READ_ACC_Y, buff) == TRUE)
		{
			value2 = (buff[1] << 8) + buff[2];
			if(value2 >= 50000)
			{
				value2 = value2 - 65535;
			}
		}
		if(sca3300_read_register(READ_ACC_Z, buff) == TRUE)
		{
			value3 = (buff[1] << 8) + buff[2];
			if(value3 >= 50000)
			{
				value3 = value3 - 65535;
			}

		}
		if(sca3300_read_register(READ_ACC_Z, buff) == TRUE)
		{
			value3 = (buff[1] << 8) + buff[2];
			if(value3 >= 50000)
			{
				value3 = value3 - 65535;
			}

		}

		value[0]=(1.0019f*(float)value1)+(0.00*1.0026f*(float)value2)+(0.00*1.0015f*(float)value3)+14.58f;;  //X轴
		value[1]=(0.00*1.0019f*(float)value1)+(1.0026f*(float)value2)+(0.00*1.0015f*(float)value3)+38.88f;  //Y轴
		value[2]=(0.00*1.0019f*(float)value1)+(0.00*1.0026f*(float)value2)+(1.0015f*(float)value3)+10.26f;  //Z轴
		
		accX = value[0] / SCA3300_RANGE;
		accY = value[1] / SCA3300_RANGE;
		accZ = value[2] / SCA3300_RANGE;

		Ax += atanf(accX/sqrtf(powf(accY,2)+powf(accZ,2)))*180/pai;  //xg--轴向g值 yg--轴向g值 zg--轴向g值 
		Ay += atanf(accY/sqrtf(powf(accX,2)+powf(accZ,2)))*180/pai;
		Az += atanf(accZ/sqrtf(powf(accX,2)+powf(accY,2)))*180/pai;				
		delay_us(500);
	}
	

	Ax /= 10;
	Ay /= 10;
	Az /= 10;
//	printf("value:%f %f %f\r\n", value[0], value[1], value[2]);	

//	printf("accX:%f  accY:%f  accZ:%f\r\n", accX, accY, accZ);	
//	printf("X:%f  Y:%f  Z:%f\r\n", accX * SCA3300_RANGE, accY * SCA3300_RANGE, accZ * SCA3300_RANGE);
//	printf("accX:%f  accY:%f  accZ:%f\r\n", accX, accY, accZ);
//	RS485_TX_EN;
//	printf("value:%d %d %d\r\n", value1, value2, value3);	
//	printf("Ax:%f  Ay:%f  Az:%f\r\n", Ax, Ay, Az);
//	RS485_RX_EN;
//	sca3300_data_send(Ax, Ay, Az);

}

void sca3300_data_send(float ax, float ay, float az)
{
	uint8_t sendBuff[20];
	memset(sendBuff, 0 , sizeof(sendBuff));
	sendBuff[0] = 0x55;
	sendBuff[1] = 0xaa;
	sendBuff[2] = 14;
	sendBuff[3] = 0x01;
	floatConvert.f_data = ax;
	sendBuff[4] = floatConvert.byte[3];
	sendBuff[5] = floatConvert.byte[2];
	sendBuff[6] = floatConvert.byte[1];
	sendBuff[7] = floatConvert.byte[0];
	floatConvert.f_data = ay;
	sendBuff[8] = floatConvert.byte[3];
	sendBuff[9] = floatConvert.byte[2];
	sendBuff[10] = floatConvert.byte[1];
	sendBuff[11] = floatConvert.byte[0];
	floatConvert.f_data = az;
	sendBuff[12] = floatConvert.byte[3];
	sendBuff[13] = floatConvert.byte[2];
	sendBuff[14] = floatConvert.byte[1];
	sendBuff[15] = floatConvert.byte[0];
	sendBuff[16] = get_verify_value(&sendBuff[2], 14);
	rs485_send_buff(sendBuff, 17);
}
uint16_t sca3300_get_register_value(uint32_t cmd)
{
	uint16_t value = 0;
	uint8_t buff[4];
	if(sca3300_read_register(cmd, buff) == TRUE)
	{
		value = (buff[1] << 8) + buff[2];
	}
	return value;
}
uint16_t sca3300_get_id(void)
{
	uint16_t id = 0;
	uint8_t buff[4];
	if(sca3300_read_register(READ_ID, buff) == TRUE)
	{
		id = (buff[1] << 8) + buff[2];
	}
	return id;
}

static void sca3300_write_register(uint32_t cmd)
{
	uint8_t writeData[4] = {0}, count;
    writeData[0] = (cmd >> 24) & 0xFF;
    writeData[1] = (cmd >> 16) & 0xFF;
    writeData[2] = (cmd >> 8) & 0xFF;
    writeData[3] = (cmd >> 0) & 0xFF;
	CS_LOW;
	for(count = 0; count < 4; count++)
	{
		spi1_write_read_byte(writeData[count], 10000);
	}
	CS_HIGH;
}


static uint8_t sca3300_read_register(uint32_t cmd, uint8_t *buff)
{
	uint8_t value = 0;
	uint32_t crc = 0;
	uint8_t writeData[4] = {0}, count;
    writeData[0] = (cmd >> 24) & 0xFF;
    writeData[1] = (cmd >> 16) & 0xFF;
    writeData[2] = (cmd >> 8) & 0xFF;
    writeData[3] = (cmd >> 0) & 0xFF;	
	CS_LOW;
	for(count = 0; count < 4; count++)
	{
		crc <<= 8;
		buff[count] = spi1_write_read_byte(writeData[count], 10000);
		crc += buff[count];
		
	}
	CS_HIGH;
	value = CalculateCRC(crc);
//	printf("state = %#x\r\n", buff[0]);
	if(value == buff[3])
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}


static uint8_t CalculateCRC(uint32_t Data)
{
	uint8_t BitIndex;
	uint8_t BitValue;
	uint8_t crc;
	crc = 0xFF;
	for (BitIndex = 31; BitIndex > 7; BitIndex--)
	{
		BitValue = (uint8_t)((Data >> BitIndex) & 0x01);
		crc = CRC8(BitValue, crc);
	}
	crc = (uint8_t)~crc;
	return crc;
}


static uint8_t CRC8(uint8_t bitValue, uint8_t crc)
{
	uint8_t Temp;
	Temp = (uint8_t)(crc & 0x80);
	if (bitValue == 0x01)
	{
		Temp ^= 0x80;
	}
	crc <<= 1;
	if (Temp > 0)
	{
		crc ^= 0x1D;
	}
	return crc;
}


uint8_t get_verify_value(uint8_t *buff, uint16_t len)
{
	uint16_t value = 0;
	uint32_t crc = 0;
	for(value = 0; value < len; value++)
	{
		crc += buff[value];
	}
	return (uint8_t)crc;
}

