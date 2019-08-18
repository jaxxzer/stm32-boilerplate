#include <isr.h>

#include <libopencm3/cm3/nvic.h>

void exti0_1_isr() { button_isr(); }

void exti2_3_isr() { button_isr(); }

void exti4_15_isr() { button_isr(); }
