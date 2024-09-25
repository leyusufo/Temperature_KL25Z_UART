#ifndef PORT_H_   /* Include guard */
#define PORT_H_ 

/*
Port pin definitions
*/
#define RED_LED_SHIFT (18) 	//on portB
#define SW1	(5) 			//on port A

/*
definition for masking bits
*/
#define MASK(x) (1UL << (x))

//INTERRUPT CONFIGURATION definitions
#define I_DIS       (0x00)//0000 Interrupt/DMA request disabled.
#define I_DMAR      (0x01)//0001 DMA request on rising edge.
#define I_DMAF      (0x02)//0010 DMA request on falling edge.
#define I_DMAE      (0x03)//0011 DMA request on either edge.
#define I_LZ        (0x08)//1000 Interrupt when logic zero.
#define I_RE        (0x09)//1001 Interrupt on rising edge.
#define I_FE        (0x0A)//1010 Interrupt on falling edge.
#define I_EE        (0x0B)//1011 Interrupt on either edge.
#define I_L1        (0x0C)//1100 Interrupt when logic one.

/*
function definitions
*/
void ports_HWconfig();
void PORTA_HWconfig();
void PORTB_HWconfig();
void PORTC_HWconfig();
void PORTD_HWconfig();
void PORTE_HWconfig();


void ports_IRQconfig();
void PORTA_IRQconfig();
void PORTD_IRQconfig();

#endif // PORT_H_
