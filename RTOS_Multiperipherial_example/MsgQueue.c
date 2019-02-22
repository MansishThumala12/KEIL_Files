
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "System_parameters.h"

/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 osMessageQId mid_Uart_Write,
							mid_Spi_Write;
//							mid_ADC1_Read;
//							mid_ADC2_Read;
 
 osEvent Uart;
 
osMessageQDef(mid_Uart_Write,12,unsigned char);
osMessageQDef(mid_Spi_Write,12,unsigned char);
//osMessageQDef(mid_ADC1_Read,48,uint16_t);
//osMessageQDef(mid_ADC2_Read,48,uint16_t);




int Init_messageQue(void){
	mid_Uart_Write = osMessageCreate(osMessageQ(mid_Uart_Write),NULL);
	mid_Spi_Write = osMessageCreate(osMessageQ(mid_Spi_Write),NULL);
//	mid_ADC1_Read = osMessageCreate(osMessageQ(mid_ADC1_Read),NULL);
//	mid_ADC2_Read = osMessageCreate(osMessageQ(mid_ADC2_Read),NULL);


	
	if (~(mid_Uart_Write || mid_Spi_Write /*|| mid_ADC1_Read || mid_ADC2_Read*/)){
		return(-1);
	}
	return(0);
}
