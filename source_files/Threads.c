#include "Threads.h"
#include "ADC.h"
#include "UART0.h"
#include <stdint.h> 
#include <math.h>
#include "MKL25Z4.h"					// Device header
#include "cmsis_os2.h" 
#include "PIT.h"

/*
variables
*/
extern volatile uint32_t adc_result;	//holds ADC result
float V_Temp; 							//voltage temperature calculated from ADC conversion
float R_Temp;							//resistance temperature calculated from ADC voltage
float neg_log_R_Temp;						//natural logarithm of the calculated thermistor resistance
float TempK;							//temperature in kelvin
float TempF; 							//temperature in Fahrenheit

osThreadId_t Tid_Convert_Temp;
osThreadId_t Tid_Prepare_Msg;
osThreadId_t Tid_Transmit_Msg;

osEventFlagsId_t EventFlagId;
osSemaphoreId_t PrepMsgSemId;
osSemaphoreId_t SendMsgSemId;

/*
configures RTOS objects
*/
void RTOS_objects_config(void){
	//create thread for temperature conversion from ADC channel
	Tid_Convert_Temp = osThreadNew(Thread_Convert_Temp, NULL, NULL); 
	
	//create a thread for preparing the temperature message to send or display
	Tid_Prepare_Msg = osThreadNew(Thread_Prepare_Msg, NULL, NULL); 
	
	//create a thread for transmitting the temperature message 
	Tid_Transmit_Msg = osThreadNew(Thread_Transmit_Msg, NULL, NULL); 
	
	//create event flag to determine ADC conversion complete
	EventFlagId = osEventFlagsNew(NULL);
	
	//create a semaphore to determine that temperature message has been prepared
	PrepMsgSemId = osSemaphoreNew(1, 0, NULL);
	
	//create a semaphore to indicate that message is ready to be sent
	SendMsgSemId = osSemaphoreNew(1, 0, NULL);
}

/*---------------------------------------------
THREAD DEFINITIONS
*---------------------------------------------*/

/*
Thread that converts the temperature from ADC voltage code
to celcius and farenheight
*/
void Thread_Convert_Temp(void *arg){
	uint32_t Event_Result;
	osStatus_t Sem_Status;
	
	while(1){
		//wait for ADC to finish conversion
		Event_Result=osEventFlagsWait(EventFlagId,ADC_TEMP_DONE,osFlagsWaitAny, osWaitForever);
		
		//check for event errors before calculating temperature
		if(Event_Result&ADC_TEMP_DONE){
			//stop the timer that triggers the ADC 
			PIT_stop(0);
			
			//convert ADC code into a voltage value using the result from ADC calculation
			V_Temp = adc_result*(VREF_TP/(TWO_POW_RES-1));
			
			//Thermistor resistance calculation from calculated Voltage 
			R_Temp = R0 * ((VREF_TP/V_Temp)-1);
			
			//take the natural log of the calculated thermistor resistance
			neg_log_R_Temp = -log(R0/R_Temp);
			
			//temperature in kelvin using the Beta value (ß) equation. 
			TempK = (1.0 / ((neg_log_R_Temp/B_CONST)+(1/T0)));
			
			//temperature in Fahrenheit
			TempF =(TempK-K_CONST)*NINE_DIV_FIVE + 32.0;
		
			//release the semaphore to indicate that temperature has been calculated
			Sem_Status = osSemaphoreRelease(PrepMsgSemId);
			if(Sem_Status == osOK){
				;
			}
		}
	}
}

/*
Thread enqueues the message that contains temperature information into the 
transmitter queue
*/
void Thread_Prepare_Msg(void *arg){
	uint8_t message[50]="Temperature in degree F: ";
	uint8_t *data1_ptr = NULL;
			
	int temp = 0;
	int cnt_digit=0;
	uint8_t data;
	
	osStatus_t Sem_Status;
	
	while(1){
		//wait for semaphore to be released by Thread_Convert_Temp
		Sem_Status = osSemaphoreAcquire(PrepMsgSemId, osWaitForever);
		if(Sem_Status == osOK){
			//enqueues message into the TX queue
			data1_ptr = message;
			while(*data1_ptr != '\0'){
				if(!UART_TXQ_Full()){
					UART_TX_Enqueque(*data1_ptr);
					data1_ptr++;
				}
			}
			
			//converts the temperature digits from integers into ASCII characters
			//and enqueues it into the transmitter queue
			if(TempF < 0 || TempF > 0){
				if(TempF<0){
					TempF = TempF * (-1);
					//enqueue the sign
					if(!UART_TXQ_Full()){
						UART_TX_Enqueque('-');
					}
				}
				
				cnt_digit=1;
				temp=TempF;
				temp/=10;
				while(temp > 0){
					cnt_digit*=10;
					temp/=10;
				}
				
				temp = TempF;
				for(;cnt_digit > 0; cnt_digit=cnt_digit/10){
					if(!UART_TXQ_Full()){
						data = '0' + (temp/cnt_digit);
						UART_TX_Enqueque(data);
					}
					data = '0' + (temp/cnt_digit);
					temp=temp%cnt_digit;
				}
			}
			else{
				//enqueue the sign
				if(!UART_TXQ_Full()){
					UART_TX_Enqueque('0');
				}
			}
				
			//enqueue a newline
			if(!UART_TXQ_Full()){
				UART_TX_Enqueque('\n');
			}
			
			//release the semaphore to indicate that message has been enqueued
			Sem_Status = osSemaphoreRelease(SendMsgSemId);
			if(Sem_Status == osOK){
				;
			}
		}
	}
}

/*
enables the transmitter to allow tranmission of temperature message
*/
void Thread_Transmit_Msg(void *arg){
	osStatus_t Sem_Status;
	
	while(1){
		//wait for the semaphore to be released indicating that the message has been 
		//enqueued into the TX queue
		Sem_Status = osSemaphoreAcquire(SendMsgSemId, osWaitForever);
		if(Sem_Status == osOK){//check for errors
			
			//enable transmitter
			UART0_TED(1);
			
			//enable transmitter interrupts
			UART0_TIED(1);
			
			//wait for transmitter interrupt to be disabled by ISR
			while(UART0_TIE()){
				;
			}
			
			//disable transmitter
			UART0_TED(0);
			
			
			//enable the timer to trigger another ADC conversion
			PIT_start(0);
			
		}
	}
}