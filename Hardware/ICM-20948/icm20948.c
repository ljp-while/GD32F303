#include "icm20948.h"


void icm20948_init(void)
{
	iic_init(IIC0_PORT);
	iic_init(IIC1_PORT);
//	printf("port = %#x %#x %#x %#x sdaPin = %#x sclPin = %#x\r\n", GPIOB, GPIOA, iic0.sclPort, iic0.sdaPort, iic0.sda, iic0.scl);
}


uint8_t icm20948_read_id(void)
{
	uint8_t id, state;
	state = rdSensorReg8_8(iic0, REG_ADD_WIA, &id);

//	
//  /* user bank 0 register */
//  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
//  wrSensorReg8_8(iic0, REG_ADD_PWR_MIGMT_1, REG_VAL_ALL_RGE_RESET);
//  delay_ms(10);
//  wrSensorReg8_8(iic0, REG_ADD_PWR_MIGMT_1, REG_VAL_RUN_MODE);

//  /* user bank 2 register */
//  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_2);
//  wrSensorReg8_8(iic0, REG_ADD_GYRO_SMPLRT_DIV, 0x08);
//  wrSensorReg8_8(iic0, REG_ADD_GYRO_CONFIG_1, REG_VAL_BIT_GYRO_DLPCFG_6
//                                            | REG_VAL_BIT_GYRO_FS_2000DPS
//                                            | REG_VAL_BIT_GYRO_DLPF);
//  wrSensorReg8_8(iic0, REG_ADD_ACCEL_SMPLRT_DIV_1,  0x00);  // 119 Hz
//  wrSensorReg8_8(iic0, REG_ADD_ACCEL_SMPLRT_DIV_2,  0x08);
//  wrSensorReg8_8(iic0, REG_ADD_ACCEL_CONFIG, REG_VAL_BIT_ACCEL_DLPCFG_6
//                                           | REG_VAL_BIT_ACCEL_FS_4g
//                                           | REG_VAL_BIT_ACCEL_DLPF);


//  /* user bank 0 register */
//  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);

////  sleep_ms(100);
////  /* offset */
////  icm20948GyroOffset();

////  icm20948MagCheck();

//  icm20948WriteSecondary(I2C_ADD_ICM20948_AK09916 | I2C_ADD_ICM20948_AK09916_WRITE,
//                         REG_ADD_MAG_CNTL2, REG_VAL_MAG_MODE_20HZ);
						 
	return id;
}

void icm20948WriteSecondary(uint8_t u8I2CAddr, uint8_t u8RegAddr, uint8_t u8data) 
{
  uint8_t u8Temp;
  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_3);  // swtich bank3
  wrSensorReg8_8(iic0, REG_ADD_I2C_SLV1_ADDR, u8I2CAddr);
  wrSensorReg8_8(iic0, REG_ADD_I2C_SLV1_REG, u8RegAddr);
  wrSensorReg8_8(iic0, REG_ADD_I2C_SLV1_DO, u8data);
  wrSensorReg8_8(iic0, REG_ADD_I2C_SLV1_CTRL, REG_VAL_BIT_SLV0_EN | 1);

  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);  // swtich bank0

  rdSensorReg8_8(iic0, REG_ADD_USER_CTRL, &u8Temp);
  u8Temp |= REG_VAL_BIT_I2C_MST_EN;
  wrSensorReg8_8(iic0, REG_ADD_USER_CTRL, u8Temp);
  delay_ms(5);
  u8Temp &= ~REG_VAL_BIT_I2C_MST_EN;
  wrSensorReg8_8(iic0, REG_ADD_USER_CTRL, u8Temp);

  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_3);  // swtich bank3

  rdSensorReg8_8(iic0, REG_ADD_I2C_SLV0_CTRL, &u8Temp);
  u8Temp &= ~((REG_VAL_BIT_I2C_MST_EN) & (REG_VAL_BIT_MASK_LEN));
  wrSensorReg8_8(iic0, REG_ADD_I2C_SLV0_CTRL, u8Temp);

  wrSensorReg8_8(iic0, REG_ADD_REG_BANK_SEL, REG_VAL_REG_BANK_0);
}


uint8_t icm20948_accel_read(float *ps16X, float *ps16Y, float *ps16Z) 
{
  uint8_t u8Buf[2];
  int16_t s16Buf[3] = { 0 };
  rdSensorReg8_8(iic0, REG_ADD_ACCEL_XOUT_L, &u8Buf[0]);
  rdSensorReg8_8(iic0, REG_ADD_ACCEL_XOUT_H, &u8Buf[1]);
  s16Buf[0] = (u8Buf[1] << 8) | u8Buf[0];

  rdSensorReg8_8(iic0, REG_ADD_ACCEL_YOUT_L, &u8Buf[0]);
  rdSensorReg8_8(iic0, REG_ADD_ACCEL_YOUT_H, &u8Buf[1]);
  s16Buf[1] = (u8Buf[1] << 8) | u8Buf[0];

  rdSensorReg8_8(iic0, REG_ADD_ACCEL_ZOUT_L, &u8Buf[0]);
  rdSensorReg8_8(iic0, REG_ADD_ACCEL_ZOUT_H, &u8Buf[0]);
  s16Buf[2] = (u8Buf[1] << 8) | u8Buf[0];

  *ps16X = s16Buf[0] * 4.0 / 32768.0;
  *ps16Y = s16Buf[1] * 4.0 / 32768.0;
  *ps16Z = s16Buf[2] * 4.0 / 32768.0;


  if (*ps16X == 0 && *ps16Y == 0 && *ps16Z == 0) {
    return FALSE;
  }
  return TRUE;
}


