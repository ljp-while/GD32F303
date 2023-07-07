#include "can.h"


void self_can_init(void)
{
	can_gpio_init();
	can_config_init();
}


void can_gpio_init(void)
{
    /* enable CAN clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    
    /* configure CAN0 GPIO */
    gpio_init(GPIOA,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_12);
    //gpio_pin_remap_config(GPIO_CAN0_FULL_REMAP,ENABLE);
	//nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn,0,0);

}


void can_config_init(void)
{
    can_parameter_struct            can_parameter;
    can_filter_parameter_struct     can_filter;
	/* initialize CAN register */
    can_deinit(CAN0);
    /* enable CAN clock */
    rcu_periph_clock_enable(RCU_CAN0);	
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
   
    /* initialize CAN parameters */
//    can_parameter.time_triggered = DISABLE;
//    can_parameter.auto_bus_off_recovery = ENABLE;
//    can_parameter.auto_wake_up = DISABLE;
//    can_parameter.auto_retrans = ENABLE;
//    can_parameter.rec_fifo_overwrite = DISABLE;
//    can_parameter.trans_fifo_order = DISABLE;
//    can_parameter.working_mode = CAN_NORMAL_MODE;
//    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_14TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
    can_parameter.prescaler = 15;

    /* initialize CAN */
    can_init(CAN0, &can_parameter);

    
    /* initialize filter */ 
    can_filter.filter_number = CAN_FIFO1;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);
   
	 /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INT_RFNE1|CAN_INT_TME);
	
	//nvic_irq_enable(CAN0_RX1_IRQn,3,0); //ÖÐ¶ÏÅäÖÃ
}


uint8_t gd32_can_send(uint8_t *buff, uint8_t len)
{  
	can_trasnmit_message_struct can_frame;
	uint8_t ret=0;
	if(len > 8)
	{
		len = 8;
	}
	can_frame.tx_efid = 0x67980206;
	can_frame.tx_sfid = 0;
	can_frame.tx_dlen = len;
	can_frame.tx_ff = CAN_FF_EXTENDED;
	can_frame.tx_ft = CAN_FT_DATA;
	memcpy(can_frame.tx_data, buff, len);
	ret = can_message_transmit(CAN0,&can_frame);
	if(ret == CAN_NOMAILBOX)
	{
		return 1;
	}
  	return 0;
}

void CAN0_RX1_IRQHandler(void)
{
	can_receive_message_struct can_mes;
	memset(&can_mes,0,sizeof(can_mes)); 
	can_message_receive(CAN0, CAN_FIFO1, &can_mes); 	
}

