/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

#include "System_parameters.h"
#include "lcd16x2.h"
/*
 * main: initialize and start the system
 */
 
 int result;
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

	USART2_Init();
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);

	result = message_create();
	result = Init_Thread ();

  osKernelStart ();                         // start thread execution 
}
