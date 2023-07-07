#include "dac.h"

#define DAC_OUT_VAL0 0x7FF0
#define DAC_OUT_VAL1 0x1FF0

void dac_init(void)
{
	dac_gpio_init();
	dac_config_init();
}

void dac_gpio_init(void)
{
	rcu_periph_clock_enable(RCU_AF);//管脚复用时钟alternate function clock使能
    rcu_periph_clock_enable(RCU_GPIOA);
    /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_5);	
}


void dac_config_init(void)
{
	rcu_periph_clock_enable(RCU_DAC);
	dac_deinit();
	/* configure the DAC1 */
	dac_trigger_disable(DAC1);
	dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE);
	dac_output_buffer_enable(DAC1);
	dac_enable(DAC1);
	dac_output_voltage(1.0);
}


void dac_output_voltage(float voltage)
{
	uint16_t value;
	if(voltage > 3.3)
	{
		voltage = 3.3;
	}
	value = (uint16_t)((voltage / 3.3) * 65535);
	dac_data_set(DAC1, DAC_ALIGN_12B_L, value);
}



void angle_value_process(void)
{
	float xAngle = 0.0, voltage;
	voltage = 0.8;
	if(Ax >= 90.0)
	{
		xAngle = 90.0;
	}
	else if(Ax <= -30.0)
	{
		xAngle = -30.0;
	}
	else
	{
		xAngle = Ax;
	}
	
	voltage += (xAngle * 0.133 / 10);
	dac_output_voltage(voltage);
//	printf("voltage = %f %f\r\n", voltage, xAngle);
}
