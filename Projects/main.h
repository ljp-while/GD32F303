#ifndef  __MAIN_H
#define  __MAIN_H

#include "stdio.h"
#include <math.h>
#include "gd32f30x.h"
#include "systick.h"
#include "rs485.h"
#include "spi.h"
#include "sca3300.h"
#include "string.h"
#include "dac.h"
#include "iic.h"
#include "can.h"
#include "time2.h"
#include "icm20948.h"

#define TRUE  1
#define FALSE 0

void delay_ms(uint16_t ms);


#endif /*__MAIN_H*/

