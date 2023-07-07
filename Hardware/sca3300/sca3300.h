#ifndef  __SCA3300_H
#define  __SCA3300_H

#include "main.h"

/*********************************************************************
 * DEFINITIONS
 */
#define ANG_X                   0
#define ANG_Y                   1
#define ANG_Z                   2

//******************************************************************/
//                          ��������                                /
//******************************************************************/
#define READ_ACC_X              0x040000F7
#define READ_ACC_Y              0x080000FD
#define READ_ACC_Z              0x0C0000FB
#define READ_TEMPERATURE        0x140000EF
#define READ_ANG_X              0x240000C7
#define READ_ANG_Y              0x280000CD
#define READ_ANG_Z              0x2C0000CB

//******************************************************************/
//                          ���Ҽ��                                /
//******************************************************************/
#define READ_STO                0x100000E9

//******************************************************************/
//                          ״̬                                    /
//******************************************************************/
#define READ_STATUS_SUMMARY     0x180000E5

//******************************************************************/
//                          �����־                                /
//******************************************************************/
#define READ_ERR_FLAG1          0x1C0000E3
#define READ_ERR_FLAG2          0x200000C1

//******************************************************************/
//                          ����                                    /
//******************************************************************/
#define READ_CMD                0x340000DF
#define CHANGE_TO_MODE1         0xB400001F  // Ĭ��ģʽ��1.8g full-scale 40 Hz 1st order low pass filter
#define CHANGE_TO_MODE2         0xB4000102  // 23.6g full-scale 70 Hz 1st order low pass filter
#define CHANGE_TO_MODE3         0xB4000225  // Inclination mode 13 Hz 1st order low pass filter
#define CHANGE_TO_MODE4         0xB4000338  // Inclination mode 13 Hz 1st order low pass filter Low noise mode
#define SET_POWER_DOWN_MODE     0xB400046B
#define WAKE_UP                 0xB400001F
#define SW_RESET                0xB4002098

//******************************************************************/
//                          �Ƕȿ���                                /
//******************************************************************/
#define ENABLE_ANGLE_OUTPUTS    0xB0001F6F

//******************************************************************/
//                          оƬID                                  /
//******************************************************************/
#define READ_ID                 0x40000091

//******************************************************************/
//                          ���п�                                  /
//******************************************************************/
#define READ_SERIAL1            0x640000A7
#define READ_SERIAL2            0x680000AD

//******************************************************************/
//                          ѡ��BANK                                /
//******************************************************************/
#define READ_CURRENT_BANK       0x7C0000B3
#define SWITCH_TO_BANK_0        0xFC000073 // Ĭ�ϣ���bank1���Զ�����
#define SWITCH_TO_BANK_1        0xFC00016E

#define SCA3300_RANGE   5400.0

extern float Ax;
extern float Ay;
extern float Az;

union float_data
{
    float f_data;
    uint8_t byte[4];
};


void sca3300_init(void);
uint16_t sca3300_get_id(void);
void sca3300_get_data(void);
uint16_t sca3300_get_register_value(uint32_t cmd);
uint8_t get_verify_value(uint8_t *buff, uint16_t len);
void sca3300_data_send(float ax, float ay, float az);
#endif /*__SCA3300_H*/



