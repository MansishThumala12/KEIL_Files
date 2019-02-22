#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "lcd16x2.h"

void Uart_Init(USART_TypeDef*);
void Uart_Interupt_Init(USART_TypeDef*);
void send_char(char,USART_TypeDef*);
void send_string(const char *,USART_TypeDef*);


char buff;


int main(void){
	SystemInit();
	DelayInit();
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	Uart_Init(USART2);//Look into the init funcion for all the paramenters
	lcd16x2_puts("SYS IS IDEL");

	while(1){
		__nop();
		__nop();
	}
}


void Uart_Init(USART_TypeDef* USART){
	
	//generic data sheet code(Direct Bit Manupilation)
	RCC->APB1ENR |= (RCC_APB1ENR_USART2EN);
	RCC->APB1ENR |= (RCC_APB2ENR_IOPAEN);
	
	//Configuring the gpio for the aternate function mode
	GPIOA->CRL |= (GPIO_CRL_MODE2_0|GPIO_CRL_CNF2_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF2_0);
	
	//USART Configuration for using in uart mode fullduplex
	USART->BRR = (0x1d4c);
	USART->CR1 |= (USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE);
	
	//After uart initilization do interrupt initilization
	Uart_Interupt_Init(USART);
	
	//enabling the uart
	USART->CR1 |= USART_CR1_UE;
	lcd16x2_puts("LCD Initilized\n");
	DelayMs(1000);
	lcd16x2_clrscr();
	
}


__inline void Uart_Interupt_Init(USART_TypeDef* USART){
	//generic data sheet code
	NVIC_EnableIRQ(USART2_IRQn );
	NVIC_SetPriority(USART2_IRQn ,1);
}


void send_char(char chr,USART_TypeDef* USART){
	//simple line code
	NVIC_DisableIRQ(USART2_IRQn );
	USART->DR = chr;
	while(!(USART->SR & USART_SR_TC));
	USART->SR &= ~(USART_SR_TC);
	NVIC_EnableIRQ(USART2_IRQn );
}

void send_string(const char* str,USART_TypeDef* USART){
	//simple inline coce
	while(strcmp(str,"\0")){
		while(!(USART->SR & (USART_SR_TXE)));
		USART->DR = *str++;
	}
}

void USART2_IRQHandler(void){
	NVIC_DisableIRQ(USART2_IRQn);
	USART2->SR &= ~(USART_SR_RXNE);
	char g = (USART1->DR & (uint16_t)0x01FF);
	lcd16x2_putc(g);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
}
