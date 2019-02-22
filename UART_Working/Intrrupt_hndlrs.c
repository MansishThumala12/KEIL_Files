#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "System_parameters.h"
#include <stdio.h>

extern osThreadId tid_Uart_Read,tid_SPI_Read,tid_Adc_Read;

void USART2_IRQHandler(void){
	USART2->SR &= ~(USART_SR_RXNE);
	osSignalSet(tid_Uart_Read,Run);
}

//void ADC1_2_IRQHandler(void){
//	ADC1->SR &= ~(ADC_SR_AWD|ADC_SR_EOC);
//	adc_value=ADC1->DR;
//	ADC1->HTR = (adc_value +20);
//	ADC1->LTR = (adc_value -20);
//	osSignalSet(tid_Adc_Read,Run);
//}

void SPI2_IRQHandler(void){
	SPI2->SR &= ~(SPI_SR_RXNE);
	
}
