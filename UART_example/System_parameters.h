
#ifndef _SYSTEM_PARAMETERS
#define _SYSTEM_PARAMETERS

/*
 *UART_Functions
 */
extern void USART2_PutString(char *s);
extern void USART2_PutChar(char c);
extern void USART2_Init(void);
extern uint16_t USART2_GetChar(void);

#define BUFF_SIZE  15
/*
 *RTX Functions
 */
extern int Init_Thread (void);


//Signals
#define Run 0x01


/*
 *Message functions
 */
extern osMessageQId mid_Lcd,mid_Uart_Write;
extern int message_create(void);
extern osEvent Lcd,Uart;
#endif
