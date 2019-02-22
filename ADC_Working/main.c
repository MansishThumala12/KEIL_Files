#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX5
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "lcd16x2.h"

#define BUFF_SIZE  14

//Data Types
char Buff[BUFF_SIZE];

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

//ADC Inititation function
void ADC1_Init(void);
void Fun_Adc_Read(void const *argument);
void Fun_Lcd(void const *argument);

//Thread ID's
osThreadId tid_Lcd,  
					 tid_Adc_Read;
osMailQId qid_Lcd;                                       

//Thread Definitations
osThreadDef (Fun_Lcd, osPriorityNormal, 1, 0);
osThreadDef (Fun_Adc_Read, osPriorityAboveNormal, 1, 0);
osMailQDef (MailQueue, MAILQUEUE_OBJECTS, mstruc_Lcd);  

int main(void){
	
	#if !defined(NO_LCD)
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	#endif
	
	qid_Lcd = osMailCreate (osMailQ(MailQueue), NULL);

  tid_Lcd = osThreadCreate (osThread(Fun_Lcd), NULL);
	tid_Adc_Read = osThreadCreate (osThread(Fun_Adc_Read), NULL);
	
	osThreadId tid_main = osThreadGetId();
	osThreadTerminate(tid_main);
	
	}

void ADC1_Init(void){
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//STEP1 : Initilizing ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	
	
	//STEP2 : Initilizing GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	
}

void Fun_Adc_Read(void const *argument){
	
	uint16_t previous=0;
	uint16_t adc_value = 0;
	mstruc_Lcd *temp;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	adc_value = ADC1->DR;
	while(1){
		if(200<=__fabs(adc_value-previous)||200>=__fabs(adc_value-previous)){
			previous = adc_value;
			temp = (mstruc_Lcd*)osMailAlloc(qid_Lcd, osWaitForever);
			temp->id = ADC_1;
			temp->data.val = (adc_value)*3.3/4096;
			osMailPut(qid_Lcd,temp);
		}
			osDelay(100);
			adc_value = ADC1->DR;
	}
}

void Fun_Lcd(void const *argument) {
	
	osEvent Lcd;
	mstruc_Lcd* Lcd_temp_struc;
	Lcd = osMailGet(qid_Lcd,osWaitForever);
  while (1) {
		Lcd_temp_struc = (mstruc_Lcd*)Lcd.value.p;
		switch(Lcd_temp_struc->id){
			case UART_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(0,1);
			lcd16x2_clear_line();
			lcd16x2_gotoxy(0,1);
			lcd16x2_puts("UR:");
			lcd16x2_puts(Lcd_temp_struc->data.str);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case SPI_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(0,1);
			lcd16x2_clear_line();
			lcd16x2_gotoxy(0,1);
			lcd16x2_puts("SP:");
			lcd16x2_puts(Lcd_temp_struc->data.str);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case ADC_1:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(3,0);
			lcd16x2_PutNum(Lcd_temp_struc->data.val,5);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			case ADC_2:
			#if !defined(NO_LCD)
			lcd16x2_gotoxy(3,0);
			lcd16x2_PutNum(Lcd_temp_struc->data.val,5);
			#endif
			osMailFree(qid_Lcd,Lcd_temp_struc);
			break;
			
			default:
			break;
		}
			
		Lcd = osMailGet(qid_Lcd,osWaitForever);
  }
}
