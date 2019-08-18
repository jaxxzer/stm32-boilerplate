#pragma once

#include "../nucleo/nucleo-32.h"

#define ADC_RCC RCC_ADC
#define ADC_CHANNEL 0

#define BUTTON_GPIO_RCC RCC_GPIOA
#define BUTTON_GPIO_PORT GPIOA
#define BUTTON_GPIO_PIN GPIO0
#define BUTTON_EXTI EXTI0
#define BUTTON_EXTI_IRQ NVIC_EXTI0_1_IRQ

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_48mhz(); }
