#include <stdint.h>
#include "uart.h"

#define FCLK 		16000000
#define BDR			115200
#define APB1CLK		FCLK

static void set_uart_bd(uint32_t periph_clk, uint32_t bd);
static void uart_write(int ch);

int __io_putchar(int ch){

	uart_write(ch);
	return ch;
}

void system_uart_init(void){

	/* Enable GPIOD Clock*/

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	/* Set PD5 and PD6 to alternate function mode*/
	GPIOD->MODER &= ~(3 <<GPIO_MODER_MODER5_Pos);
	GPIOD->MODER |= GPIO_MODER_MODER5_1;
	GPIOD->MODER &= ~(3 <<GPIO_MODER_MODER6_Pos);
	GPIOD->MODER |= GPIO_MODER_MODER6_1;

	/* Set PD5 to AF7(USART2_TX)*/
	GPIOD->AFR[0] &= ~(0xF << GPIO_AFRL_AFSEL5_Pos);
	GPIOD->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL5_Pos);

	/* Set PD6 to AF7(USART2_RX)*/
	GPIOD->AFR[0] &= ~(0xF << GPIO_AFRL_AFSEL6_Pos);
	GPIOD->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL6_Pos);

	/*Enable USART2 Clock*/
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/* Set Baud Rate for USART2 */

	set_uart_bd(APB1CLK, BDR);


	/*Enable USART2 TX*/

	USART2->CR1 |= USART_CR1_TE;

	/*Enable USART2 RX*/
	USART2->CR1 |= USART_CR1_RE;

	/*Enable USART2 RX interrupt*/

	USART2->CR1 |= USART_CR1_RXNEIE;

	/*Enable USART2 Receive interrupt handler in NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);

	/*Enable USART2*/
	USART2->CR1 |= USART_CR1_UE;

}

static void uart_write(int ch){

	/* Check if Transmit Data Register is Empty*/

	while(!(USART2->SR & USART_SR_TXE_Msk)){}

	/*write to transmit data register*/
	USART2->DR = 0xFF & ch;

}

static uint16_t compute_bd(uint32_t periph_clk, uint32_t bd){
	return (uint16_t)((periph_clk + bd/2U)/bd);
}

static void set_uart_bd(uint32_t periph_clk, uint32_t bd){
	USART2->BRR |= compute_bd(periph_clk, bd);
}
