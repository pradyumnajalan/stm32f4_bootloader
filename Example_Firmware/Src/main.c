#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"

#define VECT_TAB_BASE_ADDR		FLASH_BASE
#define VECT_TAB_OFFSET			0x4000

int main(){

	/*Enable FPU*/
	fpu_enable();

	/*Initialize UART*/
	boot_uart_init();

	/*Initialize Timebase*/
	timebase_init();

	while(1){

		printf("User application 1 running.....\n\r");
		delay(5);
	}

	return 0;
}


void SystemInit(void){

	SCB->VTOR = VECT_TAB_BASE_ADDR | VECT_TAB_OFFSET;
}

