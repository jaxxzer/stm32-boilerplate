#pragma once

#define LED_GPIO_RCC RCC_GPIOC
#define LED_GPIO_PORT GPIOC
#define LED_GPIO_PIN GPIO13

#define ADC_PERIPH ADC1
#define ADC_RCC RCC_ADC1
#define ADC_CHANNEL 0

#define ADC_GPIO_RCC RCC_GPIOA
#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO0

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_64mhz(); }
