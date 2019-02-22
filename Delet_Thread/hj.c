#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

extern int Init_Thread (void);
extern int Init_Thread2 (void);
extern int Init_Thread3 (void);

int main(){
	
	Init_Thread ();
	Init_Thread2 ();
	Init_Thread3 ();	
	while(1){
		osThreadYield();
	}
}