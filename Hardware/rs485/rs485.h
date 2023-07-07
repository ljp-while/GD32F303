#ifndef  __RS485_H
#define  __RS485_H


#include "main.h"

#define RS485_TX_EN {GPIO_BOP(GPIOB) = GPIO_PIN_8;}
#define RS485_RX_EN {GPIO_BC(GPIOB) = GPIO_PIN_8;}

#define RS485_RECEIVE_MAX  100

extern uint8_t rxbuffer[RS485_RECEIVE_MAX];
extern uint8_t rxcount;
extern uint8_t sendFlag;

void rs485_init(void);
void rs485_send_buff(uint8_t * buff, uint16_t len);

#endif /*__RS485_H*/

