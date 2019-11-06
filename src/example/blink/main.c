#include <target.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

int main() {
  //initializeSystemClocks();
  // rcc_periph_clock_enable(LED_GPIO_RCC);
  *(uint32_t*)0x4002104c |= 1;

#if defined(STM32F1)
  gpio_set_mode(LED_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_GPIO_PIN);
#else
  gpio_mode_setup(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
#endif

  while (1) {
    gpio_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
    for (long i = 0; i < 1000000; i++) {
      asm("nop");
    }
  }
}
