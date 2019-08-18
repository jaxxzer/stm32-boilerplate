#pragma once

#include "../nucleo/nucleo-64.h"

#define ADC_RCC RCC_ADC
#define ADC_CHANNEL 0

/*
 * for some reason when PC13/EXTI13 (user button) is selected, the interrupt fires
 * continuously. there is some mention in the datasheet about shared control of PC13
 * state with RTC, but the RTC bits don't appear to be set up for that
 * use A0 instead
 */

// #define BUTTON_GPIO_RCC RCC_GPIOC
// #define BUTTON_GPIO_PORT GPIOC
// #define BUTTON_GPIO_PIN GPIO13
// #define BUTTON_EXTI EXTI13
// #define BUTTON_EXTI_IRQ NVIC_EXTI4_15_IRQ

#define BUTTON_GPIO_RCC RCC_GPIOA
#define BUTTON_GPIO_PORT GPIOA
#define BUTTON_GPIO_PIN GPIO0
#define BUTTON_EXTI EXTI0
#define BUTTON_EXTI_IRQ NVIC_EXTI0_1_IRQ

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_48mhz(); }
