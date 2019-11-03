#pragma once

#include "../nucleo/nucleo-64.h"

// supported examples
#define EXAMPLE_ADC
#define EXAMPLE_BLINK
#define EXAMPLE_BUTTON_EXTI
#define EXAMPLE_DAC
#define EXAMPLE_PWM_INPUT
#define EXAMPLE_PWM_OUTPUT
#define EXAMPLE_PWM_OUTPUT_DMA

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

#define DAC_RCC RCC_DAC1
#define DAC_GPIO_RCC RCC_GPIOA
#define DAC_GPIO_PIN GPIO4
#define DAC_GPIO_PORT GPIOA

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_48mhz(); }
