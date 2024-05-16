#ifndef UART0_H_   /* Include guard */
#define UART0_H_

#include <stdint.h> 

//definition of clock sources for UART0
#define UART0_CLK_DIS         (0) //Clock disabled
#define UART0_CLK_MCGF_MCGP   (1) //MCGFLLCLK clock or MCGPLLCLK/2 clock ---bus clock up to 48MHz 
#define UART0_CLK_OSCERCLK    (2) //OSCERCLK clock
#define UART0_CLK_MCGIRCLK    (3) //MCGIRCLK clock

//definition of transmit data source select
#define UART0_TX_PIN    (0) //UART0_TX pin
#define UART_TX_PINM1   (1) //UART0_TX pin modulated with TPM1 channel 0 output
#define UART_TX_PINM2   (2) //UART010 UART0_TX pin modulated with TPM2 channel 0 output

/*
definition of queque macros
*/
//#define RXQ         (1)
#define RXQ_SIZE    (32)
#define TXQ         (1)
#define TXQ_SIZE    (32)

//UART0 baud rate generation
#define UART0_CLK   (48e6)  //uart clock rate
#define UART0_OSR   (16)    //oversampling rate

/*
function declarations
*/
void UART0_HWconfig(uint32_t baud_rate);
void UART0_IRQconfig();

void UART0_TED(uint16_t ted);
void UART0_TIED(uint16_t tied);
uint16_t UART0_TIE();
void UART_TX_Enqueque(uint8_t data);
uint8_t UART_TX_Dequeque();
uint16_t UART_TXQ_Empty();
uint16_t UART_TXQ_Full();

void UART0_RED(uint16_t red);

#endif // UART_H_
