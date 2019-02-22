
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "System_parameters.h"
#include "lcd16x2.h"
#include <stdio.h>

/*----------------------------------------------------------------------------
 *      Mail Queue creation & usage
 *---------------------------------------------------------------------------*/
 

osMailQId qid_Lcd;                                        // mail queue id
osMailQDef (MailQueue, MAILQUEUE_OBJECTS, mstruc_Lcd);     // mail queue object


int Init_MailQueue (void) {

  qid_Lcd = osMailCreate (osMailQ(MailQueue), NULL);      // create mail queue

  return(0);
}

