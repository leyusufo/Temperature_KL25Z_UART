#ifndef PIT_H_   /* Include guard */
#define PIT_H_

#include <stdint.h> 

//LDVAL values
#define LDVAL0 		(0x0E4E1BFF) //PIT channel 0 counter value to produce 10 seconds interrupt with peripheral bus clock =  24MHz 
//#define LDVAL0  	(0x55D4A7FF) //PIT channel 0 counter value to produce 1 minute interrupt with peripheral bus clock =  24MHz 
#define LDVAL1  	(0)

/*
function definitions
*/
void PIT_HWconfig(int pit_ch,uint32_t ldval);
void PIT_IRQconfig(void);
void PIT_start(int pit_ch);
void PIT_stop(int pit_ch);
uint32_t PIT_enabled(int pit_ch);

#endif // PIT_H_
