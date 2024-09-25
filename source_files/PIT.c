#include <MKL25Z4.h>
#include "PIT.h"
#include "GPIO.h"
#include "ADC.h"

/*
Periodic Interrupt timer Hardware condiguration
*/
void PIT_HWconfig(int pit_ch, uint32_t ldval){
    /*
    Enable clock gating to PIT
    SIM_SCGC6->PIT
    */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    
    /*
    Disable or enable the module clock. This field must be enabled before any other setup is done.
    PIT_MCR ->MDIS
    */
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;//enable
    
    /*
    Allow or stop timers when the device enters debug mode
    PIT_MCR ->FRZ
    */
    PIT->MCR &= ~PIT_MCR_FRZ_MASK; //run in debug
    
    /*
    Load the start value of the timer n
    PIT_LDVALn ->TSV
    */
    PIT->CHANNEL[pit_ch].LDVAL = ldval;
    
    /*
    Enable or disable chain timer mode
    PIT_TCTRLn ->CHN
    */
    PIT->CHANNEL[pit_ch].TCTRL &= ~PIT_TCTRL_CHN_MASK;//no chaining
    
    /*
    Enable or disable timer n interrupt
    PIT_TCTRLn ->TIE
    */
    PIT->CHANNEL[pit_ch].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	/*
	Clear interrupt flag by writing 1 to it
	*/
	PIT->CHANNEL[pit_ch].TFLG |= PIT_TFLG_TIF_MASK;
}

/*
Periodic Interrupt timer Interrupt Service Routine configuration
*/
void PIT_IRQconfig(void){
    // Configure NVIC to enable IRQ handler for PIT
		NVIC_SetPriority(PIT_IRQn, 3); 
		NVIC_ClearPendingIRQ(PIT_IRQn); 
		NVIC_EnableIRQ(PIT_IRQn);
}

/*
Starts the timer by enabling it vis hardware
*/
void PIT_start(int pit_ch){
    /*
    Enable or disable timer n
    PIT_TCTRLn ->TEN
    */
    PIT->CHANNEL[pit_ch].TCTRL |= PIT_TCTRL_TEN_MASK; //start counter
}

/*
disables the timer
*/
void PIT_stop(int pit_ch){
    /*
    Enable or disable timer n
    PIT_TCTRLn ->TEN
    */
    PIT->CHANNEL[pit_ch].TCTRL &= ~PIT_TCTRL_TEN_MASK; //stop counter
}

/*
Checks whether the timer is enabled or not
*/
uint32_t PIT_enabled(int pit_ch){
    /*
    determine if pit_ch is enabled by reading the TEN field of TCTRL
    PIT_TCTRLn ->TEN
    */
	uint32_t tmp = (PIT->CHANNEL[pit_ch].TCTRL & PIT_TCTRL_TEN_MASK)>> PIT_TCTRL_TEN_SHIFT;
    return tmp; 
}

/*
Timer's Interrupt service routine
*/
void PIT_IRQHandler(void){
    /*
    check which timer caused the interrupt
    */
    if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){
        PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//clear pending interrupt flag
	}
    else if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK){
        PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;//clear pending interrupt flag
	}
}
