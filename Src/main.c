#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"

int main(){

	/*Enable FPU*/
	fpu_enable();

	/*Initialize UART*/
	boot_uart_init();

	/*Initialize Timebase*/
	timebase_init();

	while(1){

		printf("Hello from STM32\n\r");
		delay(5);
	}

	return 0;
}

