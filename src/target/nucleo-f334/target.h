#pragma once

#include "../nucleo/nucleo-64.h"

#define ADC_RCC RCC_ADC12
#define ADC_CHANNEL 1

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_pll(&rcc_hsi_configs[1]); }
