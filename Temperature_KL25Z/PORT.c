#include "PORT.h"
#include <MKL25Z4.h>
#include "GPIO.h"
#include "PIT.h"
/*
Ports configuration - Hardware
*/
void ports_HWconfig(){
	PORTA_HWconfig();
    PORTB_HWconfig();
	//PORTC_HWconfig();
	//PORTD_HWconfig();
	//PORTE_HWconfig();
}

/*
configures Interrupt service routines for port A and D
*/
void ports_IRQconfig(){
	PORTA_IRQconfig();
	//PORTD_IRQconfig();
}

/*
hardware configuration for port A
*/
void PORTA_HWconfig(){
	/*
    Enable clock gating on the port A
    */ 
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
	//FOR UART0 RX on port A pin 1
    /**************************************************************************/ 
    /*
    Set pin mux control  
    PORTx_PCRn ->MUX
    */
    PORTA_PCR1 |= PORT_PCR_MUX(2); //2 for UART0 Rx
    /*******************************************************************************/
	
	//FOR UART0 TX on port A pin 2
    /**************************************************************************/
    /*
    2.	Set pin mux control  
    PORTx_PCRn ->MUX
    */
    PORTA_PCR2 |= PORT_PCR_MUX(2); //2 for UART0 Tx
    /*******************************************************************************/
	
    //FOR SW1 on port A pin 5
    /**************************************************************************/
    /*
    Enable or disable interrupt
    PORTx_PCRn ->IRQC
    */
    PORTA_PCR5 |= PORT_PCR_IRQC(I_FE); //ON FALLING EDGE
    
    /*
    Set pin mux control  
    PORTx_PCRn ->MUX
    */
	PORTA_PCR5 |= PORT_PCR_MUX(0); //clear
    PORTA_PCR5 |= PORT_PCR_MUX(1); //1 for GPIO
    
    /*
    Enable or disable the internal pull-up or pull-down resistor
    PORTx_PCRn->PE
    */
    PORTA_PCR5 |= PORT_PCR_PE_MASK; // enable
    /*******************************************************************************/
	
	//write 1s to the ISFR to clear status interrupt flags
	PORTA_ISFR = PORT_ISFR_ISF_MASK;
}

/*
hardware configuration for port B
*/
void PORTB_HWconfig(){
    /*
    Enable clock gating on the port
    */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//FOR NTC Thermistor connected to PTB1 (ADC channel 9 input)
    /**************************************************************************/
    /*
    2.	Set pin mux control  
    PORTx_PCRn ->MUX
    */
    PORTB_PCR1 |= PORT_PCR_MUX(0); //1 for ADC channel 9 input
    /*******************************************************************************/
    
    //FOR RED LED
    /**************************************************************************/
    /*
    1.  Enable or disable interrupt
    PORTx_PCRn ->IRQC
    */
    PORTB_PCR18 |= PORT_PCR_IRQC(I_DIS); //disabled
    
    /*
    2.	Set pin mux control  
    PORTx_PCRn ->MUX
    */
    PORTB_PCR18 |= PORT_PCR_MUX(1); //1 for GPIO
    /*******************************************************************************/
	
	
}

/*
hardware configuration for port C
*/
void PORTC_HWconfig(){
}

/*
hardware configuration for port D
*/
void PORTD_HWconfig(){
}

/*
hardware configuration for port E
*/
void PORTE_HWconfig(){
}

/*
Interrupt service routine configuration for port A
*/
void PORTA_IRQconfig(){
    // Configure NVIC to enable IRQ handler for PORTA
	NVIC_SetPriority(PORTA_IRQn, 2); 
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
}

/*
Interrupt service routine configuration for port D
*/
void PORTD_IRQconfig(){
    // Configure NVIC to enable IRQ handler for PORTD
	NVIC_SetPriority(PORTD_IRQn, 1); 
	NVIC_ClearPendingIRQ(PORTD_IRQn); 
	NVIC_EnableIRQ(PORTD_IRQn);
}

/*
Interrupt service routine for port A
*/
void PORTA_IRQHandler(){
	//for port A pin 5 connected to switch 1
	if(PORTA_ISFR & MASK(SW1)){//if ISR flag is set
		if(!read_input_pin(GPIO_A, SW1)){//check if the input is zero (true low logic)
			//if the PIT timer counter channel 0 is enabled
			if(PIT_enabled(0)){
				PIT_stop(0); //stop the PIT channel 0 
				set_pin(GPIO_B, RED_LED_SHIFT); //turn the red LED off (true low logic)
			}
			else{
				PIT_start(0); //if PIT ch 0 is not enabled, then start the timer
			}
		}
	}
	//write 1s to the ISFR to clear status interrupt flags
	PORTA_ISFR = PORT_ISFR_ISF_MASK;
}

