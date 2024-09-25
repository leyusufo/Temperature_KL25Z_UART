#ifndef ADC_H_   /* Include guard */
#define ADC_H_

#include <stdint.h> 

//definitions for input channels
#define TEMP_SENSOR     (0x001A) 			//ADC thermistor channel number
#define NTC_THERMISTOR   (0x0009)			//ADC channel 9 connected to NTC thermistor 

//ADC thermistor parameters
#define R0 				(10000.0f) 			//Nominal value of Resistor at 25 deg celcius
#define T0 				(298.0f)  			//Nominal temperature in Kelvin at 25 deg celcius
#define B_CONST 		(3950.0f)  			//Given B value
#define K_CONST			(273.15f)			//kelvin to Fahrenheit contant
#define VREF_TP			(3.3f) 				//voltage reference (+) for thermistor
#define VREF_TM			(0) 				//voltage reference(-) for thermistor
#define TEMP_RES		(10.0f) 			//Resolution of temperature thermistor 
#define TWO_POW_RES		(1024.0f) 			// 2^TEMP_RES= 2^10
#define NINE_DIV_FIVE	(1.8f)				//9/5 constant for celcius to Farenheight conversion
#define ADC_TEMP_DONE	(1)

//ADC clock divide select
#define DIV_ONE         (0x00) //input_clock/1
#define DIV_TWO         (0x01) //input_clock/2
#define DIV_F0UR        (0x02) //input_clock/4
#define DIV_EIGHT       (0x03) //input_clock/8

//ADC conversion mode selection
#define SE_EIGHT        (0x00) //single-ended 8-bit
#define SE_TWELVE       (0x01) //single-ended 12-bit
#define SE_TEN          (0x02) //single-ended 10-bit
#define SE_SIXTEEN      (0x03) //single-ended 16-bit

#define DIFF_NINE       (0x00) //differential 9-bit 2's complement
#define DIFF_THIRTEEN   (0x01) //differential 13-bit 2's complement
#define DIFF_ELEVEN     (0x02) //differential 11-bit 2's complement
#define DIFF_SIXTEEN    (0x03) //differential 16-bit 2's complement

//ADC input clock selection
#define BUS_CLK         (0x00) //bus clock 24MHz
#define BUS_CLK_TWO     (0x01) //bus clock divided by 2 (12MHz)
#define ALTCLK          (0x02) //alternate input clock
#define ADACK           (0x03) //asynchronous clock

//definitions for Long Sample Time Select 
#define DEFAULT         (0x00) // Default longest sample time; 20 extra ADCK cycles; 24 ADCK cycles total.
#define EXTRA_TWELVE    (0x01) //12 extra ADCK cycles; 16 ADCK cycles total sample time.
#define EXTRA_SIX       (0x02) //6 extra ADCK cycles; 10 ADCK cycles total sample time.
#define EXTRA_TWO       (0x03) //extra ADCK cycles; 6 ADCK cycles total sample time.

//compare value register values
#define CV1_VAL         (0x00) //set to zero for now
#define CV2_VAL         (0x00) //set to zero for now

//voltage reference selection
#define VREF            (0x00) // Default voltage reference pin pair, that is, external pins VREFH and VREFL
#define VALT            (0x01) // Alternate reference pair, that is, VALTH and VALTL . This pair may be additional external pins or

//hardware average selection
#define AVG_4 			(0x00) //4 samples averaged.
#define AVG_8 			(0x01) //8 samples averaged.
#define AVG_16 			(0x02) //16 samples averaged.
#define AVG_32 			(0x03) //32 samples averaged.

//hardware trigger select
#define EXTRG_IN        (0x00) //0000  External trigger pin input (EXTRG_IN)
#define HSCMP0_TRG      (0x01) //0001 HSCMP0 output
#define PIT0_TRG        (0x04) //0100 PIT trigger 0
#define PIT1_TRG        (0x05) //0101 PIT trigger 1
#define TMP0_TRG        (0x08) //1000 TPM0 overflow
#define TMP1_TRG        (0x09) //1001 TPM1 overflow
#define TMP2_TRG        (0x0A) //1010 TPM2 overflow
#define RTCA_TRG        (0x0C) //1100 RTC alarm
#define RTCS_TRG        (0x0D) //1101 RTC seconds
#define LPTMR0_TRG      (0x0E) //1110 LPTMR0 trigger

//ADC function definitions
void ADC_HWconfig(uint32_t cr_n);
void ADC_IRQconfig();
void ADC_SC1_write(uint32_t cr_n);

#endif // ADC_H_
