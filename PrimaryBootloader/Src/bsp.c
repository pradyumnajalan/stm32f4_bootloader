#include <stdint.h>
#include "bsp.h"



void bsp_init(void){

	/*Enable GPIOA Clock*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;


	/*Set PA0 to input mode*/
	GPIOA->MODER &= ~(3<<GPIO_MODER_MODER0_Pos);

}


bool read_btn(void){

	if(GPIOA->IDR & GPIO_IDR_ID0){
		return true;
	}

	return false;

}


