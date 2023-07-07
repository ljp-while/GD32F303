#include "time2.h"

uint8_t time2Count = 0;
uint8_t time2ProFlag = 0;
void time2_init(void)
{

	timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER2);		  /* 使能TIMER2时钟 */
	
	/* 初始化TIMER相关结构体默认值参数 */
    timer_struct_para_init(&timer_initpara);
    
    /* TIMER1 初始化配置 */
	timer_deinit(TIMER2);
	timer_initpara.period            = 1999;				      /* 自动重装载寄存器周期的值(计数值)  20ms*/									     
    timer_initpara.alignedmode 	     = TIMER_COUNTER_EDGE;       /* 对齐模式 */
    timer_initpara.prescaler         = (1200 - 1);			     /* 时钟预分频数   例如：时钟频率=60MHZ/(时钟预分频+1) */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		 /* 采样分频 */
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;	     /* 向上计数模式 */
    timer_init(TIMER2,&timer_initpara);
	timer_enable(TIMER2);
	nvic_irq_enable(TIMER2_IRQn, 1, 3);			    /* TIMER1中断设置，抢占优先级0，子优先级3 */
	timer_interrupt_enable(TIMER2,TIMER_INT_UP);	/* 使能更新中断 */ 

}



/* 定时器中断处理函数 */
void TIMER2_IRQHandler(void)
{
	time2Count++;
	
	if(time2Count >= 10)
	{
		time2ProFlag = TRUE;

		time2Count = 0;
	}
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);     // 清中断
}

