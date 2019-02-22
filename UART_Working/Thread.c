#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "System_parameters.h"
#include "lcd16x2.h"
#include <string.h>
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC

char Buff[BUFF_SIZE];

/*Thread Functions*/
void Fun_Uart_Read(void const *argument);                             
void Fun_Lcd(void const *argument);
void Fun_Uart_Write(void const *argument);
void Fun_Adc_Read(void const *argument);
void Fun_SPI_Read(void const *argument);
void Fun_SPI_Write(void const *argument);

osThreadId tid_Uart_Read,
					 tid_Lcd, 
					 tid_Uart_Write, 
					 tid_SPI_Write, 
					 tid_SPI_Read, 
					 tid_Adc_Read;                                          
	
osThreadDef (Fun_Uart_Read, osPriorityHigh, 1, 0);                   
osThreadDef (Fun_Uart_Write, osPriorityNormal, 1, 0);
osThreadDef (Fun_Lcd, osPriorityNormal, 1, 0);
osThreadDef (Fun_SPI_Read, osPriorityHigh, 1, 0);
osThreadDef (Fun_SPI_Write, osPriorityNormal, 1, 0);
osThreadDef (Fun_Adc_Read, osPriorityAboveNormal, 1, 0);

/*--- continue after sustem initilization ---*/
int Init_Thread (void) {

  tid_Uart_Read = osThreadCreate (osThread(Fun_Uart_Read), NULL);
	tid_Uart_Write = osThreadCreate (osThread(Fun_Uart_Write), NULL);
  tid_Lcd = osThreadCreate (osThread(Fun_Lcd), NULL);
	tid_SPI_Write = osThreadCreate (osThread(Fun_SPI_Write), NULL);
	tid_SPI_Read = osThreadCreate (osThread(Fun_SPI_Read), NULL);
	tid_Adc_Read = osThreadCreate (osThread(Fun_Adc_Read), NULL);

  if (!(tid_Uart_Read||tid_Uart_Write||tid_Lcd)) return(-1);
  
  return(0);
}

void Fun_Uart_Read(void const *argument) { 
	char Uart_Input;
	uint8_t Bi = 0;
	mstruc_Lcd *temp;

	
	osSignalWait(Run,osWaitForever);
	osSignalClear(tid_Uart_Read,Run);
  while (1) {
		Uart_Input = USART2_GetChar();
		
		if(Bi < BUFF_SIZE-1 && Uart_Input != 0x0D){
			Buff[Bi++] = Uart_Input;
		}
		else{
			//Posting Data to mail
			temp = (mstruc_Lcd*)osMailAlloc(qid_Lcd, osWaitForever);
			temp->id = UART_2;
			memcpy(temp->data.str, Buff, 13*sizeof(char));
			osMailPut(qid_Lcd,temp);
			
			int8_t temp_Bi = 0;
			while(temp_Bi < Bi){
				osMessagePut(mid_Uart_Write,Buff[temp_Bi],10);
				Buff[temp_Bi++] = ' ';;
			}
			osMessagePut(mid_Uart_Write,'\n',10);
			Bi = 0;
		}
		osSignalWait(Run,osWaitForever);
		osSignalClear(tid_Uart_Read,Run);
	}
	
}

void Fun_Uart_Write(void const *argument) {
	
	osEvent Uart;
	Uart = osMessageGet(mid_Uart_Write,osWaitForever);	
  while (1) {
		USART2_PutChar(Uart.value.v); // Insert thread code here...
		Uart = osMessageGet(mid_Uart_Write,osWaitForever);
  }
}

void Fun_Lcd(void const *argument) {
	
	osEvent Lcd;
	mstruc_Lcd* Lcd_temp_struc;
	Lcd = osMailGet(qid_Lcd,osWaitForever);
  while (1) {
		Lcd_temp_struc = (mstruc_Lcd*)Lcd.value.p;
		switch(Lcd_temp_struc->id){
			case UART_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(0,1);
			lcd16x2_clear_line();
			lcd16x2_gotoxy(0,1);
			lcd16x2_puts("UR:");
			lcd16x2_puts(Lcd_temp_struc->data.str);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case SPI_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(0,1);
			lcd16x2_clear_line();
			lcd16x2_gotoxy(0,1);
			lcd16x2_puts("SP:");
			lcd16x2_puts(Lcd_temp_struc->data.str);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case ADC_1:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(3,0);
			lcd16x2_PutNum(Lcd_temp_struc->data.val,5);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case ADC_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(3,0);
			lcd16x2_PutNum(Lcd_temp_struc->data.val,5);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			default:
			break;
		}
			
		Lcd = osMailGet(qid_Lcd,osWaitForever);
  }
}

/*Lcd Non-Inturrept version*/
void Fun_Adc_Read(void const *argument){
	
	uint16_t previous=0;
	uint16_t adc_value = 0;
	mstruc_Lcd *temp;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	adc_value = ADC1->DR;
	while(1){
		if(200<=__fabs(adc_value-previous)||200>=__fabs(adc_value-previous)){
			previous = adc_value;
			temp = (mstruc_Lcd*)osMailAlloc(qid_Lcd, osWaitForever);
			temp->id = ADC_1;
			temp->data.val = (adc_value)*3.3/4096;
			osMailPut(qid_Lcd,temp);
		}
			osDelay(100);
			adc_value = ADC1->DR;
	}
}

void Fun_SPI_Read(void const *argument){
	
	char SPI_Input;
	char Buff[BUFF_SIZE];
	uint8_t Bi = 0;
		osSignalWait(Run,osWaitForever);
		osSignalClear(tid_SPI_Read,Run);
  while (1) {
		SPI_Input = SPI2_GetChar();
		if(Bi < BUFF_SIZE-1 && SPI_Input != 0x0d){
			Buff[Bi++] = SPI_Input;
		}
		else{
			//Posting Data to mail
			mstruc_Lcd *temp;
			temp = (mstruc_Lcd*)osMailAlloc(qid_Lcd, osWaitForever);
			temp->id = SPI_2;
			memcpy(temp->data.str, Buff, 13*sizeof(char));
			osMailPut(qid_Lcd,temp);
			
			int8_t temp_Bi = 0;
			while(temp_Bi < Bi){
				osMessagePut(mid_Uart_Write,Buff[temp_Bi],10);
				temp_Bi++;
			}
			osMessagePut(mid_Uart_Write,'\n',10);
			Bi = 0;
		}
		osSignalWait(Run,osWaitForever);
		osSignalClear(tid_SPI_Read,Run);		
	}
}

void Fun_SPI_Write(void const *argument){
	
	osEvent Spi;
	Spi = osMessageGet(mid_Spi_Write,osWaitForever);	
  while (1) {
		USART2_PutChar(Spi.value.v); // Insert thread code here...
		Spi = osMessageGet(mid_Spi_Write,osWaitForever);	
  }
}
