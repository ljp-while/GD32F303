#ifndef  __CAN_H
#define  __CAN_H

#include "main.h"

/* select CAN baudrate */
/* 1MBps */
//#define CAN_BAUDRATE  1000
/* 500kBps */
/* #define CAN_BAUDRATE  500 */
/* 250kBps */
/* #define CAN_BAUDRATE  250 */
/* 125kBps */
/* #define CAN_BAUDRATE  125 */
/* 100kBps */ 
 #define CAN_BAUDRATE  100 
/* 50kBps */ 
/* #define CAN_BAUDRATE  50 */
/* 20kBps */ 
/* #define CAN_BAUDRATE  20 */


void self_can_init(void);
void can_gpio_init(void);
void can_config_init(void);
uint8_t gd32_can_send(uint8_t *buff, uint8_t len);
#endif /*__CAN_H*/


