#pragma once

#include "../nucleo/nucleo-64.h"

#define ADC_RCC RCC_ADC12
#define ADC_CHANNEL 1

#define BUTTON_GPIO_RCC RCC_GPIOA
#define BUTTON_GPIO_PORT GPIOA
#define BUTTON_GPIO_PIN GPIO0
#define BUTTON_EXTI EXTI0
#define BUTTON_EXTI_IRQ NVIC_EXTI0_IRQ

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_pll(&rcc_hsi_configs[1]); }
