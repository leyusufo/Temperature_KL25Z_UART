#ifndef GPIO_H_   /* Include guard */
#define GPIO_H_

#include <stdint.h> 

/*
definition of GPIO peripheral
*/
#define GPIO_A (0)
#define GPIO_B (1)
#define GPIO_C (2)
#define GPIO_D (3)
#define GPIO_E (4)


/*
Function definitions
*/
void GPIO_HWconfig();
void GPIOA_HWconfig();
void GPIOB_HWconfig();
void GPIOC_HWconfig();
void GPIOD_HWconfig();
void GPIOE_HWconfig();

void clear_pin(uint32_t gpio_type, uint32_t pin_num);
void set_pin(uint32_t gpio_type, uint32_t pin_num);
void toggle_pin(uint32_t gpio_type, uint32_t pin_num);
uint32_t read_input_pin(uint32_t gpio_type, uint32_t pin_num);
uint32_t read_output_pin(uint32_t gpio_type, uint32_t pin_num);

#endif // GPIO_H_