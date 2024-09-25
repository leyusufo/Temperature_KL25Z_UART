#include <MKL25Z4.h>
#include "Threads.h"                  
#include "PIT.h"
#include "PORT.h"
#include "GPIO.h"
#include "ADC.h"
#include "UART0.h"
#include "cmsis_os2.h"

/*---------------------------------------------
 MAIN function
*---------------------------------------------*/
int main (void) {
	SystemInit();
	
	/*configure peripherals*/
	
	//configure periodic interrupt timer peripheral
	PIT_HWconfig(0, LDVAL0);
	PIT_IRQconfig();
	
	//configure ports peripheral
	ports_HWconfig(); 
	ports_IRQconfig();
	
	//configure GPIO peripheral
	GPIO_HWconfig();
	
	//configure ADC peripheral
	ADC_HWconfig(0);
	ADC_IRQconfig();
	
	//configure UART0 peripheral
	UART0_HWconfig(9600);
	UART0_IRQconfig();
	
	//PIT_start(0);
	
	/*
	configure RTOS
	*/
	
	//initialize the kernel
	osKernelInitialize();
	
	//initialize RTOS objects
	RTOS_objects_config();
	
	//start the kernel
	osKernelStart();
}
