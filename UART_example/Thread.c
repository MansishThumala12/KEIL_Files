#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "System_parameters.h"
#include "lcd16x2.h"

char Uart_Input;
 
void Fun_Uart_Read(void const *argument);                             
void Fun_Lcd(void const *argument);
void Fun_Uart_Write(void const *argument);
void Fun_Clear_Line(void);

osThreadId tid_Uart_Read, tid_Lcd, tid_Uart_Write;                                          
char Buff[BUFF_SIZE];
uint8_t Bi = 0;
	
osThreadDef (Fun_Uart_Read, osPriorityAboveNormal, 1, 0);                   
osThreadDef (Fun_Uart_Write, osPriorityNormal, 1, 0);
osThreadDef (Fun_Lcd, osPriorityNormal, 1, 0);

/*--- continue after sustem initilization ---*/
int Init_Thread (void) {

  tid_Uart_Read = osThreadCreate (osThread(Fun_Uart_Read), NULL);
	tid_Uart_Write = osThreadCreate (osThread(Fun_Uart_Write), NULL);
  tid_Lcd = osThreadCreate (osThread(Fun_Lcd), NULL);

  if (!(tid_Uart_Read||tid_Uart_Write||tid_Lcd)) return(-1);
  
  return(0);
}

void Fun_Uart_Read(void const *argument) {

	osSignalWait(Run,osWaitForever);
  while (1) {
    Uart_Input = USART2_GetChar(); // Insert thread code here...
		if(Bi < BUFF_SIZE-1 && Uart_Input != 0x0d){
			Buff[Bi++] = Uart_Input;
		}
		else{
			int8_t temp_Bi = 0;
			while(temp_Bi < Bi){
				osMessagePut(mid_Lcd,Buff[temp_Bi],10);
				osMessagePut(mid_Uart_Write,Buff[temp_Bi],10);
				temp_Bi++;
			}
			osMessagePut(mid_Lcd,'\n',10);
			osMessagePut(mid_Uart_Write,'\n',10);
			Bi = 0;
		}
		osSignalWait(Run,osWaitForever);
  }
	
}

void Fun_Uart_Write(void const *argument) {

	Uart = osMessageGet(mid_Uart_Write,osWaitForever);	
  while (1) {
		USART2_PutChar(Uart.value.v); // Insert thread code here...
		Uart = osMessageGet(mid_Uart_Write,osWaitForever);	
  }
}
void Fun_Lcd(void const *argument) {
	
	Lcd = osMessageGet(mid_Lcd,osWaitForever);
  while (1) {
    lcd16x2_putc(Lcd.value.v); // Insert thread code here...
		Lcd = osMessageGet(mid_Lcd,osWaitForever);
  }
}

void Fun_Clear_Line(void){
	
}
