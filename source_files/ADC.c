#include "ADC.h"
#include "MKL25Z4.h"                    // Device header
#include "GPIO.h"
#include "cmsis_os2.h" 

volatile uint32_t adc_result;
volatile char adc_conv_complete;

extern osEventFlagsId_t EventFlagId;

/*********************************************************************************************/
//CONFIGURING THE ADC HARDWARE
void ADC_HWconfig(uint32_t cr_n){
    /*
    Clock gating: enable clock to the ADC peripheral
    */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    
	/*
    Enable/disable Interrupts upon completion of an ADC conversion
    ADCx_SC1n->AIEN
    */
    ADC0->SC1[cr_n] |= ADC_SC1_AIEN_MASK; //enabled
    
    /*
    Enable differential Mode or single-ended mode
    ADCx_SC1n->DIFF
    */
    ADC0->SC1[cr_n] &= ~ADC_SC1_DIFF_MASK; //Single-ended
    
    /*
    Select Input Channel based on differential or single-ended mode
    ADCx_SC1n->ADCH
    */
	ADC0->SC1[cr_n] &= ~ADC_SC1_ADCH_MASK;
    ADC0->SC1[cr_n] |= ADC_SC1_ADCH(NTC_THERMISTOR); //temperature sensor
	
    /*
    Choose Normal or low power. If low power is chosen, this optimizes power consumption when higher sample rates are not required.
    ADCx_CFG1->ADLPC
    */
    ADC0->CFG1 &= ~ADC_CFG1_ADLPC_MASK; //normal power
    
    /*
    Choose clock source division or the divide ratio used by the ADC to generate the internal clock ADCK
    ADCx_CFG1->ADIV
    */
    ADC0->CFG1 |= ADC_CFG1_ADIV(DIV_EIGHT); //clock source divided by 8
    
    /*
    Choose short sample time or long sample time.
    ADCx_CFG1->ADLSMP
    */
    ADC0->CFG1 &= ~ADC_CFG1_ADLSMP_MASK; //short
    
    /*
    Choose ADC resolution mode based on differential or single-ended mode
    ADCx_CFG1-> MODE 
    */
    ADC0->CFG1 |= ADC_CFG1_MODE(SE_TEN); //single-ended 10-bit 
    
    /*
    Select the input clock source to generate the internal clock, ADCK
    ADCx_CFG1->ADICLK
    */
    ADC0->CFG1 |= ADC_CFG1_ADICLK(BUS_CLK_TWO); //12MHZ 
    
    /*
    select between alternate sets of ADC channels if ADC will do conversion for multiple input channels. 
    ADCx_CFG2->MUXSEL
    */
    ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK; //ADxxa channels are selected.
    
    /*
    Choose Asynchronous Clock input and Output clock
    ADCx_CFG2->ADACKEN
    */
    ADC0->CFG2 &= ~ADC_CFG2_ADACKEN_MASK; //asynchronous clk disabled
    
    /*
    Choose normal or high speed configuration
    ADCx_CFG2->ADHSC
    */
    ADC0->CFG2 &= ~ADC_CFG2_ADHSC_MASK; //normal
    
    /*
    If long sample time is selected, then choose extended sample times available
    ADCx_CFG2->ADLSTS
    */
    ADC0->CFG2 |= ADC_CFG2_ADLSTS(DEFAULT);
    
    /*
    Load the compare value register to compare the conversion result if using compare mode
    ADCx_CVn ->CV
    */
    ADC0->CV1 |= ADC_CV1_CV(CV1_VAL);
    ADC0->CV2 |= ADC_CV2_CV(CV2_VAL);
    
    /*
    Select the conversion trigger: software or hardware
    ADCx_SC2->ADTRG
    */
    ADC0->SC2 |= ADC_SC2_ADTRG_MASK; //hardware
    
    /*
    Disable or enable compare function
    ADCx_SC2->ACFE
    */
    ADC0->SC2 &= ~ADC_SC2_ACFE_MASK; //disabled
    
    /*
    If you chose to enable compare function, then choose type of comparison (less than, greater than, etc)
    ADCx_SC2->ACFGT
    */
    ADC0->SC2 &= ~ADC_SC2_ACFGT_MASK; //not used,  ACFE disabled
    
    /*
    If you chose to enable compare function, then choose to enable or disable range comparison
    ADCx_SC2->ACREN
    */
    ADC0->SC2 &= ~ADC_SC2_ACREN_MASK; //not used,  ACFE disabled
    
    /*
    Enable or disable DMA transfers
    ADCx_SC2->DMAEN
    */
    ADC0->SC2 &= ~ADC_SC2_DMAEN_MASK; //disabled
    
    /*
    Selects the voltage reference source used for conversions. 
    ADCx_SC2->REFSEL
    */
    ADC0->SC2 |= ADC_SC2_REFSEL(VREF); //veref 3.3v
    
    /*
    Set to enable calibration sequence, else leave it alone
    ADCx_SC3->CAL
    */
    ADC0->SC3 &= ~ADC_SC3_CAL_MASK; //disabled
    
    /*
    Choose single or continuous ADC conversion
    ADCx_SC3->ADCO
    */
    ADC0->SC3 &= ~ADC_SC3_ADCO_MASK; //one conversion
    
    /*
    Disable or enable hardware average function of the ADC
    ADCx_SC3->CAL AVGE
    */
    ADC0->SC3 |= ADC_SC3_AVGE_MASK; //enabled
    
    /*
    If Hardware average is selected, then choose the number of average ADC conversions to average
    ADCx_SC3->CAL AVGS
    */
    ADC0->SC3 |= ADC_SC3_AVGS(AVG_16); //not used, average disabled
    
    /*
    Configure the SIM->SOPT7 register if hardware trigger selected
    */  
    //ADC0 alternate trigger enable: SIM->SOPT7->ADC0ALTTRGEN
    SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK; //alternate selected
    
    //ADC0 pretrigger select: SIM->SOPT7->ADC0PRETRGSEL
    SIM->SOPT7 &= ~SIM_SOPT7_ADC0PRETRGSEL_MASK; //pre trigger A
    
    //ADC0 trigger select: SIM->SOPT7->ADC0TRGSEL
    SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(PIT0_TRG); //PIT0 selected as trigger
}

//Writes to ADCx_SC1n register to start a conversions
void ADC_SC1_write(uint32_t cr_n){
	/*
    Enable Interrupts upon completion of an ADC conversion
    ADCx_SC1n->AIEN
    */
    ADC0->SC1[cr_n] |= ADC_SC1_AIEN_MASK; //enabled
    
    /*
    Enable differential Mode or single-ended mode
    ADCx_SC1n->DIFF
    */
    //ADC0->SC1[cr_n] |= ADC_SC1_DIFF_MASK; //differential
    
    /*
    Select Input Channel based on differential or single-ended mode
    ADCx_SC1n->ADCH
    */
    ADC0->SC1[cr_n] |= ADC_SC1_ADCH(TEMP_SENSOR); //temperature sensor
}

/*
For software: ISR configuration
*/
void ADC_IRQconfig(){  
    // Configure NVIC to enable IRQ handler for PIT
	NVIC_SetPriority(ADC0_IRQn, 0); 
	NVIC_ClearPendingIRQ(ADC0_IRQn); 
	NVIC_EnableIRQ(ADC0_IRQn);
}

/*
Interrupt service routine for ADC
*/
void ADC0_IRQHandler(){
    //if conversion complete 
	adc_result = ADC0->R[0]; //copy the ADC result
	osEventFlagsSet(EventFlagId, ADC_TEMP_DONE); //set the event flag to indicate conversion complete
}

