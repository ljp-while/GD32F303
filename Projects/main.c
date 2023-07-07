
#include "main.h"


void my_delay_ms(uint16_t ms)
{
	uint16_t count, num;
	for(num = 0;num < ms; num++)
		for(count = 9999;count > 0; count--);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/


uint32_t value1, value2, value3, value4, value5;
int main(void)
{
	uint16_t id, value;
	uint8_t buff[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    /* configure systick */
    systick_config();
	rs485_init();
	sca3300_init();
	dac_init();
	self_can_init();
	icm20948_init();
	time2_init();

//	printf("id = %#x\r\n", icm20948_read_id());
    while(1){

//		if(sendFlag == TRUE)
//		{
//			printf("IDIE rxcount = %d %#x %#x %#x\r\n", rxcount, rxbuffer[0], rxbuffer[1], rxbuffer[4]);
//			sca3300_get_data();
//			sca3300_data_send(Ax, Ay, Az);
//			printf("IDIE rxcount = %d %#x %#x %#x\r\n", rxcount, rxbuffer[0], rxbuffer[1], rxbuffer[4]);

//			sendFlag = FALSE;
//			rxcount = 0;
//		}
//		else
//		{
//			sca3300_get_data();
//		}
//		if(time2ProFlag == TRUE)
//		{
//			angle_value_process();
//			time2ProFlag = FALSE;
//		}
//		else
//		{
//			sca3300_get_data();
//		}
		
//		if(time2EnaFlag == TRUE)
//		{
//			time2EnaFlag = FALSE;
//		}
//		printf("program run...\r\n");
		//printf("value = %lf\r\n", 0.12345);
//		gd32_can_send(buff, 8);
		
//		delay_ms(500);
//		delay_ms(1000);

		delay_ms(100);
		printf("icm20948 id = %#x\r\n", icm20948_read_id());
    }
}
