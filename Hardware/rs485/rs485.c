#include "rs485.h"

uint8_t rxbuffer[RS485_RECEIVE_MAX];
uint8_t rxcount;
uint8_t sendFlag = FALSE;
void rs485_init(void)
{
	
	rcu_periph_clock_enable(RCU_AF);      
	gpio_pin_remap_config(GPIO_USART0_REMAP,ENABLE);
	
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	
    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);


	usart_deinit(USART0);
	usart_baudrate_set(USART0,115200);//波特率设置
	usart_parity_config(USART0,USART_PM_NONE);//校验位设置
	usart_word_length_set(USART0,USART_WL_8BIT);//数据位设置
	usart_stop_bit_set(USART0,USART_STB_1BIT);//停止位设置
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);//
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);//硬件l流控设置
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//发送使能
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//接收使能
	usart_enable(USART0);//使能串口

	nvic_irq_enable(USART0_IRQn, 0, 0);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_interrupt_enable(USART0, USART_INT_IDLE);

	RS485_TX_EN;
}

void rs485_send_buff(uint8_t * buff, uint16_t len)
{
	uint16_t count = 0;
	RS485_TX_EN;
	for(count = 0; count < len; count++)
	{
		usart_data_transmit(USART0, (uint8_t)buff[count]);
		while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));		
	}
	RS485_RX_EN;
}


void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
		
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
        /* receive data */
        rxbuffer[rxcount++] = usart_data_receive(USART0);
        if(rxcount >= RS485_RECEIVE_MAX){
            rxcount = 0;
        }
    }
	
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
        /* receive data */
        usart_data_receive(USART0);
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_IDLE);

		if(rxcount == 5)
		{
			if((rxbuffer[0] == 0x55) && (rxbuffer[1] == 0xaa) && (rxbuffer[4] == 0x03))
			{
				sendFlag = TRUE;
			}
		}
		
		
    }
//    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)){
//        /* transmit data */
//        usart_data_transmit(USART0, txbuffer[txcount++]);
//        if(txcount == tx_size){
//            usart_interrupt_disable(USART0, USART_INT_TBE);
//        }
//    }
}


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    USART1->DR = (u8) ch;      	
	usart_data_transmit(USART0, (uint8_t)ch);
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));	
	return ch;
}
#endif 
