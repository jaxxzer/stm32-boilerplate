#pragma once

#define LED_GPIO_RCC RCC_GPIOB
#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN GPIO1

#define ADC_PERIPH ADC1
#define ADC_RCC RCC_ADC1
#define ADC_CHANNEL 0

#define ADC_GPIO_RCC RCC_GPIOA
#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO0

#define BUTTON_GPIO_RCC RCC_GPIOB
#define BUTTON_GPIO_PORT GPIOB
#define BUTTON_GPIO_PIN GPIO8
#define BUTTON_EXTI EXTI8
#define BUTTON_EXTI_IRQ NVIC_EXTI9_5_IRQ

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_64mhz(); }
