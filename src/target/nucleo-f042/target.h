#pragma once

#include "../nucleo/nucleo-32.h"

#define ADC_RCC RCC_ADC
#define ADC_CHANNEL 0

#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() { rcc_clock_setup_in_hsi_out_48mhz(); }
