
#ifndef _SYSTEM_PARAMETERS
#define _SYSTEM_PARAMETERS



/*
 *UART_Functions
 */
extern void USART2_PutString(char *s);
extern void USART2_PutChar(char c);
extern void USART2_Init(void);
extern uint16_t USART2_GetChar(void);


#define BUFF_SIZE  14

/*
 *ADC1_Functions
 */
void ADC1_Init(void);
	
/*
 *SPI2_Functions
 */
extern void SPI2_Init(void);
extern void SPI2_PutChar(char);
extern void SPI2_PutString(char*);
extern uint16_t SPI2_GetChar(void);

/*
 *RTX Functions
 */
extern int Init_Thread (void);


//Signals
#define Run 0x01


/*
 *Message functions
 */
 
extern osMessageQId mid_Uart_Write,
										mid_Spi_Write,
										mid_ADC1_Read,
										mid_ADC2_Read;

extern int Init_messageQue(void);

/*
 *MAIL Functions
 */
 
 #define MAILQUEUE_OBJECTS      12                               // number of Message Queue Objects

 #define SPI_2 0x01
 #define UART_2 0x02
 #define ADC_1 0x03
 #define ADC_2 0x04
 
//Mail dataType
typedef union {
	char str[BUFF_SIZE];
	float val;
}Mail_dtype;

typedef struct {                                                // object data type
  uint8_t id;
  Mail_dtype data;
} mstruc_Lcd;

extern osMailQId qid_Lcd;

extern int Init_MailQueue(void);

#endif
