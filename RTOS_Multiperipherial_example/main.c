/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

#include "System_parameters.h"
#include "lcd16x2.h"

extern uint16_t Buff[BUFF_SIZE];

void LCD_system_Init(void);
/*
 * main: initialize and start the system
 */
 

int main (void) {
	
	USART2_Init();
	SPI2_Init();
	ADC1_Init();
	#if !defined(NO_LCD)
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	LCD_system_Init();
	#endif
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	Init_messageQue();
	Init_MailQueue();
	Init_Thread ();

  osKernelStart ();                         // start thread execution 
}


//LCD Display system init
void LCD_system_Init(void){
	lcd16x2_gotoxy(0,0);
	lcd16x2_puts("A1:");
	lcd16x2_gotoxy(8,0);
	lcd16x2_puts("A2:");
	
	for(int i=0; i<BUFF_SIZE; i++){
		Buff[i] = ' ';
	}
}
