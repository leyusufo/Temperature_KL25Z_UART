#include "GPIO.h"
#include <MKL25Z4.h>
#include "PORT.h" 
/*
Configures GPIO peripherals
*/
void GPIO_HWconfig(){
	GPIOA_HWconfig();
    GPIOB_HWconfig();
	//GPIOC_HWconfig();
	//GPIOD_HWconfig();
	//GPIOE_HWconfig();
}

/*
Configures GPIOA 
*/
void GPIOA_HWconfig(){
	//SW1 PIN #5
    /**********************************************************/
    /*
    Configure the pin as an input or output
    GPIOx_PDDR ->PDD
    */
	GPIOA_PDDR &= ~GPIO_PDDR_PDD(MASK(SW1)); //input
    /**********************************************************/
}

/*
Configures GPIOB
*/
void GPIOB_HWconfig(){
    //RED LED PIN #18
    /**********************************************************/
    /*
    Configure the pin as an input or output
    GPIOx_PDDR ->PDD
    */
    GPIOB_PDDR |= GPIO_PDDR_PDD(MASK(RED_LED_SHIFT)); //output
	set_pin(GPIO_B, RED_LED_SHIFT);	//clear LED by setting pin high
    /**********************************************************/
}

/*
Configures GPIOC
*/
void GPIOC_HWconfig(){
}

/*
Configures GPIOD
*/
void GPIOD_HWconfig(){
}

/*
Configures GPIOE
*/
void GPIOE_HWconfig(){
}

/*
sets a specific pin to zero given a pin # and a specific GPIO peripheral
*/
void clear_pin(uint32_t gpio_type, uint32_t pin_num){
	if(gpio_type == GPIO_A){
		GPIOA_PCOR |= GPIO_PCOR_PTCO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_B){
		GPIOB_PCOR |= GPIO_PCOR_PTCO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_C){
		GPIOC_PCOR |= GPIO_PCOR_PTCO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_D){
		GPIOD_PCOR |= GPIO_PCOR_PTCO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_E){
		GPIOE_PCOR |= GPIO_PCOR_PTCO(MASK(pin_num));
	}
}

/*
sets a specific pin to one given a pin # and a specific GPIO peripheral
*/
void set_pin(uint32_t gpio_type, uint32_t pin_num){
	if(gpio_type == GPIO_A){
		GPIOA_PSOR |= GPIO_PSOR_PTSO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_B){
		GPIOB_PSOR |= GPIO_PSOR_PTSO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_C){
		GPIOC_PSOR |= GPIO_PSOR_PTSO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_D){
		GPIOD_PSOR |= GPIO_PSOR_PTSO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_E){
		GPIOE_PSOR |= GPIO_PSOR_PTSO(MASK(pin_num));
	}
}

/*
toggles a specific pin to zero given a pin # and a specific GPIO peripheral
*/
void toggle_pin(uint32_t gpio_type, uint32_t pin_num){
	if(gpio_type == GPIO_A){
		GPIOA_PTOR |= GPIO_PTOR_PTTO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_B){
		GPIOB_PTOR |= MASK(pin_num);
	}
	else if(gpio_type == GPIO_C){
		GPIOC_PTOR |= GPIO_PTOR_PTTO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_D){
		GPIOD_PTOR |= GPIO_PTOR_PTTO(MASK(pin_num));
	}
	else if(gpio_type == GPIO_E){
		GPIOE_PTOR |= GPIO_PTOR_PTTO(MASK(pin_num));
	}
}

/*
reads the value of a specific input pin given a pin # and a specific GPIO peripheral
*/
uint32_t read_input_pin(uint32_t gpio_type, uint32_t pin_num){
	if(gpio_type == GPIO_A){
		return ((GPIOA_PDIR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_B){
		return ((GPIOB_PDIR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_C){
		return ((GPIOC_PDIR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_D){
		return ((GPIOD_PDIR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_E){
		return ((GPIOE_PDIR >> pin_num) & 1);
	}
	return 0;
}

/*
reads the value of a specific output pin to zero given a pin # and a specific GPIO peripheral
*/
uint32_t read_output_pin(uint32_t gpio_type, uint32_t pin_num){
	if(gpio_type == GPIO_A){
		return ((GPIOA_PDOR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_B){
		return ((GPIOB_PDOR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_C){
		return ((GPIOC_PDOR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_D){
		return ((GPIOD_PDOR >> pin_num) & 1);
	}
	else if(gpio_type == GPIO_E){
		return ((GPIOE_PDOR >> pin_num) & 1);
	}
	return 0;
}