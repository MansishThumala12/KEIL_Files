#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include <stdio.h>

int i=0;

void P_T_S(const void *arguments){
	if(osKernelRunning()){
		printf("Kernal is running from P_T_S\n");
	}
	else{
		printf("Kernal is Not running from P_T_S\n");
	}
	while(1){
	osDelay(2);
		i++;
		osThreadYield();
	}
}

void U_R_T(const void *arguments){
	if(osKernelRunning()){
		printf("Kernal is running from U_R_T\n");
	}
	else{
		printf("Kernal is Not running from U_R_T\n");
	}
	while(1){
	osDelay(2);
		i++;
		osThreadYield();
	}

}

void H_N_D(const void *arguments){
	if(osKernelRunning()){
		printf("Kernal is running from H_N_D\n");
	}
	else{
		printf("Kernal is Not running from H_N_D\n");
	}
	while(1){
	osDelay(2);
		i++;
		osThreadYield();
	}

}

osThreadId  tid_P_T_S,
						tid_U_R_T,
						tid_H_N_D;

osThreadDef(P_T_S,osPriorityNormal,1,400);
osThreadDef(U_R_T,osPriorityNormal,1,400);
osThreadDef(H_N_D,osPriorityNormal,1,400);

int main(){
	
	tid_P_T_S = osThreadCreate(osThread(P_T_S),NULL);
	tid_U_R_T = osThreadCreate(osThread(U_R_T),NULL);
	tid_H_N_D = osThreadCreate(osThread(H_N_D),NULL);
	
	if(tid_H_N_D && tid_P_T_S && tid_U_R_T){
		printf("Thread Creation Sucessfull\n");
	}
	
	else{
		printf("Thread Creation Failed\n");
	}
	if(osKernelRunning()){
		printf("Kernal is running from main\n");
	}
	else{
		printf("Kernal is Not running from main\n");
		osKernelStart();
	}
	while(1){
	}
	
}
