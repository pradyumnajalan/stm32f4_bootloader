#include <stdint.h>
#include "timebase.h"
#include "stm32f4xx.h"


#define ONE_SEC_LOAD		16000000
#define TICK_FREQ			1
#define MAX_DELAY			0xFFFFFFFF


volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;


static uint32_t get_tick(void){
	__disable_irq();

	g_curr_tick_p = g_curr_tick;

	__enable_irq();

	return g_curr_tick_p;
}

/*Delay in seconds*/
void delay(uint32_t delay){

	uint32_t tickstart = get_tick();
	uint32_t wait = delay;

	if(wait < MAX_DELAY){
		wait += (uint32_t)TICK_FREQ;
	}

	while(get_tick() - tickstart < wait){}
}

static void tick_increment(void){
	g_curr_tick += TICK_FREQ;
}

void timebase_init(void){

	/*Disable global interrupts*/
	__disable_irq();

	/*Load the timer with number of clock cycle per second*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/* clear SysTick current value register*/
	SysTick->VAL = 0;

	/*select internal clock source*/

	SysTick->CTRL |= (1<<2);

	/*enable interrupt*/

	SysTick->CTRL |= (1<<1);

	/*Enable SysTick*/
	SysTick->CTRL |= (1<<0);

	/*Enable Global Interrupts*/
	__enable_irq();

}

void SysTick_Handler(void){
	tick_increment();
}
