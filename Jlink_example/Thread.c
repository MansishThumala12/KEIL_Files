
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function

osThreadId tid_Thread_0,tid_Thread_1,tid_Thread_2;                                          // thread id

osThreadDef (Thread, osPriorityNormal, 3, 0);                   // thread object


int Init_Thread (void) {

  tid_Thread_0 = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread_0) return(-1);
  
	tid_Thread_1 = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread_1) return(-1);
  
	tid_Thread_2 = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread_2) return(-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
		osDelay(100);
  }
}
void Thread_1 (void const *argument) {

  while (1) {
		osDelay(100);
  }
}void Thread_2 (void const *argument) {

  while (1) {
		osDelay(100);
  }
}