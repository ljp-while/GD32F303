#ifndef  __DAC_H
#define  __DAC_H

#include "main.h"


void dac_init(void);
void dac_gpio_init(void);
void dac_config_init(void);
void dac_output_voltage(float voltage);
void angle_value_process(void);
#endif/*__DAC_H*/


