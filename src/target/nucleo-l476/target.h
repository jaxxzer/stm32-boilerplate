#include "../nucleo/nucleo-64.h"

#define ADC_RCC RCC_ADC
#define ADC_CCR_PRESCALE ADC_CCR_PRESC_DIV1
#define ADC_CHANNEL 0

#define BUTTON_GPIO_RCC RCC_GPIOA
#define BUTTON_GPIO_PORT GPIOA
#define BUTTON_GPIO_PIN GPIO0
#define BUTTON_EXTI EXTI0
#define BUTTON_EXTI_IRQ NVIC_EXTI0_IRQ

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>
void initializeSystemClocks() {
  // https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/l4/stm32l476g-disco/basics/basics.c
  rcc_osc_on(RCC_HSI16);

  flash_prefetch_enable();
  flash_set_ws(4);
  flash_dcache_enable();
  flash_icache_enable();
  /* 16MHz / 4 = > 4 * 40 = 160MHz VCO => 80MHz main pll  */
  rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40, 0, 0, RCC_PLLCFGR_PLLR_DIV2);
  rcc_osc_on(RCC_PLL);
  rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
  rcc_wait_for_sysclk_status(RCC_PLL);
  /* FIXME - eventually handled internally */
  rcc_ahb_frequency  = 80e6;
  rcc_apb1_frequency = 80e6;
  rcc_apb2_frequency = 80e6;
}
