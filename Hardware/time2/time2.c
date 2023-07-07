#include "time2.h"

uint8_t time2Count = 0;
uint8_t time2ProFlag = 0;
void time2_init(void)
{

	timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER2);		  /* ʹ��TIMER2ʱ�� */
	
	/* ��ʼ��TIMER��ؽṹ��Ĭ��ֵ���� */
    timer_struct_para_init(&timer_initpara);
    
    /* TIMER1 ��ʼ������ */
	timer_deinit(TIMER2);
	timer_initpara.period            = 1999;				      /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)  20ms*/									     
    timer_initpara.alignedmode 	     = TIMER_COUNTER_EDGE;       /* ����ģʽ */
    timer_initpara.prescaler         = (1200 - 1);			     /* ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=60MHZ/(ʱ��Ԥ��Ƶ+1) */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		 /* ������Ƶ */
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;	     /* ���ϼ���ģʽ */
    timer_init(TIMER2,&timer_initpara);
	timer_enable(TIMER2);
	nvic_irq_enable(TIMER2_IRQn, 1, 3);			    /* TIMER1�ж����ã���ռ���ȼ�0�������ȼ�3 */
	timer_interrupt_enable(TIMER2,TIMER_INT_UP);	/* ʹ�ܸ����ж� */ 

}



/* ��ʱ���жϴ����� */
void TIMER2_IRQHandler(void)
{
	time2Count++;
	
	if(time2Count >= 10)
	{
		time2ProFlag = TRUE;

		time2Count = 0;
	}
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);     // ���ж�
}

