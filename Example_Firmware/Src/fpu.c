#include "stm32f4xx.h"


void fpu_enable(void){

	/* Enable FPU: set CP10 and CP11 to 0b11 for full access	 */
	SCB->CPACR |= (0xF<<20);
}
