#include <isr.h>

#include <libopencm3/cm3/nvic.h>

void exti0_isr() { button_isr(); }

void exti1_isr() { button_isr(); }

void exti2_isr() { button_isr(); }

void exti3_isr() { button_isr(); }

void exti4_isr() { button_isr(); }

void exti9_5_isr() { button_isr(); }

void exti15_10_isr() { button_isr(); }
