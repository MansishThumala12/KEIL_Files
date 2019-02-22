
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "System_parameters.h"

/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 osMessageQId mid_Lcd,mid_Uart_Write;
 
 osEvent Lcd,Uart;
 
osMessageQDef(mid_Lcd,12,unsigned char);
osMessageQDef(mid_Uart_Write,12,unsigned char);

int message_create(void){
	mid_Lcd = osMessageCreate(osMessageQ(mid_Lcd),NULL);
	mid_Uart_Write = osMessageCreate(osMessageQ(mid_Uart_Write),NULL);
	
	if (~(mid_Lcd || mid_Uart_Write)){
		return(-1);
	}
	return(0);
}

