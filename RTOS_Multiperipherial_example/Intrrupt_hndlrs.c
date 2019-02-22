#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "System_parameters.h"
#include <stdio.h>

extern osThreadId tid_Uart_Read,tid_SPI_Read,tid_Adc_Read;

void USART2_IRQHandler(void){
	USART2->SR &= ~(USART_SR_RXNE);
	osSignalSet(tid_Uart_Read,Run);
}

void SPI2_IRQHandler(void){
	SPI2->SR &= ~(SPI_SR_RXNE);
	
}
