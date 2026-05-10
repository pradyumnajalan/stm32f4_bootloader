#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"

#define DEF_APP_ADDR 		0x8004000
#define APP2_ADDR			0x8008000
#define FAPP_ADDR			0x800C000
#define MSP_VERIFY_MASK		0x2FFE0000
#define EMPTY_SPACE			0xFFFFFFFF
#define G_KEY_DEFAULT		'0'

typedef void(*func_ptr)(void);

volatile char g_key;

void jmp_to_default_app(void){

	uint32_t app_start_addr;
	func_ptr jump_to_app;

	if(*(uint32_t*) DEF_APP_ADDR != EMPTY_SPACE ){

		printf("Starting Default Applictaion .....\n\r");
		delay(300);
		app_start_addr = *(uint32_t *) (DEF_APP_ADDR + 4);

		jump_to_app = (func_ptr) app_start_addr;

		/*initialize main stack pointer*/
		__set_MSP(*(uint32_t*) DEF_APP_ADDR);

		jump_to_app();
	}
	else{
		printf("Default application not found ......\n\r");
	}
}

void jmp_to_app2(void){
	uint32_t app_start_addr;
	func_ptr jmp_to_app;

	if(*(uint32_t*) APP2_ADDR != EMPTY_SPACE ){
		printf("Starting Application 2 .....\n\r");
		delay(300);

		app_start_addr = *(uint32_t *)(APP2_ADDR + 4);

		jmp_to_app = (func_ptr) app_start_addr;

		/*initialize main stack pointer*/
		__set_MSP(*(uint32_t *)APP2_ADDR);

		jmp_to_app();


	}
	else{
		printf("Application 2 not found ......\n\r");
	}
}

void jmp_to_fapp(void){
	uint32_t app_start_addr;
	func_ptr jmp_to_app;

	if(*(uint32_t*) APP2_ADDR != EMPTY_SPACE ){
		printf("Starting factory application .....\n\r");
		delay(300);

		app_start_addr = *(uint32_t *)(FAPP_ADDR + 4);

		jmp_to_app = (func_ptr) app_start_addr;

		/*initialize main stack pointer*/
		__set_MSP(*(uint32_t *)FAPP_ADDR);

		jmp_to_app();


	}
	else{
		printf("Factory application not found ......\n\r");
	}
}

int main(){

	/*Enable FPU*/
	fpu_enable();

	/*Initialize UART*/
	system_uart_init();

	/*Initialize Timebase*/
	timebase_init();

	/*Initialize BSP*/
	bsp_init();

	printf("Bootloader Started.....\n\r");

	g_key = G_KEY_DEFAULT;

	if(!read_btn()){
		jmp_to_default_app();
	}

	printf("Enter 2 for application 2 or F for factory application \n\r");

	while(1){



		if(g_key == '2'){
			jmp_to_app2();
			g_key = G_KEY_DEFAULT;
		}

		if(g_key == 'f' ){
			jmp_to_fapp();
			g_key = G_KEY_DEFAULT;
		}



		delay(5);
	}

	return 0;
}

static void uart_callback(void){

	g_key = (char) USART2->DR;
	printf("Key Pressed: %c\n\r", g_key);

}

void USART2_IRQHandler(void){
	/*check if RXNE is set*/
	if(USART2->SR & USART_SR_RXNE){
		uart_callback();
	}
}
